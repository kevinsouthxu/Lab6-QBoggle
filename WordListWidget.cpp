#include "WordListWidget.h"
#include "WordTable.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTableView>

WordListWidget::WordListWidget(QWidget *parent, QString label) : QWidget(parent)
{
    reset();

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *headLayout = new QHBoxLayout();

    nameLabel = new QLabel(this);
    scoreLabel = new QLabel(this);
    QFont font = nameLabel->font();
    font.setPointSize(20);
    nameLabel->setFont(font);
    scoreLabel->setFont(font);
    nameLabel->setText(label);
    scoreLabel->setText(QString::number(score));

    headLayout->addWidget(nameLabel, 0, Qt::AlignmentFlag::AlignLeft);
    headLayout->addWidget(scoreLabel, 0, Qt::AlignmentFlag::AlignRight);
    layout->addLayout(headLayout);

    QFrame *hline = new QFrame();
    hline->setFrameShape(QFrame::HLine);
    hline->setFrameShadow(QFrame::Sunken);
    layout->addWidget(hline);

    wordTable = new WordTable();
    layout->addWidget(wordTable);

    setLayout(layout);

    boardsize=0;
    humant=true;
}
void WordListWidget::receivewordslist(QList<QString> words)
{
    this->words=words;
}
void WordListWidget::receivewords(QString worldontable)
{
    worldontable=worldontable.toLower();
    if(worldontable=="")
    {
    reject("if you want to see AI,please enter '@'");
    return;
    }
    if(worldontable=="@")
    {emitwordslist(words);
    humant=false;return;}
    if(worldontable=="r"||worldontable=="R")
    {
     emitreplay(1);
     return;
    }
    if(!humant)
    {
     reject("you have already called AI to finish your job\nIf you want to play another round, please enter 'r'\n");
     return;
    }
    if(worldontable.length()<4)
    {
     reject("Must be at least four letters!!\nif you want to see AI,please enter '@'");
     return;
    }
    if (humanturn(worldontable,0)&&lex->contains(worldontable.toStdString())) {
     if(!words.contains(worldontable))
       {
        addScore(worldontable.length()-3);
        addWord(worldontable);
     }
     else {reject("you have already written this word~");
          }
    }
    else {reject("this word doesn't exist");}
}
void WordListWidget::computreceivewords(QString worldontable)
{
    if(worldontable=="@") {computturn();return;}
}

void WordListWidget::resetvisit()
{
    for(int i=0;i<boardsize * boardsize;i++)
    {
        visitsit[i]=false;
    }
}
void WordListWidget::computturn()
{
    resetvisit();
    helper.clear();
    for(int i=0;i < boardsize * boardsize;i++)
    {
      visitsit[i]=true;
      helper.append(currentsit[i]);
      if(currentsit[i]=="Q") helper.append("U");
      computrecur(i);
      helper.clear();
      visitsit[i]=false;
    }

}
void WordListWidget::computrecur(int p)
{
    if(lex->contains(helper.toStdString())&&helper.length() >= 4&&!words.contains(helper.toLower()))
    {addWord(helper.toLower());
     addScore(helper.length()-3);
    }
    for(int i=(p / boardsize-1);i<=(p / boardsize+1);i++)
        for(int j=(p % boardsize-1);j<=(p % boardsize+1);j++)
        {
            if(i>=0&&j>=0&&i<=boardsize-1&&j<=boardsize-1&&index(i,j)!=p&&!visitsit[index(i,j)])
            {
                helper.append(currentsit[index(i,j)]);
                visitsit[index(i,j)]=true;
                if(lex->containsPrefix(helper.toStdString())) computrecur(index(i,j));
                visitsit[index(i,j)]=false;
                helper.remove(helper.length()-1,1);
            }
        }

}
bool WordListWidget::humanturn(QString word, int n)
{
    resetvisit();
    for(int i=0;i<boardsize;i++)
    {
        for(int j=0;j<boardsize;j++)
        {if(currentsit[index(i,j)]==word.at(0).toUpper())
            {
                visitsit[index(i,j)]=true;
                if(currentsit[index(i,j)]=="Q"&&word.at(1).toUpper()=="U")  //Qu type, still got problem
                 {if(stringonboard(word,n+2,index(i,j)))
                    return true;}
                else
                 {if(stringonboard(word,n+1,index(i,j)))
                    return true;}
                visitsit[index(i,j)]=false;
            }
        }
    }
    return false;
}
bool WordListWidget::stringonboard(QString word,int n,int in)
{
if(n==word.length()) return true;
 for(int i = (in / boardsize) - 1;i <= (in / boardsize) + 1 ;i++)
 {
  for(int j = (in % boardsize) - 1;j <= (in % boardsize) + 1;j++)
    if(i>=0&&j>=0&&i<=4&&j<=4)
    if(currentsit[index(i,j)]==word.at(n).toUpper()&&!visitsit[index(i,j)])
    {
        visitsit[index(i,j)]=true;
        if((n-2)>=0&&word.at(n-2).toUpper()=="Q"&&word.at(n-1).toUpper()=="U") //Qutype
        {
         if(aroundtheletter(index(i,j),in,(n-2),word))
         {if(currentsit[index(i,j)]=="Q"&&word.at(n+1).toUpper()=="U") //Qu type
           {if(stringonboard(word,n+2,index(i,j)))
              return true;}
          else
           {if(stringonboard(word,n+1,index(i,j)))
              return true;}
        }
        }
        else
        {
          if(aroundtheletter(index(i,j),in,(n-1),word))
            {if(currentsit[index(i,j)]=="Q"&&word.at(n+1).toUpper()=="U") //Qu type
             {if(stringonboard(word,n+2,index(i,j)))
                 return true;}
             else
              {if(stringonboard(word,n+1,index(i,j)))
                 return true;}
           }
        }
        visitsit[index(i,j)]=false;
    }
 }
 return false;
}
bool WordListWidget::aroundtheletter(int pos,int pre,int n,QString string)
{
    if(n==-1) return true;
    for(int i=(pos / boardsize)-1;i<=(pos / boardsize)+1;i++)
        for(int j=(pos % boardsize)-1;j<=(pos % boardsize)+1;j++)
        {
            if(i>=0&&j>=0&&i<=boardsize-1&&j<=boardsize-1&&index(i,j)!=pos)
                if(visitsit[index(i,j)]&&currentsit[index(i,j)]==string.at(n).toUpper()&&pre==index(i,j)) return true;
        }
    return false;
}
void WordListWidget::receiveboard(int size,const QString *b)
{
    boardsize=size;
    currentsit=new QString[size * size];
    visitsit=new bool[size * size];
    visitclick=new bool[size * size];
    for(int i=0;i<size * size;i++)
    {
        currentsit[i]=b[i];
        visitsit[i]=false;
        visitclick[i]=false;
    }
}
void WordListWidget::receiveclickwords(QString string,int index)
{
    if(!humant)
    {
     reject("you have already called AI to finish your job\nIf you want to play another round, please enter 'r'\n");
     emit resetboard();
     return;
    }
    currentclick.append(string.toLower());
    if(ifaround(index)&&lex->containsPrefix(currentclick.toStdString())&&cango(index))
      {in.push(index);
       if(lex->contains(currentclick.toStdString())&&!words.contains(currentclick)&&currentclick.length()>3)
       {
           addScore(currentclick.length()-3);
           addWord(currentclick);
           in.clear();
           currentclick.clear();
           resetvisitclick();
           emit resetboard();
       }
      }
    else
    {
       in.clear();
       currentclick.clear();
       resetvisitclick();
       emit resetboard();
    }
}
void WordListWidget::resetvisitclick()
{
    for(int i=0;i<boardsize * boardsize;i++)
    {
        visitclick[i]=false;
    }
}
bool WordListWidget::ifaround(int inde)
{
    if(in.empty()) {
        visitclick[inde]=true;
        return true;}
    int temp=in.top();
    for(int i=(temp / boardsize)-1;i<=(temp / boardsize)+1;i++)
        for(int j=(temp % boardsize)-1;j<=(temp % boardsize)+1;j++)
        {
            if(i>=0&&j>=0&&i<=boardsize-1&&j<=boardsize-1&&index(i,j)!=temp&&!visitclick[index(i,j)])
                if(inde==index(i,j)) {
                    visitclick[index(i,j)]=true;
                    return true;
                }
        }
    return false;
}
bool WordListWidget::cango(int inde)
{
    if(in.empty()) return true;
    for(int i=(inde / boardsize)-1;i<=(inde / boardsize)+1;i++)
        for(int j=(inde % boardsize)-1;j<=(inde % boardsize)+1;j++)
        {
            if(i>=0&&j>=0&&i<=boardsize-1&&j<=boardsize-1&&index(i,j)!=inde)
                if(!in.contains(index(i,j))) {
                    return true;
                }
        }
    return false;
}
void WordListWidget::receivelexicon(Lexicon *l)
{
    lex=l;
}
void WordListWidget::addScore(int score)
{
    this->score += score;
    scoreLabel->setText(QString::number(this->score));
}
void WordListWidget::addWord(QString word)
{
    this->words.append(word);
    this->wordTable->addWord(word);
}
void WordListWidget::reset()
{
    this->score = 0;
    this->words.clear();

}
void WordListWidget::replay()
{
    reset();
    humant=true;
    wordTable->clear();
    wordTable->reset();
    wordTable->viewport()->update();
    scoreLabel->setText(QString::number(this->score));
    in.clear();
    currentclick.clear();
    emit resetboard();
}
