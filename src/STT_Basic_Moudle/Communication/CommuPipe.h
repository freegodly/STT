#pragma once
#include "CommuBase.h"
#include <windows.h>
//管道通讯 
class CCommuPipe : public ICommuBase
{
public:
	CCommuPipe();
	virtual ~CCommuPipe();
public:
	//数据接收线程
	static long fnThreadPipeRecv(unsigned long ulParam);
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//打开连接
	//参数说明:
	//pData[IN]打开连接所需要的数据结构指针
	//strError[OUT]错误信息返回
	//返回说明:
	//打开连接成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnOpen(void *pData,std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//关闭连接
	//参数说明:
	//strError[OUT]错误信息返回
	//返回说明:
	//关闭连接成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnClose(std::string &strError);
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
	virtual bool fnSendData(const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError);
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
	virtual bool fnReceiveString(std::string &strRecv,std::string &strError);
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
	//接收数据的线程
	HANDLE					m_hThreadRecvBuf;
	//通讯状态
	bool					m_bCommstatus;
	ST_CMDWND_INFO			m_stCmdwndInfo;
	//进程相关信息
	PROCESS_INFORMATION		m_piCmdProcess;
	//数据接收缓冲
	std::string   m_strBuffer;

	//数据接收互斥
	HANDLE					m_hPipeBufMutex;
};
//////////////////////////////////////////////////////////////////////////
//函数说明
//根据当前进程ID,找出相应的子进程ID,并把相应的进程关闭
//参数说明:
//dwCurProID[IN]当前进程ID
//返回说明:
//成功返回TRUE,失败返回FALSE
////////////////////////////////////////////////////////////////////////////
BOOL fnCloseSubProcess(DWORD dwCurProID); 

