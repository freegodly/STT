
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
    //���ٹ�����
    if (0 != m_pCommuFactory)
    {
        delete m_pCommuFactory;
        m_pCommuFactory = 0;
    }
    //����ͨѶ��
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
//����˵��:
//����ͨ�����ͽ�������
//����˵��:
//eCommutype[IN]ͨ������
//pData[IN]������������Ҫ�����ݽṹ���ָ��(����ͨ������ȷ��)
//strError[OUT]������Ϣ����
//����˵��:
//�ɹ��������ӷ���true,���򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CCommunication::fnOpen(const ECOMMTYPE eCommutype, void* pData, std::string &strError)
{
    m_bStop = false;
    if(0 != m_pCommuBase)
    {
        strError = "ͨѶ�Ѿ�����";
        return true;
    }
    //��¼ͨѶ����
    m_eCommutype = eCommutype;
    //����ͨѶ���ͻ�ȡָ������Ѷ��
    m_pCommuBase = m_pCommuFactory->fnCreateCommu(eCommutype,strError);
    if (0 == m_pCommuBase)
    {
        strError = "����ͨѶ��ʧ��: " + strError;
        return false;
    }
    //���ýӿڴ�ͨѶ
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
//����˵��:
//�ر�����
//����˵��:
//strError[OUT]������Ϣ����
//����˵��:
//�ɹ��ر����ӷ���true,���򷵻�false
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
//����˵��:
//ע��ص�����
//����˵��:
//pFnCallBack[IN]�ص�����ָ��
//����˵��:
//��
//////////////////////////////////////////////////////////////////////////
void CCommunication::fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2)
{
    if (0 != m_pCommuBase)
    {
        m_pCommuBase->fnRegCallBack(pFnCallBack,ulParam2);
    }
}
//////////////////////////////////////////////////////////////////////////
//����˵��:
//�����ַ�
//����˵��:
//strSend[IN]�����͵��ַ���
//ulTime[IN]��ʱʱ��
//bSendByChar[IN]true���ֽڷ���,false���ַ�������
//strError[OUT]������Ϣ����
//����˵��:
//����ڳ�ʱʱ���ڷ��ͳɹ�����true,���򷵻�false
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
    strError = "ͨѶ����û�д���";
    ReleaseMutex((HANDLE)m_hMutex);
    return false;
}

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
    strError = "ͨѶ����û�д���";
    ReleaseMutex((HANDLE)m_hMutex);
    return false;
}

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
int CCommunication::fnReceiveString(std::string &strRecv,const unsigned long ulTime,std::string &strError)
{

    if (0 == m_pCommuBase)
    {
        strError = "û�д���ͨѶ����!";
        return  -1;
    }
    unsigned int iRecvTime = 0;
    while (iRecvTime <= ulTime)
    {
        std::string  strTemp;

        WaitForSingleObject((HANDLE)m_hMutex,INFINITE);
        //��ͣ�˳�
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
//����˵��:
//�����ļ�
//����˵��:
//strFilePath[IN]�ļ�·��
//eProt[IN]����Э��
//strError[OUT]������Ϣ����
//����˵��:
//���ͳɹ�����true,���򷵻�false
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
bool CCommunication::fnFindString(const std::string strFind,std::vector<std::string> &vstrResult,unsigned long ulTimeOut,std::string &strError)
{
    //��ȡ�շ������յ�������
    std::string strReceive = "";
    if (0 == m_pCommuBase)
    {
        strError = "ͨѶ��ָ��û�д���";
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
        //������ƥ���ȡָ��������
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
//����˵��:
//��¼������
//����˵��:
//pData[IN]��¼����Ҫ�����ݽṹ(VEC_DATA_LOGIN)ָ��
//strError[OUT]������Ϣ����
//����˵��:
//��¼�ɹ�����true,���򷵻�false
//////////////////////////////////////////////////////////////////////////
bool CCommunication::fnLogin(void* pData,bool bSendByChar,std::string &strError)
{
    //�����ǰ������cmd.exe,����Ҫ��¼--IPTS_QA_004
    if (eCTypePipe == m_eCommutype)
    {
        return true;
    }
    VEC_DATA_LOGIN *vecLogin = (VEC_DATA_LOGIN*)pData;
    unsigned int i = 0;
    for (i = 0; i < vecLogin->size();i++)
    {
        //��������
        int k = 0;
        for (k = 0; k < 2; k++)
        {
            //��������
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
            //��������
            int iResult = this->fnReceiveString(strRecv,(*vecLogin)[i].ulTimeOut,strError);
            if(1 == iResult)
            {
                //ƥ������
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
                strError = "��ͣ";
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
        strError += "  ��¼ʧ��!";
        return false;
    }
    return true;
}
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
bool CCommunication::fnGetThoughputValue(const std::string strSend,std::string &strTXRate,std::string &strRXRate)
{
    std::string strError;
    //��������
    std::string strCmd = strSend + "\r\n";
    if (!m_pCommuBase->fnSendData(strCmd.c_str(),strCmd.length(),500,strError))
    {
        return false;
    }
    std::string strReceive;
    std::string strRecvCur;
    //����ʽ,ƥ��������
    std::string strRegThrougth = "\\[SUM\\].*?sec\\s*.*?MBytes\\s*(\\d+.\\d*)\\s*Mbits/sec";
    //����ʽ,ƥ������Ӵ���
    std::string strRegErr = "connect\\s*?failed:(.*?\\.)";
    int iMaxCount = 30;
    while (0 < (iMaxCount--))
    {
        strRecvCur = "";
        int iResult = this->fnReceiveString(strRecvCur,1000,strError);
        if(1 == iResult)
        {
            strReceive += strRecvCur;
            //ƥ���Ƿ����Ӵ���
            //CRegex regex;
            std::vector<std::string > vecResult;
            if (fnSearch(strReceive.c_str(),strRegErr.c_str(),vecResult,strError))
            {
                return false;
            }
            //ƥ�������������
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
            strError = "��ͣ";
            return false;
        }
    }
    return false;
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
bool CCommunication::fnSendData( const char* pbData,const unsigned long ulDataLen, const unsigned long ulTime,std::string &strError )
{
    //�Ѿ����͵����ݳ���
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
