
#include "CommuPipe.h"


// CommuPipe.cpp: implementation of the CCommuPipe class.
//
//////////////////////////////////////////////////////////////////////

#include "CommuPipe.h"
#include <stdio.h>
#include <exception>
#include <tlhelp32.h>
#include <iostream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  CLOSE_HANDLE(handle)      if(0 != handle)\
{CloseHandle(handle);\
handle = 0;}
//////////////////////////////////////////////////////////////////////////
//����˵��:
//ö��ϵͳ��ǰ���еĴ���
//����˵��:
//wnd[IN]���ھ��
//lParam[IN/OUT]ST_CMDWND_INFOָ��
//
static bool  fnEnumWindowsfunc(HWND hWnd, LPARAM lParam)
{
    DWORD pid;
	CCommuPipe::ST_CMDWND_INFO * psCmdInfo = (CCommuPipe::ST_CMDWND_INFO*)lParam;
    GetWindowThreadProcessId(hWnd, &pid);
    if(pid == psCmdInfo->dwProcessId)// || (IsWindowVisible(wnd)) || !(IsWindowEnabled(wnd));
	{
		psCmdInfo->hwnd = hWnd;
		return false;
	}
    return true;
}

//////////////////////////////////////////////////////////////////////////
//����˵��
//���ݵ�ǰ����ID,�ҳ���Ӧ���ӽ���ID,������Ӧ�Ľ��̹ر�
//����˵��:
//dwCurProID[IN]��ǰ����ID
//����˵��:
//�ɹ�����TRUE,ʧ�ܷ���FALSE
////////////////////////////////////////////////////////////////////////////
BOOL fnCloseSubProcess(DWORD dwCurProID)
{
	try
	{		
		PROCESSENTRY32  pe={sizeof pe};
		HANDLE hModuleSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,dwCurProID);//create system snap handle
		if(INVALID_HANDLE_VALUE==hModuleSnap)
		{
			return FALSE;//ʧ��
		}
		if(Process32First(hModuleSnap,&pe))
		{
			do{
				//���������ID�ǵ�ǰ����ID,����Ҫ�ر�
				if (dwCurProID == pe.th32ParentProcessID)
				{
					HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,pe.th32ProcessID);
					if (0 != hProcess)
					{
						TerminateProcess(hProcess,-1);
						CloseHandle(hProcess);
					}
				}
				
			}
			while(Process32Next(hModuleSnap,&pe));
			//Sleep(5000);
		}//if(Process32First(hModuleSnap,&pe))
		CloseHandle(hModuleSnap);
	}	
	catch(...)
	{
		
	}
	return TRUE;//�ɹ�
}


CCommuPipe::CCommuPipe()
{
	m_hPipeBufMutex = CreateMutex(0,false,0);
	m_pfnCommCallBack = 0;
	
	m_hNewstdin = 0;
	m_hNewstdout = 0;
	m_hReadStdout = 0;
	m_hWriteStdin = 0;

	m_hThreadRecvBuf = 0;
	m_bCommstatus = false;
	m_strBuffer = "";

    //���������Ϣ
    //PROCESS_INFORMATION		m_piCmdProcess;
    ZeroMemory( &m_piCmdProcess, sizeof(m_piCmdProcess) );
}

CCommuPipe::~CCommuPipe()
{
	CLOSE_HANDLE(m_hNewstdin);
	CLOSE_HANDLE(m_hNewstdout);
	CLOSE_HANDLE(m_hReadStdout);
	CLOSE_HANDLE(m_hWriteStdin);
	CLOSE_HANDLE(m_hPipeBufMutex);
	CLOSE_HANDLE(m_hThreadRecvBuf);
}


// wchar_t to string
void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
wchar_t * wText = wchar;
DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);// WideCharToMultiByte������
char *psText; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
psText = new char[dwNum];
WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);// WideCharToMultiByte���ٴ�����
szDst = psText;// std::string��ֵ
delete []psText;// psText�����
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
bool CCommuPipe::fnOpen(void *pData,std::string &strError)
{
	
	unsigned long lWrite;
	if (0 != m_hWriteStdin)
	{
		return true;
	}
	SECURITY_DESCRIPTOR  sd;
	SECURITY_ATTRIBUTES  sa;
	STARTUPINFO          si;
    ZeroMemory( &si, sizeof(si) );
	//��ȡ��ǰϵͳ�汾
	OSVERSIONINFO osv;
    osv.dwOSVersionInfoSize = sizeof(osv);
    GetVersionEx(&osv);
    if(VER_PLATFORM_WIN32_NT == osv.dwPlatformId)
    {
        /* initialize security descriptor (Windows NT) */
        InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
        SetSecurityDescriptorDacl(&sd, true, NULL, false);
        sa.lpSecurityDescriptor = &sd;
    }
    else
    {
  	    sa.lpSecurityDescriptor = NULL;
    }
  
  
    /* allow inheritable handles */
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = true;

    /* create stdin pipe */
    if(!CreatePipe(&m_hNewstdin, &m_hWriteStdin, &sa, 0))
    {
        strError = "[CMD] Create stdin Pipe Fail.";
        return false;
    }

    /* create stdout pipe */
    if(!CreatePipe(&m_hReadStdout, &m_hNewstdout, &sa, 0))  //
    {
        strError = "[CMD] Create stdout Pipe Fail.";
        CLOSE_HANDLE(m_hNewstdin);
        CLOSE_HANDLE(m_hWriteStdin);
        return false;
    }

    /* set startupinfo for the spawned process */
    GetStartupInfo(&si);

    /*
        The dwFlags member tells CreateProcess how to make the process.
        STARTF_USESTDHANDLES validates the hStd* members. STARTF_USESHOWWINDOW
        validates the wShowWindow member.
    */
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;//SW_SHOWMINIMIZED;

    /* set the new handles for the child process */
    si.hStdOutput = m_hNewstdout;
    si.hStdError  = m_hNewstdout;
    si.hStdInput  = m_hNewstdin;

    /* Spawn the child process */
	//��ȡcmd.exe����·��
	WCHAR  chPathBuf[MAX_PATH] = {0};
    WCHAR chPathName[MAX_PATH] = L"\\cmd.exe";
	GetSystemDirectory(chPathBuf,MAX_PATH);
    //swprintf(chPathName,TEXT("%s\\cmd.exe"),chPathBuf);//(LPWSTR)chPathName
    wcscat(chPathBuf,chPathName);


    if(!CreateProcess(NULL, chPathBuf, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &m_piCmdProcess))
    {
        strError = "[CMD] CreateProcess FAIL.";
//        printf( "CreateProcess failed (%d)\n", GetLastError() );
//        std::string err;
//        Wchar_tToString(err,chPathBuf);
//        std::cout<<err<<std::endl;

        CLOSE_HANDLE(m_hNewstdin);
        CLOSE_HANDLE(m_hNewstdout);
        CLOSE_HANDLE(m_hReadStdout);
        CLOSE_HANDLE(m_hWriteStdin);
        return false;
    }

    /* Wait for cmd.exe up */

    /* Find HWND of cmd.exe */
	//ö�ٻ�ȡ��ǰcmd.exe������ӵ�еĴ��ھ��
	m_stCmdwndInfo.dwProcessId = m_piCmdProcess.dwProcessId;
	m_stCmdwndInfo.hProcess = m_piCmdProcess.hProcess;
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		Sleep(100);
		if (EnumWindows((WNDENUMPROC)fnEnumWindowsfunc,(long)&m_stCmdwndInfo))
		{
			break;
		}
	}
    if(0 == m_stCmdwndInfo.hwnd)
    {
        strError = ("[CMD] Cann't Find HWND.");
		
		WriteFile(m_hWriteStdin, "exit\r\n", 6, &lWrite, NULL);
        CLOSE_HANDLE(m_hNewstdin);
        CLOSE_HANDLE(m_hNewstdout);
        CLOSE_HANDLE(m_hReadStdout);
        CLOSE_HANDLE(m_hWriteStdin);
        return false;
    }
	m_bCommstatus = true;
	//�����������ݵ��߳�
	m_hThreadRecvBuf = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fnThreadPipeRecv, this, 0, 0);
	if (0 == m_hThreadRecvBuf)
	{
		
		WriteFile(m_hWriteStdin, "exit\r\n", 6, &lWrite, NULL);
        strError = ("[CMD] Create receive data thread fail.");
        CLOSE_HANDLE(m_hNewstdin);
        CLOSE_HANDLE(m_hNewstdout);
        CLOSE_HANDLE(m_hReadStdout);
        CLOSE_HANDLE(m_hWriteStdin);
		m_bCommstatus = false;
        return false;
	}   
	//iteFile(m_hWriteStdin, "cd dll\r\n", 8, &lWrite, NULL);
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
bool CCommuPipe::fnClose(std::string &strError)
{
	try
	{
		//����״̬false,���˳�ͨѶģʽ
		m_bCommstatus = false;
		//�ȴ������߳��˳�
		WaitForSingleObject(m_hThreadRecvBuf,INFINITE);
		if(NULL != m_stCmdwndInfo.hwnd)
		{
			unsigned long lWrite = 0;
			WriteFile(m_hWriteStdin, "exit\r\n", 6, &lWrite, NULL);
			//::SendMessage(m_stCmdwndInfo.hwnd,WM_CLOSE,0,0);
			DWORD dwExitcode = 0;
			//�ر��ӽ���
			fnCloseSubProcess(m_stCmdwndInfo.dwProcessId);
			GetExitCodeProcess(m_stCmdwndInfo.hProcess,&dwExitcode);
			TerminateProcess(m_stCmdwndInfo.hProcess,dwExitcode);
			CloseHandle(m_stCmdwndInfo.hProcess); 
			m_stCmdwndInfo.hwnd = NULL;
		}
		CLOSE_HANDLE(m_hThreadRecvBuf);
		CLOSE_HANDLE(m_hNewstdin);
		CLOSE_HANDLE(m_hNewstdout);
		CLOSE_HANDLE(m_hReadStdout);
		CLOSE_HANDLE(m_hWriteStdin);
        CLOSE_HANDLE(m_piCmdProcess.hProcess);
		CLOSE_HANDLE(m_piCmdProcess.hThread);
	}
	catch (...)
	{
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
bool CCommuPipe::fnReceiveString(std::string &strRecv,std::string &strError)
{

	if (0 < m_strBuffer.length())
	{
		WaitForSingleObject(m_hPipeBufMutex,INFINITE);
		strRecv = m_strBuffer;
		m_strBuffer = "";
		ReleaseMutex(m_hPipeBufMutex);
		return true;		
	}
	return false;
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
bool CCommuPipe::fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError)
{
    return true;
}

//���ݽ����߳�
long CCommuPipe::fnThreadPipeRecv(unsigned long ulParam)
{
	CCommuPipe *pThis = (CCommuPipe*)ulParam;
	const long lBufLen = 4096;
	//���յ�緳���
	char  buffer[lBufLen] = {0};
	//һ�ζ������ַ�����
	unsigned long iRead = 0;
	//�ܶ������ַ�����
	unsigned long iReadCount = 0;
	//�������ַ�����
	unsigned long iTotalBuf = 0;

	while (pThis->m_bCommstatus)
	{
		iTotalBuf = 0;
		try
		{
			//��黺������û�����ݴ���
			int bResult = PeekNamedPipe(pThis->m_hReadStdout, buffer, 0, 0, &iTotalBuf, NULL);
			while(0 < iTotalBuf)
			{
				WaitForSingleObject(pThis->m_hPipeBufMutex,INFINITE);
				//��ȡ����
				bResult = ReadFile(pThis->m_hReadStdout,buffer,(lBufLen-1)>iTotalBuf?iTotalBuf:(lBufLen-1),&iRead,0);
				buffer[iRead] = '\0';
				pThis->m_strBuffer += buffer;
				ReleaseMutex(pThis->m_hPipeBufMutex);
				iTotalBuf -= iRead;
				//���ûص�����
				if (0 != pThis->m_pfnCommCallBack)
				{
					pThis->m_pfnCommCallBack((unsigned long)buffer,pThis->m_ulCallBackParam2);
				}
			}
		}
		catch(...)
		{

		}
		Sleep(500);
	}
	return 0;
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
bool CCommuPipe::fnSendData( const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError )
{	//���ûص�����
	if (0 != m_pfnCommCallBack)
	{
		m_pfnCommCallBack((unsigned long)pbData,m_ulCallBackParam2);
	}
	unsigned long  lWrite = 0;
    if(WriteFile(m_hWriteStdin, pbData, ulDataLen, &lWrite, NULL))
	{
		return true;
	}
	strError = "����ʧ��";
    return false;
}
