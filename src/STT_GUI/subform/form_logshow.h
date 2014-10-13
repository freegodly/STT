#ifndef FORM_LOGSHOW_H
#define FORM_LOGSHOW_H

#include <QWidget>
#include "form_logtext.h"
#include <QMap>
namespace Ui {
class Form_LogShow;
}

class Form_LogShow : public QWidget
{
    Q_OBJECT

public:
    explicit Form_LogShow(QWidget *parent = 0);
    ~Form_LogShow();

     QSize sizeHint() const ;

   QMap<int,Form_LogText*> log_map;

   void add_platform_log(QString log);
   void loadTestLog();

   void save_log_show(int test_id);
   void clear_log_show(int test_id);

signals:

public slots:


private:
    Ui::Form_LogShow *ui;



};

#endif // FORM_LOGSHOW_H
