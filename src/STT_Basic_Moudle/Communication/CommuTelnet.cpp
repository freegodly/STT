
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
//函数说明:
//打开连接
//参数说明:
//pData[IN]打开连接所需要的数据结构指针
//strError[OUT]错误信息返回
//返回说明:
//打开连接成功返回true,否则返回false
//////////////////////////////////////////////////////////////////////////
bool CCommuTelnet::fnOpen(void *pData,std::string &strError)
{
	ST_DATA_TELNET *pTelnet = (ST_DATA_TELNET*)pData;
	//创建SOCKET
	if (0 == m_pTelnet)
	{
		m_pTelnet = new CTelnet();
	}
	m_pTelnet->TELNET_Initial(0);
	if(!m_pTelnet->TELNET_Connect(pTelnet->strTargetIP,pTelnet->usPort))
	{
		strError = "telnet 连接失败";
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
//函数说明:
//关闭连接
//参数说明:
//strError[OUT]错误信息返回
//返回说明:
//关闭连接成功返回true,否则返回false
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
//函数说明:
//接收定时时间内所有收到的数据并返回
//参数说明:
//strRecv[OUT]接收的字符串
//ulTime[IN]定时时间
//strError[OUT]错误信息返回
//返回说明:
//如果在时间内接收到数据返回true,否则返回false
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
//函数说明:
//发送文件
//参数说明:
//strFilePath[IN]文件路径
//eProt[IN]发送协议
//strError[OUT]错误信息返回
//返回说明:
//发送成功返回true,否则返回false
//////////////////////////////////////////////////////////////////////////
bool CCommuTelnet::fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////
//函数说明:
//发送字符
//参数说明:
//pbData[IN]被发送的数据
//ulDataLen[IN]数据长度
//ulTime[IN]超时时间
//strError[OUT]错误信息返回
//返回说明:
//如果在超时时间内发送成功返回true,否则返回false
//////////////////////////////////////////////////////////////////////////
bool CCommuTelnet::fnSendData( const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError )
{	
	//调用回调函数
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
//函数说明:
//注册回调函数
//参数说明:
//pFnCallBack[IN]回调函数指针
//返回说明:
//无
//////////////////////////////////////////////////////////////////////////
void CCommuTelnet::fnRegCallBack( const fnCommCallBack pFnCallBack,unsigned long ulParam2 )
{
	ICommuBase::fnRegCallBack(pFnCallBack,ulParam2);

	if (0 != m_pTelnet)
	{
		m_pTelnet->fnRegCallBack(pFnCallBack,ulParam2);
	}
}
