#ifndef BOGGLEWINDOW_H
#define BOGGLEWINDOW_H

#include "Board.h"
#include "Console.h"
#include "WordListWidget.h"
#include "lexicon.h"
#include <QMainWindow>

class BoggleWindow : public QMainWindow
{
    Q_OBJECT

public:
    BoggleWindow(QWidget *parent = 0);
    ~BoggleWindow();
private:
    WordListWidget *me;
    WordListWidget *computer;
    Board *board;
    Console *console;
    Lexicon *lex;
    static const int BOGGLE_WINDOW_WIDTH = 1600;
    static const int BOGGLE_WINDOW_HEIGHT = 1200;
};

#endif // BOGGLEWINDOW_H
