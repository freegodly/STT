#include "form_helper.h"
#include "ui_form_helper.h"
#include "stt_global.h"
#include <QStringListModel>
#include <QAbstractItemModel>
#include <QStringList>
#include <vector>
#include <string>
#include <map>
Form_Helper::Form_Helper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Helper)
{
    ui->setupUi(this);

    ui->gridLayout->setColumnStretch(0,1);
    ui->gridLayout->setColumnStretch(1,5);
    ui->gridLayout->setRowStretch(0,1);
    ui->gridLayout->setRowStretch(1,2);


    QStringList moudles;

    for(int i=0; i < STT_Global::PlugsList.size(); i++)
    {
        moudles.append(STT_Global::PlugsList[i].c_str());
    }
    moudles.append("STT_MAIN");
    QStringListModel *mod=new QStringListModel(moudles);

    ui->listView_moudles->setModel(mod);

}

Form_Helper::~Form_Helper()
{
    delete ui;
}

void Form_Helper::on_listView_moudles_clicked(const QModelIndex &index)
{
    QString item = index.data().toString();
    x3::Object<IBasicModule> test(item.toStdString().c_str());
    if (test)
    {
        ui->plainTextEdit_moudle->clear();
        ui->plainTextEdit_moudle->appendPlainText(test->get_moudle_version().c_str());
        ui->plainTextEdit_moudle->appendPlainText(test->get_moudle_describe().c_str());

    }

    //G_Test_Fun_Info
    QStringList funs;
    std::map<std::string,TEST_FUN_INFO>::iterator iter;
    for(iter = G_Test_Fun_Info.begin(); iter != G_Test_Fun_Info.end(); iter++)
    {
        TEST_FUN_INFO tf= iter->second;
        if(item.compare(tf.modle_name.c_str()) == 0 )
        {
            funs.append(tf.fun_name.c_str());
        }
    }
    QStringListModel *mod=new QStringListModel(funs);
    ui->listView_funs->setModel(mod);

}

void Form_Helper::on_listView_funs_clicked(const QModelIndex &index)
{
     QString item = index.data().toString();

     std::map<std::string,TEST_FUN_INFO>::iterator iter;
     for(iter = G_Test_Fun_Info.begin(); iter != G_Test_Fun_Info.end(); iter++)
     {
         TEST_FUN_INFO tf= iter->second;
         if(item.compare(tf.fun_name.c_str()) == 0 )
         {
             ui->plainTextEdit_funs->clear();
             ui->plainTextEdit_funs->appendPlainText(tf.fun_describe.c_str());
             break;
         }
     }
}
