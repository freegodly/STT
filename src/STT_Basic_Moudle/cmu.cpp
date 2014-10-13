#include "cmu.h"
#include "stt_basic_moudle.h"
#include <regex>
#include  <cctype>
#include  <algorithm>
#include "Communication/cping.h"
/*
 *  IP_TABLE_%d
 * TELNET_PORT
 * TELNET_PROMPT
 * TELNET_ACCOUNT
 * TELNET_PASSWORD_PROMPT
 * TELNET_PASSWORD
 * TELNET_SUCCESS_PROMPT
 *
 *
 * SERIAL_PORT_TABLE_%d
 * SERIAL_RATE
 * SERIAL_PROMPT
 * SERIAL_ACCOUNT
 * SERIAL_PASSWORD_PROMPT
 * SERIAL_PASSWORD
 * SERIAL_SUCCESS_PROMPT
 *
 *
 * */



bool connect (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9)
{
    if(CMU_Table.find(test_id)==CMU_Table.end())  // add CMU_INFO
    {
        CMU_INFO  CI;
        CI.anonymous_commu = NULL;
        CMU_Table.insert(std::pair<int,CMU_INFO>(test_id,CI));
    }
    ECOMMTYPE  etype;
    CCommunication *cc=new CCommunication();
    std::string err;
    ST_DATA_LOGIN   stDataLogin;
    VEC_DATA_LOGIN  vecLogin;


    /*
     *  arg1
     * */
    if(arg1 == NULL)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 is must!","","","","","","","","");
        delete cc;
        return false;
    }
    else if(  std::string(arg1).find("telnet")!= std::string::npos)
    {
        etype=eCTypeTelnet;
        ST_DATA_TELNET sdt;

        std::string TELNET_PROMPT =  IBasicModule::STT_G_Test_Info[test_id].variable_map["TELNET_PROMPT"];
        std::string TELNET_ACCOUNT =  IBasicModule::STT_G_Test_Info[test_id].variable_map["TELNET_ACCOUNT"];
        std::string TELNET_PASSWORD_PROMPT = IBasicModule::STT_G_Test_Info[test_id].variable_map["TELNET_PASSWORD_PROMPT"];
        std::string TELNET_PASSWORD =  IBasicModule::STT_G_Test_Info[test_id].variable_map["TELNET_PASSWORD"];
        std::string TELNET_SUCCESS_PROMPT =  IBasicModule::STT_G_Test_Info[test_id].variable_map["TELNET_SUCCESS_PROMPT"];

        //
        stDataLogin.strSend = "";
        stDataLogin.strWait =TELNET_PROMPT;
        stDataLogin.ulTimeOut = 2000;
        vecLogin.push_back(stDataLogin);
        //
        stDataLogin.strSend = TELNET_ACCOUNT;
        stDataLogin.strWait = TELNET_PASSWORD_PROMPT;
        stDataLogin.ulTimeOut = 1000;
        vecLogin.push_back(stDataLogin);
        //
        stDataLogin.strSend = TELNET_PASSWORD;
        stDataLogin.strWait = TELNET_SUCCESS_PROMPT;
        stDataLogin.ulTimeOut = 500;
        vecLogin.push_back(stDataLogin);


        char ip_table_name[24];
        sprintf(ip_table_name,"IP_TABLE_%d",test_id);
        sdt.strLocalIP    =   "";
        sdt.strTargetIP   =   IBasicModule::STT_G_Test_Info[test_id].variable_map[ip_table_name];
        sdt.usPort          =  atoi(IBasicModule::STT_G_Test_Info[test_id].variable_map["TELNET_PORT"].c_str());

        cc->fnOpen(etype,(void *)&sdt,err);
    }
    else if( std::string(arg1).find("console")!= std::string::npos)
    {
        etype=eCTypeSerial;
        ST_DATA_SERIAL sds;

        std::string SERIAL_PROMPT =  IBasicModule::STT_G_Test_Info[test_id].variable_map["SERIAL_PROMPT"];
        std::string SERIAL_ACCOUNT =  IBasicModule::STT_G_Test_Info[test_id].variable_map["SERIAL_ACCOUNT"];
        std::string SERIAL_PASSWORD_PROMPT = IBasicModule::STT_G_Test_Info[test_id].variable_map["SERIAL_PASSWORD_PROMPT"];
        std::string SERIAL_PASSWORD =  IBasicModule::STT_G_Test_Info[test_id].variable_map["SERIAL_PASSWORD"];
        std::string SERIAL_SUCCESS_PROMPT =  IBasicModule::STT_G_Test_Info[test_id].variable_map["SERIAL_SUCCESS_PROMPT"];

        //
        stDataLogin.strSend = "";
        stDataLogin.strWait =SERIAL_PROMPT;
        stDataLogin.ulTimeOut = 2000;
        vecLogin.push_back(stDataLogin);
        //
        stDataLogin.strSend = SERIAL_ACCOUNT;
        stDataLogin.strWait = SERIAL_PASSWORD_PROMPT;
        stDataLogin.ulTimeOut = 1000;
        vecLogin.push_back(stDataLogin);
        //
        stDataLogin.strSend = SERIAL_PASSWORD;
        stDataLogin.strWait = SERIAL_SUCCESS_PROMPT;
        stDataLogin.ulTimeOut = 500;
        vecLogin.push_back(stDataLogin);



        char serial_table_name[24];
        sprintf(serial_table_name,"SERIAL_PORT_TABLE_%d",test_id);

        sds.ulBaudRate = atoi(IBasicModule::STT_G_Test_Info[test_id].variable_map["SERIAL_RATE"].c_str());
        sds.usComNum = atoi(IBasicModule::STT_G_Test_Info[test_id].variable_map[serial_table_name].c_str());

        cc->fnOpen(etype,(void *)&sds,err);
    }
    else if( std::string(arg1).find("cmd")!= std::string::npos)
    {
        etype=eCTypePipe;
        ST_DATA_PIPE sdp;

        cc->fnOpen(etype,(void *)&sdp,err);

    }
    else
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,err.c_str(),"","","","","","","","");
        delete cc;
        return false;
    }

    /*
     *  arg2
     * */
    if(std::string(arg1).find("cmd")== std::string::npos || arg2==NULL || std::string(arg2).empty() ||  std::string(arg2).find("0")!=std::string::npos)  //  login
    {
        if ( cc->fnLogin((void*)&vecLogin,true,err) )
        {

        }
        else
        {
            IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,err.c_str(),"","","","","","","","");
            cc->fnClose(err);
            delete cc;
            return false;
        }
    }
    else   //no login
    {

    }

    /*
     *  arg3
     * */
    if(arg3==NULL || std::string(arg3).empty())//anonymous_commu
    {
        if(CMU_Table[test_id].anonymous_commu!=NULL)  //发现未关闭的命名通讯则关闭
        {
            CMU_Table[test_id].anonymous_commu->fnClose(err);
        }

        CMU_Table[test_id].anonymous_commu = cc;
    }
    else                        //name_commu
    {
        CMU_Table[test_id].name_commu.insert(std::pair<std::string,CCommunication*>(std::string(arg3),cc));
    }

    return true;
}


bool close(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    std::string err;
    if(arg2==NULL || std::string(arg2).empty())
    {
        if(CMU_Table[test_id].anonymous_commu!=NULL)  //发现未关闭的命名通讯则关闭
        {

            CMU_Table[test_id].anonymous_commu->fnClose(err);
            CMU_Table[test_id].anonymous_commu =NULL;
        }
    }
    else
    {
        if( CMU_Table[test_id].name_commu.find(std::string(arg2)) != CMU_Table[test_id].name_commu.end() )
        {
            CCommunication* cc =  CMU_Table[test_id].name_commu[std::string(arg2)];
            cc->fnClose(err);
            CMU_Table[test_id].name_commu.erase(std::string(arg2));
        }
    }

    return true;
}


bool send(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    std::string err;
    bool flag = true;
    if(arg1 == NULL)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 is must!","","","","","","","","");
        return false;
    }

    if(arg2==NULL || std::string(arg2).empty())
    {
        if(CMU_Table[test_id].anonymous_commu!=NULL)
        {
            flag= CMU_Table[test_id].anonymous_commu->fnSendString(std::string(arg1),500,true,err);
        }
    }
    else
    {
        if( CMU_Table[test_id].name_commu.find(std::string(arg2)) != CMU_Table[test_id].name_commu.end() )
        {
            flag =  CMU_Table[test_id].name_commu[std::string(arg2)]->fnSendString(std::string(arg1),500,true,err);
        }
    }

    return flag;
}


bool receive(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    std::string err;
    std::string strReceive;
    bool flag = true;

    if(arg1 == NULL || arg2==NULL|| arg3==NULL)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 arg2 arg3  is must!","","","","","","","","");
        return false;
    }

    if(arg5==NULL || std::string(arg5).empty())
    {
        if(CMU_Table[test_id].anonymous_commu!=NULL)
        {
            flag= CMU_Table[test_id].anonymous_commu->fnReceiveString(strReceive,atoi(arg1),err);
        }
    }
    else
    {
        if( CMU_Table[test_id].name_commu.find(std::string(arg5)) != CMU_Table[test_id].name_commu.end() )
        {
            flag =  CMU_Table[test_id].name_commu[std::string(arg5)]->fnReceiveString(strReceive,atoi(arg1),err);
        }
    }
    IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,strReceive.c_str(),"","","","","","","","");
    if(flag)
    {
        const std::regex pattern(arg2);
        std:: match_results<std::string::const_iterator> result;
        regex_search(strReceive,result,pattern);
        int serc_len = result.length();
        if(serc_len>= atoi(arg3))
        {
            //RECEIVE
            if(arg4 == NULL){
                IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>("RECEIVE",result[atoi(arg3)-1].str()));
            }
            else
                IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>(arg4,result[atoi(arg3)-1].str()));
        }
        else
        {
            flag = false;
        }

    }

    return flag;
}


bool ping(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    CPing cping;
    bool flag = true;
    if(arg1 == NULL)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1  is must!","","","","","","","","");
        return false;
    }

    if(arg3 != NULL && arg4 != NULL && arg5 != NULL)
    {
        return  cping.Ping(arg1,arg2,atoi(arg3),atoi(arg4),atoi(arg5));
    }

    if(arg3 != NULL && arg4 != NULL )
    {
        return  cping.Ping(arg1,arg2,atoi(arg3),atoi(arg4));
    }

    if(arg3 != NULL  )
    {
        return  cping.Ping(arg1,arg2,atoi(arg3));
    }

    return  cping.Ping(arg1,arg2);

}


bool sleep(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    if(arg1 == NULL || std::string(arg1).empty())
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1  is must!","","","","","","","","");
        return false;
    }
    Sleep(atoi(arg1));
    return true;
}


bool find(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    bool flag = true;

    if(arg1 == NULL || arg2==NULL)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 arg2   is must!","","","","","","","","");
        return false;
    }

    std::string  strFind   = arg2;
    std::vector<std::string>  vecResult;
    std::string               strError;

    if(arg3==NULL || std::string(arg3).empty())
    {
        if(CMU_Table[test_id].anonymous_commu!=NULL)
        {
            flag= CMU_Table[test_id].anonymous_commu->fnFindString(strFind,vecResult,atoi(arg1),strError);
        }
    }
    else
    {
        if( CMU_Table[test_id].name_commu.find(std::string(arg3)) != CMU_Table[test_id].name_commu.end() )
        {
            flag =  CMU_Table[test_id].name_commu[std::string(arg3)]->fnFindString(strFind,vecResult,atoi(arg1),strError);
        }
    }

    flag = (vecResult.size() > 0);
    if (!flag)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,strError.c_str(),"","","","","","","","");
    }


    return flag;
}


bool loopchk(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    long start = 0;
    long end  = 0;
    long totaltime = 0 ;
    long sleeptime =0;
    int grouptimes = 0;
    CCommunication *cc = NULL;
    bool flag = true;
    if(arg1 == NULL || arg2==NULL || arg3 == NULL || arg4==NULL)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 arg2   is must!","","","","","","","","");
        return false;
    }
    else
    {
        totaltime = atoi(arg2);
        grouptimes = atoi(arg4);
    }

    if(arg5 != NULL && !std::string(arg5).empty())
    {
        sleeptime = atoi(arg5);
    }

    if(arg7 != NULL && !std::string(arg7).empty()  && CMU_Table[test_id].name_commu.find(std::string(arg7)) != CMU_Table[test_id].name_commu.end() )
    {
        cc = CMU_Table[test_id].name_commu[std::string(arg7)];
    }
    else
    {
        cc =  CMU_Table[test_id].anonymous_commu;
    }
    if(cc == NULL)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"Connection does not exist !","","","","","","","","");
        return false;
    }

    start = GetTickCount();

    //在一定的时间内循环，通过下指令等到需要的值
    do
    {

        //发送指令
        std::string strSend = arg1;
        std::string strError;
        std::string strReceive ="";
        flag =cc->fnSendString(strSend,500,false,strError);

        Sleep(sleeptime);

        if(!flag)
        {
            return false;
        }

        std::vector<std::string>  vecResult;
        if(cc->fnFindString(arg3,vecResult,1000,strError))
        {

            if(vecResult.size()>= grouptimes)
            {
                //RECEIVE
                if(arg6 == NULL || std::string(arg6).empty()){
                    IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>("RECEIVE", vecResult[grouptimes-1]));
                }
                else
                    IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>(arg6, vecResult[grouptimes-1]));

                flag = true;
                break;
            }
        }

        flag = false;
        end = GetTickCount();

    }while((end - start) < totaltime);

    return flag;
}


bool check(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{

    float up = 0;
    float down = 0;
    bool isnumcompare = false;
    std::string tagstring = "RECEIVE";
    if(arg1 == NULL || std::string(arg1).empty() )
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1   is must!","","","","","","","","");
        return false;
    }
    if( arg2 != NULL &&  !std::string(arg2).empty() )  // 数值比较
    {
        up     =  atof(arg1);
        down =  atof(arg2);
        isnumcompare = true;
    }
    if( arg3 != NULL &&  !std::string(arg3).empty() )
    {
        tagstring = arg3;
    }

    if(isnumcompare)
    {
        float tagnum = atof(IBasicModule::STT_G_Test_Info[test_id].variable_map[tagstring].c_str());
        if(up>=tagnum && down<=tagnum)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if( 0 == IBasicModule::STT_G_Test_Info[test_id].variable_map[tagstring].compare(arg1) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}


//字符串单字符转为十六进制
std::string fnChangeHex( const char * pszBuffer )
{
    string strResult;
    char  ch = 0;
    for (int i = 0; i < strlen(pszBuffer); i++)
    {
        ch = *(pszBuffer+i);
        char asTmp[2];
        sprintf(asTmp,"%2x",ch);
        strResult.append(asTmp);
    }
    return strResult;
}

std::vector<std::string> split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}


//////////////////////////////////////////////////////////////////////////
//函数说明:
//数据增加
//参数说明:
//strBase[IN]被加数
//strAdd[IN]加数
//iRadix[IN]模,十六进制或十进制
//返回说明:
//相加后的结果
//////////////////////////////////////////////////////////////////////////
std::string fnIncrease(const std::string &strBase,const std::string& strAdd,int iRadix)
{
    int i = 0;
    int iInc = 0;
    std::string strFirst = strBase.length() >= strAdd.length()?strBase:strAdd;
    std::string strSecond = strBase.length() < strAdd.length()?strBase:strAdd;
    while (strFirst.length() != strSecond.length())
    {
        strSecond.insert(strSecond.begin(),'0');
    }
    std::string strResult = "";

    for (i = strFirst.length()-1; i >= 0 ; i--)
    {
        std::string strFIndex = strFirst.substr(i,1);
        std::string strSIndex = strSecond.substr(i,1);
        char *p = 0;
        int iF = strtol(strFIndex.c_str(),&p,iRadix);
        p = 0;
        int iS = strtol(strSIndex.c_str(),&p,iRadix);
        int iResult = iF+iS+iInc;
        iInc = iResult / iRadix;
        iResult = iResult % iRadix;
        char buffer[20] = {0};
        ltoa(iResult,buffer,iRadix);
        strResult.insert(strResult.begin(),buffer[0]);
    }
    if (0 < iInc)
    {
        char buffer[20] = {0};
        ltoa(iInc,buffer,iRadix);
        strResult.insert(strResult.begin(),buffer[0]);
    }
    return strResult;
}

//////////////////////////////////////////////////////////////////////////
//函数说明:
//数据相减
//参数说明:
//strBase[IN]被减数
//strAdd[IN]减数
//iRadix[IN]模,十六进制或十进制
//返回说明:
//相减后的结果
//////////////////////////////////////////////////////////////////////////
std::string fnSubtract(const std::string &strBase,const std::string& strSub,int iRadix)
{
    int i = 0;
    int iInc = 0;
    std::string strFirst = strBase.length() >= strSub.length()?strBase:strSub;
    std::string strSecond = strBase.length() < strSub.length()?strBase:strSub;

    while (strFirst.length() != strSecond.length())
    {
        strSecond.insert(strSecond.begin(),'0');
    }
    if (0 > strcmp(strFirst.c_str(),strSecond.c_str()))
    {
        std::swap(strFirst,strSecond);
    }
    std::string strResult = "";
    for (i = strFirst.length()-1; i >= 0 ; i--)
    {
        std::string strFIndex = strFirst.substr(i,1);
        std::string strSIndex = strSecond.substr(i,1);
        char *p = 0;
        int iF = strtol(strFIndex.c_str(),&p,iRadix);
        p = 0;
        int iS = strtol(strSIndex.c_str(),&p,iRadix);
        int iResult = (iF - iS) - iInc;
        iInc = 0;
        if (0 > iResult)
        {
            iResult += iRadix;
            iInc = 1;
        }
        char buffer[20] = {0};
        ltoa(iResult,buffer,iRadix);
        strResult.insert(strResult.begin(),buffer[0]);

    }
    return strResult;
}

bool set(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    if(arg1 == NULL || std::string(arg1).empty() )
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1   is must!","","","","","","","","");
        return false;
    }

    if(arg2 == NULL || std::string(arg2).empty() )
    {
        std::string receive =  IBasicModule::STT_G_Test_Info[test_id].variable_map["RECEIVE"];
        IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>(arg1, receive));
    }

    if(arg3 == NULL || std::string(arg3).empty() )
    {
        IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>(arg1, arg2));
    }

    if(arg3 != NULL  &&  arg4 != NULL )
    {
        int mode = atoi(arg3);
        int sp_num = atoi(arg4);
        std::string decollator="";
        switch(mode)
        {
        case 0:
            decollator = "";
            break;
        case 1:
            decollator = ":";
            break;
        case 2:
            decollator = "-";
            break;
        case 3:
            decollator = ".";
            break;
        case 4:
            decollator = "%3A";
            break;
        case 5:
            decollator = " ";
            break;
        case 6:
            decollator = " ";
            break;//除了空格，数据会变成十六进制
        default:
            decollator = "";
            break;
        }

        std::string  StrOld = arg2;
        std::string  StrNew = "";
        std::string  tmp;
        for(int l = 0;l<StrOld.length()/sp_num; l++)
        {
            tmp = StrOld.substr( l *sp_num,sp_num);
            //如果是6则返单字节数据转为十六进制显示
            if (6 == mode)
            {
                tmp = fnChangeHex(tmp.c_str());
            }
            tmp.append(decollator);
            StrNew.append(tmp);
        }

        IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>(arg1, StrNew));
    }

    return true;
}


bool stringAdd(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{

    std::string arglist[9] ={arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9};


    if(arg1 == NULL ||  arg2 == NULL || arg1 == NULL || arg4 == NULL || std::string(arg1).empty() )
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 arg2 arg3 arg 4  is must!","","","","","","","","");
        return false;
    }

    std::string StrNew ="";
    int  dif = atoi(arg2);
    for(int i=2; i<dif; i++)
    {
        if(arglist[i].find("Str") != string::npos)
        {
            StrNew.append( arglist[i].substr(5,arglist[i].find(")")-5 ) );   // 此处为 Str(string),取string
        }

        if(arglist[i].find("Sub") != string::npos)                                      // 此处为 Sub(&Str&,2,1)，取Str.substring(2,1)
        {
            std::string  StrTemp = arglist[i].substr(5,arglist[i].find(")") -5 ) ;
            std::vector<std::string>  StrTemppara  = split(StrTemp,";");

            if(arglist[i].find("&") != string::npos)
            {
                StrTemppara[0] = StrTemppara[0].replace(arglist[i].find("&"),1,"");
                StrTemppara[0] = StrTemppara[0].replace(arglist[i].find("&"),1,"");
            }
            else
            {
                IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"stringAdd的Sub函数编写不正确","","","","","","","","");
                return false;
            }

            string StrTempTitle = IBasicModule::STT_G_Test_Info[test_id].variable_map[StrTemppara[0]];
            StrNew.append(StrTempTitle.substr(atoi(StrTemppara[1].c_str()),atoi(StrTemppara[2].c_str())));
        }
    }
    IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>(arg1, StrNew));
    return true;
}


bool compare(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    if(arg1 == NULL ||  arg2 == NULL || std::string(arg1).empty() )
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 arg2  is must!","","","","","","","","");
        return false;
    }
    std::string strarg1=arg1;

    if( arg3 != NULL && arg4 == NULL )
    {
        strarg1 = strarg1.substr(atoi(arg3),strarg1.length()-atoi(arg3));

    }
    if(arg4 != NULL &&  !std::string(arg4).empty())
    {
        strarg1 = strarg1.substr(atoi(arg3),atoi(arg4));
    }

    if( strarg1.compare(arg2)  == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool add(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    if(arg1 == NULL ||  arg2 == NULL ||  arg3 == NULL || arg4 == NULL || arg5 == NULL || arg6 == NULL ||std::string(arg1).empty() )
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 arg2 arg3 arg4 arg5 arg6 is must!","","","","","","","","");
        return false;
    }
    //格式
    int iFormat =  atoi(arg4);
    std::string asFormat = "";
    switch(iFormat)
    {
    case 0:
        asFormat = "";
        break;
    case 1:
        asFormat = ":";
        break;
    case 2:
        asFormat = "-";
        break;
    case 3:
        asFormat = ".";
        break;
    case 4:
        asFormat = "%3A";
        break;
    case 5:
        asFormat = " ";
        break;
    default:
        asFormat = "";
        break;
    }
    //间隔段长度
    int iLen = atoi(arg5);
    //加/减标记
    int bAdd = atoi(arg6);
    std::string asSource =arg2;
    string strBase     = arg2;

    string  strAdded  = arg3;
    int iRadix = 64;
    if(bAdd == 1)
        asSource = fnIncrease(strBase,strAdded,iRadix).c_str();
    else
        asSource = fnSubtract(strBase,strAdded,iRadix).c_str();

    char asRegOrg[24];
    char asRegNew[24];
    sprintf(asRegOrg,"(.{%d})",iLen);
    sprintf(asRegNew,"\\1%s",asFormat.c_str());
    std::string replace = asRegNew;
    //替换
    string  strDest;
    regex pattern(asRegOrg);
    strDest = std::regex_replace(asSource, pattern, replace);
    strDest = strDest.substr(0,strDest.size()-asFormat.length());

    IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>(arg1, strDest));

}


bool loopfind(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    if(arg1 == NULL ||  arg2 == NULL ||  arg3 == NULL || arg4 == NULL  )
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 arg2 arg3 arg4 arg5 arg6 is must!","","","","","","","","");
        return false;
    }

    long start = 0;
    long end  = 0;
    long totaltime = 0 ;
    long steptime =0;
    CCommunication *cc = NULL;
    bool flag = true;
    std::string strError;
    totaltime = atoi(arg2);
    steptime = atoi(arg3);
    std::string  strSend = arg1;
    std::string  strFind = arg4;

    if(arg5 != NULL && !std::string(arg5).empty()  && CMU_Table[test_id].name_commu.find(std::string(arg5)) != CMU_Table[test_id].name_commu.end() )
    {
        cc = CMU_Table[test_id].name_commu[std::string(arg5)];
    }
    else
    {
        cc =  CMU_Table[test_id].anonymous_commu;
    }
    if(cc == NULL)
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"Connection does not exist !","","","","","","","","");
        return false;
    }

    start = GetTickCount();
    if ("NULL" == strSend)
    {
        strSend = "";
    }
    do
    {

        flag = cc->fnSendString(strSend,500,false,strError);

        if(!flag)
        {
            IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id, strError.c_str(),"","","","","","","","");
            return false;
        }
        std::vector<std::string >   vecResult;
        flag = cc->fnFindString(strFind,vecResult,steptime,strError);
        flag = (vecResult.size() > 0);
        if(flag)
        {
            break;
        }

        end = ::GetTickCount();


    }while((end - start) < totaltime);

    return flag;
}


bool change(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    if(arg1 == NULL ||  arg2 == NULL ||  arg3 == NULL || arg4 == NULL  )
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"arg1 arg2 arg3 arg4 arg5 arg6 is must!","","","","","","","","");
        return false;
    }
    std::string   strTmp="";
    const std::regex pattern(arg3);
    std:: match_results<std::string::const_iterator> result;
    regex_search(std::string(arg2),result,pattern);
    int serc_len = result.length();
    if(serc_len>= atoi(arg4))
    {
        strTmp = result[atoi(arg4)].str();
    }
    else
    {
        IBasicModule::STT_G_Test_Run_Fun("stt_print",test_id,"change 正则式匹配不过","","","","","","","","");
        return false;
    }
    int  iChangeType  = atoi(arg5);
    switch(iChangeType)
    {
        //不需要作操作
        case 0:
        {

            break;
        }
            //转为小写
        case 1:
        {
            transform(strTmp.begin(), strTmp.end(), strTmp.begin(),:: tolower);
            break;
        }
            //转为大写
        case 2:
        {
             transform(strTmp.begin(), strTmp.end(), strTmp.begin(), ::toupper);
            break;
        }
        default:
        {
            break;
        }

    }

     IBasicModule::STT_G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string>(arg1, strTmp));
}
