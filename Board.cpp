#include "Board.h"
#include "Cube.h"
#include <QMouseEvent>
#include <QGridLayout>


const QString Board::STANDARD_CUBES[16]  = {
        "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
        "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
        "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
        "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const QString Board::BIG_BOGGLE_CUBES[25]  = {
        "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
        "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
        "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
        "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
        "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Board::Board(QWidget *parent, int size, const QString *cubeLetters) : QWidget(parent)
{
    this->size = size;
    this->cubes = new Cube*[size * size];
    this->letters = new QString[size * size];
    this->currentvisit = new bool[size * size];
    this->currentcubeletter = new QString[size * size];
    for (int i = 0; i < size * size; ++i)
     {this->letters[i] = cubeLetters[i];
      this->currentvisit[i] = false;}

    QGridLayout *layout = new QGridLayout();
    layout->setMargin(20);
    layout->setSpacing(10);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)] = new Cube(this);
            layout->addWidget(this->cubes[index(i, j)], i, j, Qt::AlignmentFlag::AlignCenter);
            connect(cubes[index(i,j)],SIGNAL(judgesit()),this,SLOT(judgemouse()));
            connect(this,SIGNAL(mouseincu(bool)),cubes[index(i,j)],SLOT(receivejudge(bool)));
            connect(this,SIGNAL(resetcube()),cubes[index(i,j)],SLOT(resetcolor()));
        }
    }
    setLayout(layout);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)]->setLetter(this->letters[index(i, j)].at(0)); //设置了cube的letter，在打散中依然可用这个方法，
            vec.append(letters[index(i,j)]);
            this->currentcubeletter[index(i,j)]=letters[index(i,j)].at(0);
        }
    }



    shake();
    // this->setStyleSheet("background-color:grey; border: 3px solid");
}


Board::~Board()
{
    if (cubes) delete[] cubes;
    if (letters) delete[] letters;
    vec.clear();
}

void Board::shake()
{
    // Shake Cubes
    random();
    int t;
    for(int i=0;i<vec.size()-1;i++) //随机每个筛子的位置
    {
        t=qrand() % vec.size();
        while(t<=i) {random();
        t=qrand() % vec.size();}
        QString temp;
        temp = vec[i];
        vec[i] = vec[t];
        vec[t] = temp;
    }
    for (int i = 0; i < size; ++i) { //在完成给每个筛子赋letter值的情况下，随机每个骰子的面
            for (int j = 0; j < size; ++j) {
                random();
                int t=qrand() % 6;
                if(this->vec[index(i,j)].at(t)=="Q")
                {
                    QString temp;
                    temp.append("Q");temp.append("u");
                    this->cubes[index(i, j)]->setLetter(temp);
                    this->currentcubeletter[index(i,j)]=vec[index(i,j)].at(t);
                }
                else{
                this->cubes[index(i, j)]->setLetter(this->vec[index(i, j)].at(t));
                this->currentcubeletter[index(i,j)]=vec[index(i,j)].at(t);
                }
            }
    }
   emitboard(size,currentcubeletter);
}
void Board::random()
{
    static int count=0;
    QTime time;
    time=QTime::currentTime();
    qsrand(time.msec()+time.second()*(1000+count*4));
    count++;
}
void Board::replay()
{
    shake();
}
void Board::judgemouse()
{
  for(int i=0;i<size * size;i++)
  {
      if(cubes[i]->returnpress())
          if(cubes[i]->geometry().contains(this->mapFromGlobal(QCursor::pos())))
          {emit mouseincu(true);
           currentcli.append(cubes[i]->returnletter());
           emit emitclickword(currentcli,i);
           currentcli.clear();
           return;
          }
  }
  emit mouseincu(false);
}
void Board::resetclickedboard()
{
    emit resetcube();
}
