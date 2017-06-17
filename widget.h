#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QStringList>
#include<QListWidgetItem>
#include<QKeyEvent>
#include<QFileInfoList>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QString getfilename(QString dirname,int index);
    void calfilesize(QString filename);
    void initlistwidget(QStringList list);
    void inittablewidget();

    void keyPressEvent(QKeyEvent *event);

signals:
    void imagenamechanged(QString filename);
    void poplist();
    void modechanged(bool mode);
    void savetriggled(bool state);
    void edittext(QString);
public slots:
    void onopenbtn();
    void onopenpathbtn();
    void ondarkbtn();
    void onundobtn();
    void onsavebtn(bool state);
    void onsavebtn();
    void onpriviousbtn();
    void onnextbtn();
    void getnamefromlistwidget(QListWidgetItem *);
    void settablewidget(QStringList list);
private:
    Ui::Widget *ui;
    QString filename;
    QString dirname;
    QStringList filelist;
    int index;
    int num;
    qreal Mb;
    QFileInfoList list;
};

#endif // WIDGET_H
