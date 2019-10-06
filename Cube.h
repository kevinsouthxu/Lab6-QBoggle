#ifndef CUBE_H
#define CUBE_H

#include <QLabel>
#include <QWidget>

class Cube : public QWidget
{
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = nullptr);
    void setLetter(QString l);
    bool returnpress(){return press;}
    QString returnletter(){return le;}
signals:
    void clicked();
    void judgesit();
public slots:
    void mouseClicked();
    void receivejudge(bool flag);
    void resetcolor();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QLabel *label; //cube在界面上的表现形式，包括框和字体
    QPoint mousePos; //鼠标的坐标
    bool press; //鼠标是否被按下
    bool mouseinthecube;
    QString le;
    bool resetcolour;
};

#endif // CUBE_H
