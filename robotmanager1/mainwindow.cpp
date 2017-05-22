#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QChar>
#include <QLineEdit>

#include <conio.h>
//#include <sys/wait.h>

/*
 *замечание - чтение - кусок не более 32 символов
 *замечание - в командах нельзя использовать пробелы, |, запятые и двоеточия
 *замечание - при обновлении списка портов текущий порт автоматом сбрасывается
 *замечание - принимает файлы только в утф8(наверное)
 *проблема кодировки при чтении фаила(проще файл перевести в нужную кодировку)
 *
 *
 *хотелось бы отсылать команды с параметрами
 *хотелось бы сделать режим "одной клавиши"...но не в этот раз((
 *хотелось бы, чтобы список портов обновлялся сам
*/
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
    //portlist.clear();
    port = NULL;
    plu=false;
    sending=false;
    //if(!r)printf("porterror((\n");
    updPortList();
    //exit(0);
    //ui->comboBox->ins
    //QComboBox
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(setPort(QString)));
    //connect(ui->comboBox,SIGNAL(highlighted(QString)),this,SLOT(setPort(QString)));
    connect(ui->updP,&QPushButton::clicked,this,&MainWindow::updPortList);
    connect(ui->save,&QPushButton::clicked,this,&MainWindow::save);
    connect(ui->send,&QPushButton::clicked,this,&MainWindow::send);
    connect(ui->setMainF,&QPushButton::clicked,this,&MainWindow::setBaseFile);
    //connect(ui->podskazkifname,SIGNAL(editingFinished()),this,SLOT(setBaseFile(QString)));
    connect(ui->vvod,&QLineEdit::editingFinished,this,&MainWindow::send);
    connect(ui->clearOut,&QPushButton::clicked,this,&MainWindow::clear);
    connect(ui->frer,&QPushButton::clicked,this,&MainWindow::svobrej);
}
void MainWindow::svobrej()
{
    BaseFN.clear();
    int s=comlist.size();
    int i;
    for (i=0;i<s;i++)
    {
        free(comlist[i]);
    }
    comlist.clear();
    ui->commands->clear();
    ui->mfn->setText("Режим свободной связи");
    nm();
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
//найти команду
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
//отослать сигнал (дан текст)
void MainWindow::sendt(QString txt)
{
    if(sending)return;
    if(txt==QString(""))return;
    sending=true;
    QString spesh;
    QString ol;
    QString res=QString("нет команд");
    if(comlist.size()!=0)
    {

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
                nm();
                QString otvet=voprosOtvet(spesh);
                res=spesh+QString(" :: ")+otvet+QString("\n");
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
        int j=txt.count(QString(", "));
        int i;
        for(i=0;i<=j;i++)
        {
            ol=txt.section(", ",i,i);
            spesh=ol.replace(QString(" "),QString(""));
            nm();
            QString otvet=voprosOtvet(spesh);
            res=spesh+QString(" :: ")+otvet+QString("\n");
            updOut(res);
        }
        ui->vvod->clear();
    }
    sending=false;
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
    QString v;
    //QString aaa;
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
                   v=QString(nya)+QString(" ::")+b.section(" ::",1);
                   ui->commands->insertPlainText(v);
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

//Обновить вывод
void MainWindow::updOut(QString res)
{
    //QString buf=zapros+QString(" :: ")+otvet;
    ui->result->insertPlainText(res);
    //nm();
}
//set port
void MainWindow::setPort(QString txt)//set new port
{
    if(plu)return ;
    if(port!=NULL)
    {
        delete port;
        port=NULL;
    }
    if(txt.contains(QString("COM")))
    {
        int r;
        char *b=makechar(txt);
        //TComPort *p=port;
        if(port!=NULL)
        {
            delete port;
            port=NULL;
        }
        port=new TComPort(b, 9600,r);
        if(r)
        {
            /*char* imp=makechar(voprosOtvet(QString("")));
            printf("%s/n",imp);
            free(imp);
            unsigned char otv;
            if(port->Read(&otv))
            {
                printf("ok\n");
            }
            else{
                char dd=(char)otv;
                printf("%c\n",dd);
            }*/
            nm();
        }
        else
        {
            //potr=NULL;
            printf("oshibka porta!(\n");
            ui->status->setText(QString("Ошибка порта"));
            port=NULL;
        }
    }
    else
    {
        printf("port ne zadan\n");
        ui->status->setText(QString("Порт не задан"));
        port=NULL;
    }

}
//write reqest & returns answer
QString MainWindow::voprosOtvet(QString vopr)const
{

    QString aaa=vopr.replace(QString("|"),QString(""))+QString("|");

    char*m=makechar(aaa);
    if(port!=NULL)
    {
        int res=port->WriteData(m);
        if(res!=0)
        {
            QString t=QString("Ошибка отправки в ")+QString(m)+QString(" в ")+QString(res);
            ui->status->setText(t);
            return QString("writing error");
        }
        else
        {
            QString rd=QString("");
            unsigned char buf;
            int i;
            char uu;
            for(i=0;i<32;i++)
            {
                if(!port->Read(&buf))
                {

                        ui->status->setText("Ошибка чтения");
                        return QString("reading mistake");

                }
                else
                {
                    uu=(char)buf;
                    if(uu=='\n')
                    {
                        rd=rd.replace(QString("/r/n"),QString(""));
                        return rd;
                    }
                    rd.push_back(uu);

                }
            }
        }

    }
    else
    {
        ui->status->setText("Порт не задан!");
        return QString("port error");
    }
    return QString("long request");
}
//updates portlist
void MainWindow::updPortList()
{
    plu=true;
    TComPort *p;
    //portlist.clear();
    ui->comboBox->clear();
    if(port!=NULL)
    {
        delete port;
        port=NULL;
    }
    char text[6]="COM0\0";
    int r;
    int i;
    //char *b;
    for(i=1;i<10;i++)
    {
        text[3]=i+'0';
        p=new TComPort(text, 19200,r);
        if(r)
        {
            //printf("%s\n",text);
            //b=(char*)malloc(sizeof(char)*5);
            //strcpy(b,text);
            //portlist.push_back(b);
            ui->comboBox->insertItem(30,QString(text));
        }
        delete p;

    }
    text[3]='1';
    text[4]='0';
    text[5]='\0';
    for(i=0;i<10;i++)
    {
        text[4]=i+'0';
        p=new TComPort(text, 19200,r);
        if(r)
        {
            //b=(char*)malloc(sizeof(char)*6);
            //strcpy(b,text);
            //portlist.push_back(b);
            ui->comboBox->insertItem(30,QString(text));
        }
        delete p;

    }
    text[3]='2';
    for(i=0;i<10;i++)
    {
        text[4]=i+'0';
        p=new TComPort(text, 19200,r);
        if(r)
        {
            //b=(char*)malloc(sizeof(char)*6);
            //strcpy(b,text);
            //portlist.push_back(b);
            ui->comboBox->insertItem(30,QString(text));
        }
        delete p;
    }
    ui->comboBox->insertItem(0,QString("не задан"));
    if(port==NULL)ui->comboBox->setCurrentIndex(0);
    //delete p;
    plu=false;
    nm();
}

MainWindow::~MainWindow()
{
    if(port!=NULL)delete port;
    int s=comlist.size();
    int i;
    for (i=0;i<s;i++)
    {
        free(comlist[i]);
    }
    comlist.clear();
    delete ui;
}
