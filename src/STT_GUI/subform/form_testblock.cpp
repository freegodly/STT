#include "form_testblock.h"
#include "ui_form_testblock.h"

#include "stt_global.h"
#include <QPalette>

Form_TestBlock::Form_TestBlock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_TestBlock)
{
    ui->setupUi(this);
    ui->gridLayout->setRowStretch(0,1);
    ui->gridLayout->setRowStretch(1,2);
    ui->gridLayout->setRowStretch(2,1);
    ui->gridLayout->setRowStretch(3,1);
    ui->lcdNumber->display("00:00:00");

    //关闭则释放资源
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update_show_time()));

    //timer->start(1000);
}

Form_TestBlock::~Form_TestBlock()
{
    delete ui;
}

void changeLable(QLabel *l)
{
    //自动调整字体大小
    QSize  newsize = l->size();
    int length =  l->text().length()+1;

    //设置字体为20Point。 那么字体的像素大小是 90 * 20 / 72 约等于 25.
    int point1 =  newsize.width()/length -1;
    int point2 = newsize.height()  -1;
    int point = point1  < point2 ? point1:point2;
    QFont  qf;
    if(point <1) point =1;
    qf.setPixelSize(point);
    l->setFont(qf);
}

void Form_TestBlock::resizeEvent(QResizeEvent *e)
{

    changeLable(ui->label_result);
    changeLable(ui->label_name);
    changeLable(ui->label_tip);
    changeLable(ui->label_mac);
}

void Form_TestBlock::Init(int test_id)
{
    ui->lcdNumber->display("00:00:00");
    ui->label_name->setText(QString("T")+QString::number(test_id));
    ui->label_result->setText(tr("---------"));
    ui->label_mac->setText(tr("--------"));
    ui->label_tip->setText(tr("--------"));
    run_time = 0;
}

void Form_TestBlock::setTimer(int test_id, bool state)
{
    if(state)
    {
        timer->start(1000);
        run_time = 0;
    }
    else
    {
        //更新显示时间
        int hour = run_time/60/60;
        int mi =  run_time/60%60;
        int sec = run_time%60%60;
        QString shour =QString::number(hour);
        QString smi = QString::number(mi);;
        QString ssec =QString::number(sec);;
        if(hour<10)  shour = "0"+shour;
        if(mi<10)  smi = "0"+smi;
        if(sec<10)  ssec = "0"+ssec;
        emit STT_Global::mw->signal_add_Log(test_id,tr("测试时间:")+QString("%1:%2:%3").arg(shour,smi,ssec));
        timer->stop();
    }
}

void Form_TestBlock::setShow(int show_location, QString info, QColor color)
{
    QPalette pal;
     pal.setColor(QPalette::WindowText, color);

    if(show_location == 1)
    {
         ui->label_result->setText(info);
         ui->label_result->setPalette(pal);

    }
    else if(show_location == 2)
    {
        ui->label_mac->setText(info);
        ui->label_mac->setPalette(pal);

    }
    else if(show_location == 3)
    {
        ui->label_tip->setText(info);
        ui->label_tip->setPalette(pal);

    }


    changeLable(ui->label_result);
    changeLable(ui->label_name);
    changeLable(ui->label_tip);
    changeLable(ui->label_mac);

}

void Form_TestBlock::update_show_time()
{
    run_time++;
    //更新显示时间
    int hour = run_time/60/60;
    int mi =  run_time/60%60;
    int sec = run_time%60%60;
    QString shour =QString::number(hour);
    QString smi = QString::number(mi);;
    QString ssec =QString::number(sec);;
    if(hour<10)  shour = "0"+shour;
    if(mi<10)  smi = "0"+smi;
    if(sec<10)  ssec = "0"+ssec;
    ui->lcdNumber->display(QString("%1:%2:%3").arg(shour,smi,ssec));
}
