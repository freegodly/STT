#pragma once
#include "CommuBase.h"
#include <windows.h>
//�ܵ�ͨѶ 
class CCommuPipe : public ICommuBase
{
public:
	CCommuPipe();
	virtual ~CCommuPipe();
public:
	//���ݽ����߳�
	static long fnThreadPipeRecv(unsigned long ulParam);
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

	struct ST_CMDWND_INFO 
	{
		HWND    hwnd;
		DWORD dwProcessId;
		HANDLE hProcess;
	};
private:

	HANDLE                 m_hNewstdin;
	HANDLE                 m_hNewstdout;
	HANDLE                 m_hReadStdout;
	HANDLE                 m_hWriteStdin;
	//�������ݵ��߳�
	HANDLE					m_hThreadRecvBuf;
	//ͨѶ״̬
	bool					m_bCommstatus;
	ST_CMDWND_INFO			m_stCmdwndInfo;
	//���������Ϣ
	PROCESS_INFORMATION		m_piCmdProcess;
	//���ݽ��ջ���
	std::string   m_strBuffer;

	//���ݽ��ջ���
	HANDLE					m_hPipeBufMutex;
};
//////////////////////////////////////////////////////////////////////////
//����˵��
//���ݵ�ǰ����ID,�ҳ���Ӧ���ӽ���ID,������Ӧ�Ľ��̹ر�
//����˵��:
//dwCurProID[IN]��ǰ����ID
//����˵��:
//�ɹ�����TRUE,ʧ�ܷ���FALSE
////////////////////////////////////////////////////////////////////////////
BOOL fnCloseSubProcess(DWORD dwCurProID); 

