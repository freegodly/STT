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
#ifndef STT_BASIC_MOUDLE_H
#define STT_BASIC_MOUDLE_H

#include <module/plugininc.h>
#include <module/classmacro.h>

#include <Basic_Module_Interface.h>
#include <iostream>

const char* const clsidSimple = "STT_Basic_Moudle";


class  STT_Basic_Moudle:public IBasicModule
{

    X3BEGIN_CLASS_DECLARE(STT_Basic_Moudle, clsidSimple) // 指定类ID
           X3DEFINE_INTERFACE_ENTRY(IBasicModule)       // 指定实现的接口
    X3END_CLASS_DECLARE()
public:
    STT_Basic_Moudle();

    // IBasicModule interface
public:
    std::string get_moudle_version() const;
    std::string get_moudle_describe() const;
    void reg_fun(int test_id, REG_FUN reg_f, std::map<int, TEST_INFO> &test_info, std::map<std::string, TEST_FUN_INFO> &test_fun_info, std::map<std::string, std::string> &moudles_config, RUN_FUN run_fun) const;
    bool is_config() const;
    void show_config() const;
};

#endif // STT_BASIC_MOUDLE_H
