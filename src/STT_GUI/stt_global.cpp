#include "stt_global.h"
#include <QDir>
#include <QDebug>
#include <QRegExp>


std::string STT_Global::version =std::string("Soft Version: V1.0.0 , " ) + std::string("Build Time: ")+std::string(__TIME__)+std::string(" ")+std::string(__DATE__);

MainWindow      *STT_Global::mw         =     NULL;
Form_Loading   *STT_Global::fl             =     NULL;
QSettings           *STT_Global::TestItem =     NULL;
vector<std::string>  STT_Global::PlugsList;
std::map<std::string,Test_Item_Info>   STT_Global::Test_Script_Info;
QList<Test_Block_Info>  STT_Global::Test_Info_List;


/* ############################################################################################################# */
// 来自 Basic_Module_Interface.h 文件全局数据的定义
/************************************/

///
/// \brief G_Test_Info
///
std::map<int,TEST_INFO> G_Test_Info;

///
/// \brief G_Test_Fun_Info
///
std::map<std::string,TEST_FUN_INFO>  G_Test_Fun_Info;


///
/// \brief G_Moudles_Config
///
std::map<std::string,std::string>     G_Moudles_Config;


/* ############################################################################################################# */


///
/// \brief G_STT_Interpreter [STT解释器]
///
std::map<int,lua_State *> G_STT_Interpreter;


/* ############################################################################################################# */

void G_STT_Reg_Fun(int test_id,const char * moudle_name,const char * fun_name ,const char * fun_describe,FUN f )
{

    if(G_Test_Fun_Info.find(fun_name)==G_Test_Fun_Info.end())
    {
        TEST_FUN_INFO tfi={moudle_name,fun_name,fun_describe};
        G_Test_Fun_Info.insert(std::pair<std::string,TEST_FUN_INFO>(fun_name,tfi));
    }

    lua_tinker::def(G_STT_Interpreter[test_id], fun_name, f);
}

bool G_STT_Run_Fun(const char * fun_name, int test_id,const  char * v1,const  char * v2,const  char * v3,const  char * v4, const  char * v5,const  char * v6,const char * v7,const  char * v8,const  char * v9)
{
    bool flag =false;
    //qDebug()<<QString(fun_name);

    lua_pushcclosure(G_STT_Interpreter[test_id], lua_tinker::on_error, 0);
    int errfunc = lua_gettop(G_STT_Interpreter[test_id]);

    lua_pushstring(G_STT_Interpreter[test_id], fun_name);
    lua_gettable(G_STT_Interpreter[test_id], LUA_GLOBALSINDEX);
    if(lua_isfunction(G_STT_Interpreter[test_id],-1))
    {
        lua_tinker::push(G_STT_Interpreter[test_id], test_id);
        lua_tinker::push(G_STT_Interpreter[test_id], v1);
        lua_tinker::push(G_STT_Interpreter[test_id], v2);
        lua_tinker::push(G_STT_Interpreter[test_id], v3);
        lua_tinker::push(G_STT_Interpreter[test_id], v4);
        lua_tinker::push(G_STT_Interpreter[test_id], v5);
        lua_tinker::push(G_STT_Interpreter[test_id], v6);
        lua_tinker::push(G_STT_Interpreter[test_id], v7);
        lua_tinker::push(G_STT_Interpreter[test_id], v8);
        lua_tinker::push(G_STT_Interpreter[test_id], v9);
        if(lua_pcall(G_STT_Interpreter[test_id], 10, 1, errfunc) != 0)
        {
            lua_pop(G_STT_Interpreter[test_id], 1);
            flag =false;
        }
         flag =true;
    }
    else
    {
        flag =false;
        //print_error(L, "lua_tinker::call() attempt to call global `%s' (not a function)", fun_name);
        G_STT_Run_Fun("stt_print",test_id,fun_name,"命令未发现","","","","","","","");
    }

   lua_remove(G_STT_Interpreter[test_id], -2);


   return flag;
    //return  lua_tinker::call<bool,int,const char *,const char *,const char *,const char *,const char *,const char *,const char *,const char *,const char *>(G_STT_Interpreter[test_id],fun_name,test_id,v1,v2,v3,v4,v5,v6,v7,v8,v9);
}


/* ############################################################################################################# */
bool stt_print(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
    std::string log =std::string(arg1)+ std::string(arg2)+ std::string(arg3)+ std::string(arg4)+ std::string(arg5)+std::string(arg6)+ std::string(arg7)+ std::string(arg8)+ std::string(arg9);
    emit STT_Global::mw->signal_add_Log(test_id, log.c_str());
    return true;
}

/* ############################################################################################################# */



#include <portability/x3port.h>
#include <nonplugin/scanplugins.h>
//#include <portability/x3port.h>
//#include <nonplugin/useplugins.h>

STT_Global::STT_Global()
{


}

void STT_Global::LoadPlugs()
{
    GetPlugsName();
    x3::loadScanPlugins();

}

void STT_Global::GetPlugsName()
{
    QDir dir("plugins");
    if(!dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();


    int file_count = list.count();
    if(file_count <= 0)
    {
        return;
    }

    for(int i=0; i < file_count ;i++)
    {
        QFileInfo file_info = list.at(i);
        QString suffix = file_info.suffix();
        if(QString::compare(suffix, QString("pln"), Qt::CaseInsensitive) == 0)
        {
            if(file_info.baseName() != "x3manager")
            {
                STT_Global::PlugsList.push_back( file_info.baseName().toStdString());
                qDebug()<< file_info.baseName();
            }
        }
    }


}

void STT_Global::Reg_Fun(int test_id)
{
    //添加自定义参数
    QDomDocument doc_config;
    doc_config.setContent(QString(G_Moudles_Config["PLATFORM"].c_str()));
    // ip tables      IP_TABLE_%d
    QDomNode node = STT_Global::FindXml(doc_config,"table_Block");
    if( !node.isNull() )
    {
        int Value_R =  node.attributes().namedItem("Value_R").nodeValue().toInt();
        int Value_C =  node.attributes().namedItem("Value_C").nodeValue().toInt();
        for(int i =0 ; i<Value_R ;i++)
        {
            QDomNode item= node.childNodes().at(i);
            for(int j=0 ;j < Value_C ; j++)
            {
                QString Key = "IP_TABLE_"+item.attributes().namedItem("C"+QString::number(0)).nodeValue();
                QString Value = item.attributes().namedItem("C"+QString::number(1)).nodeValue();
                G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> (Key.toStdString(),Value.toStdString()));

                Key = "SERIAL_PORT_TABLE_"+item.attributes().namedItem("C"+QString::number(0)).nodeValue();
                Value = item.attributes().namedItem("C"+QString::number(2)).nodeValue();
                G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> (Key.toStdString(),Value.toStdString()));
            }
        }
    }

    // label 标贴
    node = STT_Global::FindXml(doc_config,"groupBox_label");
    if(!node.isNull() )
    {
        if(node.attributes().namedItem("Value").nodeValue() == "1" )
        {
                //TODO
        }
    }

    //Telnet 串口登录信息
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("TELNET_PORT", \
                                                                                  STT_Global::GetValueXml(doc_config,"TELNET_PORT","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("TELNET_PROMPT", \
                                                                                  STT_Global::GetValueXml(doc_config,"TELNET_PROMPT","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("TELNET_ACCOUNT", \
                                                                                  STT_Global::GetValueXml(doc_config,"TELNET_ACCOUNT","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("TELNET_PASSWORD_PROMPT", \
                                                                                  STT_Global::GetValueXml(doc_config,"TELNET_PASSWORD_PROMPT","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("TELNET_PASSWORD", \
                                                                                  STT_Global::GetValueXml(doc_config,"TELNET_PASSWORD","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("TELNET_SUCCESS_PROMPT", \
                                                                                  STT_Global::GetValueXml(doc_config,"TELNET_SUCCESS_PROMPT","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("SERIAL_RATE", \
                                                                                  STT_Global::GetValueXml(doc_config,"SERIAL_RATE","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("SERIAL_PROMPT", \
                                                                                  STT_Global::GetValueXml(doc_config,"SERIAL_PROMPT","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("SERIAL_ACCOUNT", \
                                                                                  STT_Global::GetValueXml(doc_config,"SERIAL_ACCOUNT","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("SERIAL_PASSWORD_PROMPT", \
                                                                                  STT_Global::GetValueXml(doc_config,"SERIAL_PASSWORD_PROMPT","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("SERIAL_PASSWORD", \
                                                                                  STT_Global::GetValueXml(doc_config,"SERIAL_PASSWORD","Value") .toStdString()));
     G_Test_Info[test_id].variable_map.insert(std::pair<std::string,std::string> ("SERIAL_SUCCESS_PROMPT", \
                                                                                  STT_Global::GetValueXml(doc_config,"SERIAL_SUCCESS_PROMPT","Value") .toStdString()));

    // 注册函数
    G_STT_Reg_Fun(test_id,"STT_MAIN","stt_print","log打印消息",stt_print);
}



QDomNode STT_Global::FindXml(QDomDocument &doc, QString nodename)
{
    QDomNode node;
    QDomElement root = doc.documentElement();
    QDomNodeList  li = root.childNodes();
    for(int i=0; i<li.count(); i++)
    {
        node= li.at(i);
        if(  node.nodeName() == nodename )
        {
            return node;
        }
    }
    return node;
}

QString STT_Global::GetValueXml(QDomDocument &doc,QString nodename, QString att)
{
    QDomNode node = FindXml(doc,QString(nodename));
    if(!node.isNull() )
    {
        return node.attributes().namedItem(att).nodeValue();
    }
    return "";
}

void STT_Global::SaveConfig()
{
    QDomDocument doc_config;
    QDomElement qe= doc_config.createElement("root");
    doc_config.appendChild(qe);
    std::map<string,std::string>::iterator it;
    for(it=G_Moudles_Config.begin();it!=G_Moudles_Config.end();++it)
    {
        QDomElement cf= doc_config.createElement(QString(it->first.c_str()));
        cf.setAttribute("Value",QString(it->second.c_str()));
        qe.appendChild(cf);
    }
    QFile filexml("config.xml");
    if( filexml.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream ts(&filexml);
        ts.reset();
        ts.setCodec("utf-8");
        doc_config.save(ts, 4, QDomNode::EncodingFromTextStream);
        filexml.close();
    }

}

void STT_Global::LoadConfig()
{
    QDomDocument doc_config;
    QFile filexml("config.xml");
    if( filexml.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        doc_config.setContent(QString(filexml.readAll()));
        filexml.close();
    }

    QDomElement node =  doc_config.firstChildElement("root");

    if( !node.isNull() )
    {
        G_Moudles_Config.clear();
        for(int i=0; i < node.childNodes().length() ;i++ )
        {
            QDomNode ch =  node.childNodes().at(i);
            G_Moudles_Config.insert(std::pair<std::string,std::string>(ch.nodeName().toStdString(),ch.attributes().namedItem("Value").nodeValue().toStdString()));
        }
    }
}

void STT_Global::InitTestBlock()
{
    QDomDocument doc_config;
    doc_config.setContent(QString(G_Moudles_Config["PLATFORM"].c_str()));

    QDomNode node = STT_Global::FindXml(doc_config,"table_Block");
    if( !node.isNull() )
    {

        STT_Global::Test_Info_List.clear();

        int Value_R =  node.attributes().namedItem("Value_R").nodeValue().toInt();
        int Value_C =  node.attributes().namedItem("Value_C").nodeValue().toInt();

        for(int i =0 ; i<Value_R ;i++)
        {
            Test_Block_Info tb;

            QDomNode item= node.childNodes().at(i);

            tb.test_id = item.attributes().namedItem("C0").nodeValue().toInt();
            tb.test_thread=NULL;

            STT_Global::Test_Info_List.append(tb);

        }

    }
}

void STT_Global::LoadTestScriptInfo()
{
    QDomDocument doc_config ;
    STT_Global::Test_Script_Info.clear();
    doc_config.setContent(QString(G_Moudles_Config["SCRIPT"].c_str()));
    QDomNode node = STT_Global::FindXml(doc_config,"main");
    if(!node.isNull())
    {
        QTextStream qts(STT_Global::GetValueXml(doc_config,"main","Value").toLocal8Bit());
        while(!qts.atEnd())
        {
            QString s = qts.readLine();

            QRegExp rx("^--.*");
            bool match = rx.exactMatch(s);     //判断此行是否是注释
            if( match )  continue ;

            rx.setPattern("^\\[(.+)\\]");
            match = rx.exactMatch(s);     //判断此行是否是节
            if(match)
            {
                QString sesionname = rx.cap(1);
                //qDebug()<<sesionname;
                std::list<std::string> cmds;

                while(true)
                {
                    if( qts.atEnd())
                    {
                        Test_Item_Info tii;
                        tii.ItemName = sesionname.toStdString();
                        tii.KeysInfo = cmds;
                        STT_Global::Test_Script_Info.insert(std::pair<std::string,Test_Item_Info>(sesionname.toStdString(),tii));
                        break;
                    }
                    s = qts.readLine();
                    rx.setPattern("^--.*");
                    match = rx.exactMatch(s);     //判断此行是否是注释
                    if( match )  continue ;
                    rx.setPattern("^\s*\t*$");
                    match = rx.exactMatch(s);     //判断此行是否是结束

                    if( match )
                    {
                        Test_Item_Info tii;
                        tii.ItemName = sesionname.toStdString();
                        tii.KeysInfo = cmds;
                        STT_Global::Test_Script_Info.insert(std::pair<std::string,Test_Item_Info>(sesionname.toStdString(),tii));
                        break;
                    }
                    //其他则插入
                    cmds.push_back(s.toStdString());
                    //qDebug()<<s;
                }
            }
        }
    }
}
