#include "form_teststack.h"
#include "ui_form_teststack.h"
#include <QDebug>
#include "stt_global.h"

Form_TestStack::Form_TestStack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_TestStack)
{
     ui->setupUi(this);

      basicsize = QSize(640, 350);

     QGS = new QGraphicsScene(0,0, 1640,1350);

     TestBlockWidget = new QWidget();
     TestBlockWidget->setFixedSize(QGS->width(),QGS->height());
     //TestBlockWidget->setGeometry(-QGS->width()/2,-QGS->height()/2,QGS->width()/2,QGS->height()/2);
     TestBlockGridLayout = new QGridLayout();
     TestBlockWidget->setLayout(TestBlockGridLayout);

     QGS->addWidget(TestBlockWidget);
     QGS->addEllipse(0,0,10,10);

    ui->graphicsView->setScene(QGS);
    ui->graphicsView->centerOn(0,0);
    loadTestBlock();


}

void Form_TestStack::loadTestBlock()
{

    STT_Global::InitTestBlock();

   //移除原来的,并关闭
     for (int key : test_block_map.keys())
     {
         TestBlockGridLayout->removeWidget(test_block_map[key]);

        test_block_map[key]->close();
     }

     test_block_map.clear();

    //添加
   for ( int i=0; i!=STT_Global::Test_Info_List.size(); ++i )
   {
      Form_TestBlock *FT = new Form_TestBlock();
      FT->Init(STT_Global::Test_Info_List.at(i).test_id);
      test_block_map.insert(STT_Global::Test_Info_List.at(i).test_id,FT);
      TestBlockGridLayout->addWidget(FT,i/6,i%6);
   }
   TestBlockWidget->update();

}

void Form_TestStack::init_block(int test_id)
{
    test_block_map[test_id]->Init(test_id);
}

void Form_TestStack::TimeShow(int test_id,bool state)
{
  test_block_map[test_id]->setTimer(test_id,state);

}

void Form_TestStack::Item_Show(int test_id, int show_location, QString info, QColor color)
{
    test_block_map[test_id]->setShow(show_location,info,color);
}

Form_TestStack::~Form_TestStack()
{
    delete ui;
}

void Form_TestStack::resizeEvent(QResizeEvent *e)
{
    QSize newsize=  ui->graphicsView->size();

    //ui->graphicsView->scale(newsize.width()/(basicsize.width()*1.0),newsize.height()/(basicsize.height()*1.0));
    TestBlockWidget->setFixedSize(newsize.width(),newsize.height());
    QGS->setSceneRect(0,0,newsize.width(),newsize.height());
    QGS->update();
     ui->graphicsView->update();
     basicsize = newsize;
}


