#include "mainwindow.h"
#include <QApplication>
#include "form_loading.h"
#include "stt_global.h"
#include <QDebug>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.addLibraryPath(a.applicationDirPath().append("/plugins"));
    qDebug()<<a.applicationDirPath().append("/plugins");

    STT_Global::fl = new  Form_Loading();
    STT_Global::fl->show();
    STT_Global::fl->start();
    bool load = STT_Global::fl->is_load_success;
    STT_Global::fl->close();


//    ofstream out("out.txt");
//    ifstream in("in.txt");
//    cin.rdbuf(in.rdbuf());
//    cout.rdbuf(out.rdbuf());

    if(load && STT_Global::mw != NULL)
    {
        STT_Global::mw->show();
        int  rcode =a.exec();
//        in.close();
//        out.close();
        return rcode;
    }
    else
    {
        //a.exit(-1);
        return -1;
    }
}
