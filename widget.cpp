#include "widget.h"
#include "ui_widget.h"
#include"showlabel.h"
#include<QDebug>
#include<QFileDialog>
#include<QDir>
#include<QFileInfo>
#include<QScrollBar>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->openbtn,SIGNAL(clicked()),this,SLOT(onopenbtn()));
    connect(ui->openpathbtn,SIGNAL(clicked()),this,SLOT(onopenpathbtn()));
    connect(ui->darkbtn,SIGNAL(clicked()),this,SLOT(ondarkbtn()));
    connect(ui->undobtn,SIGNAL(clicked()),this,SLOT(onundobtn()));
    connect(ui->savebtn,SIGNAL(clicked()),this,SLOT(onsavebtn()));
    connect(ui->priviousbtn,SIGNAL(clicked()),this,SLOT(onpriviousbtn()));
    connect(ui->nextbtn,SIGNAL(clicked()),this,SLOT(onnextbtn()));
    connect(this,SIGNAL(imagenamechanged(QString)),ui->showlabel,SLOT(openimage(QString)));
    connect(this,SIGNAL(poplist()),ui->showlabel,SLOT(popList()));
    connect(this,SIGNAL(modechanged(bool)),ui->showlabel,SLOT(setmode(bool)));
    connect(this,SIGNAL(savetriggled(bool)),ui->showlabel,SLOT(setsave(bool)));
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(getnamefromlistwidget(QListWidgetItem*)));
    connect(this,SIGNAL(edittext(QString)),ui->lineEdit,SLOT(setText(QString)));
    connect(ui->showlabel,SIGNAL(statedraw(bool)),this,SLOT(onsavebtn(bool)));
    connect(ui->showlabel,SIGNAL(imageinfo(QStringList)),this,SLOT(settablewidget(QStringList)));

    index=0;
    ui->savebtn->setEnabled(false);
    ui->undobtn->setEnabled(false);
    ui->darkbtn->setCheckable(true);
    ui->darkbtn->setChecked(false);
    ui->darkbtn->setText("预览");
    ui->listWidget->setSortingEnabled(true);
    inittablewidget();
    setFocus();

}

void Widget::inittablewidget()
{
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    QStringList header;
        header<<"所在目录"<<"图片名字"<<"文件大小"<<"图片格式"<<"图片宽度"<<"图片高度"<<""<<""<<""<<"";
        ui->tableWidget->setVerticalHeaderLabels(header);
    QStringList header1;
    header1<<"";
        ui->tableWidget->setHorizontalHeaderLabels(header1);
}

void Widget::settablewidget(QStringList list)
{
    int i=0;
    for (i;i<list.count();i++)
    ui->tableWidget->setItem(1,i-1,new QTableWidgetItem(list.at(i)));
}

void Widget::getnamefromlistwidget(QListWidgetItem *item)
{
    emit imagenamechanged(dirname+"/"+item->data(0).toString());

    QFileInfo fileInfo(dirname+"/"+item->data(0).toString());
    index=list.indexOf(fileInfo);//获取文件在目录下的序号
    fileInfo = list.at(index);
    Mb=fileInfo.size()/1024/1024.0;
    QString tip="路径："+dirname+"/"+item->data(0).toString()+\
        "  ("+QString::number (index+1,10 )+"/"+QString::number (num,10 )+")"+"   大小："\
        +QString::number (Mb,'f',2)+"Mb";

    emit edittext(tip);
}



Widget::~Widget()
{
    delete ui;
}

 QString Widget::getfilename(QString dirname,int index)
 {
     QFileInfo fileInfo;   
     fileInfo = list.at(index);
     Mb=fileInfo.size()/1024/1024.0;
     calfilesize(fileInfo.absoluteFilePath());
     return fileInfo.absoluteFilePath();
 }

void Widget::onopenbtn()
{

    QString tmpname=QFileDialog::getOpenFileName(this,tr("Open File"),\
                                         NULL,\
                                         "Images (*.jpeg *.jpg)",\
                                         0);
    if(!tmpname.isNull())
    {
        QFileInfo fileInfo(tmpname);
        dirname=fileInfo.absolutePath();
        QDir dir(dirname);
        QStringList filters;
        filters << "*.jpg" << "*.jpeg" ;
        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Name);

        filelist.clear();
        filelist=dir.entryList();
        initlistwidget(filelist);

        QFileInfoList list;
        list = dir.entryInfoList();
        num=list.count();
        index=list.indexOf(fileInfo);//获取文件在目录下的序号
        fileInfo = list.at(index);
        Mb=fileInfo.size()/1024/1024.0;
        filename=fileInfo.absoluteFilePath();
        ui->lineEdit->setText(filename);
        calfilesize(filename);
        emit imagenamechanged(filename);
    }
}

void Widget::initlistwidget(QStringList list)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(list);
    ui->listWidget->setCurrentRow(0);
}


void Widget::onopenpathbtn()
{
    dirname=QFileDialog::getExistingDirectory ();

    if(!dirname.isNull())
    {
        QDir dir(dirname);
        QStringList filters;
        filters << "*.jpg" << "*.jpeg" ;
        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Name);
        filelist.clear();
        filelist=dir.entryList();
        initlistwidget(filelist);
        list = dir.entryInfoList();
        num=list.count();
        filename=getfilename(dirname,index);
        calfilesize(filename);
        emit imagenamechanged(filename);
    }
}

void Widget::onpriviousbtn()
{
    --index;
    if(index>=0&&index<num-1)
    {

    }else
    {
        index=0;
    }

    filename=getfilename(dirname,index);
    //ui->listWidget->verticalScrollBar()->setValue(index);
    ui->listWidget->setCurrentRow(index);
    ui->savebtn->setEnabled(true);
    emit imagenamechanged(filename);
}

void Widget::onnextbtn()
{
        ++index;
    if(index<num-1&&index>=0)
    {

    }else
    {
        index=num-1;
    }

    filename=getfilename(dirname,index);
    //ui->listWidget->verticalScrollBar()->setValue(index-30);
    ui->listWidget->setCurrentRow(index);
    ui->savebtn->setEnabled(true);
    emit imagenamechanged(filename);
}


void Widget::ondarkbtn()
{
    if(ui->darkbtn->isChecked())
    {
        ui->darkbtn->setText("涂黑");
        ui->savebtn->setEnabled(true);
        ui->undobtn->setEnabled(true);
        emit modechanged(true);
    }
    else
    {
        ui->darkbtn->setText("预览");
        emit modechanged(false);
        ui->savebtn->setEnabled(false);
        ui->undobtn->setEnabled(false);
    }

}

void Widget::onundobtn()
{
    emit poplist();
    ui->savebtn->setEnabled(true);
}

void Widget::onsavebtn(bool state)
{
    qDebug()<<"ok5";
    //emit savetriggled(true);
    if(state==true)
    ui->savebtn->setEnabled(true);
    else
        ui->savebtn->setEnabled(false);
}

void Widget::onsavebtn()
{
    emit savetriggled(true);
    ui->savebtn->setEnabled(false);
}

void Widget::calfilesize(QString filename)
{
    QString tip="路径："+filename+\
        "  ("+QString::number (index+1,10 )+"/"+QString::number (num,10 )+")"+"   大小："\
        +QString::number (Mb,'f',2)+"Mb";
    //QStringList imageinfolist;
    //imageinfolist<<dirname<<QFileInfo(filename).completeBaseName()<<QString::number (Mb,'f',2)+"Mb";
    //settablewidget(imageinfolist);
    emit edittext(tip);

}
void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Space:
          onnextbtn();
        break;
    case Qt::Key_B:
          onpriviousbtn();
        break;
    case Qt::Key_S:
        onsavebtn();
        break;


    }

}
