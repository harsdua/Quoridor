#include "friendsmenu.h"
#include "database.h"
#include <QString>
#include <QDebug>
#include <Qt>
FriendsMenu::FriendsMenu(QWidget *parent, string pseudonyme, Client *client): QWidget(parent), pseudo(pseudonyme), client(client)
{

    titleFriend = new QLabel(this);
    titleFriend->setStyleSheet("background-color: rgb(63, 63, 63);");
    titleFriend->setText("<html><head/><body><p align=center><span style= font-size:20pt; text-decoration: underline;>Gestion des amis !</span></p></body></html>");
    titleFriend->setGeometry(20, 20, 800, 100);

    friends = new QLabel(this);
    friends->setStyleSheet("background-color: rgb(63, 63, 63);");
    friends->setText("<html><head/><body><span style= font-size:15pt; text-decoration: underline;>Liste d'amis</span></p></body></html>");
    friends->setGeometry(20, 120, 200, 30);

    friendsList = new QListWidget(this);
    friendsList->setStyleSheet("background-color: rgb(84, 84, 84);");
    friendsList->setGeometry(20, 150, 400, 450);
    connect(friendsList, &QListWidget::itemClicked, this, &FriendsMenu::clickedFriend);
    //friendsList->setReadOnly(true);

    requests = new QLabel(this);
    requests->setStyleSheet("background-color: rgb(63, 63, 63);");
    requests->setText("<html><head/><body><span style= font-size:15pt; text-decoration: underline;>Demandes</span></p></body></html>");
    requests->setGeometry(20, 610, 150, 50);

    friendsNotif = new QListWidget(this);
    friendsNotif->setStyleSheet("background-color: rgb(84, 84, 84);");
    friendsNotif->setGeometry(20, 650, 400, 180);

    search = new QLabel(this);
    search->setStyleSheet("background-color: rgb(63, 63, 63);");
    search->setText("<html><head/><body><span style= font-size:15pt; text-decoration: underline;>Faire une demande</span></p></body></html>");
    search->setGeometry(20, 840, 200, 30);

    inputName = new QLineEdit(this);
    inputName->setStyleSheet("background-color: rgb(84, 84, 84);");
    inputName->setGeometry(20, 870, 400, 50);
    inputName->setPlaceholderText("Entrez un nom");
    connect(inputName, &QLineEdit::returnPressed, this, &FriendsMenu::addFriendToList);



    acceptFriend = new QPushButton(this);
    acceptFriend->setText("Accepter la demande d'ami");
    acceptFriend->setStyleSheet("background-color: rgb(0, 255, 0);");
    acceptFriend->setGeometry(450, 670, 300, 40);
    connect(acceptFriend, SIGNAL(clicked(bool)), this, SLOT(acceptFriendRequest()));

    declineFriend = new QPushButton(this);
    declineFriend->setText("Refuser la demande d'ami");
    declineFriend->setStyleSheet("background-color: rgb(255, 0, 0);");
    declineFriend->setGeometry(450, 775, 300, 40);
    connect(declineFriend, SIGNAL(clicked(bool)), this, SLOT(declineFriendRequest()));

    addFriend = new QPushButton(this);
    addFriend->setText("Envoyer une demande d'ami");
    addFriend->setStyleSheet("background-color: rgb(0, 0, 255);");
    addFriend->setGeometry(450, 875, 300, 40);
    connect(addFriend, SIGNAL(clicked(bool)), this, SLOT(addFriendToList()));

    removeFriend = new QPushButton(this);
    removeFriend->setText("Supprimer de sa liste d'amis");
    removeFriend->setStyleSheet("background-color: rgb(255, 0, 0);");
    removeFriend->setGeometry(450, 505, 300, 40);
    connect(removeFriend, SIGNAL(clicked(bool)), this, SLOT(removeFriendToList()));

    titleChat = new QLabel(this);
    titleChat->setStyleSheet("background-color: rgb(63, 63, 63);");
    titleChat->setText("<html><head/><body><p align=center><span style= font-size:20pt; text-decoration: underline;>Tchat !</span></p></body></html>");
    titleChat->setGeometry(900, 20, 800, 100);

    messageOutput = new QPlainTextEdit(this);
    messageOutput->setStyleSheet("background-color: rgb(134, 135, 127);");
    messageOutput->setGeometry(900, 150, 800, 630);
    messageOutput->setReadOnly(true);

    messageInput = new QLineEdit(this);
    messageInput->setStyleSheet("background-color: rgb(134, 135, 127);");
    messageInput->setGeometry(1150, 850, 550, 80);
    messageInput->setPlaceholderText("Ecrivez votre message");
    connect(messageInput, SIGNAL(returnPressed()), this, SLOT(writeMessage()));

    nameInput = new QLineEdit(this);
    nameInput->setStyleSheet("background-color: rgb(134, 135, 127);");
    nameInput->setGeometry(900, 850, 200, 80);
    nameInput->setPlaceholderText("Ecrivez un pseudo");


    joinLobbyButton = new QPushButton(this);
    joinLobbyButton->setText("Rejoindre le lobby");
    joinLobbyButton->setStyleSheet("background-color: rgb(67, 239, 255);");
    joinLobbyButton->setGeometry(450, 205, 300, 40);
    joinLobbyButton->setEnabled(false);
    connect(joinLobbyButton, SIGNAL(clicked(bool)), this, SLOT(joinLobby())); // TODO bug sur le fait que les boutons pour les personnes ajoutées ne sont pas affichés


    updateFriendsMenu();
}

FriendsMenu::~FriendsMenu(){

}

void FriendsMenu::updateFriendsMenu(){
    friendsList->clear();
    for(auto f : db.getFriendsList(pseudo)){
        friendsList->addItem(QString::fromStdString(db.getUsernameFromUUID(f)));
    }
    friendsNotif->clear();
    for(auto f : db.getFriendRequestsList(pseudo)){
        friendsNotif->addItem(QString::fromStdString(db.getUsernameFromUUID(f)));
    }
    friendsList->update();
    friendsNotif->update();
}

void FriendsMenu::addFriendToList(){
    db.disconnect();
    db.connect();
    string sendRequestTo = inputName->text().toStdString();
    if(sendRequestTo == pseudo){
        QMessageBox::information(this, "Infos", "Tu ne peux pas t'envoyer une demande d'ami à toi-même");
    }else if(!db.isUserRegistered(sendRequestTo)){
        QMessageBox::information(this, "Infos", "Cette personne n'existe pas !");
    }else if(!db.isFriendRequestExist(pseudo, sendRequestTo)){
        string req = "Tu viens d'envoyer une demande d'ami à " + sendRequestTo;
        string sendRequest = "/sendFriendRequest " + sendRequestTo;
        client->TrySendMessage(sendRequest);
        QMessageBox::information(this, "Infos", QString::fromStdString(req));
        inputName->clear();
        //attendre la notif pour savoir si la demande est acceptee puis appeler updateFriendsMenu;
    }else{
        QMessageBox::information(this, "Infos", "Tu as déjà envoyé une requête d'ami à cette personne !");
    }
}

void FriendsMenu::removeFriendToList(){
    db.disconnect();
    db.connect();
    if(friendsList->count() > 0){
        if(friendsList->currentItem() != NULL){
            string sendRequestTo = friendsList->currentItem()->text().toStdString();
            if(!db.isUserRegistered(sendRequestTo)){
                QMessageBox::information(this, "Infos", "Cette personne n'existe pas !");
            }else if(db.isFriends(pseudo, sendRequestTo)){
                db.removeFriends(pseudo, sendRequestTo);
                string req = "Tu viens de supprimer " + sendRequestTo + " de tes amis !";
                string acceptRequest = "/deleteFriend " + sendRequestTo;
                client->TrySendMessage(acceptRequest);
                QMessageBox::information(this, "Infos", QString::fromStdString(req));
                friendsList->takeItem(friendsList->currentRow());
                updateFriendsMenu();

            }
        } else {
            QMessageBox::information(this, "Infos", "Sélectionnez un ami de la liste");
        }
    } else {
        QMessageBox::information(this, "Infos", "Tu n'as pas d'amis");
    }
}

void FriendsMenu::acceptFriendRequest(){
    db.disconnect();
    db.connect();
    if(friendsNotif->count() > 0){
        if(friendsNotif->currentItem() != NULL){
            string sendRequestTo = friendsNotif->currentItem()->text().toStdString();
            if(!db.isUserRegistered(sendRequestTo)){
                QMessageBox::information(this, "Infos", "Cette personne n'existe pas/plus !");
            }else if(db.isFriendRequestExist(sendRequestTo, pseudo)){
                //db.respToFriendRequestFrom(sendRequestTo, pseudo, true);
                string acceptRequest = "/acceptedRequest " + sendRequestTo;
                client->TrySendMessage(acceptRequest);
                string req = "Tu viens d'accepter la demande d'ami de " + sendRequestTo;
                QMessageBox::information(this, "Infos", QString::fromStdString(req));
                removeRequestFromList(sendRequestTo);
                updateFriendsMenu();
            }else{
                QMessageBox::information(this, "Infos", "Tu n'as pas de demande d'ami de cette personne !");
            }
        } else {
            QMessageBox::information(this, "Infos", "Sélectionnez une demande de la liste");
        }
    } else {
        QMessageBox::information(this, "Infos", "Tu n'as pas de demandes d'amis");
    }
}

void FriendsMenu::declineFriendRequest(){
    db.disconnect();
    db.connect();
    if(friendsNotif->count() > 0){
        if(friendsNotif->currentItem() != NULL){
            string sendRequestTo = friendsNotif->currentItem()->text().toStdString();
            if(!db.isUserRegistered(sendRequestTo)){
                QMessageBox::information(this, "Infos", "Cette personne n'existe pas/plus !");
            }else if(db.isFriendRequestExist(sendRequestTo, pseudo)){

                string declineRequest = "/declinedRequest " + sendRequestTo;
                client->TrySendMessage(declineRequest);
                string req = "Tu viens de refuser la demande d'ami de " + sendRequestTo;
                QMessageBox::information(this, "Infos", QString::fromStdString(req));

                removeRequestFromList(sendRequestTo);
                updateFriendsMenu();
            }
        } else{
            QMessageBox::information(this, "Infos", "Sélectionnez une demande de la liste");
        }
     } else {
        QMessageBox::information(this, "Infos", "Tu n'as pas de demandes d'amis");
    }
}

void FriendsMenu::addRequestToList(string request){
    friendsNotif->addItem(QString::fromStdString(request));
    friendsNotif->update();
}

void FriendsMenu::removeRequestFromList(string request){
    QListWidgetItem * item = friendsNotif->findItems(QString::fromStdString(request), Qt::MatchExactly)[0];
    friendsNotif->setCurrentItem(item);
    friendsNotif->takeItem(friendsNotif->currentRow());
}

void FriendsMenu::joinLobby(){
    string lobbyName = db.getLobbyNameFromPlayer(friendsList->currentItem()->text().toStdString());
    emit connectToLobby(lobbyName);
}

void FriendsMenu::clickedFriend(){
    if(friendsList->count() > 0){
        QString clicked = friendsList->currentItem()->text();
        nameInput->setText(clicked);
        if(db.isInLobby(clicked.toStdString())){
            joinLobbyButton->setEnabled(true);
        }else{
            joinLobbyButton->setEnabled(false);
        }
    }
}

void FriendsMenu::displayMessage(QString message){

    messageOutput->appendPlainText(QString("message à %1").arg(message));
}

void FriendsMenu::writeMessage(){

    QString message, name;
    message = messageInput->text();
    name = nameInput->text();
    string msg = "/sendMsg " + name.toStdString() + ' ' + '"' + message.toStdString()+ '"';
    client->TrySendMessage(msg);
    //displayMessage(message); // à mettre à la réception des messages

    messageInput->clear();
    nameInput->clear();
}

