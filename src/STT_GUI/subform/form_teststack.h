#ifndef FORM_TESTSTACK_H
#define FORM_TESTSTACK_H

#include <QWidget>
#include <QGraphicsScene>
#include <QLabel>
#include <QMap>
#include <QGridLayout>
#include <QWidget>
#include "form_testblock.h"
namespace Ui {
class Form_TestStack;
}

class Form_TestStack : public QWidget
{
    Q_OBJECT

public:
    explicit Form_TestStack(QWidget *parent = 0);
    ~Form_TestStack();
    void resizeEvent(QResizeEvent *e);

    void loadTestBlock();

    void init_block(int test_id);

    void TimeShow(int test_id, bool state);

    void Item_Show(int  test_id, int show_location /*1 info  2 mac  3 tip*/ , QString info,QColor color );

private:
    Ui::Form_TestStack *ui;
    QGraphicsScene *QGS;
    QGridLayout   * TestBlockGridLayout;
    QWidget * TestBlockWidget;
    QMap<int,Form_TestBlock*>  test_block_map;
    QSize basicsize;

};

#endif // FORM_TESTSTACK_H
