#ifndef STT_TEST_THREAD_H
#define STT_TEST_THREAD_H

#include <QThread>
#include "3rd/lua_tinker/lua_tinker.h"
#include "stt_interface/Basic_Module_Interface.h"
#include "stt_global.h"


class STT_Test_Thread : public QThread
{
    Q_OBJECT
public:
     STT_Test_Thread(int test_id=0,QObject *parent =0);
signals:

public slots:

private:
     void get_Test_List();

     bool test_init();

     bool run_test_item(QString itemName);


    // QThread interface
protected:
    void run();

private :
    int test_id;
    long test_time;
    QStringList TestItemList;
};

#endif // STT_TEST_THREAD_H
