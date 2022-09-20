#ifndef LOBBYGAME_H#define LOBBYGAME_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include "buttonbox.h"
#include "buttonwall.h"
#include "client.h"
#include "board.h"

#include <QDebug>

using namespace std;

class LobbyGame : public QWidget
{
public:
    Q_OBJECT

public:
    LobbyGame(QWidget *parent = nullptr, QString *pseudonyme = nullptr, string nameLobby = "", Client * client=nullptr, Lobby *lobby = nullptr, bool isHost = false);
    ~LobbyGame();
    void displayMessage(string message);
    void displayBoard(string board, string wall);
    void setStateButton(bool b);
    void initGame();
    bool isHost;

private slots:
    void exitLobby();
    void startGamePressed();
    void writeMessage();
    void displayRules();
    void sendMove(string move);
    void sendWall(string move, string dir);


signals:

    void returnMainClient();

private:
    int wallCount = 0;
    string dirWall;
    string wallToSend;
    QWidget *principalWidget;
    QList<ButtonBox*> buttonBoxList;
    QList<ButtonWall*> buttonWallH;
    QList<ButtonWall*> buttonWallV;
    QPlainTextEdit *tchatGame;
    QPushButton *border1, *border2;
    QLineEdit *inputMessage, *timerLabel;
    QString *pseudonyme, gamemodeSelect;
    QPushButton *saveGame, *surrendGame, *validMove, *validWall, *startGame, *rulesButton;
    QComboBox *gamemodeBox;
    QMainWindow *rulesWindow;

    Client * client;
    Lobby * lobby;
    string nameLobby;
    QPushButton* getBorderHost();
    QPushButton* getBorderGuest();
    void resetWallClicked();

};

#endif // LOBBYGAME_H
