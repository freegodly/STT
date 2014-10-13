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


#include <interface/module/plugininc.h>
#include <interface/module/modulemacro.h>     // 登记实现类的宏定义
#include <interface/module/pluginimpl.h>      // 实现插件的导出函数

#include "stt_basic_moudle.h"


XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY(STT_Basic_Moudle) // 登记普通实现类或单实例类
    //XDEFINE_CLASSMAP_ENTRY_Singleton(YourSingletonClass)
XEND_DEFINE_MODULE_DLL()            // 插件动态库


OUTAPI bool x3InitializePlugin()    // 插件加载时执行，用于额外初始化
{
    std::cout<<"STT_Basic_Moudle Load"<<std::endl;
    return true;
}

OUTAPI void x3UninitializePlugin()  // 插件卸载时执行，用于释放额外数据
{
}

