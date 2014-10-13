// CommuBase.h: interface for the CCommuBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "Communication.h"


//ͨѶ����
class   ICommuBase  
{ 
public:
	ICommuBase():m_pfnCommCallBack(0){}
	virtual ~ICommuBase(){}

	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//������
	//����˵��:
	//pData[IN]����������Ҫ�����ݽṹָ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//�����ӳɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnOpen(void *pData,std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//�ر�����
	//����˵��:
	//strError[OUT]������Ϣ����
	//����˵��:
	//�ر����ӳɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnClose(std::string &strError) = 0;
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
	virtual bool fnReceiveString(std::string &strRecv,std::string &strError) = 0;    //////////////////////////////////////////////////////////////////////////
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
	virtual bool fnSendData(const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError)= 0;
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
	virtual bool fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//ע��ص�����
	//����˵��:
	//pFnCallBack[IN]�ص�����ָ��
	//����˵��:
	//��
	//////////////////////////////////////////////////////////////////////////
	virtual void fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2)
	{
		m_pfnCommCallBack = pFnCallBack;
		m_ulCallBackParam2 = ulParam2;
	}
protected:

	fnCommCallBack		m_pfnCommCallBack;
	unsigned long m_ulCallBackParam2;
};

//ͨѶ�๤����,���ڴ���ָ����ͨ�����ͽӿ���
class   ICommuFactory
{

public:
	virtual ~ICommuFactory(){}

	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//����ͨѶ���ʹ�����Ӧ��ͨ����
	//����˵��:
	//strSend[IN]��Ҫ���͵�����
	//strTXRate[OUT]��������
	//strRXRate[OUT]��������
	//����˵��:
	//��¼�ɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual ICommuBase *fnCreateCommu(ECOMMTYPE eCommuType,std::string &strError) = 0;

};//ͨѶ�๤����,���ڴ���ָ����ͨ������ʵ����
class   CCommuFactory: public ICommuFactory
{

public:
	CCommuFactory();
	virtual ~CCommuFactory();

	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//����ͨѶ���ʹ�����Ӧ��ͨ����
	//����˵��:
	//strSend[IN]��Ҫ���͵�����
	//strTXRate[OUT]��������
	//strRXRate[OUT]��������
	//����˵��:
	//��¼�ɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual ICommuBase *fnCreateCommu(ECOMMTYPE eCommuType,std::string &strError);
private:
	ECOMMTYPE m_eCommuType;

};

