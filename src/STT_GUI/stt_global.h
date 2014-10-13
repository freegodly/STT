#ifndef STT_GLOBAL_H
#define STT_GLOBAL_H
#include <iostream>
#include "mainwindow.h"
#include "form_loading.h"
#include "3rd/lua_tinker/lua_tinker.h"
#include "stt_interface/Basic_Module_Interface.h"
#include <QMap>
#include <QList>
#include <QReadWriteLock>
#include "subform/form_testblock.h"
#include "stt_test/stt_test_thread.h"
#include <QStringList>
#include <list>
#include <vector>
#include <QDomNode>
#include <QDomDocument>
#include <QDomElement>
#include <QSettings>
#include <list>

using namespace std;
/* ############################################################################################################# */
///
/// \brief G_STT_Interpreter
///
extern std::map<int,lua_State *> G_STT_Interpreter;

///
/// \brief G_Test_Info
///
extern std::map<int,TEST_INFO> G_Test_Info;

///
/// \brief G_Test_Fun_Info
///
extern std::map<std::string,TEST_FUN_INFO>  G_Test_Fun_Info;

///
/// \brief G_Moudles_Config
///
extern std::map<std::string,std::string>     G_Moudles_Config;



/* ############################################################################################################# */

void G_STT_Reg_Fun(int test_id,const char * moudle_name,const char * fun_name ,const char * fun_describe,FUN f );

bool G_STT_Run_Fun(const char * fun_name, int test_id,const  char * v1 ="",const  char * v2="",const  char * v3="",const  char * v4="", const  char * v5="",const  char * v6="",const char * v7="",const  char * v8="",const  char * v9="");

/* ############################################################################################################# */

bool stt_print (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


/* ############################################################################################################# */

class STT_Test_Thread;

///
/// \brief The Test_Block_Info struct
///  测试线程的信息  和 gui显示信息
///
struct Test_Block_Info
{
     int                             test_id;
     STT_Test_Thread      *test_thread;
};


///
/// \brief The Test_Script_Info struct
///主测试脚本值的保存
///
struct Test_Item_Info
{
     std::string ItemName;
     std::list<std::string> KeysInfo;
};



class MainWindow;
class Form_Loading;
class STT_Global
{
public:
    STT_Global();
    ///
    /// \brief LoadPlugs
    ///加载外部插件
    ///
    static void LoadPlugs();
    static void GetPlugsName();

    ///
    /// \brief PlugsList
    ///测试用到的模块
    ///
    static vector<std::string> PlugsList;

    static std::string version;

    static QSettings *TestItem;

    static std::map<std::string,Test_Item_Info> Test_Script_Info;

    static MainWindow *mw;
    static Form_Loading *fl;

   ///
   /// \brief Test_Info_List
   /// 多线程中的使用
   ///
   static QList<Test_Block_Info>  Test_Info_List;
   // static QReadWriteLock Test_Info_List_Lock;

   static void Reg_Fun(int test_id);


   static QDomNode FindXml(QDomDocument &doc, QString nodename);

   static QString GetValueXml(QDomDocument &doc,QString nodename, QString att);

   static void SaveConfig();

   static void LoadConfig();

   static void InitTestBlock();

   static void LoadTestScriptInfo();

};

#endif // STT_GLOBAL_H
