#include "stt_manage_thread.h"
#include "stt_test_thread.h"
STT_Manage_Thread::STT_Manage_Thread(QObject *parent) :
    QThread(parent)
{
   total_time =0;
   AutoNext = false;
}

void STT_Manage_Thread::run()
{
     total_time =0;
    // 运行测试管理
    while(true)
    {
        if(this->state  != Start ) break;

        update_ui();
        init_sub_test_thread();
        start_sub_test_thread();
        this->msleep(1000);
        total_time++;
    }

    //停止测试线程
    if(this->state  == UserStop)
    {
        stop_sub_test_thread();
    }
    else   //正常结束
    {
          emit STT_Global::mw->signal_all_test_finsh(tr("测试状态: 测试完成"));
    }


    finsh_sub_test_thread();

}

///
/// \brief STT_Manage_Thread::init_sub_test_thread
/// 判断测试线程是否存在，不存在择创建
///
void STT_Manage_Thread::init_sub_test_thread()
{

    for ( int i=0; i!=STT_Global::Test_Info_List.size(); ++i )
    {
        if(STT_Global::Test_Info_List[i].test_thread == NULL)
        {
            STT_Test_Thread* stt=  new  STT_Test_Thread(STT_Global::Test_Info_List[i].test_id);
            STT_Global::Test_Info_List[i].test_thread  = stt;
        }
    }
}

///
/// \brief STT_Manage_Thread::start_sub_test_thread
/// 判断线程是否启动或者是否结束  ，结束后或者未启动则启动
///
void STT_Manage_Thread::start_sub_test_thread()
{

    int  finshnum = 0;

    for ( int i=0; i!=STT_Global::Test_Info_List.size(); ++i )
    {
        if(STT_Global::Test_Info_List[i].test_thread != NULL)
        {

            if(AutoNext)
            {
                if( !STT_Global::Test_Info_List[i].test_thread->isRunning()  ||  STT_Global::Test_Info_List[i].test_thread->isFinished())
                {
                    STT_Global::Test_Info_List[i].test_thread->start();
                }
            }
            else
            {
                if( !STT_Global::Test_Info_List[i].test_thread->isRunning()  &&  !STT_Global::Test_Info_List[i].test_thread->isFinished())
                {
                    STT_Global::Test_Info_List[i].test_thread->start();
                }

                if(STT_Global::Test_Info_List[i].test_thread->isFinished())  //全部测试完成，并且不进行下一轮
                {
                    finshnum++;
                    if(finshnum >= STT_Global::Test_Info_List.size() )
                    {
                            this->state=Stop;
                    }
                }
            }
        }
    }
}

void STT_Manage_Thread::stop_sub_test_thread()
{
    for ( int i=0; i!=STT_Global::Test_Info_List.size(); ++i )
    {
        if(STT_Global::Test_Info_List[i].test_thread != NULL)
        {
            if( STT_Global::Test_Info_List[i].test_thread->isFinished() || !STT_Global::Test_Info_List[i].test_thread->isRunning() )
            {
                //TODO 停止子线程
               // STT_Global::Test_Info_List[i].test_thread->sto
            }
        }
    }
}

void STT_Manage_Thread::finsh_sub_test_thread()
{
    for ( int i=0; i!=STT_Global::Test_Info_List.size(); ++i )
    {
        STT_Global::Test_Info_List[i].test_thread = NULL;
    }
}

void STT_Manage_Thread::update_ui()
{
      //更新显示时间
       int hour = total_time/60/60;
       int mi =  total_time/60%60;
       int sec = total_time%60%60;
       QString shour =QString::number(hour);
       QString smi = QString::number(mi);;
       QString ssec =QString::number(sec);;
       if(hour<10)  shour = "0"+shour;
       if(mi<10)  smi = "0"+smi;
       if(sec<10)  ssec = "0"+ssec;
      emit STT_Global::mw->signal_update_time_show(QString("%1:%2:%3").arg(shour,smi,ssec));
}
