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


#include <QCoreApplication>
#include <QTextCodec>

#include "3rd/lua_tinker/lua_tinker.h"
#include "stt_interface/Basic_Module_Interface.h"
#include "stt_global.h"

#include <iostream>
#include <stdio.h>
using namespace std;





#include <portability/x3port.h>
//#include <nonplugin/useplugins.h>

//#include <portability/x3port.h>
#include <nonplugin/scanplugins.h>







#include <time.h>

std::string data=__DATE__;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));





     lua_State* L;
     L = lua_open();
    luaopen_base(L);
    G_STT_Interpreter.insert(std::pair<int,lua_State*>(1,L));
    STT_Global::Reg_Fun(1);




    const char* plugins[] = {
        "x3manager.pln", "STT_Basic_Moudle.pln", NULL
    };
    x3::loadScanPlugins();
     // x3::AutoLoadPlugins autoload(plugins, "plugins");
    const char* const clsidSimple = "STT_Basic_Moudle";
    x3::Object<IBasicModule> test(clsidSimple);     // 给定类ID创建对象


    if (test)
    {
        printf("The plugin is loaded (%s in %s).\n",  test->getInterfaceName(), test->getClassName());

        test->reg_fun(1,G_STT_Reg_Fun,G_Test_Info,G_Test_Fun_Info,G_STT_Run_Fun);
    }
    else
    {
        printf("The plugin is not loaded.\n");
        //return 1;
    }




    uchar data[4] ={0x41,0xd8,0x00,0x00};
    float da = 27.5;
    float * p ;
    p=&da;
    uchar *pp ;
    pp = (uchar *)p;

    printf("%x",*(pp));  // 00
    printf("%x",*(pp+1));   // 00
    printf("%x",*(pp+2));   //dc
    printf("%x",*(pp+3));  //41


    while(true)
    {
        std::string cmd;
        getline(cin,cmd);


        lua_tinker::dostring(G_STT_Interpreter[1],cmd.c_str());

        // cout<<cmd;

    }

    //lua_tinker::def(L, "cpp_func", cpp_func);

    // lua_tinker::dofile(L, "sample1.lua");

    // int result = lua_tinker::call<int>(L, "lua_func", 3, 4);




    return a.exec();
}
