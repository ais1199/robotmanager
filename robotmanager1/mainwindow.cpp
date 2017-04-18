#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    ui->commands->setReadOnly(true);
    ui->result->setReadOnly(true);
    connect(ui->save,&QPushButton::clicked,this,&MainWindow::save);
    connect(ui->send,&QPushButton::clicked,this,&MainWindow::send);
    connect(ui->podskazkifname,SIGNAL(textChanged(QString)),this,SLOT(setBaseFile(QString)));
}

void MainWindow::save()
{
    QString c=QFileDialog::getSaveFileName(this,"сохранить данные","",tr("Text files(*.txt)"));
}

char* MainWindow::makechar(const QString text)const
{
    //printf("187\n");
    int n=text.size();
    QChar *kuchar=text.data();//ооопс!!!
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

void MainWindow::send()
{

}

void MainWindow::setBaseFile(QString n)
{
    QDir d;
    QString home=d.homePath();
    BaseFN=home+QString("/tips/")+n;
    updCommands();
}

void MainWindow::updCommands()
{
    QFile f(BaseFN);
    char buf[128];
    if(f.open(QIODevice::ReadOnly))
    {

        while(!f->atEnd())
        {
           f->readLine(buf,128);
        }
        /*if(dada.fo(buf)!=zeroobinf)
        {
            QString*st2= new QString(buf);
            ui->Curbase->setText(c);
            ui->Curbasetipe->setText(*st2);
            curbast.resize(0);
            dada.loading(c,&curbast);
            cot=dada.fo(buf);

            connect(ui->savenew,&QPushButton::clicked,this,&MainWindow::save);
            connect(ui->sas1,&QPushButton::clicked,this,&MainWindow::saveas1);

            connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(pa(int)));
            connect(ui->serch,&QPushButton::clicked,this,&MainWindow::serch);
            connect(ui->del1,&QPushButton::clicked,this,&MainWindow::delone);
            connect(ui->opensecond,&QPushButton::clicked,this,&MainWindow::open2);
            wrex(cot);
            int tarabara=curbast.size();

            ui->spinBox->setMaximum(tarabara);
            if(ui->spinBox->value()!=tarabara)ui->spinBox->setValue(tarabara);
            else
            {
                showdata(1,true);
            }
            ui->lim1->setText(QString(dada.toChar(curbast.size())));
            ui->status2->setText("нет ошибок");
        }*/
    }
    else
    {
        ui->status2->setText("ошибка загрузки");
    }
    f.close();
}
}

void MainWindow::updOut()
{
    QString buf=zapros+QString(" :: ")+otvet;
    ui->result->insertPlainText(buf);
}

void MainWindow::setPort()//set new port
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
