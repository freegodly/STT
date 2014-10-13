
#include "CommuTelnet.h"

// CommuTelnet.cpp: implementation of the CCommuTelnet class.
//
//////////////////////////////////////////////////////////////////////


#include "Telnet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommuTelnet::CCommuTelnet()
{
	m_pTelnet = 0;
}

CCommuTelnet::~CCommuTelnet()
{
	if (0 != m_pTelnet)
	{
		delete m_pTelnet;
		m_pTelnet = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//����˵��:
//������
//����˵��:
//pData[IN]����������Ҫ�����ݽṹָ��
//strError[OUT]������Ϣ����
//����˵��:
//�����ӳɹ�����true,���򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CCommuTelnet::fnOpen(void *pData,std::string &strError)
{
	ST_DATA_TELNET *pTelnet = (ST_DATA_TELNET*)pData;
	//����SOCKET
	if (0 == m_pTelnet)
	{
		m_pTelnet = new CTelnet();
	}
	m_pTelnet->TELNET_Initial(0);
	if(!m_pTelnet->TELNET_Connect(pTelnet->strTargetIP,pTelnet->usPort))
	{
		strError = "telnet ����ʧ��";
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
//����˵��:
//�ر�����
//����˵��:
//strError[OUT]������Ϣ����
//����˵��:
//�ر����ӳɹ�����true,���򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CCommuTelnet::fnClose(std::string &strError)
{
	if (0 != m_pTelnet)
	{
		m_pTelnet->TELNET_Disconnect();
		delete m_pTelnet;
		m_pTelnet = 0;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//����˵��:
//���ն�ʱʱ���������յ������ݲ�����
//����˵��:
//strRecv[OUT]���յ��ַ���
//ulTime[IN]��ʱʱ��
//strError[OUT]������Ϣ����
//����˵��:
//�����ʱ���ڽ��յ����ݷ���true,���򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CCommuTelnet::fnReceiveString(std::string &strRecv,std::string &strError)
{
	if (0 != m_pTelnet)
	{
		return  m_pTelnet->TELNET_RevString(&strRecv);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//����˵��:
//�����ļ�
//����˵��:
//strFilePath[IN]�ļ�·��
//eProt[IN]����Э��
//strError[OUT]������Ϣ����
//����˵��:
//���ͳɹ�����true,���򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CCommuTelnet::fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////
//����˵��:
//�����ַ�
//����˵��:
//pbData[IN]�����͵�����
//ulDataLen[IN]���ݳ���
//ulTime[IN]��ʱʱ��
//strError[OUT]������Ϣ����
//����˵��:
//����ڳ�ʱʱ���ڷ��ͳɹ�����true,���򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CCommuTelnet::fnSendData( const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError )
{	
	//���ûص�����
	if (0 != m_pfnCommCallBack)
	{
		m_pfnCommCallBack((unsigned long)pbData,m_ulCallBackParam2);
	}
	if (0 != m_pTelnet)
	{
		char * pSend = (char*)pbData;
		if(!strcmp(pSend , "CtrlC"))
		{
			m_pTelnet->TELNET_SendString("\x03");
		}
		else
		{
			m_pTelnet->TELNET_SendString_hex(pSend,ulDataLen);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//����˵��:
//ע��ص�����
//����˵��:
//pFnCallBack[IN]�ص�����ָ��
//����˵��:
//��
//////////////////////////////////////////////////////////////////////////
void CCommuTelnet::fnRegCallBack( const fnCommCallBack pFnCallBack,unsigned long ulParam2 )
{
	ICommuBase::fnRegCallBack(pFnCallBack,ulParam2);

	if (0 != m_pTelnet)
	{
		m_pTelnet->fnRegCallBack(pFnCallBack,ulParam2);
	}
}
