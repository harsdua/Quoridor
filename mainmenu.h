#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <cmath>

#include "database.h"

using namespace std;

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    MainMenu(QWidget *parent = nullptr, string pseudo = "");
    ~MainMenu();
    void displayLeaderBoard();

private:
    QLabel *title, *ladderLabel, *createLabel, *loadingLabel;
    QPlainTextEdit *ladder;
    QPushButton *createLobby, *loadParty, *refreshLadder;
    QLineEdit *lobbyName, *partyName;
    QPushButton *disconnectButton;
    Database db;

private slots:
    void startLobby();
    void leaveSystem();
    void updateRanking();

signals:
    void addLobbyToTab(string nameLobby);
    void askLeaveSystem();
};

#endif // MAINMENU_H
