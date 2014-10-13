#ifndef FORM_LOADING_H
#define FORM_LOADING_H

#include <QWidget>
#include "stt_global.h"
#include <QThread>

namespace Ui {
class Form_Loading;
}


class LoadingThread : public QThread
{
     Q_OBJECT

public:

    // 0 未开始  -1 异常   1 完成
    int state;
protected:
     void run();

};


class Form_Loading : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Loading(QWidget *parent = 0);
    ~Form_Loading();
    void start();
   void closeEvent(QCloseEvent *event);
    void MainSleep(unsigned msec);
    bool is_load_success;
private slots:
    void on_label_movie_destroyed();
     void show_label(QString text);
     void process(int value);

signals:
    void signal_show_label(QString text);
    void signal_process(int value);

private:
    Ui::Form_Loading *ui;
    bool error_exit;

    LoadingThread *loadThread;

};

#endif // FORM_LOADING_H
