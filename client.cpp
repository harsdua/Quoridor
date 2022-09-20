#include "client.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>

//#include <QDebug>


#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <cstdlib>

#include <vector>
#include <iostream>

using namespace std;


Client::Client(string pseudo, const int& port, bool inUI) : pseudo{pseudo}, port_server{port}, inUi{inUI}, host{false} {
    me = this; // utilisé pour accéder aux attrbuts dans un thread

    if (tryConnect() < 0) {

    } else if (port==8080) {
        isConnect = true;
        cout << "Vous êtes connecté avec succès au masterServer de Quoridor" << endl;
        cout << "Pour voir les commandes disponibles, tapez /help." << endl << endl;
    } else {
        isConnect = true;
        system("clear");
        cout << "Vous, " << pseudo << ", est connecté à un lobby" << endl;
        cout << "Pour voir les commandes disponibles, tapez /help." << endl << endl;
    }

  if (isTerminal()) {
    string line;
    //reçoit les input et les gère

    while(getline(cin,line)){
      TrySendMessage(line);
    }
  }
  
}


string Client::getPseudo() {
  return pseudo;
}

string Client::getIP() {
  return ip;
}
  
pthread_t Client::getTid(){
  return tids[0];
}

int Client::getSocket() {
  return sock;
}

int Client::getPortLobby() {
  return port_lobby;
}

void Client::setSocket(const int &newSocket) {
  sock=newSocket;
}

void Client::setHost() {
  host=true;
}

void Client::removeHost() {
  host=false;
}

void Client::setLobbyPort(const int& newPort) {
  port_lobby=newPort;
}


bool Client::isHost() {
  return host;
}


bool Client::isTerminal() {
    return !inUi;
}

bool Client::isConnected(){
    return isConnect;
}

void Client::connectClientToLobby(const string &nameLobby) {
    int port = database.getPortFromNameLobby(nameLobby);
    setLobbyPort(port);
    string pseudonyme(pseudo);
    database.addInLobby(pseudonyme, nameLobby);

    if (isTerminal()) {
        string terminalcmd = "gnome-terminal -- bash -c \"./client_terminal ";
        terminalcmd.append(getPseudo());
        terminalcmd.append(" ");
        terminalcmd.append(to_string(port));
        terminalcmd.append("\"");
        int exit_status = system(terminalcmd.c_str());
    }

}

void Client::createLobby() {
    if (isTerminal()) {
        string terminalcmd = "gnome-terminal -- bash -c \"./serveur lobby ";
        terminalcmd.append(to_string(getPortLobby()));
        terminalcmd.append("\"");
        int exit_status = system(terminalcmd.c_str());
    }
}


int Client::setLobby(const string& nameLobby) {
    if (isHost()) {
        printf("Un lobby existe déjà pour le joueur, terminez-le avant d'en créer un nouveau.\n");
        return 0;
    }
    if(database.isLobbyNameInDB(nameLobby)){
        printf("Vous essayez de créer un lobby sous un nom qui existe déjà ! \n"); return false;
    }
    
    int randomPort = database.getFreePort();

    database.createLobbyInDB(nameLobby, randomPort, 0);

    setLobbyPort(randomPort);
    
    setHost();
    string port=to_string(randomPort);

    printf("\n \n \n En tant qu'hôte, vous avez la responsabilité de terminer le hall : \n");
    printf("Si le lobby se termine, en raison d'un crash, de la fin d'un jeu...  \n");
    printf("ou si vous décidez de terminer le lobby.\n \n \n");
    printf("n'oubliez pas de fermer la fenêtre du terminal et tapez /endLobby dans cette fenêtre du terminal \n");
    
    if (isTerminal()) {
        createLobby();
        connectClientToLobby(nameLobby);
    }
    return randomPort;
}

void Client::killLobby() {
    if (isHost()) {
        printf("Lobby fermé \n");
        removeHost();
    } else {

        setLobbyPort(0);
    }
}


void * Client::userThreadFun(void *vargp){
    //int sock = *(int*) vargp;
    Message msg;

    while(readMessage(me->getSocket(),msg)){
        if (msg.message[0]=='/') {
            me->processCmd(msg);
        } else {
            if(!me->inUi){
                cout<<endl<<msg.message<<endl;
            }else{
                string toSend(msg.message);
                me->notifyObserversChat(toSend);

            }
        }
        free(msg.message);
    }
    //_exit(EXIT_FAILURE);
    return nullptr;
}


void Client::sendUserName() {
    //initialise le message6
    Message msg;
    time(&msg.timestamp);
    msg.message = strdup(getPseudo().c_str());
    msg.size = strlen(msg.message);

    //envoie le message contenant le pseudo
    sendMessage(getSocket(), msg);
    free(msg.message);
}


int Client::tryConnect() {

  //create socket
  struct sockaddr_in serv_addr;
  setSocket(socket(AF_INET, SOCK_STREAM, 0));
  if (getSocket() < 0)
  {
    printf("\n Erreur de création de socket \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET; //ipv4 families
  serv_addr.sin_port = htons(port_server);//convert to network byte order

  // Convert IP addresses from text to binary form
  if(inet_pton(AF_INET, getIP().c_str(), &serv_addr.sin_addr)<=0)
    //error for incompatible address provided
  {
    printf("\n Adresse non valide/ Adresse non supportée \n");
    return -1;
  }

  //socket connection
  if (connect(getSocket(), (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\n Échec de la connexion \n");
    return -1;
  }

  //init message with username
  sendUserName();

  //thread responsible for recieved messages
  pthread_create(&tids[0],NULL,Client::userThreadFun, NULL);

  return 1;
}


void Client::TrySendMessage(const string &line) {
  
    Message msg;
    time(&msg.timestamp);
    msg.message = strdup(line.c_str());
    msg.size = strlen(msg.message);
    
    if (msg.size == 0){
        cout << "Essayez /help pour afficher les commandes disponibles" << endl;
    }else if (HandleCmdSend(msg)) {
        sendMessage(getSocket(),msg);
    }
    
    free(msg.message);
}


void Client::consultLeaderBoard() {
    cout << "Classement:" << endl;
    vector<string> leaderBoard = database.getLeaderBoard();
    for (size_t i=0; i < leaderBoard.size(); i++){
        cout << leaderBoard[i] << "  " << "Taux de victoire: " << database.getWinrate(leaderBoard[i]) << endl;
    }
}

void Client::consultFriendsList() {
    string user = getPseudo();
    vector<string> friendsList = database.getFriendsList(user);
    cout << "Friends list:" << endl;
    for (size_t i = 0; i < friendsList.size(); i++){
        cout << database.getUsernameFromUUID(friendsList[i]) << endl;
    }
}


void Client::whoAmI() {
    cout << "Votre pseudonyme est : "<< pseudo << endl;
}

void Client::showHelp() {

    cout << "Bienvenue à Quoridor, " << pseudo << endl<< endl;
    cout << "[ --- Concernant les amis --- ]" << endl;
    cout << "Pour communiquer ou jouer avec quelqu'un, il faut qu'il soit votre ami" << endl;
    cout << "Pour envoyer un demande d'ami, tape: /sendFriendRequest username" << endl;
    cout << "Lorsque vous recevez une demande, vous recevez des instructions pour l'accepter." << endl;
    cout << "Pour envoyer des messages, tapez : /sendMsg nom d'utilisateur \"message\". \ 
    N'oubliez pas d'écrire le message entre guillemets !" << endl;
    cout << "Vous pouvez consulter les classements en tapant : /leaderBoard" << endl;
    cout << "Vous pouvez consulter votre liste d'amis en tapant : /getFriendList" << endl;
    cout << "Pour supprimer quelqu'un, tapez : /deleteFriend friendName" << endl;


    cout << endl<< "[ --- Concernant le jeu --- ]" << endl;
    cout << "Pour lancer le jeu, vous devez rejoindre un lobby. L'hôte est chargé de lancer le jeu" << endl;
    cout << "Vous pouvez héberger un lobby en tapant : /startLobby port. Le port saisi doit être libre d'utilisation." << endl;
    cout << "Lors de l'hébergement, vous rejoignez automatiquement le lobby que vous avez créé" << endl;
    cout << "Vous pouvez commencer une partie lorsque 2 ou 4 personnes se trouvent dans votre lobby en tapant : /startGame" << endl;
    cout << "En tant qu'hôte, vous pouvez inviter vos amis en tapant /inviteUser friendName" << endl;
    cout << "Vous pouvez abandonner une partie en tapant /abandon" << endl;
    cout << "Par le biais de l'invitation, vous pourrez rejoindre le lobby" << endl;


    cout << endl<< "[ --- Misc --- ]" << endl;
    cout << "Vous pouvez effacer le terminal en tapant /clear"<< endl;
    cout << "Vous pouvez retrouver votre nom d'utilisateur par /whoAmI"<< endl;
}


void Client::processCmd(Message& msg) {
    vector<string> command;
    parseMsg(msg, command);
    if (command.size()<1) {
        return;
    }

    if (command[0] == "ENDLOBBY") {
    	exit(0);
        if(isHost()) {
            removeHost();
        } else {
            setLobbyPort(0);
        }
    } else if (command[0] == "pendingRequest"){
        notifyObserverFR(command[1]);
    } else if (command[0] == "acceptedRequest"){
        notifyAcceptedFR(command[1]);
    } else if (command[0] == "declinedRequest"){
        notifyDeclinedFR(command[1]);
    } else if (command[0] == "allChat"){
        notifyObserversAllChat(command[1]);
    } else if (command[0] == "displayBoard"){
        //qDebug()<<"command0 = "+ QString::fromStdString(command[0]) +"command1 = "+ QString::fromStdString(command[1]) + "command2 = "+ QString::fromStdString(command[2]);
        notifyObserversDisplayBoard(command[1], command[2]);
        //qDebug()<<"command0 = "+ QString::fromStdString(command[0]) +"command1 = "+ QString::fromStdString(command[1]) + "command2 = "+ QString::fromStdString(command[2]);
    } else if (command[0] == "deleteFriend"){
        notifyObserversDeleteFriend(command[1]);
    } else if(command[0] == "launchGame"){
        notifyObserversGameLaunch();
    }

}

bool Client::HandleCmdSend(Message &msg) {
    vector<string> command;
    parseMsg(msg, command);

    if (command[0] == "inviteUser") {
        if (port_lobby!=0) {
            string newCommand="";
            newCommand.append(msg.message);
            newCommand.append(" ");
            newCommand.append(to_string(getPortLobby()));

            //if(msg.message) {free(msg.message);}
            msg.message = strdup(newCommand.c_str());
            msg.size = strlen(msg.message);

        } else {
            printf("Vous n'etes pas un hote.\n");
            return false;
        }
    } else if (command[0] == "startLobby") {
        if (command.size() == 2) {
            if (!setLobby(command[1])) { return false; }
        } else {
            printf("port non valide.\n");
            return false;
        }
    }
    else if (command[0] == "endLobby") {
        if (command.size() == 1) {
            killLobby();
            return false;
        } else {
            printf("Erreur: Le lobby n'a pas fermé");
            return false;
        }
    }
    else if (command[0] == "leaderBoard") {
        if (command.size() == 1) {
            consultLeaderBoard();
            return false;
        }
    }
    else if (command[0] == "help") {
        if (command.size() == 1) {
            showHelp();
            return false;
        }
    }
    else if (command[0] == "whoAmI") {
        if (command.size() == 1) {
            whoAmI();
            return false;
        }

    }else if (command[0] == "clear") {
        if (command.size() == 1) {
            system("clear");
            return false;
        }
    }
    else if (command[0] == "connect") {
        if (command.size() == 2) {
            connectClientToLobby(command[1]);
            return false;
        } else {
            printf("Invalid port.\n");
            return false;
        }
      }
    else if (command[0] == "getFriendList") {
        consultFriendsList();
    }
    return true;
}


void Client::setConnection(bool newCo){
    isConnect = newCo;
    if(!newCo){
        Message msg;
        string str = "/disconnect";
        msg.message = strdup(str.c_str());
        msg.size = str.size();
        sendMessage(getSocket(), msg);
        free(msg.message);
    }
}

void Client::changePseudo(string &newName) {
    string strSendNewName = "/changePseudo ";
    strSendNewName.append(newName);
    Message msg;
    time(&msg.timestamp);
    msg.message = strdup(strSendNewName.c_str());
    msg.size = strlen(msg.message);
    sendMessage(getSocket(),msg);
    free(msg.message);
}

