#pragma once
#include "CommuBase.h"
class CCommuSerial  : public ICommuBase
{ 
public:
	CCommuSerial();
	virtual ~CCommuSerial();

private:
	short m_sComPort;
	std::string   m_strBuffer;
	unsigned char* chrBuffer;
public:


public:
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//������
	//����˵��:
	//pData[IN]����������Ҫ�����ݽṹָ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//�����ӳɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnOpen(void *pData,std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//�ر�����
	//����˵��:
	//strError[OUT]������Ϣ����
	//����˵��:
	//�ر����ӳɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnClose(std::string &strError);
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
	virtual bool fnSendData(const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError);

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
	virtual bool fnReceiveString(std::string &strRecv,std::string &strError);
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
	virtual bool fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//ע��ص�����
	//����˵��:
	//pFnCallBack[IN]�ص�����ָ��
	//����˵��:
	//��
	//////////////////////////////////////////////////////////////////////////
	virtual void fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2);
protected:
	int fnChangeBaud(unsigned long ulBaud);
	std::string fnGetErrorMsg(int iErrorCode);
};
