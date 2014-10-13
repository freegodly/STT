

#include "CommuBase.h"
#include "CommuPipe.h"
#include "CommuTelnet.h"
#include "CommuSerial.h"
CCommuFactory::CCommuFactory()
{
	m_eCommuType = eCTypeUnknow;
}
CCommuFactory::~CCommuFactory()
{

}

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
ICommuBase *CCommuFactory::fnCreateCommu(ECOMMTYPE eCommuType,std::string &strError)
{
	ICommuBase * pResult = 0;
	switch (eCommuType)
	{
	case eCTypePipe:
		{
			pResult = new CCommuPipe();
			break;
		}
	case eCTypeSerial:
		{
			pResult = new CCommuSerial();
			break;
		}
	case eCTypeTelnet:
		{
			pResult = new CCommuTelnet();
			break;
		}
	case eCTypeUnknow:
		{
			strError = "未知的通讯类型";
			break;
		}
	default:
		{
			strError = "无效的通讯类型";
			break;
		}
	}
	m_eCommuType = eCommuType;
	return pResult;
}
