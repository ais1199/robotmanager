#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QString BaseFN;//current base fname
    QString Port;//current port name
    QString zapros;
    QString otvet;
    char*makechar(const QString text)const;
    void save();//save function
    void send();//send command to bot
    void setBaseFile(QString);//change base fname
    void updCommands();//change commands list
    void updOut();//upd output list
    void setPort();//set new port
};

#endif // MAINWINDOW_H
