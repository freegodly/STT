#ifndef FORM_PLATFORMCONFIGURATION_H
#define FORM_PLATFORMCONFIGURATION_H

#include <QWidget>
#include <QMenu>
#include <QDomDocument>
#include <QDomElement>
#include "CodeEditor.h"
namespace Ui {
class Form_PlatformConfiguration;
}

class Form_PlatformConfiguration : public QWidget
{
    Q_OBJECT

public:
    explicit Form_PlatformConfiguration(QWidget *parent = 0);
    ~Form_PlatformConfiguration();


    void traversalControl(const QObjectList& q);
    void LoadConfig(const QObjectList &q);

private slots:
    void on_pushButton_exit_clicked();

    void on_action_tb_addonerow_triggered();

    void on_table_Block_customContextMenuRequested(const QPoint &pos);

    void on_action_tb_addrows_triggered();

    void on_action_tb_deleteonerow_triggered();

    void on_action_tb_deleteallrows_triggered();

    void on_action_delete_args_triggered();

    void on_action_add_one_labels_triggered();

    void on_action_delete_labels_triggered();

    void on_table_args_customContextMenuRequested(const QPoint &pos);

    void on_table_labels_customContextMenuRequested(const QPoint &pos);

    void on_action_add_one_args_triggered();

    void on_pushButton_saveas_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::Form_PlatformConfiguration *ui;
    QMenu *popMenu_tb;
    QMenu *popMenu_args;
    QMenu *popMenu_label;


    QDomDocument doc_config;


};

#endif // FORM_PLATFORMCONFIGURATION_H
