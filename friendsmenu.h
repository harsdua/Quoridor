#ifndef FRIENDSMENU_H
#define FRIENDSMENU_H

#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidget>

#include "database.h"
#include "client.h"
#include "observer_subject.h"


using namespace std;

class FriendsMenu: public QWidget, public Subject
{
    Q_OBJECT
public:
    FriendsMenu(QWidget *parent = nullptr, string pseudonyme = "", Client *client = nullptr);
    ~FriendsMenu();
    void displayMessage(QString message);
    void addRequestToList(string request);
     void updateFriendsMenu(); // Via les boutons

private:
    QLabel *friends, *requests, *search, *titleChat, *titleFriend;
    QListWidget *friendsList, *friendsNotif;
    QPlainTextEdit *messageOutput;
    QLineEdit *messageInput, *inputName, *nameInput;
    QPushButton *addFriend, *removeFriend, *acceptFriend, *declineFriend, *joinLobbyButton, *inviteLobbyButton;
    QList<QString> name; //pour les tests
    QList<QPushButton *> listButton;
    pthread_t update_thread;

    string pseudo;
    Client * client;
    Database db;


private slots:

    void addFriendToList();
    void removeFriendToList();


    void acceptFriendRequest();
    void declineFriendRequest();
    void removeRequestFromList(string request);


    void joinLobby();
    void clickedFriend();
    void writeMessage();

signals:
    void connectToLobby(string lobbyName);

};

#endif // FRIENDSMENU_H
