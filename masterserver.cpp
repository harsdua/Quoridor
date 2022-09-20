#include "masterserver.h"

#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <signal.h>

#include <iostream>
#include <vector>

using namespace std;

MasterServer::MasterServer(const int& port, bool inUI): Server(inUI) { 
    setPort(port);
    createSocket();

    while (isRunning()) {
        listening();
    }
}


void MasterServer::sendFriendRequest(const string user_username) {

    setStrToSend("utilisateur inconnu. Veuillez réessayer.");
    setToSend(true);

    if (strcmp(user_username.c_str(), getUserName(getSenderIndex()))) { // test si l'utilisateur==le receveur;
        string sender_username;
        sender_username.assign(getUserName(getSenderIndex()));
        if(db.isFriendRequestExist(sender_username, user_username)){
            setStrToSend("You already have a pending request sent to the user");
            return;
        } else {
            db.createFriendRequest(sender_username, user_username);
            for (int j=0; j<MAX_USERS; j++) {
                if (!strcmp(getUserName(j), user_username.c_str())) {
                    setToSend(false);
                    if (!isTerminal()){
                        Message notif;
                        string notifStr = "/pendingRequest " + sender_username;
                        cout << "pending: " << notifStr<< endl;
                        notif.size = notifStr.size();
                        notif.message = strdup(notifStr.c_str());

                        sendMessage(getClientSocket(j), notif);
                        free(notif.message);
                    } else {
                        string friendRequest = "Invitation en amis de '";
                        friendRequest.append(sender_username);
                        friendRequest.append("' Pour accepter, tapez '/acceptedRequest ");
                        friendRequest.append(sender_username);

                        Message invitation;
                        time(&invitation.timestamp);

                        invitation.message = strdup(friendRequest.c_str());
                        invitation.size=friendRequest.size();

                        sendMessage(getClientSocket(j), invitation);

                        
                        free(invitation.message);
                    }
                 }
            }
        }
    } else {
        setStrToSend("Vous ne pouvez pas vous envoyer une invitation.");
    }
}

void MasterServer::acceptFriend(const string user_username) {
    setStrToSend("Utilisateur introuvable. Réessayez.");
    string sender_username;
    sender_username.assign(getUserName(getSenderIndex()));
    for (int j=0; j<MAX_USERS; j++) {
        if (strcmp(getUserName(j), getUserName(getSenderIndex()))) {
            if (!strcmp(getUserName(j), user_username.c_str())) {
                db.respToFriendRequestFrom(user_username, sender_username, true);
                db.addFriends(user_username, sender_username);
                if (!isTerminal()){
                    setToSend(false);    
                    Message notif;
                    string notifStr = "/acceptedRequest " + sender_username;
                    cout << "accepted by: " << notifStr<< endl;
                    notif.size = notifStr.size();
                    notif.message = strdup(notifStr.c_str());

                    sendMessage(getClientSocket(j), notif);
                    free(notif.message);
                } else {
                
                    string temporaryStr="Vous avez accepter la demande d'amis de ";
                    temporaryStr.append(user_username);
                    setStrToSend(temporaryStr);

                    string acceptedMessage=getUserName(getSenderIndex());
                    acceptedMessage.append(" a accepté votre demande d'amis");

                    Message acceptMsg;
                    time(&acceptMsg.timestamp);
                    acceptMsg.message = strdup(acceptedMessage.c_str());
                    acceptMsg.size=acceptedMessage.size();
                    sendMessage(getClientSocket(j), acceptMsg);

                    free(acceptMsg.message);
                }
            }
        }
    }
}

void MasterServer::declineFriend(const string user_username) {
    setStrToSend("Utilisateur introuvable. Réessayez.");
    for (int j=0; j<MAX_USERS; j++) {
        if (strcmp(getUserName(j), getUserName(getSenderIndex()))) {
            if (!strcmp(getUserName(j), user_username.c_str())) {
                string sender_username;
                sender_username.assign(getUserName(getSenderIndex()));
                db.respToFriendRequestFrom(user_username, sender_username, false);

                if (!isTerminal()){
                    setToSend(false);    
                    Message notif;
                    string notifStr = "/declinedRequest " + sender_username;
                    cout << "declined by: " << notifStr<< endl;
                    notif.size = notifStr.size();
                    notif.message = strdup(notifStr.c_str());

                    sendMessage(getClientSocket(j), notif);
                    free(notif.message);
                    
                }else{
                    string temporaryStr="Vous avez refusé la demande d'ami de ";
                    temporaryStr.append(user_username);
                    setStrToSend(temporaryStr);

                    string acceptedMessage=getUserName(getSenderIndex());
                    acceptedMessage.append(" a refusé votre demande d'amis");

                    Message acceptMsg;
                    time(&acceptMsg.timestamp);
                    acceptMsg.message = strdup(acceptedMessage.c_str());
                    acceptMsg.size=acceptedMessage.size();
                    sendMessage(getClientSocket(j), acceptMsg);

                    free(acceptMsg.message);
                }
                
            }
        }
    }
}


void MasterServer::removeFriend(const string user_username){
    for (int j=0; j<MAX_USERS; j++) {
        if (strcmp(getUserName(j), getUserName(getSenderIndex()))) {
            if (!strcmp(getUserName(j), user_username.c_str())) {
                string sender_username;
                sender_username.assign(getUserName(getSenderIndex()));
                db.removeFriends(user_username, getUserName(getSenderIndex()));
                if (!isTerminal()){
                    setToSend(false);
                    Message notif;
                    string notifStr = "/deleteFriend " + sender_username;
                    cout << "declined by: " << notifStr<< endl;
                    notif.size = notifStr.size();
                    notif.message = strdup(notifStr.c_str());
                    sendMessage(getClientSocket(j), notif);
                    free(notif.message);
                } else {
                    string temporaryStr="Vous n'êtes plus amis avec " ;
                    temporaryStr.append(user_username);

                    setStrToSend(temporaryStr);
                    setToSend(true);
                }
            }
        }
    }
};



void MasterServer::inviteUser(const string friend_socket, const string port) {
    for (int j=0; j<MAX_USERS; j++) {
        if (strcmp(getUserName(j), getUserName(getSenderIndex()))) {
            if (!strcmp(getUserName(j), friend_socket.c_str())) {

                string invitation_message = "Invitation à un lobby de '";
                invitation_message.append(getUserName(getSenderIndex()));
                invitation_message.append("' Pour rejoindre, tapez '/connect ");
                invitation_message.append(db.getNameLobbyFromPort(stoi(port)));

                Message invitation;
                time(&invitation.timestamp);
                setToSend(false);

                invitation.message = strdup(invitation_message.c_str());
                invitation.size=invitation_message.size();

                sendMessage(getClientSocket(j), invitation);
                free(invitation.message);
                break;
            }
        }
    }
}

void MasterServer::tryCommand() {

    Server::tryCommand(); 

    if (getCommandsSize()>1) {
        string cmd=getCommandAt(0);
        string first_cmd=getCommandAt(1);

        //besoins de "" pour mettre des espaces dans le message
        
        if (cmd == "sendFriendRequest") { // "/sendFriendRequest {username}"
            sendFriendRequest(first_cmd);

        } else if (cmd == "acceptedRequest") { // "/accept {username}"
            acceptFriend(first_cmd);

        } else if (cmd == "declinedRequest") { // "/decline {username}"
            declineFriend(first_cmd);

        } else if (cmd == "changePseudo") { 
            setUsername(getSenderIndex(), first_cmd.c_str()); 
        } else if (cmd == "deleteFriend") { //"/deleteFriend {username}"
            removeFriend(first_cmd);

        } else if (cmd == "inviteUser") { //"/inviteUser {username} {port}"
            if(getCommandsSize()>2) {
                inviteUser(first_cmd, getCommandAt(2));
            } else {
                setStrToSend("Erreur. Essayez /inviteUser {username}");
            }
        }
    }   
}
