#include "form_logshow.h"
#include "ui_form_logshow.h"
#include "stt_global.h"
#include <QWidget>
Form_LogShow::Form_LogShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_LogShow)
{
    ui->setupUi(this);
    loadTestLog();
}


void Form_LogShow::loadTestLog()
{
    //移除原来的
    for(int i = 0; i < ui->LogtabWidget->count() ;i++)
    {
        ui->LogtabWidget->removeTab(i+1);
    }
   //关闭tab
     for (int key : log_map.keys())
     {
         log_map[key]->close();
     }

   log_map.clear();
   // 添加新的logtext
   for ( int i=0; i!=STT_Global::Test_Info_List.size(); ++i )
   {
      Form_LogText *flt = new Form_LogText();
      log_map.insert(STT_Global::Test_Info_List.at(i).test_id,flt);
      ui->LogtabWidget->addTab(flt ,QString("T")+QString::number(STT_Global::Test_Info_List.at(i).test_id));
   }
}

void Form_LogShow::save_log_show(int test_id)
{
    log_map[test_id]->save_log();
}

void Form_LogShow::clear_log_show(int test_id)
{
    log_map[test_id]->clearn_log();
}

void Form_LogShow::add_platform_log(QString log)
{
    ui->widget_platform->append_log(log);
}

Form_LogShow::~Form_LogShow()
{
    delete ui;
}

QSize Form_LogShow::sizeHint() const
{
      return QSize(this->width(), this->height());
}
