#include "profil.h"
#include <QDebug>

Profil::Profil(QWidget *parent, string pseudo_): QWidget(parent), pseudonyme(pseudo_)
{

    informations = new QLabel(this);
    informations->setStyleSheet("background-color: rgb(63, 63, 63);");
    informations->setGeometry(20, 20, 1700, 100);
    informations->setText(QString("<html><head/><body><p align=center><span style= font-size:20pt; text-decoration: underline;>Informations relatives à votre profil !</span></p></body></html>"));

    pseudoLabel = new QLabel(this);
    pseudoLabel->setStyleSheet("background-color: rgb(63, 63, 63);");
    pseudoLabel->setGeometry(20, 180, 800, 50);
    pseudoLabel->setText(QString("<html><head/><body><p><span style= font-size:12pt; font-weight:700;>Votre pseudonyme :</span></p></body></html>"));

    pseudo = new QLineEdit(this);
    pseudo->setStyleSheet("background-color: rgb(84, 84, 84);");
    pseudo->setGeometry(20, 230, 800, 50);
    pseudo->setText(QString::fromStdString(pseudonyme));
    pseudo->setReadOnly(true);

    pseudoInput = new QLineEdit(this);
    pseudoInput->setStyleSheet("background-color: rgb(84, 84, 84);");
    pseudoInput->setGeometry(20, 300, 800, 50);
    pseudoInput->setPlaceholderText("Nouveau pseudo");

    updatePseudo = new QPushButton(this);
    updatePseudo->setText("Mettre à jour");
    updatePseudo->setStyleSheet("background-color: rgb(0, 0, 255);");
    updatePseudo->setGeometry(850, 305, 100, 40);
    connect(updatePseudo, SIGNAL(clicked(bool)), this, SLOT(changePseudo()));

    passwordLabel = new QLabel(this);
    passwordLabel->setStyleSheet("background-color: rgb(63, 63, 63);");
    passwordLabel->setGeometry(20, 370, 800, 50);
    passwordLabel->setText(QString("<html><head/><body><p><span style= font-size:12pt; font-weight:700;>Votre mot de passe :</span></p></body></html>"));

    password = new QLineEdit(this);
    password->setStyleSheet("background-color: rgb(84, 84, 84);");
    password->setGeometry(20, 420, 800, 50);
    password->setText(QString::fromStdString(db.getUserData(pseudonyme)[2]));
    password->setEchoMode(QLineEdit::Password);
    password->setReadOnly(true);

    passwordInput = new QLineEdit(this);
    passwordInput->setStyleSheet("background-color: rgb(84, 84, 84);");
    passwordInput->setGeometry(20, 490, 800, 50);
    passwordInput->setPlaceholderText("Nouveau mot de passe");
    passwordInput->setEchoMode(QLineEdit::Password);

    confirmPasswordInput = new QLineEdit(this);
    confirmPasswordInput->setStyleSheet("background-color: rgb(84, 84, 84);");
    confirmPasswordInput->setGeometry(20, 540, 800, 50);
    confirmPasswordInput->setPlaceholderText("confirmation le mot de passe");
    confirmPasswordInput->setEchoMode(QLineEdit::Password);

    updatePassword = new QPushButton(this);
    updatePassword->setText("Mettre à jour");
    updatePassword->setStyleSheet("background-color: rgb(0, 0, 255);");
    updatePassword->setGeometry(850, 545, 100, 40);
    connect(updatePassword, SIGNAL(clicked(bool)), this, SLOT(changePassword()));

    winrateLabel = new QLabel(this);
    winrateLabel->setStyleSheet("background-color: rgb(63, 63, 63);");
    winrateLabel->setGeometry(20, 610, 200, 50);
    winrateLabel->setText(QString("<html><head/><body><p><span style= font-size:12pt; font-weight:700;>Votre winrate :</span></p></body></html>"));

    winrate = new QLineEdit(this);
    winrate->setStyleSheet("background-color: rgb(84, 84, 84);");
    winrate->setGeometry(20, 660, 800, 50);
    winrate->setText(QString::fromStdString(to_string(db.getWinrate(pseudonyme))));
    winrate->setReadOnly(true);

    deleteAccountButton = new QPushButton(this);
    deleteAccountButton->setText("Supprimer mon compte");
    deleteAccountButton->setStyleSheet("background-color: rgb(255, 0, 0);");
    deleteAccountButton->setGeometry(20, 800, 500, 50);
    connect(deleteAccountButton, SIGNAL(clicked(bool)), this, SLOT(deleteAccount()));

    notifs = new QLabel(this);
    notifs->setStyleSheet("background-color: rgb(63, 63, 63);");
    notifs->setGeometry(1000, 180, 200, 50);
    notifs->setText(QString("<html><head/><body><p><span style= font-size:12pt; font-weight:700;>Vos notifications :</span></p></body></html>"));

    notificationList = new QPlainTextEdit(this);
    notificationList->setStyleSheet("background-color: rgb(84, 84, 84);");
    notificationList->setGeometry(1000, 230, 600, 650);
    notificationList->setReadOnly(true);

}

Profil::~Profil(){

}

void Profil::changePassword(){
    QString inputP, inputCP;
    inputP = passwordInput->text();
    inputCP = confirmPasswordInput->text();
    if (inputP == inputCP){
        db.changePassword(pseudonyme, inputCP.toStdString());
        QMessageBox::information(this, "Infos", "Ton mot de passe a été changé !");
    }else{
        QMessageBox::information(this, "Infos", "Les mots de passe ne sont pas identiques !");
    }
}

void Profil::changePseudo(){
    QString updatePs;
    updatePs = pseudoInput->text();
    if(!db.isUserRegistered(updatePs.toStdString())){
        db.changeUsername(pseudonyme, updatePs.toStdString());
        QMessageBox::information(this, "Infos", "Vous venez de changer votre pseudo !");
        pseudo->clear();
        pseudo->setText(updatePs);
        emit pseudoChanged(updatePs.toStdString());
    }else{
        QMessageBox::information(this, "Infos", "Ce pseudo est déjà utilisé, veuillez en choisir un nouveau !");
    }
}

void Profil::deleteAccount(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "Suppression de compte", "Vous allez supprimer votre compte de jeu actuel.\nCette action est irréversible, "
                                                         "êtes vous sur de vouloir continuer ?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if(reply == QMessageBox::Yes){
        emit deleteAccountSignal();
        db.deleteAccount(pseudonyme);
        db.disconnect();
        db.connect();
    }
}

void Profil::displayChatNotif(){
    notificationList->appendPlainText("Tu as reu un message, ouvre l'onglet Amis pour le consulter");
}

void Profil::displayNewFRNotif(){
    notificationList->appendPlainText("Tu as recu une nouvelle demande d'amis, ouvre l'onglet Amis pour la consulter");
}

void Profil::displayAcceptedFRNotif(string name){
    QString notif = QString::fromStdString(name) + " a accepté votre demande d'ami !";
    notificationList->appendPlainText(notif);
}

void Profil::displayDeclinedFRNotif(string name){
    QString notif = QString::fromStdString(name) + " a refusé votre demande d'ami !";
    notificationList->appendPlainText(notif);
}
void Profil::displayDeleteFriendNotif(string name){
    QString notif = QString::fromStdString(name) + " vous a retiré de sa liste d'amis, vous n'êtes plus amis avec lui !";
    notificationList->appendPlainText(notif);
}
