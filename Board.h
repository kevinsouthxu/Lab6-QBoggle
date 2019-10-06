#ifndef BOARD_H
#define BOARD_H

#include "Cube.h"

#include <QWidget>
#include <QVector>
#include <QTime>
class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr, int size = 5, const QString *cubeLetters = BIG_BOGGLE_CUBES);
    virtual ~Board();
    void shake();
    int returnsize(){return size;}
    const QString* returncurrent(){return currentcubeletter;}
    QString cubeletter(int x,int y){return currentcubeletter[index(x,y)];}//返回某一行（x）某一列（y）的cube的朝上的字母
signals:
    void emitboard(int size,const QString *board);
    void mouseincu(bool flag);
    void emitclickword(QString string,int index);
    void resetcube();
public slots:
    void replay();
    void judgemouse();
    void resetclickedboard();
private:
    int size;
    Cube **cubes;
    QString *letters;
    QString *currentcubeletter; //用于储存当前状态的cube朝上的字母
    bool *currentvisit;
    inline int index(int i, int j) { return i * size + j; }
    static const QString STANDARD_CUBES[16];
    static const QString BIG_BOGGLE_CUBES[25];
    QVector<QString> vec;       //用来shake的容器
    void random();              //简单地随机当前种子
    QString currentcli; //用来存储cube点击的单词
};

#endif // BOARD_H
