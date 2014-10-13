#ifndef FORM_SCRIPTEDIT_H
#define FORM_SCRIPTEDIT_H

#include <QWidget>
#include <QMenu>
#include <QDomDocument>
#include <QDomElement>
#include <map>
#include <iostream>
#include <QListWidgetItem>

namespace Ui {
class Form_ScriptEdit;
}

class Form_ScriptEdit : public QWidget
{
    Q_OBJECT

public:
    explicit Form_ScriptEdit(QWidget *parent = 0);
    ~Form_ScriptEdit();

 private:

    void loadscript();
    void savescript();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_close_clicked();

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_action_add_triggered();

    void on_action_delete_triggered();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::Form_ScriptEdit *ui;

     QMenu *popMenu_tb;

     QDomDocument doc_config;

     std::map<std::string,std::string>  script;

     QString lastName;
     QString deleteName;
};

#endif // FORM_SCRIPTEDIT_H
