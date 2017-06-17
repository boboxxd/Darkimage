#ifndef SHOWLABEL_H
#define SHOWLABEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include<QPaintEvent>
#include<QMouseEvent>
#include<QWheelEvent>


class ShowLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ShowLabel(QWidget *parent=0);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    QRect getrect();
    void zeropoint();
    void calrate(QPixmap oldp,QPixmap newp);
    void scaleimage(QPixmap image,int controlwidth,int controlheight);
    void getimageinfo(QString imagename);
    ~ShowLabel();
signals:
    void statedraw(bool state);
    void imageinfo(QStringList list);
public slots:
    void openimage(QString filename);
    void popList();
    void setmode(bool state);
    void setsave(bool state);
private:
    QString imagename;
    QPixmap pix;
    QPixmap temppix;
    QPixmap tt;
    QPixmap scaledpix;
    QPoint startpoint;
    QPoint endpoint;
    int x,y,width,height;
    bool isdrawing;
    bool issave;
    QPainter p;
    int controlwidth;
    int controlheight;
    qreal wrate,hrate;
    QList<QRect> list;
    QList<QRect> showlist;
    int rectnum;
};

#endif // SHOWLABEL_H
