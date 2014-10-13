#include "form_loading.h"
#include "ui_form_loading.h"
#include <QMovie>
#include <QThread>
#include <time.h>
#include <QMessageBox>

Form_Loading::Form_Loading(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Loading)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//设置窗体无边框
    is_load_success = false;
    error_exit = false;

    loadThread = new LoadingThread();
    ui->progressBar->setValue(0);

    //add SIGNAL
    connect(this, SIGNAL(signal_show_label(QString )), this, SLOT(show_label(QString )),Qt::QueuedConnection);
    //add SIGNAL
    connect(this, SIGNAL(signal_process(int  )), this, SLOT(process(int  )),Qt::QueuedConnection);
}

Form_Loading::~Form_Loading()
{
    delete ui;
}


void Form_Loading::MainSleep(unsigned msec)
{

    clock_t start = clock();
    while(loadThread->state == 0)
    {
        clock_t diff = clock()-start;
        if(diff > msec) return;
        qApp->processEvents();
        if(error_exit)
        {
            break;
        }
    }

}

void Form_Loading::start()
{
    QMovie mv(":/image/loading2.gif");
    ui->label_movie->setMovie (&mv);
    mv.start ();

    //TODO  加载资源线程
    loadThread->start();
    MainSleep(1000000);
    STT_Global::mw = new MainWindow();
    if(!error_exit && loadThread->state == 1)
    {
        is_load_success = true;
    }


}

void Form_Loading::closeEvent(QCloseEvent *event)
{
    error_exit = true;
}

void Form_Loading::on_label_movie_destroyed()
{
}

void Form_Loading::show_label(QString text)
{
    ui->label_log->setText(text);
}

void Form_Loading::process(int value)
{
    ui->progressBar->setValue(value);
}



void LoadingThread::run()
{
    state = 0;
    //这个要花些时间 载入资源


    emit STT_Global::fl->signal_show_label(tr("正在扫描模块目录..."));
    ///
    /// \brief STT_Global::LoadPlugs
    ///  加载插件
    ///
    STT_Global::LoadPlugs();
    //msleep(1000);
    emit STT_Global::fl->signal_process(20);
    emit STT_Global::fl->signal_show_label(tr("正在获取模块信息..."));

    lua_State* L;
    L = lua_open();
    luaopen_base(L);
//    luaopen_table(L);     // 加载table库
//     //luaopen_io(L);           // 加载IO库
//     luaopen_string(L);    // 加载string库
//     luaopen_math(L);     // 加载math库

//    lua_pushcfunction(L, luaopen_base); /* 间接调用luaopen_base */
//    lua_pushstring(L, "");
//    lua_call(L, 1, 0);
//    lua_pushcfunction(L, luaopen_math); /* 间接调用luaopen_math */
//    lua_pushstring(L, LUA_MATHLIBNAME);
//    if (lua_pcall(L, 1, 0, 0) != 0)
//     throw "EXITAPP";
//    if (lua_cpcall(L, luaopen_io, NULL) != 0) /* 间接调用luaopen_io */
//     throw "EXITAPP";
//    if (lua_cpcall(L, luaopen_os, NULL) != 0)
//     throw "EXITAPP";
//    if (lua_cpcall(L, luaopen_package, NULL) != 0)
//     throw "EXITAPP";
//    if (lua_cpcall(L, luaopen_string, NULL) != 0)
//     throw "EXITAPP";
//    if (lua_cpcall(L, luaopen_table, NULL) != 0)
//     throw "EXITAPP";

    //平台 -1索引
    G_STT_Interpreter.insert(std::pair<int,lua_State*>(-1,L));

    //创建信息表
    if( G_Test_Info.find(-1) != G_Test_Info.end())  G_Test_Info.erase(-1);
    TEST_INFO tfi;
    tfi.id = -1;
    G_Test_Info.insert(std::pair<int,TEST_INFO>(-1,tfi));

    //注册平台的命令
    STT_Global::Reg_Fun(-1);

    //注册模块指令
    for(int i=0; i < STT_Global::PlugsList.size(); i++)
    {
        x3::Object<IBasicModule> test(STT_Global::PlugsList[i].c_str());     // 给定类ID创建对象
        if (test)
        {
            char  loadinfo[1024];
            sprintf(loadinfo,"The plugin is loaded (%s in %s).\n",  test->getInterfaceName(), test->getClassName());
            emit STT_Global::fl->signal_show_label(loadinfo);

            test->reg_fun(-1,G_STT_Reg_Fun,G_Test_Info,G_Test_Fun_Info,G_Moudles_Config,G_STT_Run_Fun);
        }
       //msleep(1000);
       emit STT_Global::fl->signal_process(20+ 80 * i / STT_Global::PlugsList.size());
    }

    //加载配置
    STT_Global::LoadConfig();

    //初始化测试块
    STT_Global::InitTestBlock();



    emit STT_Global::fl->signal_process(100);
    emit STT_Global::fl->signal_show_label(tr("加载完成"));
    //msleep(500);
    state=1;
}
