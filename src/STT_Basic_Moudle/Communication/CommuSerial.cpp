
// CommuSerial.cpp: implementation of the CCommuSerial class.
//
//////////////////////////////////////////////////////////////////////

#include "CommuSerial.h"
#include <stdio.h>
#include <windows.h>
#include <map>
#pragma comment(lib, "Pcomm.lib")
#include "Pcomm.h"
#include <exception>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
struct ST_CALLBACK_DATA
{
	std::string * pStrData;
	unsigned long ulData;
	fnCommCallBack   pCallBack;
	ST_CALLBACK_DATA()
	{
		pStrData = 0;
		ulData = 0;
		pCallBack = 0;
	}
	ST_CALLBACK_DATA(const ST_CALLBACK_DATA & obj)
	{
		pStrData = obj.pStrData;
		ulData = obj.ulData;
		pCallBack = obj.pCallBack;
	}
	ST_CALLBACK_DATA & operator = (const ST_CALLBACK_DATA & obj)
	{
		if (this == &obj)
		{
			return *this;
		}
		pStrData = obj.pStrData;
		ulData = obj.ulData;
		pCallBack = obj.pCallBack;
		return *this;
	}
};
//绶冲数据
typedef std::map<int,ST_CALLBACK_DATA>  MAP_RECEIVE_BUF;
MAP_RECEIVE_BUF		g_mapRecvBuf;
std::string					g_burrer;
//串口接收数据互斥量
HANDLE	g_hSerialBufMutex = CreateMutex(0,false,0);
//用于互斥使用计数
unsigned long    g_ulMutexCount = 0;
//串口数据回调函数,用于接收数据
void CALLBACK  fnSerialCallBack(int Port)
{

	try
	{
		char buffer[4096] = {0};


		::Sleep(200);
		int iLen = sio_read(Port,buffer,4096);
		buffer[iLen] = '\0';

		if (0 == strlen(buffer))
		{
			return;
		}
		MAP_RECEIVE_BUF::iterator iter = g_mapRecvBuf.find(Port);
		if (iter != g_mapRecvBuf.end())
		{
			WaitForSingleObject(g_hSerialBufMutex,INFINITE);
			std::string *pRecv = iter->second.pStrData;

			for(int i= 0; i< iLen; i++)
			{
				if(buffer[i] == '\0')
				{
					unsigned char* b1 = new  unsigned char[1];
					b1[0] = 0xff;
					(*pRecv) += b1[0];
				}
				else
				{
					(*pRecv) += buffer[i];
				}
			}
			//			(*pRecv) += buffer;

			//调用回调函数
			if (0 != iter->second.pCallBack)
			{
				iter->second.pCallBack((unsigned long)buffer,iter->second.ulData);
			}
			ReleaseMutex(g_hSerialBufMutex);
		}

	}
	catch(...)
	{
	}
}
CCommuSerial::CCommuSerial()
{
	if (0 == g_hSerialBufMutex)
	{
		g_hSerialBufMutex = CreateMutex(0,false,0);
	}
	g_ulMutexCount += 1;
	m_sComPort = -1;
}

CCommuSerial::~CCommuSerial()
{
	g_ulMutexCount -= 1;
	if (0 == g_ulMutexCount)
	{
		CloseHandle(g_hSerialBufMutex);
		g_hSerialBufMutex = 0;
	}
	//    g_mapRecvBuf.erase(m_sComPort);
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
bool CCommuSerial::fnOpen(void *pData,std::string &strError)
{
	//
	if (-1 != m_sComPort)
	{
		return true;
	}
	char buffer[1024] = {0};
	//指针转换
	ST_DATA_SERIAL*  pTemp =(ST_DATA_SERIAL*)pData;
	if (0 == pTemp)
	{
		strError = "错误的数据结构";
		return false;
	}
	//打开串口
    int iResult=SIO_FAIL;
    try
    {
        iResult = sio_open(pTemp->usComNum);
    }
    catch (std::exception& e)
    {
    }
	if (SIO_OK != iResult)
	{
		sprintf(buffer,"Open COM%d  Port Failed!\r\n",pTemp->usComNum);
		strError = buffer;
		sio_close(pTemp->usComNum);
		return false;
	}
	//{{设置波特率
	long    mode;
	long    Hw;
	long    Sw;	

	mode = P_NONE | BIT_8 | STOP_1;
	Hw = 0 ;    /* No H/W Flow control. */
	Sw = 0 ;    /* No S/W Flow Control. */

	iResult = sio_ioctl(pTemp->usComNum, fnChangeBaud(pTemp->ulBaudRate), mode);
	if (SIO_OK != iResult)
	{
		sprintf(buffer,"COM%d set Error",pTemp->usComNum);
		strError = buffer;
		sio_close(pTemp->usComNum);
		return false;
	}
	iResult = sio_flowctrl(pTemp->usComNum, Hw | Sw);
	if (SIO_OK != iResult)
	{
		sio_close(pTemp->usComNum);
		return false;
	}
	//}}设置波特率
	//        ::Sleep(1000);
	iResult = sio_cnt_irq(pTemp->usComNum,fnSerialCallBack,1);
	if (SIO_OK != iResult)
	{
		sio_close(pTemp->usComNum);
		return false;
	}
	m_sComPort = pTemp->usComNum;
	//设置接收绶冲区
	ST_CALLBACK_DATA  stCallData;
	stCallData.pStrData = &m_strBuffer;
	stCallData.ulData = m_ulCallBackParam2;
	g_mapRecvBuf.insert(MAP_RECEIVE_BUF::value_type(m_sComPort,stCallData));
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
bool CCommuSerial::fnClose(std::string &strError)
{
	if (-1 != m_sComPort)
	{
		int iResult = -1;
		iResult = sio_close(m_sComPort);
		if (SIO_OK != iResult)
		{
			strError = "关闭串口失败";
			return false;
		}
		//移除绶冲区
		WaitForSingleObject(g_hSerialBufMutex,INFINITE);
		g_mapRecvBuf.erase(m_sComPort);
		ReleaseMutex(g_hSerialBufMutex);
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
bool CCommuSerial::fnReceiveString(std::string &strRecv,std::string &strError)
{
	if (0 < m_strBuffer.length())
	{
		WaitForSingleObject(g_hSerialBufMutex,INFINITE);
		strRecv = m_strBuffer;
		m_strBuffer = "";
		ReleaseMutex(g_hSerialBufMutex);
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
bool CCommuSerial::fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError)
{
	return true;
}


int CCommuSerial::fnChangeBaud( unsigned long ulBaud )
{
	switch(ulBaud)
	{
	case 9600:
		return  B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	default:
		return B9600;
	}
}

std::string CCommuSerial::fnGetErrorMsg( int iErrorCode )
{
	std::string strMessage = "";
	switch(iErrorCode)
	{
	case -1:
		strMessage = "no such port or port not opened";break;
	case -2:
		strMessage = "can't control the board";break;
	case -4:
		strMessage = "no data to read or no buffer to write";break;
	case -5:
		strMessage = "no such port or port has be opened";break;
	case -6:
		strMessage = "RTS can't set because H/W flowctrl";break;
	case -7:
		strMessage = "bad parameter";break;
	case -8:
		strMessage = "call win32 function fail, please call";break;
	case -9:
		strMessage = "Does not support this board";break;
	case -10:
		strMessage = "PComm function run result fail";break;
	case -11:
		strMessage = "write has blocked, and user abort write";break;
	case -12:
		strMessage = "write timeoue has happened";break;
	default:break;
	}
	return strMessage;
}

void CCommuSerial::fnRegCallBack( const fnCommCallBack pFnCallBack,unsigned long ulParam2 )
{
	ICommuBase::fnRegCallBack(pFnCallBack,ulParam2);


	//设置接收绶冲区
	WaitForSingleObject(g_hSerialBufMutex,INFINITE);
	ST_CALLBACK_DATA  stCallData;
	stCallData.pStrData = &m_strBuffer;
	stCallData.ulData = m_ulCallBackParam2;
	stCallData.pCallBack = m_pfnCommCallBack;
	g_mapRecvBuf.erase(m_sComPort);
	g_mapRecvBuf.insert(MAP_RECEIVE_BUF::value_type(m_sComPort,stCallData));
	ReleaseMutex(g_hSerialBufMutex);
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
bool CCommuSerial::fnSendData( const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError )
{
	//调用回调函数
	if (0 != m_pfnCommCallBack)
	{
		m_pfnCommCallBack((unsigned long)pbData,m_ulCallBackParam2);
	}
	if (-1 != m_sComPort)
	{
		int iSend = sio_write(m_sComPort,(char*)pbData,ulDataLen);
		if(0 >= iSend)
		{
			strError = "发送数据(";
			strError += pbData ;
			strError += ")失败";
			return false;
		}
		return true;
	}
	return false;
}
