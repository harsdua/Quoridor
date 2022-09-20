#include "chatmessage.h"

ChatMessage::ChatMessage(QWidget *parent, string pseudo): QWidget(parent)
{
    title = new QLabel(this);
    title->setStyleSheet("background-color: rgb(63, 63, 63);");
    title->setText("<html><head/><body><p align=center><span style= font-size:20pt; text-decoration: underline;>Tchat !</span></p></body></html>");
    title->setGeometry(20, 20, 1700, 100);

    messageOutput = new QPlainTextEdit(this);
    messageOutput->setStyleSheet("background-color: rgb(134, 135, 127);");
    messageOutput->setGeometry(20, 150, 1710, 630);
    messageOutput->setReadOnly(true);

    messageInput = new QLineEdit(this);
    messageInput->setStyleSheet("background-color: rgb(134, 135, 127);");
    messageInput->setGeometry(450, 850, 1280, 80);
    messageInput->setPlaceholderText("Ecrivez votre message");
    connect(messageInput, SIGNAL(returnPressed()), this, SLOT(writeMessage()));

    nameInput = new QLineEdit(this);
    nameInput->setStyleSheet("background-color: rgb(134, 135, 127);");
    nameInput->setGeometry(20, 850, 400, 80);
    nameInput->setPlaceholderText("Ecrivez le pseudo de la personne à qui envoyer le message");

}

ChatMessage::~ChatMessage(){

}

void ChatMessage::displayMessage(QString message, QString name){

    messageOutput->appendPlainText(QString("message de %1: %2").arg(name).arg(message));
}

void ChatMessage::writeMessage(){

    QString message, name;
    message = messageInput->text();
    name = nameInput->text();

    displayMessage(message, name); // à mettre à la réception des messages

    messageInput->clear();
    nameInput->clear();
}
