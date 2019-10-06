#include "BoggleWindow.h"
#include "lexicon.h"

#include <QFile>
#include <QHBoxLayout>
#include <QTextEdit>
#include <iostream>

BoggleWindow::BoggleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("QBoggle!");
    this->setFixedSize(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);

    board = new Board(this);
    me = new WordListWidget(this, "Me");
    computer = new WordListWidget(this, "Computer");
    console = new Console(this);

    me->setGeometry(40, 40, 460, 600);
    board->setGeometry(500, 40, 600, 600);
    computer->setGeometry(1600 - 40 - 460, 40, 460, 600);
    console->setGeometry(60, 640, 1480, 520);


    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }
    lex = new Lexicon(qFile);

   /*
    for (std::string s: lex) {
        std::cout << s << std::endl;
    }
   */
    console->write("Welcome to the game Boggle!\n");

    me->receivelexicon(lex);
    computer->receivelexicon(lex);
    me->receiveboard(board->returnsize(),board->returncurrent());
    computer->receiveboard(board->returnsize(),board->returncurrent());
    connect(board,SIGNAL(emitboard(int ,const QString *)),me,SLOT(receiveboard(int,const QString*)));
    connect(board,SIGNAL(emitboard(int ,const QString *)),computer,SLOT(receiveboard(int,const QString*)));
    connect(console,SIGNAL(newLineWritten(QString)),me,SLOT(receivewords(QString)));
    connect(me,SIGNAL(reject(QString)),console,SLOT(write(QString)));
    connect(console,SIGNAL(newLineWritten(QString)),computer,SLOT(computreceivewords(QString)));
    connect(me,SIGNAL(emitwordslist(QList<QString> )),computer,SLOT(receivewordslist(QList<QString> )));
    connect(me,SIGNAL(emitreplay(int)),me,SLOT(replay()));
    connect(me,SIGNAL(emitreplay(int)),computer,SLOT(replay()));
    connect(me,SIGNAL(emitreplay(int)),board,SLOT(replay()));
    connect(board,SIGNAL(emitclickword(QString,int)),me,SLOT(receiveclickwords(QString,int)));
    connect(me,SIGNAL(resetboard()),board,SLOT(resetclickedboard()));
}

BoggleWindow::~BoggleWindow()
{
    disconnect(console,SIGNAL(newLineWritten(QString)),me,SLOT(receivewords(QString)));
    disconnect(me,SIGNAL(reject(QString)),console,SIGNAL(write(QString)));
    disconnect(console,SIGNAL(newLineWritten(QString)),computer,SLOT(computreceivewords(QString)));
    disconnect(me,SIGNAL(emitwordslist(QList<QString> )),computer,SLOT(receivewordslist(QList<QString> )));
}
