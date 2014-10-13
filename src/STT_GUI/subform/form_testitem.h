#ifndef FORM_TESTITEM_H
#define FORM_TESTITEM_H

#include <QWidget>
#include <QResizeEvent>
#include <QDomDocument>
#include <QDomElement>

namespace Ui {
class Form_TestItem;
}

class Form_TestItem : public QWidget
{
    Q_OBJECT

public:
    explicit Form_TestItem(QWidget *parent = 0);
    ~Form_TestItem();
    QSize sizeHint() const ;
    void resizeEvent(QResizeEvent *e);

    void LoadItem();

private:
    Ui::Form_TestItem *ui;
};

#endif // FORM_TESTITEM_H
