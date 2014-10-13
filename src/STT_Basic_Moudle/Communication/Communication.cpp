
// Communication.cpp: implementation of the CCommunication class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Communication.h"
#include "CommuBase.h"
#include <iostream>
#include <exception>
#include <string>
#include <regex>
//#include "IRegexp.h"
using namespace std;
bool fnSearch(const char* strReceive,const char* strFind,std::vector<std::string> &vstrResult,std::string &strError)
{
    string str(strReceive);
    const std::regex pattern(strFind);
    std::match_results<std::string::const_iterator> result;
    bool valid=false;
    try
    {
        valid = std::regex_search(str,result,pattern);
        vstrResult.clear();
        for (std::match_results<std::string::const_iterator>::const_iterator iter=result.begin();iter!=result.end();iter++)
        {
            vstrResult.push_back(*iter);
        }
    }
    catch (std::exception* e)
    {
        strError=string(e->what());
    }

    return valid;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommunication::CCommunication():
    m_pCommuFactory(0),m_pCommuBase(0),m_eCommutype(eCTypeUnknow)
{
    m_pCommuFactory = new CCommuFactory();
    m_bStop = false;
    m_hMutex = (long)CreateMutex(0,false,0);
}

CCommunication::~CCommunication()
{
    //销毁工厂类
    if (0 != m_pCommuFactory)
    {
        delete m_pCommuFactory;
        m_pCommuFactory = 0;
    }
    //销毁通讯类
    if (0 != m_pCommuBase)
    {
        delete m_pCommuBase;
        m_pCommuBase = 0;
    }
    if(0 != m_hMutex)
    {
        CloseHandle((HANDLE)m_hMutex);
    }
}

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
bool CCommunication::fnOpen(const ECOMMTYPE eCommutype, void* pData, std::string &strError)
{
    m_bStop = false;
    if(0 != m_pCommuBase)
    {
        strError = "通讯已经建立";
        return true;
    }
    //记录通讯类型
    m_eCommutype = eCommutype;
    //根据通讯类型获取指定的能讯类
    m_pCommuBase = m_pCommuFactory->fnCreateCommu(eCommutype,strError);
    if (0 == m_pCommuBase)
    {
        strError = "创建通讯类失败: " + strError;
        return false;
    }
    //调用接口打开通讯
    for(int i = 0; i < 50; i++)
    {

        if (m_bStop)
        {
            break;
        }
        if(m_pCommuBase->fnOpen(pData,strError))
        {
            return true;
        }
        if (m_bStop)
        {
            break;
        }
        m_pCommuBase->fnClose(strError);
        ::Sleep(100);
    }
    return false;
}
//////////////////////////////////////////////////////////////////////////
//函数说明:
//关闭连接
//参数说明:
//strError[OUT]错误信息返回
//返回说明:
//成功关闭连接返回true,否则返回false
//////////////////////////////////////////////////////////////////////////
bool CCommunication::fnClose(std::string &strError)
{
    m_bStop = true;
    WaitForSingleObject((HANDLE)m_hMutex,INFINITE);
    if (0 != m_pCommuBase)
    {
        m_pCommuBase->fnClose(strError);
        delete m_pCommuBase;
        m_pCommuBase = 0;
    }
    ReleaseMutex((HANDLE)m_hMutex);
    return true;
}
//////////////////////////////////////////////////////////////////////////
//函数说明:
//注册回调函数
//参数说明:
//pFnCallBack[IN]回调函数指针
//返回说明:
//无
//////////////////////////////////////////////////////////////////////////
void CCommunication::fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2)
{
    if (0 != m_pCommuBase)
    {
        m_pCommuBase->fnRegCallBack(pFnCallBack,ulParam2);
    }
}
//////////////////////////////////////////////////////////////////////////
//函数说明:
//发送字符
//参数说明:
//strSend[IN]被发送的字符串
//ulTime[IN]超时时间
//bSendByChar[IN]true按字节发送,false按字符串发送
//strError[OUT]错误信息返回
//返回说明:
//如果在超时时间内发送成功返回true,否则返回false
//////////////////////////////////////////////////////////////////////////
bool CCommunication::fnSendString(const std::string& strSend,const unsigned long ulTime,bool bSendByChar,std::string &strError)
{

    WaitForSingleObject((HANDLE)m_hMutex,INFINITE);
    if (0 != m_pCommuBase)
    {
        if (!bSendByChar)
        {
            std::string strSendAdd = strSend + "\r\n";
            return m_pCommuBase->fnSendData(strSendAdd.c_str(),strSendAdd.size(),ulTime,strError);
        }
        else
        {

            std::string strSendAdd = strSend + "\r";
            for (unsigned int i = 0; i < strSendAdd.length(); i++)
            {
                char  chBuf[2];
                chBuf[0] = strSendAdd[i];
                if(!m_pCommuBase->fnSendData(chBuf,1,ulTime,strError))
                {
                    return false;
                }
                ::Sleep(100);
            }
            return true;
        }
    }
    strError = "通讯基类没有创建";
    ReleaseMutex((HANDLE)m_hMutex);
    return false;
}

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
bool CCommunication::fnSendByte(unsigned char* strSend, const unsigned long ulDataLen, const unsigned long ulTime, std::string &strError)
{

    WaitForSingleObject((HANDLE)m_hMutex,INFINITE);
    if (0 != m_pCommuBase)
    {
        return m_pCommuBase->fnSendData((const char *)strSend, ulDataLen, ulTime, strError);
        /*
        for (int i = 0; i < ulDataLen; i++)
        {
            char  chBuf[2];
        chBuf[0] = strSend[i];
            if(!m_pCommuBase->fnSendData(chBuf,1,ulTime,strError))
            {
                return false;
            }
            ::Sleep(100);
        }
        return true;
*/
    }
    strError = "通讯基类没有创建";
    ReleaseMutex((HANDLE)m_hMutex);
    return false;
}

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
int CCommunication::fnReceiveString(std::string &strRecv,const unsigned long ulTime,std::string &strError)
{

    if (0 == m_pCommuBase)
    {
        strError = "没有创建通讯连接!";
        return  -1;
    }
    unsigned int iRecvTime = 0;
    while (iRecvTime <= ulTime)
    {
        std::string  strTemp;

        WaitForSingleObject((HANDLE)m_hMutex,INFINITE);
        //暂停退出
        if (0 == m_pCommuBase || m_bStop)
        {
            return 2;
        }
        bool bRecv = m_pCommuBase->fnReceiveString(strTemp,strError);
        if (0 != strTemp.size())
        {
            strRecv += strTemp;
        }
        ReleaseMutex((HANDLE)m_hMutex);
        Sleep(100);
        iRecvTime += 100;
    }

    if (0 != strRecv.size())
    {
        return 1;
    }
    else
    {
        return 0;
    }
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
bool CCommunication::fnSendFile(const std::string strFilePath,const ESENDFILEPROT eProt,std::string &strError)
{
    if (0 != m_pCommuBase)
    {
        return m_pCommuBase->fnSendFile(strFilePath,eProt,strError);
    }
    return true;
}
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
bool CCommunication::fnFindString(const std::string strFind,std::vector<std::string> &vstrResult,unsigned long ulTimeOut,std::string &strError)
{
    //获取收发器接收到的数据
    std::string strReceive = "";
    if (0 == m_pCommuBase)
    {
        strError = "通讯类指针没有创建";
        return false;
    }
    unsigned long ulStartTime = GetTickCount();
    unsigned long ulCurTime = GetTickCount();
    while (ulTimeOut >= (ulCurTime - ulStartTime) && !m_bStop)
    {
        ulCurTime = GetTickCount();
        std::string  strRecvTmp;
        int iResult = this->fnReceiveString(strRecvTmp,1000,strError);
        if(0 == iResult)
        {
            continue;
        }
        else if(1 == iResult)
        {

        }
        else if(2 == iResult)
        {
            break;
        }

        strReceive += strRecvTmp;
        //用正则匹配获取指定的数据
        /*CRegex  regex;
        if (regex.fnSearch(strReceive.c_str(),strFind.c_str(),vstrResult,strError))
        {
            return true;
        }*/
        /*const std::regex pattern(strFind.c_str());
        std::match_results<std::string::const_iterator> result;
        bool valid = std::regex_match(strReceive.c_str(), result, pattern);
        vstrResult.clear();
        for (std::string::const_iterator iter=result.begin();iter!=result.end();iter++)
        {
            vstrResult.push_back(*iter);
        }

        return valid;*/

        if (fnSearch(strReceive.c_str(),strFind.c_str(),vstrResult,strError))
        {
            return true;
        }
    }

    return false;
}
//////////////////////////////////////////////////////////////////////////
//函数说明:
//登录服务器
//参数说明:
//pData[IN]登录所需要的数据结构(VEC_DATA_LOGIN)指针
//strError[OUT]错误信息返回
//返回说明:
//登录成功返回true,否则返回false
//////////////////////////////////////////////////////////////////////////
bool CCommunication::fnLogin(void* pData,bool bSendByChar,std::string &strError)
{
    //如果当前链接是cmd.exe,则不需要登录--IPTS_QA_004
    if (eCTypePipe == m_eCommutype)
    {
        return true;
    }
    VEC_DATA_LOGIN *vecLogin = (VEC_DATA_LOGIN*)pData;
    unsigned int i = 0;
    for (i = 0; i < vecLogin->size();i++)
    {
        //重试两次
        int k = 0;
        for (k = 0; k < 2; k++)
        {
            //发送数据
            //if (0 < (*vecLogin)[i].strSend.length())
            {
                std::string strSend = (*vecLogin)[i].strSend + "\r";
                if (bSendByChar)
                {
                    char buf[2] = {0};
                    for (int i=  0; i < strSend.length(); i++)
                    {
                        buf[0] = strSend[i];
                        m_pCommuBase->fnSendData(buf,1,500,strError);
                        ::Sleep(10);
                    }
                }
                else
                {
                    m_pCommuBase->fnSendData(strSend.c_str(),strSend.size(),(*vecLogin)[i].ulTimeOut,strError);
                }
            }
            std::string strRecv;
            ::Sleep(100);
            //接收数据
            int iResult = this->fnReceiveString(strRecv,(*vecLogin)[i].ulTimeOut,strError);
            if(1 == iResult)
            {
                //匹配数据
                if (-1 != strRecv.find((*vecLogin)[i].strWait.c_str()))
                {
                    break;
                }
            }
            else if(0 == iResult)
            {
                continue;
            }
            else if(2 == iResult)
            {
                strError = "暂停";
                return false;
            }
        }//for (k = 0; k < 2; k++)
        if (2 <= k)
        {
            break;
        }

    }//for (i = 0; i < vecLogin->size();i++)
    if (i < vecLogin->size())
    {
        strError += "  登录失败!";
        return false;
    }
    return true;
}
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
bool CCommunication::fnGetThoughputValue(const std::string strSend,std::string &strTXRate,std::string &strRXRate)
{
    std::string strError;
    //发送命令
    std::string strCmd = strSend + "\r\n";
    if (!m_pCommuBase->fnSendData(strCmd.c_str(),strCmd.length(),500,strError))
    {
        return false;
    }
    std::string strReceive;
    std::string strRecvCur;
    //正则式,匹配上下行
    std::string strRegThrougth = "\\[SUM\\].*?sec\\s*.*?MBytes\\s*(\\d+.\\d*)\\s*Mbits/sec";
    //正则式,匹配出连接错误
    std::string strRegErr = "connect\\s*?failed:(.*?\\.)";
    int iMaxCount = 30;
    while (0 < (iMaxCount--))
    {
        strRecvCur = "";
        int iResult = this->fnReceiveString(strRecvCur,1000,strError);
        if(1 == iResult)
        {
            strReceive += strRecvCur;
            //匹配是否连接错误
            //CRegex regex;
            std::vector<std::string > vecResult;
            if (fnSearch(strReceive.c_str(),strRegErr.c_str(),vecResult,strError))
            {
                return false;
            }
            //匹配出上下行数据
            //regex.fnGrep(strReceive.c_str(),strRegThrougth.c_str(),vecResult,strError);
            fnSearch(strReceive.c_str(),strRegThrougth.c_str(),vecResult,strError);
            if (2 != vecResult.size())
            {
                continue;
            }
            else
            {
                std::vector<std::string > vecSearch;
                if (fnSearch(vecResult[0].c_str(),strRegThrougth.c_str(),vecSearch,strError))
                {
                    strTXRate = vecSearch[1];
                }
                vecSearch.clear();

                if (fnSearch(vecResult[1].c_str(),strRegThrougth.c_str(),vecSearch,strError))
                {
                    strRXRate = vecSearch[1];
                    return true;
                }
            }
        }
        else if (0 == iResult)
        {
            continue;
        }
        else if (2 == iResult)
        {
            strError = "暂停";
            return false;
        }
    }
    return false;
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
bool CCommunication::fnSendData( const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError )
{
    //已经发送的数据长度
    unsigned long ulSended = 0;
    unsigned long ulStep = 512;
    while (ulDataLen < ulSended)
    {
        int iSend = (ulDataLen-ulSended) > ulStep ? ulStep : (ulDataLen-ulSended);
        if (m_pCommuBase->fnSendData(pbData+ulSended,iSend,ulTime,strError))
        {
            ulSended += iSend;
        }
        else
        {
            return false;
        }
    }
    return true;
}
