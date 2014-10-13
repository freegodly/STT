#pragma once
#include <string>
#include <VECTOR>

class ICommuFactory;
class ICommuBase;
//通信类型
 typedef enum _ECOMMTYPE
{ 
	//无效通讯类型
	eCTypeUnknow = 0,
	//管道通讯类型,主要用于连接cmd.exe
	eCTypePipe = 1,
	//telnet通讯类型,由SOCKET实现
	eCTypeTelnet,
	//串口通讯类型
	eCTypeSerial
}ECOMMTYPE;

//发送文件时所用的发送协议
typedef enum _ESENDFILEPROT
{
	//无效协议
	eProtNon = 0,
	//一般协议
	eProtNormal = 1
}ESENDFILEPROT;

//通信模块回调函数指针,主要用于通讯过程中收发数据时调用
//uiParam1:字符串指针(std::string),uiParam2:用户自定义的数据
typedef void (*fnCommCallBack)(unsigned long uiParam1,unsigned long uiParam2);

//管道通信所需要的数据结构
typedef struct _ST_DATA_PIPE
{
	std::string strPipeName;
}ST_DATA_PIPE;

//串口通信所需要的数据结构
typedef struct _ST_DATA_SERIAL
{
	//串口号
	unsigned short usComNum;
	//波特率
	unsigned long  ulBaudRate;
}ST_DATA_SERIAL;
//telnet通信所需要的数据结构
typedef struct _ST_DATA_TELNET
{
	//本地IP
	std::string strLocalIP;
	//目标IP
	std::string strTargetIP;
	//目标端口
	unsigned short usPort;
}ST_DATA_TELNET;

//登录时所使用的结构体
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

//通讯模块接口类
class __declspec(dllexport)  ICommunication  
{
public:
	virtual ~ICommunication(){}
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//根据通信类型建立连接
	//参数说明:
	//eCommutype[IN]通信类型
	//pData[IN]建立连接所需要的数据结构体的指针(根据通信类型确定)
	//strError[OUT]错误信息返回
	//返回说明:
	//成功建立连接返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnOpen(const ECOMMTYPE eCommutype, void* pData, std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//关闭连接
	//参数说明:
	//strError[OUT]错误信息返回
	//返回说明:
	//成功关闭连接返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnClose(std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//注册回调函数
	//参数说明:
	//pFnCallBack[IN]回调函数指针
	//返回说明:
	//无
	//////////////////////////////////////////////////////////////////////////
	virtual void fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2) = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//发送字符
	//参数说明:
	//strSend[IN]被发送的字符串
	//ulTime[IN]超时时间
	//strError[OUT]错误信息返回
	//返回说明:
	//如果在超时时间内发送成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnSendString(const std::string& strSend,const unsigned long ulTime,bool bSendByChar,std::string &strError)= 0;
	virtual bool fnSendByte(unsigned char* strSend, const unsigned long ulDataLen, const unsigned long ulTime, std::string &strError)=0;
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
	virtual bool fnSendData(const  char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError)= 0;
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//接收定时时间内所有收到的数据并返回
	//参数说明:
	//strRecv[OUT]接收的字符串
	//ulTime[IN]定时时间
	//strError[OUT]错误信息返回
	//返回说明:
	//如果在时间内接收到数据返回1,超时还没有接收到数据返回0,暂停则返回2
	//////////////////////////////////////////////////////////////////////////
	virtual int fnReceiveString(std::string &strRecv,const unsigned long ulTime,std::string &strError) = 0;
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
	virtual bool fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//在指定的时间内接收返回信息并从信息中查找指定类型的字符串
	//参数说明:
	//strFind[IN]被查找的字符串,可接受正则表达式
	//vstrResult[OUT]查找结果返回
	//ulTimeOut[IN]超时时间
	//strError[OUT]错误信息返回
	//返回说明:
	//查找成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnFindString(const std::string strFind,std::vector<std::string> &vstrResult,unsigned long ulTimeOut,std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//登录服务器
	//参数说明:
	//pData[IN]登录所需要的数据结构(VEC_DATA_LOGIN)指针
	//strError[OUT]错误信息返回
	//返回说明:
	//登录成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnLogin(void* pData,bool bSendByChar,std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//吞吐量测试
	//参数说明:
	//strSend[IN]需要发送的命令
	//strTXRate[OUT]发送速率
	//strRXRate[OUT]接收速率
	//返回说明:
	//登录成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnGetThoughputValue(const std::string strSend,std::string &strTXRate,std::string &strRXRate) = 0;


};
//通讯模块实现类
class __declspec(dllexport)  CCommunication : public ICommunication
{
public:
	CCommunication();
	virtual ~CCommunication();
private:
	/////////////////////成员变量////////////////////////////////////////////
	ICommuFactory *          m_pCommuFactory;
	ICommuBase *             m_pCommuBase;
	ECOMMTYPE                m_eCommutype;
	bool					 m_bStop;
	long					 m_hMutex;
public:
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//根据通信类型建立连接
	//参数说明:
	//eCommutype[IN]通信类型
	//pData[IN]建立连接所需要的数据结构体的指针(根据通信类型确定)
	//strError[OUT]错误信息返回
	//返回说明:
	//成功建立连接返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnOpen(const ECOMMTYPE eCommutype, void* pData, std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//关闭连接
	//参数说明:
	//strError[OUT]错误信息返回
	//返回说明:
	//成功关闭连接返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnClose(std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//注册回调函数
	//参数说明:
	//pFnCallBack[IN]回调函数指针
	//返回说明:
	//无
	//////////////////////////////////////////////////////////////////////////
	virtual void fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2);
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//发送字符
	//参数说明:
	//strSend[IN]被发送的字符串
	//ulTime[IN]超时时间
	//strError[OUT]错误信息返回
	//返回说明:
	//如果在超时时间内发送成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnSendString(const std::string& strSend,const unsigned long ulTime,bool bSendByChar,std::string &strError);
	virtual bool fnSendByte(unsigned char* strSend, const unsigned long ulDataLen, const unsigned long ulTime, std::string &strError);
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
	virtual bool fnSendData(const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError);

	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//接收定时时间内所有收到的数据并返回
	//参数说明:
	//strRecv[OUT]接收的字符串
	//ulTime[IN]定时时间
	//strError[OUT]错误信息返回
	//返回说明:
	//如果在时间内接收到数据返回1,超时还没有接收到数据返回0,暂停则返回2
	//////////////////////////////////////////////////////////////////////////
	virtual int fnReceiveString(std::string &strRecv,const unsigned long ulTime,std::string &strError);
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
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//在指定的时间内接收返回信息并从信息中查找指定类型的字符串
	//参数说明:
	//strFind[IN]被查找的字符串,可接受正则表达式
	//vstrResult[OUT]查找结果返回
	//ulTimeOut[IN]超时时间
	//strError[OUT]错误信息返回
	//返回说明:
	//查找成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnFindString(const std::string strFind,std::vector<std::string> &vstrResult,unsigned long ulTimeOut,std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//登录服务器
	//参数说明:
	//pData[IN]登录所需要的数据结构(VEC_DATA_LOGIN)指针
	//strError[OUT]错误信息返回
	//返回说明:
	//登录成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnLogin(void* pData,bool bSendByChar,std::string &strError);
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//吞吐量测试
	//参数说明:
	//strSend[IN]需要发送的命令
	//strTXRate[OUT]发送速率
	//strRXRate[OUT]接收速率
	//返回说明:
	//登录成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnGetThoughputValue(const std::string strSend,std::string &strTXRate,std::string &strRXRate);
};

