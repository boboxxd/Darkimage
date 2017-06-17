#include "showlabel.h"
#include<QDebug>
#include<QPainter>
#include<QPaintEngine>
#include<QPaintDevice>
#include<QFileInfo>
ShowLabel::ShowLabel(QWidget *parent): QLabel(parent)
{

    wrate=0,hrate=0;
    isdrawing=false;
    issave=false;

}

ShowLabel::~ShowLabel()
{

}

void ShowLabel::getimageinfo(QString imagename)
{
    qreal Mb=QFileInfo(imagename).size()/1024/1024.0;
    QString size=QString::number (Mb,'f',2)+" Mb";
    QString pathname=QFileInfo(imagename).absolutePath();
    QString basename=QFileInfo(imagename).completeBaseName();
    QString width=QString::number (QPixmap(imagename).width(),10);
    QString height=QString::number (QPixmap(imagename).height(),10);
    QString type=QFileInfo(imagename).suffix();

    QStringList list;
    list<<pathname<<basename<<size<<type<<width<<height;
    emit imageinfo(list);
}


void ShowLabel::openimage(QString filename)
{
    qDebug()<<"接受到文件名为:"<<filename;
    getimageinfo(filename);
    list.clear();
    imagename=filename;
    pix.load(imagename);  
    scaleimage(pix,QLabel::width(),QLabel::height());
    update();
}

void ShowLabel::scaleimage(QPixmap image,int controlwidth,int controlheight)
{
    scaledpix=image.scaled(controlwidth, controlheight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    temppix=scaledpix;
}

void ShowLabel::popList()
{
    if(!list.isEmpty())
    {
        list.removeLast();
        qDebug()<<list.count();
        temppix=scaledpix;
        update();
    }
}

void ShowLabel::paintEvent(QPaintEvent *event)
{
     qDebug()<<"in painterevent";
     if(isdrawing == true)
     {
         //涂黑模式
        calrate(pix,temppix);//计算缩放后图片与原图片的长宽比例
        if(!temppix.isNull())
        {
            getrect();
            p.begin(&temppix);
            p.setBrush(Qt::black);
            for(int i=0;i<list.count();i++)
            {
             p.drawRect(list.at(i).x(),list.at(i).y(),list.at(i).width(),list.at(i).height());
        }
         if(startpoint.x()==endpoint.x()==0&&startpoint.y()==endpoint.y()==0)
            p.drawRect(x,y,width,height);
            p.end();
            p.begin(this);
            p.drawPixmap(0,0,temppix);
            p.end();

            tt.load(imagename);
            qDebug()<<"depth1:"<<tt.depth();
            p.begin(&tt);
            p.setBrush(Qt::black);
            for(int i=0;i<list.count();i++)
            {
                p.drawRect(list.at(i).x()/wrate,list.at(i).y()/hrate,list.at(i).width()/wrate,list.at(i).height()/hrate);
            }
            p.end();
        }
     }else
     {
         //预览模式
         p.begin(this);
        // scaleimage(pix,controlwidth,controlheight);
         p.drawPixmap(0,0,scaledpix);

         p.end();
     }     
}

void ShowLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {

        if(isdrawing==true)
        setCursor(Qt::CrossCursor);
        startpoint=event->pos();
       // isdrawing=true;
    }
}

void ShowLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        endpoint=event->pos();
        temppix=scaledpix;
        update();
    }
}

void ShowLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(isdrawing==true)
    {

    if(event->button()==Qt::LeftButton)
    {
        setCursor(Qt::ArrowCursor);
        endpoint=event->pos();
        //isdrawing=false;
        list.append(getrect());
        emit statedraw(true);
        zeropoint();
        update();
    }
}else
    {
        zeropoint();
    }
}

void  ShowLabel::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"大小以改变";
    scaleimage(tt,QLabel::width(),QLabel::height());
    scaleimage(pix,QLabel::width(),QLabel::height());
    update();
}

QRect ShowLabel::getrect()
{
    x=startpoint.x();
    y=startpoint.y();
    width=endpoint.x()-x;
    height=endpoint.y()-y;
    QRect rect(x,y,width,height);
    showlist.append(rect);
    return rect;
}
void ShowLabel::calrate(QPixmap oldp,QPixmap newp)
{
    hrate=newp.height()/qreal(oldp.height());
    wrate=newp.width()/qreal(oldp.width());
    qDebug()<<wrate<<":"<<hrate;
}

void ShowLabel::zeropoint()
{
    startpoint.setX(0);
    startpoint.setY(0);
    endpoint.setX(0);
    endpoint.setY(0);
}
void ShowLabel::setmode(bool state)
{
    isdrawing=state;
    if(state==true)
    {
       // setMouseTracking(true);
        update();
    }
    else
    {
       // setMouseTracking(false);
        update();
    }
}
void ShowLabel::setsave(bool state)
{
    issave=state;
    if(issave==true)
    {
        qDebug()<<"is saveing...";
        tt.load(imagename);
        qDebug()<<"depth1:"<<tt.depth();
        p.begin(&tt);
        p.setBrush(Qt::black);
        for(int i=0;i<list.count();i++)
        {
            p.drawRect(list.at(i).x()/wrate,list.at(i).y()/hrate,list.at(i).width()/wrate,list.at(i).height()/hrate);
        }
        p.end();
        tt.save(imagename,0,95);
        openimage(imagename);
        issave=false;
    }
}

void ShowLabel::wheelEvent(QWheelEvent *event)
{
/*
     if(event->delta()>0)
     {
          controlheight+=100;
          controlwidth+=100;
     }

     if(event->delta()<0)
     {
          controlheight-=100;
          controlwidth-=100;
          if(controlheight<100||controlwidth<100)
          {
              controlheight+=100;
              controlwidth+=100;
          }
     }
     qDebug()<<cursor().pos();

     if(isdrawing==true)
     scaleimage(tt,controlwidth,controlheight);
     else
     scaleimage(pix,controlwidth,controlheight);
     update();
  */
}
