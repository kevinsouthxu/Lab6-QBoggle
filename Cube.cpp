#include "Cube.h"

#include <QHBoxLayout>
#include <QMouseEvent>
Cube::Cube(QWidget *parent) : QWidget(parent)
{
    label = new QLabel();
    label->setText("");
    label->setAlignment(Qt::AlignCenter); //字体居中
    QFont font = label->font();
    font.setPointSize(35); //字体大小
    label->setFont(font);

    this->setFixedSize(120, 120); //外框大小
    this->setStyleSheet("background-color: white; border-radius: 20px; border: 2px solid");

    QHBoxLayout *layout = new QHBoxLayout(); //水平布局
    layout->addWidget(label);
    setLayout(layout);
    connect(this,SIGNAL(clicked()),this,SLOT(mouseClicked()));
}
void Cube::setLetter(QString l)
{
    le=l;
    label->setText(l);
}
void Cube::resetcolor()
{
    this->setStyleSheet("background-color: white; border-radius: 20px; border: 2px solid");
    resetcolour=true;
}

void Cube::mousePressEvent(QMouseEvent *ev)
{
   this->setStyleSheet("background-color: grey; border-radius: 20px; border: 2px solid");
   mousePos=QPoint(ev->x(),ev->y());
   press=true;
   resetcolour=false;
}
void Cube::mouseReleaseEvent(QMouseEvent *)
{
    judgesit();
    if(mouseinthecube)
    {
        emit clicked();
        press=false;
    }
    else
    {
        this->setStyleSheet("background-color: white; border-radius: 20px; border: 2px solid");
        press=false;
    }
    if(resetcolour)
    {
        this->setStyleSheet("background-color: white; border-radius: 20px; border: 2px solid");
    }
}
void Cube::receivejudge(bool flag)
{
    if(press&&flag)
        mouseinthecube=true;
    else
        mouseinthecube=false;
}
void Cube::mouseClicked()
{
    this->setStyleSheet("background-color: yellow; border-radius: 20px; border: 2px solid");
}
