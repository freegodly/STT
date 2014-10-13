#include "form_testitem.h"
#include "ui_form_testitem.h"
#include <QSettings>
#include "stt_global.h"

Form_TestItem::Form_TestItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_TestItem)
{
    ui->setupUi(this);
    //

    LoadItem();
}

Form_TestItem::~Form_TestItem()
{
    delete ui;
}

QSize Form_TestItem::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void Form_TestItem::resizeEvent(QResizeEvent *e)
{
    ui->label->setBaseSize(e->size());
}

void Form_TestItem::LoadItem()
{

     //ui-> listWidget->clear();

    QString Items="";
    int item_num = 1;
    STT_Global::LoadTestScriptInfo();
     std::map<std::string,Test_Item_Info>::iterator it = STT_Global::Test_Script_Info.find("Testitem");
     if(  it != STT_Global::Test_Script_Info.end())
     {
         std::list<std::string> KeysInfo = it->second.KeysInfo;

         std::list<std::string>::iterator  kit = KeysInfo.begin();
         while(kit != KeysInfo.end())
         {
             Items += QString::number(item_num) + "、" + QString((*kit).c_str()) + "\r\n" ;
              //ui-> listWidget->addItem((*kit).c_str());
              kit++;
              item_num++;
         }

     }

     ui->label->setText(Items);
}
