#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lobbygame.h"
#include "login.h"
#include "chatmessage.h"
#include "profil.h"
#include "mainmenu.h"
#include "friendsmenu.h"
#include "lobby.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
using namespace std;
class MainWindow : public QMainWindow, public Observer
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Login *loginTab;
    LobbyGame *lobbyTab;
    ChatMessage *chatMessageTab;
    Profil *profilTab;
    MainMenu *mainMenuTab;
    FriendsMenu *friendsTab;

    bool isConnectedToMaster = false;

    QString *pseudo;
    string pp;

    pthread_t thread_client;
    pthread_t thread_client_lobby;
    pthread_t thread_lobby;

    Client* client_p = nullptr;
    Client* client_lobby = nullptr;
    Lobby* lobby_p = nullptr;



private slots:
    void setupTabs(string pseudo);
    void addLobbyTab(string nameLobby);
    void showProfile();
    void returnLoginTab();
    void updateWindowTitle(string pseudo);

    std::string getPseudo(); // ne sert plus
    void setPseudo(string str);

    pthread_t getThreadClient();
    pthread_t getThreadLobby();

    void createThreadClient();
    void createThreadLobby(string nameOfLobby);
    void connectToThreadLobby(string nameOfLobby, bool isHost = false);

    static void * handle_thread_client(void* client);
    static void * handle_thread_lobby(void* lobby);

    void subjectChat(string message)override;
    void subjectNotifFR(string name)override;
    void subjectAccept(string name)override;
    void subjectAllChat(string message)override;
    void subjectDecline(string name) override;
    void subjectMove(string board) override;
    void subjectDeleteFriend(string name)override;

    void subjectDisplayBoard(string board, string wall) override;
    void subjectTurnPlayer(int playerId) override;
    void subjectGameLaunch() override;

};
#endif // MAINWINDOW_H
