#ifndef PROFIL_H
#define PROFIL_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QPlainTextEdit>
#include <QMessageBox>
#include "database.h"


using namespace std;

class Profil : public QWidget
{
    Q_OBJECT
public:
    Profil(QWidget *parent = nullptr, string pseudo = "");
    ~Profil();
    void displayChatNotif();
    void displayStateFRNotif();
    void displayNewFRNotif();
    void displayInvitationNotif();
    void displayAcceptedFRNotif(string name);
    void displayDeclinedFRNotif(string name);
    void displayDeleteFriendNotif(string name);

private:
    QPushButton *updatePseudo, *updatePassword, *deleteAccountButton;
    QPlainTextEdit *notificationList;
    QLineEdit *pseudoInput, *passwordInput, *confirmPasswordInput, *password, *pseudo, *winrate;
    QLabel *informations, *winrateLabel, *pseudoLabel, *passwordLabel, *notifs;
    Database db;

    string pseudonyme;

private slots:
    void changePassword();
    void changePseudo();
    void deleteAccount();

signals:
    void deleteAccountSignal();
    void pseudoChanged(string pseudo);
};

#endif // PROFIL_H
