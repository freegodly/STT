/******************************************************************************************************
* Copyright (C) 2014, All right reserved.

* file   Basic_Module_Interface.h
* version  1.0
* author   NingJian (freegodly@gmail.com)
* brief

* detail
      平台插件接口文件
* TODO
* history  2014-9-17 created by NingJian
*
* note
/******************************************************************************************************/

#ifndef BASIC_MODULE_INTERFACE_H
#define BASIC_MODULE_INTERFACE_H


#include <iostream>
#include <map>
#include <module/plugininc.h>
#include <objptr.h>
#include <iobject.h>


/* ############################################################################################################# */

///
///  > 方便获取软件编译时间
///
#ifndef  STT_BUILD_TIME
#define STT_BUILD_TIME std::string("Build Time: ")+std::string(__TIME__)+std::string(" ")+std::string(__DATE__)
#endif

/* ############################################################################################################# */
///
///  > 定义测试结构信息
///
#ifndef STT_TEST_INFO
#define STT_TEST_INFO

///
/// \brief The TEST_INFO struct
///
struct TEST_INFO
{
    ///
    /// \brief id [ID信息]
    ///
    int id;

    std::string name;
    ///
    /// \brief variable_map  [测试结构的自定义数据存储]
    ///
    std::map<std::string,std::string> variable_map;

};

///
/// \brief The TEST_FUN_INFO struct
///
struct TEST_FUN_INFO
{
    std::string modle_name;
    std::string fun_name;
    std::string fun_describe;
};

#endif

/* ############################################################################################################# */

///
///  > 定义模块函数指针类型
///
#ifndef STT_FUN_REG
#define STT_FUN_REG
///
///   > 要注册的函数的类型  定义 [函数的参数最多为9个]
///
///
typedef  bool  (FUN) (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);

///
///   > 实现注册指令的函数类型 定义
///
typedef  void (*REG_FUN) (int test_id,const char * moudle_name,const char * fun_name ,const char * fun_describe,FUN f );

///
///   > 实现运行指令的函数类型 定义
///
typedef bool (*RUN_FUN)(const char * fun_name, int test_id,const  char * arg1 ,const  char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);

#endif

/* ############################################################################################################# */

class IBasicModule : public x3::IObject
{
    X3DEFINE_IID(IBasicModule);

    ///
    /// \brief reg_fun
    ///  注册命令的函数 需要实现要注册到平台的指令
    ///  保存STT平台传来的函数和结构信息供该类以后调用
    ///
    /// \param rf
    /// 注册指令的平台回调函数指针
    /// 如果模块有自定义数据导入，需要向test_info中添加数据
    ///
    virtual void reg_fun(int test_id,REG_FUN reg_f,std::map<int,TEST_INFO> &test_info,std::map<std::string,TEST_FUN_INFO> &test_fun_info,std::map<std::string,std::string>  &moudles_config,RUN_FUN run_fun ) const = 0;

    ///
    /// \brief get_moudle_version
    /// 获取模块的版本信息
    /// \return
    ///
    virtual  std::string get_moudle_version() const =0;


    ///
    /// \brief is_config
    /// \return
    ///返回模块是否有配置信息
    ///
    virtual bool is_config() const = 0;

    ///
    /// \brief show_config
    /// 显示模块配置信息
    ///
    virtual void show_config() const = 0;


    ///
    /// \brief get_moudle_describe
    /// 获取模块的描述信息
    /// \return
    ///
    virtual  std::string get_moudle_describe() const =0;

public:
    ///
    /// \brief G_Test_Info
    ///
    static std::map<int,TEST_INFO> STT_G_Test_Info;

    ///
    /// \brief G_Test_Fun_Info
    ///
    static std::map<std::string,TEST_FUN_INFO>  STT_G_Test_Fun_Info;


    ///
    /// \brief G_Test_Run_Fun
    ///
    static RUN_FUN STT_G_Test_Run_Fun;

    ///
    /// \brief STT_G_Moudles_Config
    ///
    static std::map<std::string,std::string>     STT_G_Moudles_Config;
};


#endif // BASIC_MODULE_INTERFACE_H
