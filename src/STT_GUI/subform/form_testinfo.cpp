#include "form_testinfo.h"
#include "ui_form_testinfo.h"

Form_TestInfo::Form_TestInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_TestInfo)
{
    ui->setupUi(this);
}

Form_TestInfo::~Form_TestInfo()
{
    delete ui;
}
