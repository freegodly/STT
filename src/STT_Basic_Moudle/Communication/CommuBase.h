// CommuBase.h: interface for the CCommuBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "Communication.h"


//通讯基类
class   ICommuBase  
{ 
public:
	ICommuBase():m_pfnCommCallBack(0){}
	virtual ~ICommuBase(){}

	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//打开连接
	//参数说明:
	//pData[IN]打开连接所需要的数据结构指针
	//strError[OUT]错误信息返回
	//返回说明:
	//打开连接成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnOpen(void *pData,std::string &strError) = 0;
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//关闭连接
	//参数说明:
	//strError[OUT]错误信息返回
	//返回说明:
	//关闭连接成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual bool fnClose(std::string &strError) = 0;
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
	virtual bool fnReceiveString(std::string &strRecv,std::string &strError) = 0;    //////////////////////////////////////////////////////////////////////////
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
	virtual bool fnSendData(const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError)= 0;
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
	//注册回调函数
	//参数说明:
	//pFnCallBack[IN]回调函数指针
	//返回说明:
	//无
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

//通讯类工厂类,用于创建指定的通信类型接口类
class   ICommuFactory
{

public:
	virtual ~ICommuFactory(){}

	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//根据通讯类型创建相应的通信类
	//参数说明:
	//strSend[IN]需要发送的命令
	//strTXRate[OUT]发送速率
	//strRXRate[OUT]接收速率
	//返回说明:
	//登录成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual ICommuBase *fnCreateCommu(ECOMMTYPE eCommuType,std::string &strError) = 0;

};//通讯类工厂类,用于创建指定的通信类型实现类
class   CCommuFactory: public ICommuFactory
{

public:
	CCommuFactory();
	virtual ~CCommuFactory();

	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//根据通讯类型创建相应的通信类
	//参数说明:
	//strSend[IN]需要发送的命令
	//strTXRate[OUT]发送速率
	//strRXRate[OUT]接收速率
	//返回说明:
	//登录成功返回true,否则返回false
	//////////////////////////////////////////////////////////////////////////
	virtual ICommuBase *fnCreateCommu(ECOMMTYPE eCommuType,std::string &strError);
private:
	ECOMMTYPE m_eCommuType;

};

