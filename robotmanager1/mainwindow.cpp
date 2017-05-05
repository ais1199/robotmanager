#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QChar>
#include <QLineEdit>

//проблемы с вводом (изменением строк)
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    ui->commands->setReadOnly(true);
    ui->result->setReadOnly(true);

    //instruct=false;
    BaseFN=QString("");
    comlist.clear();

    connect(ui->save,&QPushButton::clicked,this,&MainWindow::save);
    connect(ui->send,&QPushButton::clicked,this,&MainWindow::send);
    connect(ui->setMainF,&QPushButton::clicked,this,&MainWindow::setBaseFile);
    //connect(ui->podskazkifname,SIGNAL(editingFinished()),this,SLOT(setBaseFile(QString)));
    //connect(ui->vvod,SIGNAL(editingFinished()),this,&MainWindow::send);
    connect(ui->clearOut,&QPushButton::clicked,this,&MainWindow::clear);
}
//сохранить работу в файл
void MainWindow::save()
{
    QString c=QFileDialog::getSaveFileName(this,"сохранить данные","",tr("Text files(*.txt)"));
    QFile f(c);
    if(f.open(QIODevice::WriteOnly))
    {
        char e[]="\r\n";
        char*buf;
        int i=0;
        QString b=ui->result->toPlainText();
        QString c=QString("\n");
        QString t=b.section(c,i,i);
        while(t.length())
        {
            buf=makechar(t);
            f.write(buf,strlen(buf));
            f.write(e,2);
            i++;
            t=b.section(c,i,i);
        }

    }
    else
    {
        ui->status->setText("Ошибка сохранения");
    }
    f.close();
}
//перевести из КуСтринг в чар
char* MainWindow::makechar(const QString text)const
{
    //printf("187\n");
    int n=text.size();
    const QChar*kuchar=text.data();
    char*res=(char*)malloc((n+1)*sizeof(char));
    int i;
    QChar dop;
    for(i=0;i<n;i++)
    {
        dop=kuchar[i];
        res[i]=dop.toLatin1();
    }
    res[n]='\0';
    return res;
}
//отослать сигнал (сам добудь текст)
void MainWindow::send()
{
    QString t=ui->vvod->text();
    sendt(t);
}
//отослать сигнал (дан текст)

bool MainWindow::comFound(QString txt) const
{
    //char*d=makechar(txt);
    int i;
    QString emm;
    int s=comlist.size();
    for(i=0;i<s;i++)
    {
        emm=QString(comlist[i]);
        if(emm==txt)return true;
    }
    return false;
}
void MainWindow::sendt(QString txt)
{
    if(comlist.size()!=0)
    {
        QString spesh;
        QString ol;
        QString res=QString("нет команд");
        //char*op;
        int j=txt.count(QString(", "));
        int i;
        for(i=0;i<=j;i++)
        {
            ol=txt.section(", ",i,i);
            /*op=makechar(ol);
            repairProbels(op);
            spesh=QString(op);*/
            spesh=ol.replace(QString(" "),QString(""));
            if(comFound(spesh))
            {
                res=spesh+QString(" :: ")+QString(" otvet\n");
                updOut(res);
            }
            else
            {
                break;
            }
        }
        if(i>j)ui->vvod->clear();
        else
        {
            if(j!=0)
            {
                res=spesh+QString(" - ")+QString(" нет такой команды");
            }
            ui->status->setText(res);
        }
    }
    else
    {
        QString s=QString("Не установлен Файл команд");
        ui->status->setText(s);
    }
}
//очистить поле вывода
void MainWindow::clear()
{
    ui->result->clear();
    nm();
}
//статус "нет ошибок"
void MainWindow::nm()
{
    QString t=QString("Нет ошибок");
    ui->status->setText(t);
}
//ликвидировать пробелы - возможны проблемы с памятью
void MainWindow::repairProbels(char *s) const
{
    QString buf(s);
    QString r=buf.replace(QString(" "),QString(""));
    free(s);
    s= makechar(r);
}
//установить файл подсказок
void MainWindow::setBaseFile()
{
    BaseFN=QFileDialog::getOpenFileName(this,"открыть файл с командами","",tr("Text files(*.txt)"));
    QFile f(BaseFN);
    char buf[128];
    QString b;
    QString c;
    char*nya;
    if(f.open(QIODevice::ReadOnly))
    {
        ui->commands->clear();
        comlist.clear();
        while(!f.atEnd())
        {
           f.readLine(buf,128);
           if(buf[0]!='/')
           {
               b=QString(buf);
               c=b.section(" ::",0,0);
               nya=makechar(c);
               repairProbels(nya);
               if(strlen(nya)!=0)
               {
                   ui->commands->insertPlainText(b);
                   comlist.push_back(nya);
               }
           }
        }
        nm();
        ui->mfn->setText(BaseFN);
        //instruct=true;
    }
    else
    {
        ui->status->setText("ошибка загрузки");
    }
    f.close();
}
//обновить список доступных команд
/*void MainWindow::updCommands()
{

}*/
//Обновить вывод
void MainWindow::updOut(QString res)
{
    //QString buf=zapros+QString(" :: ")+otvet;
    ui->result->insertPlainText(res);
    nm();
}

void MainWindow::setPort()//set new port
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
