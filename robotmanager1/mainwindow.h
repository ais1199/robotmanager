#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <vector>
#include "rs232lib.h"

using namespace std;
/*
 * Не написно ничего по роботу - написать функцию приема
 * Установить кодировку при чтении в поле подсказки
 * Пофиксить пробелы (repaired)
 * заставить команды принимать параметры
 */
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void sendt(QString);
private:
    Ui::MainWindow *ui;
    QString BaseFN;//current base fname
    QString Port;//current port name
    //QString zapros;
    //QString otvet;
    char*makechar(const QString text)const;
    bool SkipRemarkLine(QFile*f,char* res)const;
    void save();//save function
    void send();//send command to bot при случае поставить убирание пробелов через  замену их на пустоту
    void clear();
    void updCommands();//change commands list
    void updOut(QString);//upd output list
    void setPort();//set new port
    void nm();
    vector<char*> comlist;
    void setBaseFile();//change base fname УКАЗАТЬ КОДИРОВКУ при чтении фаила QTextCodec
    void repairProbels(char*s)const;
    bool comFound(QString)const;
    //bool instruct;
    QString voprosOtvet(char*)const;
    TComPort *port;
};

#endif // MAINWINDOW_H
