#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QTextStream>
#include <conio.h>
#include <stdio.h>
//#include "C:\QTprogects\tmu\prog\lib\win\lib\rs232lib.h"
//#include "C:\QTprogects\tmu\prog\lib\win\lib\popular.cpp"

//#include "C:\QTprogects\tmu\prog\lib\common\rcproto\rcproto.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    //QTextStream stream;
    //stream.setCodec("UTF-8");
    MainWindow w;
    w.show();

    return a.exec();
}
