/******************************************************************************************************
* Copyright (C) 2014, All right reserved.

* file
* version  1.0
* author   NingJian (freegodly@gmail.com)
* brief

* detail

* TODO
* history  2014-9-17 created by NingJian
*
* note
/******************************************************************************************************/

#include "stt_basic_moudle.h"
#include "cmu.h"

std::map<int,TEST_INFO> IBasicModule::STT_G_Test_Info;

std::map<std::string,TEST_FUN_INFO> IBasicModule::STT_G_Test_Fun_Info;

RUN_FUN IBasicModule::STT_G_Test_Run_Fun;

std::map<std::string,std::string>     IBasicModule::STT_G_Moudles_Config;


STT_Basic_Moudle::STT_Basic_Moudle()
{

}


std::string STT_Basic_Moudle::get_moudle_version() const
{
    return std::string("Version: V1.0.0  \r\n" ) + STT_BUILD_TIME ;
}

std::string STT_Basic_Moudle::get_moudle_describe() const
{
    return "基本功能模块";
}

void STT_Basic_Moudle::reg_fun(int test_id, REG_FUN reg_f, std::map<int, TEST_INFO> &test_info, std::map<string, TEST_FUN_INFO> &test_fun_info, std::map<string, string> &moudles_config, RUN_FUN run_fun) const
{

    IBasicModule::STT_G_Test_Info = test_info;
    IBasicModule::STT_G_Test_Fun_Info = test_fun_info;
    IBasicModule::STT_G_Test_Run_Fun = run_fun;
    IBasicModule::STT_G_Moudles_Config = moudles_config;

    reg_f(test_id, this->getClassName(),   "connect",         D_CONNECT,            connect);
    reg_f(test_id, this->getClassName(),   "close",             D_CLOSE,                  close);
    reg_f(test_id, this->getClassName(),   "send",              D_SEND,                    send);
    reg_f(test_id, this->getClassName(),   "receive",          D_RECEIVE,              receive);
    reg_f(test_id, this->getClassName(),   "ping",               D_PING,                      ping);
    reg_f(test_id, this->getClassName(),   "sleep",             D_SLEEP,                  sleep);
    reg_f(test_id, this->getClassName(),   "find",                D_FIND,                       find);
    reg_f(test_id, this->getClassName(),   "loopchk",         D_LOOPCHK,            loopchk);
    reg_f(test_id, this->getClassName(),   "check",            D_CHECK,                 check);
    reg_f(test_id, this->getClassName(),   "set",                 D_SET,                        set);
    reg_f(test_id, this->getClassName(),   "stringAdd",      D_STRINGADD,        stringAdd);
    reg_f(test_id, this->getClassName(),   "compare",       D_COMPARE,           compare);
    reg_f(test_id, this->getClassName(),   "add",                D_ADD,                       add);
    reg_f(test_id, this->getClassName(),   "loopfind",         D_LOOPFIND,            loopfind);
    reg_f(test_id, this->getClassName(),   "change",          D_CHANGE,               change);
}

bool STT_Basic_Moudle::is_config() const
{
    return false;
}

void STT_Basic_Moudle::show_config() const
{

}



