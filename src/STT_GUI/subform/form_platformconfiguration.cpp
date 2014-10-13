#include "form_platformconfiguration.h"
#include "ui_form_platformconfiguration.h"
#include "stt_global.h"
#include <QList>
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QComboBox>
#include <QDomNode>
#include <QDebug>

Form_PlatformConfiguration::Form_PlatformConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_PlatformConfiguration)
{
    ui->setupUi(this);

    ui->gridLayout_basic->setRowStretch(0,1);
    ui->gridLayout_basic->setRowStretch(1,6);
    ui->gridLayout_basic->setRowStretch(2,2);
    ui->gridLayout_basic->setRowStretch(3,1);

    //关闭则释放资源
    this->setAttribute(Qt::WA_DeleteOnClose,true);


    ui->table_Block->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->table_Block->verticalHeader()->setVisible(false);
    popMenu_tb = new QMenu(ui->table_Block);

    ui->table_args->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->table_args->verticalHeader()->setVisible(false);
    popMenu_args = new QMenu(ui->table_args);

    ui->table_labels->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->table_labels->verticalHeader()->setVisible(false);
    popMenu_label = new QMenu(ui->table_labels);


     doc_config.setContent(QString(G_Moudles_Config["PLATFORM"].c_str()));
     LoadConfig(this->children());


}

Form_PlatformConfiguration::~Form_PlatformConfiguration()
{
    delete ui;
}


///
/// \brief Form_PlatformConfiguration::traversalControl
/// \param q
/// save
///
void Form_PlatformConfiguration::traversalControl(const QObjectList& q)
{
    for(int i=0;i<q.length();i++)
    {

        if(!q.at(i)->children().empty())
        {
            traversalControl(q.at(i)->children());
        }

        QObject* o = q.at(i);
        if (o->inherits("QLineEdit"))
        {
            QLineEdit* b = qobject_cast<QLineEdit*>(o);
            QDomElement qe= doc_config.createElement(b->objectName());
            qe.setAttribute("Value",b->text());
            qe.setAttribute("Type","QLineEdit");
            doc_config.firstChildElement("root").appendChild(qe);
        }
        else if (o->inherits("QGroupBox"))
        {
            QGroupBox* b = qobject_cast<QGroupBox*>(o);
            QDomElement qe= doc_config.createElement(b->objectName());
            qe.setAttribute("Value",b->isChecked());
            qe.setAttribute("Type","QGroupBox");
            doc_config.firstChildElement("root").appendChild(qe);
        }
        else if (o->inherits("QTableWidget"))
        {
            QTableWidget * b = qobject_cast<QTableWidget*>(o);
            int col_rate = b->objectName() == "table_labels" ? 1:0;
            QDomElement qe= doc_config.createElement(b->objectName());
            qe.setAttribute("Value_R",b->rowCount());
            qe.setAttribute("Value_C",b->columnCount());
            qe.setAttribute("Type","QTableWidget");
            for(int i =0 ; i<b->rowCount() ;i++)
            {
                QDomElement item= doc_config.createElement("R"+QString::number(i));
                for(int j=0 ;j <b->columnCount()  - col_rate; j++)
                {
                    item.setAttribute("C"+QString::number(j), b->item(i,j)->text());
                }
                qe.appendChild(item);
            }
            doc_config.firstChildElement("root").appendChild(qe);
        }
    }

}

void Form_PlatformConfiguration::LoadConfig(const QObjectList &q)
{
    for(int i=0;i<q.length();i++)
    {

        if(!q.at(i)->children().empty())
        {
            LoadConfig(q.at(i)->children());
        }

        QObject* obj = q.at(i);

        if (obj->inherits("QLineEdit"))
        {
            QLineEdit *b = qobject_cast<QLineEdit*>(obj);
            QString Name = obj->objectName();
            QDomNode node = STT_Global::FindXml(doc_config,Name);
            if(!node.isNull() ) b->setText(node.attributes().namedItem("Value").nodeValue() );

        }
        else if (obj->inherits("QGroupBox"))
        {
            QGroupBox* b = qobject_cast<QGroupBox*>(obj);
            QDomNode node = STT_Global::FindXml(doc_config,b->objectName());
            if(!node.isNull() ) b->setChecked( node.attributes().namedItem("Value").nodeValue() == "1" ? true:false);
        }
        else if (obj->inherits("QTableWidget"))
        {
            QTableWidget* b = qobject_cast<QTableWidget*>(obj);
            QDomNode node = STT_Global::FindXml(doc_config,b->objectName());
            if( !node.isNull() )
            {
              int Value_R =  node.attributes().namedItem("Value_R").nodeValue().toInt();
              int Value_C =  node.attributes().namedItem("Value_C").nodeValue().toInt();
              b->setRowCount(Value_R);

              for(int i =0 ; i<Value_R ;i++)
              {
                  QDomNode item= node.childNodes().at(i);
                  for(int j=0 ;j < Value_C ; j++)
                  {
                      b->setItem(i, j, new QTableWidgetItem(item.attributes().namedItem("C"+QString::number(j)).nodeValue()));
                  }
              }
            }
        }
    }
}

void Form_PlatformConfiguration::on_pushButton_saveas_clicked()
{

}


void Form_PlatformConfiguration::on_pushButton_save_clicked()
{
    doc_config.clear();
    QDomElement qe= doc_config.createElement("root");
    doc_config.appendChild(qe);
    traversalControl(this->children());

    G_Moudles_Config["PLATFORM"] =doc_config.toString().toStdString();

    STT_Global::SaveConfig();
}

// 退出基本配置
void Form_PlatformConfiguration::on_pushButton_exit_clicked()
{
    STT_Global::mw->Switch_StackWindow ( STT_Global::mw->form_TestStack);
    //关闭则释放资源
    STT_Global::mw->form_PlatformConfigurationStack =NULL;
    this->close();

}

void Form_PlatformConfiguration::on_action_tb_addonerow_triggered()
{

    int r =ui->table_Block->rowCount();
    ui->table_Block->setRowCount(r+1);
    ui->table_Block->setItem(r, 0, new QTableWidgetItem(QString::number(r+1)));
    ui->table_Block->setItem(r, 1, new QTableWidgetItem("192.168.1.1"));
    ui->table_Block->setItem(r, 2, new QTableWidgetItem(QString::number(r+1)));
}

void Form_PlatformConfiguration::on_table_Block_customContextMenuRequested(const QPoint &pos)
{

    QList<QAction *>  al;
    al.append(ui->action_tb_addonerow);
    al.append((ui->action_tb_addrows));
    al.append(ui->action_tb_deleteallrows);
    al.append(ui->action_tb_deleteonerow);

    popMenu_tb->addActions(al);
    popMenu_tb->exec(QCursor::pos());


}

void Form_PlatformConfiguration::on_action_tb_addrows_triggered()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("请输入你要插入的行数"),
                                 tr("行数:"), 8, 0, 100, 1, &ok);
    if(ok)
    {

        int j =ui->table_Block->rowCount();
        ui->table_Block->setRowCount(j+i);

        for( int r =j; r< i+j;r++)
        {
            ui->table_Block->setItem(r, 0, new QTableWidgetItem(QString::number(r+1)));
            ui->table_Block->setItem(r, 1, new QTableWidgetItem("192.168.1.1"));
            ui->table_Block->setItem(r, 2, new QTableWidgetItem(QString::number(r+1)));
        }
    }
}

void Form_PlatformConfiguration::on_action_tb_deleteonerow_triggered()
{
    ui->table_Block->removeRow(ui->table_Block->currentRow());
}

void Form_PlatformConfiguration::on_action_tb_deleteallrows_triggered()
{
    for(int i =ui->table_Block->rowCount()-1;i>=0;i--)
        ui->table_Block->removeRow(i);
}


void Form_PlatformConfiguration::on_action_delete_args_triggered()
{
    ui->table_args->removeRow(ui->table_args->currentRow());

}

void Form_PlatformConfiguration::on_action_add_one_labels_triggered()
{
    int r =ui->table_labels->rowCount();
    ui->table_labels->setRowCount(r+1);
    ui->table_labels->setItem(r, 0, new QTableWidgetItem("Name"));
    ui->table_labels->setItem(r, 1, new QTableWidgetItem("0"));
    ui->table_labels->setItem(r, 2, new QTableWidgetItem("0"));
    ui->table_labels->setItem(r, 3, new QTableWidgetItem("0"));
}

void Form_PlatformConfiguration::on_action_delete_labels_triggered()
{
    ui->table_labels->removeRow(ui->table_labels->currentRow());

}

void Form_PlatformConfiguration::on_table_args_customContextMenuRequested(const QPoint &pos)
{
    QList<QAction *>  al;

    al.append(ui->action_add_one_args);
    al.append(ui->action_delete_args);
    popMenu_args->addActions(al);
    popMenu_args->exec(QCursor::pos());
}

void Form_PlatformConfiguration::on_table_labels_customContextMenuRequested(const QPoint &pos)
{
    QList<QAction *>  al;

    al.append(ui->action_add_one_labels);
    al.append(ui->action_delete_labels);
    popMenu_label->addActions(al);
    popMenu_label->exec(QCursor::pos());
}

void Form_PlatformConfiguration::on_action_add_one_args_triggered()
{
    int r =ui->table_args->rowCount();
    ui->table_args->setRowCount(r+1);
    ui->table_args->setItem(r, 0, new QTableWidgetItem("Name"));
    ui->table_args->setItem(r, 1, new QTableWidgetItem("Value"));
}




