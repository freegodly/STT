#include "form_logtext.h"
#include "ui_form_logtext.h"

Form_LogText::Form_LogText(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_LogText)
{
    ui->setupUi(this);
    //关闭则释放资源
     this->setAttribute(Qt::WA_DeleteOnClose,true);
}

Form_LogText::~Form_LogText()
{
    delete ui;
}

void Form_LogText::append_log(QString log)
{
    ui->plainTextEdit->appendPlainText(log);
}

void Form_LogText::save_log()
{

}

void Form_LogText::clearn_log()
{
    ui->plainTextEdit->clear();
}
