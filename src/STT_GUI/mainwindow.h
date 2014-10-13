#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include "subform/form_testitem.h"
#include "subform/form_logshow.h"
#include "subform/form_testinfo.h"
#include "subform/form_teststack.h"
#include "subform/form_platformconfiguration.h"
#include "subform/form_helper.h"
#include "stt_global.h"
#include <QListView>
#include <QLabel>
#include "stt_test/stt_manage_thread.h"
#include "subform/form_scriptedit.h"
#include <iostream>
#include <QColor>

using namespace std;

namespace Ui {
class MainWindow;
}

class STT_Manage_Thread;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



    void Switch_StackWindow(QWidget * w);




private slots:
    void on_action_start_test_triggered();

    void on_action_stop_test_triggered();

    void on_action_plaf_set_triggered();


    void on_action_hight_set_triggered();

    void on_action_help_triggered();

signals:
        void signal_add_Log(int test_id,QString log);
        void signal_update_time_show(QString totaltime);
        void signal_item_time_show(int  test_id, bool state);
        void signal_item_info_show(int  test_id, QString iteminfo,QColor color );
        void signal_item_mac_show(int  test_id, QString mac,QColor color);
        void signal_item_tip_show(int  test_id, QString iteminfo,QColor color );
        void signal_save_log_show(int  test_id);
        void signal_clearn_log_show(int test_id);
        void signal_init_test_block(int test_id);

        void signal_all_test_finsh(QString stateinfo);

public slots:
        void add_Log(int test_id, QString log);
        void update_time_show(QString totaltime);
        void item_time_show(int  test_id,bool state);
        void item_info_show(int  test_id, QString iteminfo,QColor color );
        void item_mac_show(int  test_id, QString mac,QColor color );
        void item_tip_show(int  test_id, QString iteminfo,QColor color );
        void save_log_show(int  test_id);
        void clearn_log_show(int test_id);
        void init_test_block(int test_id);

        void all_test_finsh(QString stateinfo);


private:
     Ui::MainWindow *ui;
     QDockWidget *dock_ShowTestItem;
     Form_TestItem    * form_TestItem;
     QDockWidget *dock_ShowLog;
     Form_LogShow   * form_LogShow;
     QDockWidget *dock_ShowTestInfo;
     Form_TestInfo   * form_TestInfo;
     Form_Helper    *form_Helper;

     //显示的gui版本信息
     QLabel *softversion;
     QLabel *TestStateInfo;
     QLabel *TestTotalTime;

  public:
     //
     Form_TestStack   * form_TestStack;
     Form_PlatformConfiguration  *form_PlatformConfigurationStack;
     Form_ScriptEdit   *form_ScriptEditStack;
     ///
     /// \brief SMT
     ///测试管理线程
     ///
     STT_Manage_Thread  *SMT;
};

#endif // MAINWINDOW_H
