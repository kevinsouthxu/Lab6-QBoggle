#ifndef WORDLISTWIDGET_H
#define WORDLISTWIDGET_H

#include "WordTable.h"
#include <QLabel>
#include <QQueue>
#include <QStack>
#include <QWidget>
#include<QFont>
#include "lexicon.h"
class WordListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordListWidget(QWidget *parent = nullptr, QString label = "");
    void addScore(int s);
    void addWord(QString word);
    void reset();

    void receivelexicon(Lexicon *l);
    //void receiveboard(int size,const QString *current);
    void resetvisit();
signals:
     void reject(QString rejectwords);
     void emitwordslist(QList<QString> words);
     void emitreplay(int t);
     void resetboard();
public slots:
     void receivewords(QString wordontable);
     void computreceivewords(QString worldontable);
     void receivewordslist(QList<QString> words);
     void receiveboard(int size,const QString *current);
     void replay();

     void receiveclickwords(QString string,int index);
private:
     bool aroundtheletter(int pos,int pre,int n,QString string);
    inline int index(int i, int j) { return i * boardsize + j; }
    QString label;
    QList<QString> words;
    int score;
    WordTable *wordTable;
    QLabel *nameLabel;
    QLabel *scoreLabel;


    QString helper;
    void computturn();
    void computrecur(int p);

    bool humanturn(QString word,int n);
    bool stringonboard(QString word,int n,int index);
    //保存board相关属性新建的变量，后期尝试去掉
    int boardsize;
    Lexicon *lex;
    QString *currentsit;
    bool *visitsit;

    bool humant;

    QString currentclick;
    QStack<int> in;
    bool *visitclick;
    void resetvisitclick();
    bool ifaround(int index);
    bool cango(int index);
};

#endif // WORDLISTWIDGET_H
