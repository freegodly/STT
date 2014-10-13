#pragma once
#include <string>
#include <VECTOR>

class ICommuFactory;
class ICommuBase;
//ͨ������
 typedef enum _ECOMMTYPE
{ 
	//��ЧͨѶ����
	eCTypeUnknow = 0,
	//�ܵ�ͨѶ����,��Ҫ��������cmd.exe
	eCTypePipe = 1,
	//telnetͨѶ����,��SOCKETʵ��
	eCTypeTelnet,
	//����ͨѶ����
	eCTypeSerial
}ECOMMTYPE;

//�����ļ�ʱ���õķ���Э��
typedef enum _ESENDFILEPROT
{
	//��ЧЭ��
	eProtNon = 0,
	//һ��Э��
	eProtNormal = 1
}ESENDFILEPROT;

//ͨ��ģ��ص�����ָ��,��Ҫ����ͨѶ�������շ�����ʱ����
//uiParam1:�ַ���ָ��(std::string),uiParam2:�û��Զ��������
typedef void (*fnCommCallBack)(unsigned long uiParam1,unsigned long uiParam2);

//�ܵ�ͨ������Ҫ�����ݽṹ
typedef struct _ST_DATA_PIPE
{
	std::string strPipeName;
}ST_DATA_PIPE;

//����ͨ������Ҫ�����ݽṹ
typedef struct _ST_DATA_SERIAL
{
	//���ں�
	unsigned short usComNum;
	//������
	unsigned long  ulBaudRate;
}ST_DATA_SERIAL;
//telnetͨ������Ҫ�����ݽṹ
typedef struct _ST_DATA_TELNET
{
	//����IP
	std::string strLocalIP;
	//Ŀ��IP
	std::string strTargetIP;
	//Ŀ��˿�
	unsigned short usPort;
}ST_DATA_TELNET;

//��¼ʱ��ʹ�õĽṹ��
typedef struct _ST_DATA_LOGIN 
{
	std::string  strSend;
	std::string  strWait;
	unsigned long ulTimeOut; 
	_ST_DATA_LOGIN()
	{
		ulTimeOut = 0;
	}
	_ST_DATA_LOGIN(const _ST_DATA_LOGIN& obj)
	{
		strSend = obj.strSend;
		strWait = obj.strWait;
		ulTimeOut = obj.ulTimeOut;
	}
	_ST_DATA_LOGIN& operator = (const _ST_DATA_LOGIN& obj)
	{
		if(this != &obj)
		{			
			strSend = obj.strSend;
			strWait = obj.strWait;
			ulTimeOut = obj.ulTimeOut;
		}
		return *this;
	}

}ST_DATA_LOGIN;

typedef std::vector<ST_DATA_LOGIN>	VEC_DATA_LOGIN;

//ͨѶģ��ӿ���
class __declspec(dllexport)  ICommunication  
{
public:
	virtual ~ICommunication(){}
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//����ͨ�����ͽ�������
	//����˵��:
	//eCommutype[IN]ͨ������
	//pData[IN]������������Ҫ�����ݽṹ���ָ��(����ͨ������ȷ��)
	//strError[OUT]������Ϣ����
	//����˵��:
	//�ɹ��������ӷ���true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnOpen(const ECOMMTYPE eCommutype, void* pData, std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//�ر�����
	//����˵��:
	//strError[OUT]������Ϣ����
	//����˵��:
	//�ɹ��ر����ӷ���true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnClose(std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//ע��ص�����
	//����˵��:
	//pFnCallBack[IN]�ص�����ָ��
	//����˵��:
	//��
	//////////////////////////////////////////////////////////////////////////
	virtual void fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2) = 0;
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//�����ַ�
	//����˵��:
	//strSend[IN]�����͵��ַ���
	//ulTime[IN]��ʱʱ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//����ڳ�ʱʱ���ڷ��ͳɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnSendString(const std::string& strSend,const unsigned long ulTime,bool bSendByChar,std::string &strError)= 0;
	virtual bool fnSendByte(unsigned char* strSend, const unsigned long ulDataLen, const unsigned long ulTime, std::string &strError)=0;
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
	virtual bool fnSendData(const  char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError)= 0;
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//���ն�ʱʱ���������յ������ݲ�����
	//����˵��:
	//strRecv[OUT]���յ��ַ���
	//ulTime[IN]��ʱʱ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//�����ʱ���ڽ��յ����ݷ���1,��ʱ��û�н��յ����ݷ���0,��ͣ�򷵻�2
	//////////////////////////////////////////////////////////////////////////
	virtual int fnReceiveString(std::string &strRecv,const unsigned long ulTime,std::string &strError) = 0;
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
	//��ָ����ʱ���ڽ��շ�����Ϣ������Ϣ�в���ָ�����͵��ַ���
	//����˵��:
	//strFind[IN]�����ҵ��ַ���,�ɽ���������ʽ
	//vstrResult[OUT]���ҽ������
	//ulTimeOut[IN]��ʱʱ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//���ҳɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnFindString(const std::string strFind,std::vector<std::string> &vstrResult,unsigned long ulTimeOut,std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//��¼������
	//����˵��:
	//pData[IN]��¼����Ҫ�����ݽṹ(VEC_DATA_LOGIN)ָ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//��¼�ɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnLogin(void* pData,bool bSendByChar,std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//����������
	//����˵��:
	//strSend[IN]��Ҫ���͵�����
	//strTXRate[OUT]��������
	//strRXRate[OUT]��������
	//����˵��:
	//��¼�ɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnGetThoughputValue(const std::string strSend,std::string &strTXRate,std::string &strRXRate) = 0;


};
//ͨѶģ��ʵ����
class __declspec(dllexport)  CCommunication : public ICommunication
{
public:
	CCommunication();
	virtual ~CCommunication();
private:
	/////////////////////��Ա����////////////////////////////////////////////
	ICommuFactory *          m_pCommuFactory;
	ICommuBase *             m_pCommuBase;
	ECOMMTYPE                m_eCommutype;
	bool					 m_bStop;
	long					 m_hMutex;
public:
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//����ͨ�����ͽ�������
	//����˵��:
	//eCommutype[IN]ͨ������
	//pData[IN]������������Ҫ�����ݽṹ���ָ��(����ͨ������ȷ��)
	//strError[OUT]������Ϣ����
	//����˵��:
	//�ɹ��������ӷ���true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnOpen(const ECOMMTYPE eCommutype, void* pData, std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//�ر�����
	//����˵��:
	//strError[OUT]������Ϣ����
	//����˵��:
	//�ɹ��ر����ӷ���true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnClose(std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//ע��ص�����
	//����˵��:
	//pFnCallBack[IN]�ص�����ָ��
	//����˵��:
	//��
	//////////////////////////////////////////////////////////////////////////
	virtual void fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2);
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//�����ַ�
	//����˵��:
	//strSend[IN]�����͵��ַ���
	//ulTime[IN]��ʱʱ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//����ڳ�ʱʱ���ڷ��ͳɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnSendString(const std::string& strSend,const unsigned long ulTime,bool bSendByChar,std::string &strError);
	virtual bool fnSendByte(unsigned char* strSend, const unsigned long ulDataLen, const unsigned long ulTime, std::string &strError);
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
	//�����ʱ���ڽ��յ����ݷ���1,��ʱ��û�н��յ����ݷ���0,��ͣ�򷵻�2
	//////////////////////////////////////////////////////////////////////////
	virtual int fnReceiveString(std::string &strRecv,const unsigned long ulTime,std::string &strError);
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
	//��ָ����ʱ���ڽ��շ�����Ϣ������Ϣ�в���ָ�����͵��ַ���
	//����˵��:
	//strFind[IN]�����ҵ��ַ���,�ɽ���������ʽ
	//vstrResult[OUT]���ҽ������
	//ulTimeOut[IN]��ʱʱ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//���ҳɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnFindString(const std::string strFind,std::vector<std::string> &vstrResult,unsigned long ulTimeOut,std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//��¼������
	//����˵��:
	//pData[IN]��¼����Ҫ�����ݽṹ(VEC_DATA_LOGIN)ָ��
	//strError[OUT]������Ϣ����
	//����˵��:
	//��¼�ɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnLogin(void* pData,bool bSendByChar,std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//����˵��:
	//����������
	//����˵��:
	//strSend[IN]��Ҫ���͵�����
	//strTXRate[OUT]��������
	//strRXRate[OUT]��������
	//����˵��:
	//��¼�ɹ�����true,���򷵻�false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnGetThoughputValue(const std::string strSend,std::string &strTXRate,std::string &strRXRate);
};

