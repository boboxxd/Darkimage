#include "myrect.h"

MyRect::MyRect()
{
    rectcolor=Qt::black;
    //painter.setPen(rectcolor);
   // painter.setBrush(rectcolor);
}
MyRect::MyRect(int x,int y,int width,int height,QColor color,QPixmap pix):startx(x),starty(y),rectwidth(width),rectheight(height)
{
    rectcolor=color;
    this->pix=pix;
}

void MyRect::createRect(QPixmap pix)
{

    painter.begin(&pix);
    painter.setPen(rectcolor);
    painter.setBrush(rectcolor);
    painter.drawRect(startx,starty,rectwidth,rectheight);
    painter.end();
}

void MyRect::setrect(int x,int y,int width,int height,QColor color,QPixmap pix)
{
    startx=x;
    starty=y;
    rectwidth=width;
    rectheight=height;
    rectcolor=color;
    this->pix=pix;
}

void MyRect::setcolor(QColor color)
{
    rectcolor=color;
}


