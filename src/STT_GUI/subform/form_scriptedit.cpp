#include "form_scriptedit.h"
#include "ui_form_scriptedit.h"
#include "stt_global.h"
#include <QInputDialog>
#include <QDebug>


Form_ScriptEdit::Form_ScriptEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_ScriptEdit)
{
    ui->setupUi(this);
    ui->gridLayout->setColumnStretch(0,8);
    ui->gridLayout->setColumnStretch(1,1);


    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    popMenu_tb = new QMenu(ui->listWidget);

    //关闭则释放资源
    this->setAttribute(Qt::WA_DeleteOnClose,true);


    lastName = "";
    deleteName = "";

    loadscript();
}

Form_ScriptEdit::~Form_ScriptEdit()
{
    delete ui;
}

void Form_ScriptEdit::loadscript()
{

    if(G_Moudles_Config.find("SCRIPT") == G_Moudles_Config.end())
    {
        return ;
    }

    doc_config.setContent(QString(G_Moudles_Config["SCRIPT"].c_str()));

    QDomElement node=  doc_config.firstChildElement("root");
    if(!node.isNull())
    {
        script.clear();
        ui->listWidget->clear();
        for(int i=0; i < node.childNodes().length() ;i++ )
        {
            QDomNode ch =  node.childNodes().at(i);
            ui->listWidget->addItem(ch.nodeName());
            script.insert(std::pair<std::string,std::string>(ch.nodeName().toStdString(),ch.attributes().namedItem("Value").nodeValue().toStdString()));
        }
    }
}

void Form_ScriptEdit::savescript()
{
    doc_config.clear();
    QDomElement qe= doc_config.createElement("root");
    doc_config.appendChild(qe);
    std::map<string,std::string>::iterator it;

    for(it=script.begin();it!=script.end();++it)
    {
        QDomElement cf= doc_config.createElement(QString(it->first.c_str()));
        cf.setAttribute("Value",QString(it->second.c_str()));
        qe.appendChild(cf);
    }


    G_Moudles_Config["SCRIPT"] =doc_config.toString().toStdString();

    STT_Global::SaveConfig();

}

void Form_ScriptEdit::on_pushButton_save_clicked()
{

    if(lastName != ""  )
    {
        if(script.find(lastName.toStdString()) != script.end() )
        {
            script[lastName.toStdString()] = ui->plainTextEdit->toPlainText().toStdString();
        }
        else
        {
            script.insert(std::pair<std::string,std::string>(lastName.toStdString(),ui->plainTextEdit->toPlainText().toStdString()));
        }
    }

    savescript();
}

void Form_ScriptEdit::on_pushButton_close_clicked()
{
    STT_Global::mw->Switch_StackWindow( STT_Global::mw->form_TestStack);
    //关闭则释放资源
    STT_Global::mw->form_ScriptEditStack =NULL;
    this->close();
}

void Form_ScriptEdit::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QList<QAction *>  al;
    al.append(ui->action_add);
    al.append(ui->action_delete);

    popMenu_tb->addActions(al);
    popMenu_tb->exec(QCursor::pos());
}

void Form_ScriptEdit::on_action_add_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("请输入脚本名称"),
                                         tr("名称（主脚本需要命名为mian）:"), QLineEdit::Normal,"",&ok);
    if(ok)
    {
        ui->listWidget->addItem(name);
    }
}

void Form_ScriptEdit::on_action_delete_triggered()
{

    script.erase(ui->listWidget->currentItem()->text().toStdString());
    deleteName = ui->listWidget->currentItem()->text();
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}

void Form_ScriptEdit::on_listWidget_clicked(const QModelIndex &index)
{

}

void Form_ScriptEdit::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(lastName != ""  )
    {
        if(script.find(lastName.toStdString()) != script.end() )
        {
            script[lastName.toStdString()] = ui->plainTextEdit->toPlainText().toStdString();
        }
        else
        {
            if(deleteName!=""  &&   deleteName != lastName )
                script.insert(std::pair<std::string,std::string>(lastName.toStdString(),ui->plainTextEdit->toPlainText().toStdString()));
        }
    }

    lastName  = ui->listWidget->currentItem()->text();
    if(script.find(lastName.toStdString()) != script.end())
    {
        ui->plainTextEdit->setPlainText(script[lastName.toStdString()].c_str());
    }
    else
    {
        ui->plainTextEdit->setPlainText("");
    }
}
