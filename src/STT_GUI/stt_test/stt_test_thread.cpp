#include "stt_test_thread.h"
#include <portability/x3port.h>
#include <vector>
#include <QDebug>
#include "stt_global.h"
STT_Test_Thread::STT_Test_Thread(int id, QObject *parent) :
    QThread(parent)
{
    test_id = id;
    lua_State* L;
    L = lua_open();
    luaopen_base(L);



    // 关闭原来的
    if( G_STT_Interpreter.find(test_id) != G_STT_Interpreter.end() )
    {
        lua_State *lua = G_STT_Interpreter[test_id];
        G_STT_Interpreter.erase(test_id);
        if( lua != NULL )
        {
            lua_close(lua);
        }
    }

    G_STT_Interpreter.insert(std::pair<int,lua_State*>(test_id,L));

    // //创建信息表
    if( G_Test_Info.find(test_id) != G_Test_Info.end())  G_Test_Info.erase(test_id);
    TEST_INFO tfi;
    tfi.id = test_id;
    G_Test_Info.insert(std::pair<int,TEST_INFO>(test_id,tfi));

    //注册平台的命令
    STT_Global::Reg_Fun(test_id);

    //注册模块指令
    for(int i=0; i < STT_Global::PlugsList.size(); i++)
    {
        x3::Object<IBasicModule> test(STT_Global::PlugsList[i].c_str());     // 给定类ID创建对象
        if (test)
        {
            char  loadinfo[1024];
            sprintf(loadinfo,"The plugin is loaded (%s in %s).\n",  test->getInterfaceName(), test->getClassName());
            emit STT_Global::mw->signal_add_Log(test_id,loadinfo);

            test->reg_fun(test_id,G_STT_Reg_Fun,G_Test_Info,G_Test_Fun_Info,G_Moudles_Config,G_STT_Run_Fun);
        }
        else
        {
            char  loadinfo[1024];
            sprintf(loadinfo,"%s plugin is  load fail.\n", STT_Global::PlugsList[i].c_str());
            emit STT_Global::mw->signal_add_Log(test_id,loadinfo);
        }
    }

    //导入脚本lua函数
    QDomDocument doc_config ;
    doc_config.setContent(QString(G_Moudles_Config["SCRIPT"].c_str()));
    QDomElement node =doc_config.firstChildElement("root");
    if(!node.isNull())
    {
        for(int i =0 ; i<node.childNodes().count() ;i++)
        {

            QDomNode item= node.childNodes().at(i);
            if( item.nodeName() != "main" )
            {
                QString luacmd = item.attributes().namedItem("Value").nodeValue();
                lua_tinker::dostring(G_STT_Interpreter[test_id],luacmd.toLocal8Bit().data());
            }
        }
    }


}



void STT_Test_Thread::get_Test_List()
{
    TestItemList.clear();
    std::map<std::string,Test_Item_Info>::iterator it = STT_Global::Test_Script_Info.find("Testitem");
    if(  it != STT_Global::Test_Script_Info.end())
    {
        std::list<std::string> KeysInfo = it->second.KeysInfo;

        std::list<std::string>::iterator  kit = KeysInfo.begin();
        while(kit != KeysInfo.end())
        {
            TestItemList.append((*kit).c_str());
            kit++;
        }

    }
}

bool STT_Test_Thread::test_init()
{
    get_Test_List();
    if(TestItemList.length() < 1)
    {
        emit STT_Global::mw->signal_item_info_show(test_id,tr("FAIL"),QColor(255,0,0));
        emit STT_Global::mw->signal_item_tip_show(test_id,tr("没有测试项"),QColor(255,0,0));

        stt_print(test_id,"error......","没有测试项","","","","","","","");
        stt_print(-1,"error......","没有测试项","","","","","","","");

        return false;
    }

    // 初始话

    return true;
}

bool STT_Test_Thread::run_test_item(QString itemName)
{
    std::map<std::string,Test_Item_Info>::iterator  it =STT_Global::Test_Script_Info.find(itemName.toStdString());
    if( it == STT_Global::Test_Script_Info.end())
    {
        G_STT_Run_Fun("stt_print",test_id,itemName.toStdString().c_str(),"测试项未发现","","","","","","","");
        return false;
    }
    Test_Item_Info  tii = it->second ;

    std::list<std::string> KeysInfo = tii.KeysInfo;

    for(std::list<std::string>::iterator i = KeysInfo.begin();i != KeysInfo.end(); ++i)
    {
        QString line = QString( (*i).c_str() );

        QStringList args =  line.split("=");
        if(args.count() < 2 )   //至少有一个参数
        {
            G_STT_Run_Fun("stt_print",test_id,args[0].toStdString().c_str(),"参数错误","","","","","","","");
            return false;
        }
        QString  MathName = args[0].trimmed();
        QString  argsline = "";

        for(int i=1;i<args.count();i++)
        {
            argsline +=  args[i];
        }
        args.clear();
        args =  argsline.split(",");

        for(int i =0; i<args.count();i++)
        {
            args[i] = args[i].trimmed();
        }

        int nowlength = args.count();
        for(int i =0 ; i< 9- nowlength;i++)
        {
            args.append("");
        }


        bool result =G_STT_Run_Fun(MathName.toStdString().c_str(),test_id,
                                   args[0].toStdString().c_str(),
                args[1].toStdString().c_str(),
                args[2].toStdString().c_str(),
                args[3].toStdString().c_str(),
                args[4].toStdString().c_str(),
                args[5].toStdString().c_str(),
                args[6].toStdString().c_str(),
                args[7].toStdString().c_str(),
                args[8].toStdString().c_str());
        if(result  == false)
        {
            G_STT_Run_Fun("stt_print",test_id,MathName.toStdString().c_str(),"执行失败","","","","","","","");
            return false;
        }
    }
    return true;
}


void STT_Test_Thread::run()
{
    if(!test_init())  return;

    bool result = true;
    QString info;

    emit STT_Global::mw->signal_init_test_block(test_id);
    emit STT_Global::mw->signal_item_time_show(test_id,true);
    emit STT_Global::mw->signal_clearn_log_show(test_id);



    G_STT_Run_Fun("stt_print",test_id,"测试开始...","","","","","","","","");

    //测试
    for(int i=0;i<TestItemList.count();i++)
    {
        bool re =run_test_item(TestItemList.at(i));
        if(!re)
        {
            info = TestItemList.at(i)+" FAIL";
            result = false;
            break;
        }
    }

    if(!result)
    {
        emit STT_Global::mw->signal_item_info_show(test_id,info.toStdString().c_str(),QColor(255,0,0));
    }
    else
    {
        emit STT_Global::mw->signal_item_info_show(test_id,"PASS",QColor(0,255,0));
    }

    G_STT_Run_Fun("stt_print",test_id,"测试结束","","","","","","","","");
    emit STT_Global::mw->signal_item_time_show(test_id,false);
    emit STT_Global::mw->signal_save_log_show(test_id);
}
