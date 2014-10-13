#ifndef FORM_HELPER_H
#define FORM_HELPER_H

#include <QWidget>

namespace Ui {
class Form_Helper;
}

class Form_Helper : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Helper(QWidget *parent = 0);
    ~Form_Helper();

private slots:
    void on_listView_moudles_clicked(const QModelIndex &index);

    void on_listView_funs_clicked(const QModelIndex &index);

private:
    Ui::Form_Helper *ui;
};

#endif // FORM_HELPER_H
