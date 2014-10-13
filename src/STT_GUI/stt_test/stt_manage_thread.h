#ifndef STT_MANAGE_THREAD_H
#define STT_MANAGE_THREAD_H
#include "stt_global.h"
#include <QThread>
 enum   Manage_State{Stop=0,Start =1,Pause =2 , Error =3 ,UserStop = 4};

class STT_Manage_Thread : public QThread
{
    Q_OBJECT
public:
    explicit STT_Manage_Thread(QObject *parent = 0);
    Manage_State  state;
    bool AutoNext;
signals:

public slots:


    // QThread interface
protected:
    void run();

private:
    void init_sub_test_thread();
    void start_sub_test_thread();
    void stop_sub_test_thread();

    void finsh_sub_test_thread();
    void update_ui();

private:
     long total_time;
};

#endif // STT_MANAGE_THREAD_H
