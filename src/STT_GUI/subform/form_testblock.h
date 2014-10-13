#ifndef FORM_TESTBLOCK_H
#define FORM_TESTBLOCK_H
#include <QColor>
#include <QWidget>
#include <QTimer>
namespace Ui {
class Form_TestBlock;
}

class Form_TestBlock : public QWidget
{
    Q_OBJECT

public:
    explicit Form_TestBlock(QWidget *parent = 0);
    ~Form_TestBlock();
     void resizeEvent(QResizeEvent *e);


     ///
     /// \brief timer
     /// 显示计时
     ///
     QTimer *timer;

     long run_time;

     ///
     /// \brief Init
     ///初始化状态显示
     ///
     void Init(int test_id);

     void setTimer(int test_id, bool state);

     void setShow( int show_location /*1 info  2 mac  3 tip*/ , QString info,QColor color );

public slots:
     void update_show_time();

private:
    Ui::Form_TestBlock *ui;

};

#endif // FORM_TESTBLOCK_H
