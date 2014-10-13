
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
//函数说明:
//枚举系统当前所有的窗口
//参数说明:
//wnd[IN]窗口句柄
//lParam[IN/OUT]ST_CMDWND_INFO指针
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
//函数说明
//根据当前进程ID,找出相应的子进程ID,并把相应的进程关闭
//参数说明:
//dwCurProID[IN]当前进程ID
//返回说明:
//成功返回TRUE,失败返回FALSE
////////////////////////////////////////////////////////////////////////////
BOOL fnCloseSubProcess(DWORD dwCurProID)
{
	try
	{		
		PROCESSENTRY32  pe={sizeof pe};
		HANDLE hModuleSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,dwCurProID);//create system snap handle
		if(INVALID_HANDLE_VALUE==hModuleSnap)
		{
			return FALSE;//失败
		}
		if(Process32First(hModuleSnap,&pe))
		{
			do{
				//如果父进程ID是当前进程ID,则需要关闭
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
	return TRUE;//成功
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

    //进程相关信息
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
DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);// WideCharToMultiByte的运用
char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
psText = new char[dwNum];
WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);// WideCharToMultiByte的再次运用
szDst = psText;// std::string赋值
delete []psText;// psText的清除
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
	//获取当前系统版本
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
	//获取cmd.exe绝对路径
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
	//枚举获取当前cmd.exe进程所拥有的窗口句柄
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
	//创建接收数据的线程
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
//函数说明:
//关闭连接
//参数说明:
//strError[OUT]错误信息返回
//返回说明:
//关闭连接成功返回true,否则返回false
//////////////////////////////////////////////////////////////////////////
bool CCommuPipe::fnClose(std::string &strError)
{
	try
	{
		//设置状态false,即退出通讯模式
		m_bCommstatus = false;
		//等待接收线程退出
		WaitForSingleObject(m_hThreadRecvBuf,INFINITE);
		if(NULL != m_stCmdwndInfo.hwnd)
		{
			unsigned long lWrite = 0;
			WriteFile(m_hWriteStdin, "exit\r\n", 6, &lWrite, NULL);
			//::SendMessage(m_stCmdwndInfo.hwnd,WM_CLOSE,0,0);
			DWORD dwExitcode = 0;
			//关闭子进程
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
//函数说明:
//接收定时时间内所有收到的数据并返回
//参数说明:
//strRecv[OUT]接收的字符串
//ulTime[IN]定时时间
//strError[OUT]错误信息返回
//返回说明:
//如果在时间内接收到数据返回true,否则返回false
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
//函数说明:
//发送文件
//参数说明:
//strFilePath[IN]文件路径
//eProt[IN]发送协议
//strError[OUT]错误信息返回
//返回说明:
//发送成功返回true,否则返回false
//////////////////////////////////////////////////////////////////////////
bool CCommuPipe::fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError)
{
    return true;
}

//数据接收线程
long CCommuPipe::fnThreadPipeRecv(unsigned long ulParam)
{
	CCommuPipe *pThis = (CCommuPipe*)ulParam;
	const long lBufLen = 4096;
	//接收的绶冲区
	char  buffer[lBufLen] = {0};
	//一次读到的字符长度
	unsigned long iRead = 0;
	//总读到的字符长度
	unsigned long iReadCount = 0;
	//缓冲区字符长度
	unsigned long iTotalBuf = 0;

	while (pThis->m_bCommstatus)
	{
		iTotalBuf = 0;
		try
		{
			//检查缓冲区有没有数据存在
			int bResult = PeekNamedPipe(pThis->m_hReadStdout, buffer, 0, 0, &iTotalBuf, NULL);
			while(0 < iTotalBuf)
			{
				WaitForSingleObject(pThis->m_hPipeBufMutex,INFINITE);
				//读取数据
				bResult = ReadFile(pThis->m_hReadStdout,buffer,(lBufLen-1)>iTotalBuf?iTotalBuf:(lBufLen-1),&iRead,0);
				buffer[iRead] = '\0';
				pThis->m_strBuffer += buffer;
				ReleaseMutex(pThis->m_hPipeBufMutex);
				iTotalBuf -= iRead;
				//调用回调函数
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
bool CCommuPipe::fnSendData( const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError )
{	//调用回调函数
	if (0 != m_pfnCommCallBack)
	{
		m_pfnCommCallBack((unsigned long)pbData,m_ulCallBackParam2);
	}
	unsigned long  lWrite = 0;
    if(WriteFile(m_hWriteStdin, pbData, ulDataLen, &lWrite, NULL))
	{
		return true;
	}
	strError = "发送失败";
    return false;
}
