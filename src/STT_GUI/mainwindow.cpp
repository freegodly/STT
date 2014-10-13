#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    form_PlatformConfigurationStack =NULL;
    form_ScriptEditStack = NULL;
    form_Helper = NULL;

    ui->mainToolBar->setWindowTitle(tr("快速工具"));

    form_TestStack = new Form_TestStack(ui->CenterstackedWidget);
    ui->CenterstackedWidget->addWidget(form_TestStack);
    ui->CenterstackedWidget->setCurrentWidget(form_TestStack);


    dock_ShowTestItem = new QDockWidget(tr("Test Item"), this);
    dock_ShowTestItem->setAllowedAreas( Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea  );
    dock_ShowTestItem->setFeatures(QDockWidget::AllDockWidgetFeatures );
    dock_ShowTestItem->setSizePolicy (QSizePolicy::Expanding,QSizePolicy::Expanding);
    form_TestItem = new Form_TestItem();
    dock_ShowTestItem->setWidget( form_TestItem);
    dock_ShowTestItem->raise();

    dock_ShowTestInfo = new QDockWidget(tr("Test Info"), this);
    dock_ShowTestInfo->setAllowedAreas( Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea  );
    dock_ShowTestInfo->setFeatures(QDockWidget::AllDockWidgetFeatures );
    dock_ShowTestInfo->setSizePolicy (QSizePolicy::Expanding,QSizePolicy::Expanding);
    form_TestInfo = new Form_TestInfo();
    dock_ShowTestInfo->setWidget( form_TestInfo);
    dock_ShowTestInfo->raise();

    dock_ShowLog = new QDockWidget(tr("Show Log"), this);
    dock_ShowLog->setAllowedAreas( Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea  );
    dock_ShowLog->setFeatures(QDockWidget::AllDockWidgetFeatures );
    dock_ShowLog->setSizePolicy (QSizePolicy::Expanding,QSizePolicy::Expanding);
    form_LogShow = new Form_LogShow();
    dock_ShowLog->setWidget( form_LogShow);
    dock_ShowLog->raise();



    addDockWidget(Qt::LeftDockWidgetArea, dock_ShowTestItem);
    addDockWidget(Qt::LeftDockWidgetArea, dock_ShowTestInfo);
    addDockWidget(Qt::BottomDockWidgetArea, dock_ShowLog);


    setCentralWidget(ui->CenterstackedWidget);


    // statebar
    softversion = new QLabel(tr(STT_Global::version .c_str()),this);
    ui->statusBar->addPermanentWidget(softversion);

    TestStateInfo = new QLabel(tr("测试状态: 未开始测试"),this);
    ui->statusBar->addWidget(TestStateInfo,1);

    TestTotalTime = new QLabel(tr("总测试时间: 00:00:00"),this);
    ui->statusBar->addWidget(TestTotalTime,2);



    //add SIGNAL
    connect(this, SIGNAL(signal_add_Log(int ,QString )), this, SLOT(add_Log(int ,QString )),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_update_time_show(QString )), this, SLOT(update_time_show(QString )),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_item_time_show(int ,bool )), this, SLOT(item_time_show(int,bool )),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_item_info_show(int  , QString ,QColor)), this, SLOT(item_info_show(int  , QString ,QColor)),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_item_mac_show(int  , QString ,QColor)), this, SLOT(item_mac_show(int  , QString ,QColor)),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_item_tip_show(int  , QString ,QColor)), this, SLOT(item_tip_show(int  , QString ,QColor)),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_save_log_show(int )), this, SLOT(save_log_show(int )),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_clearn_log_show(int )), this, SLOT(clearn_log_show(int )),Qt::QueuedConnection);
    connect(this, SIGNAL(signal_init_test_block(int )), this, SLOT(init_test_block(int )),Qt::QueuedConnection);
     connect(this, SIGNAL(signal_all_test_finsh(QString )), this, SLOT(all_test_finsh(QString )),Qt::QueuedConnection);

    SMT = new STT_Manage_Thread(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Switch_StackWindow(QWidget *w)
{
    ui->CenterstackedWidget->setCurrentWidget(w);


    if(w ==  form_TestStack )
    {
       form_TestStack->loadTestBlock();
       form_LogShow->loadTestLog();
       form_TestItem->LoadItem();
    }
}

void MainWindow::on_action_start_test_triggered()
{
    form_LogShow->loadTestLog();
    form_TestStack->loadTestBlock();
    SMT->state = Start;
    QString start=tr("Start Test  --- ")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    emit signal_add_Log(-1,start.toStdString().c_str());
    TestStateInfo->setText(tr("测试状态: 测试中..."));
    SMT->start();

}

void MainWindow::on_action_stop_test_triggered()
{
    SMT->state = UserStop;
    TestStateInfo->setText(tr("测试状态: 测试停止"));
    QString start=tr("Stop  Test  --- ")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    emit signal_add_Log(-1,start.toStdString().c_str());
}


void MainWindow::all_test_finsh(QString stateinfo)
{
    TestStateInfo->setText(stateinfo);
    QString start=tr("Finsh  Test  --- ")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    emit signal_add_Log(-1,start.toStdString().c_str());
}


void MainWindow::on_action_plaf_set_triggered()
{
    if(form_PlatformConfigurationStack == NULL)
    {
        form_PlatformConfigurationStack = new Form_PlatformConfiguration();
    }

    ui->CenterstackedWidget->addWidget(form_PlatformConfigurationStack);
    ui->CenterstackedWidget->setCurrentWidget(form_PlatformConfigurationStack);
}

void MainWindow::update_time_show(QString totaltime)
{
    TestTotalTime->setText(tr("总测试时间:") + totaltime);
}

void MainWindow::item_time_show(int test_id, bool state)
{
    form_TestStack->TimeShow(test_id,state);
}

void MainWindow::item_info_show(int test_id, QString iteminfo, QColor color)
{
     form_TestStack->Item_Show(test_id,1,iteminfo,color);
}

void MainWindow::item_mac_show(int test_id, QString mac, QColor color)
{
      form_TestStack->Item_Show(test_id,2,mac,color);
}

void MainWindow::item_tip_show(int test_id, QString iteminfo, QColor color)
{
      form_TestStack->Item_Show(test_id,3,iteminfo,color);
}

void MainWindow::save_log_show(int test_id)
{
    form_LogShow->save_log_show(test_id);
}

void MainWindow::clearn_log_show(int test_id)
{
    form_LogShow->clear_log_show(test_id);
}

void MainWindow::init_test_block(int test_id)
{
    form_TestStack->init_block(test_id);
}



///
/// \brief MainWindow::add_Log
/// \param test_id    如果是-1 择添加平台log
/// \param log
///  添加 log
///
void MainWindow::add_Log(int test_id, QString log)
{
    if(test_id== -1)
    {
        form_LogShow->add_platform_log(log);
    }
    else
        form_LogShow->log_map[test_id]->append_log(log);
}

void MainWindow::on_action_hight_set_triggered()
{
    if(form_ScriptEditStack == NULL)
    {
        form_ScriptEditStack = new Form_ScriptEdit();
    }

    ui->CenterstackedWidget->addWidget(form_ScriptEditStack);
    ui->CenterstackedWidget->setCurrentWidget(form_ScriptEditStack);
}

void MainWindow::on_action_help_triggered()
{

    if(form_Helper == NULL)
    {
        form_Helper = new Form_Helper();
    }

    form_Helper->show();
}
