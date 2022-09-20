#include "server.h"

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

#include <string>
#include <iostream>


using namespace std;

Server::Server(bool inUi):inUi(inUi) {
    db.connect();
    me = this;
}



int Server::getPort(){
    return port;
}

int Server::getClientSocket(const int& sock_index) {
    return client_socket[sock_index];
}

int Server::getSenderIndex() {
    return sender_index;
}

int Server::getSocketIndex(const char* username) {
    for (int i=0; i<MAX_USERS; i++) {
        if (getUserName(i) == username) {
            return i;
        }
    }
    return -1; // erreur, pseudo non présent dans un socket
}

const char* Server::getUserName(const int& sock_index){
    return usernames[sock_index];
}

void Server::setCommands(vector<string> newVector) {
    commands.clear();
    for (size_t i=0; i<newVector.size(); i++) {
        commands.push_back(newVector[i]);
    }
}

void Server::setSenderIndex(int newIndex) {
    sender_index=newIndex;
}

void Server::setStrToSend(string newStr) {
    strMsgToSend=newStr;
}

void Server::setToSend(bool toSet) {
    send_to_sender=toSet;
}


void Server::setPort(const int& newPort) {
    port = newPort;
}

void Server::setUsername(const int index, const char* name) {
    usernames[index]=name;
}

string Server::getCommandAt(int index) {
    return commands.at(index);
}

size_t Server::getCommandsSize() {
    return commands.size();
}

Message Server::getCurrentMessage() {
    return currentMessage;
}

bool Server::isStrToSend() {
    return send_to_sender;
}

bool Server::isRunning(){
    return keepRunning;
}

bool Server::isTerminal() {
    return !inUi;
}


void Server::signal_handler(int sig){
    for(int i=0;i<MAX_USERS;i++){
        if (me->client_socket[i] == 0) continue;
        close(me->client_socket[i]);
    }
    close(me->master_socket);
    me->keepRunning = false;
    exit(sig);
}

string Server::createMsgString(const string message, const time_t time_stamp){
    
    char buff[5];
    strftime(buff, 20, "%H:%M", localtime(&time_stamp));
    
    //link username with message into newMsg
    string newMsg= "Private message recieved from : ";
    newMsg.append(getUserName(sender_index));
    newMsg.append(" (");
    newMsg.append(buff);
    newMsg.append(") ");
    newMsg.append(message);

    return newMsg;

}

void Server::newConnection(const int index, const int new_socket) {
    this->client_socket[index] = new_socket;
    printf("Adding to list of sockets as %d\n" , index);
}

void Server::newDisconnection() {
    getpeername(sd , (struct sockaddr*)&address , \
                            (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,
                            inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

    //Close the socket and mark as 0 in list for reuse
    close( sd );    
    this->client_socket[getSenderIndex()] = 0;
    usernames[getSenderIndex()] = "";
}


bool Server::isUserAlreadyConnected(const char* username) {
    for (int i=0; i<MAX_USERS; i++) {
        if ((getSocketIndex(username) != -1) && 
            (getSocketIndex(getUserName(i)) != -1) && 
            (getSocketIndex(username) != getSocketIndex(getUserName(i)))) { // ne pas tester le client avec lui même
            
            if(!strcmp(username, getUserName(i))) {
                return true;
            }
        }
    }
    return false;
}

void Server::refuseConnection() {
    usernames[getSenderIndex()]="";
    Message RefuseConnection;
    string strRefuseConnection="Cet utilisateur est déjà connecté";
    RefuseConnection.message = strdup(strRefuseConnection.c_str());
    RefuseConnection.size=strRefuseConnection.size();

    sendMessage(getClientSocket(getSenderIndex()), RefuseConnection);

    free(RefuseConnection.message);
    newDisconnection();
}

int Server::createSocket() {

    signal(SIGINT, Server::signal_handler);
    
    int i;

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < MAX_USERS; i++)
    {
        this->client_socket[i] = 0;
        this->usernames[i] = (const char*) "";
    }

    for (int i=0; i<MAX_USERS; i++) {
    }

    //create a master socket
    if( (this->master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if( setsockopt(this->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                    sizeof(opt)) < 0 ) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    //bind the socket to localhost port 8888
    if (bind(this->master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", port);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(this->master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");


    return 1;
}

int Server::listening(){

    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(this->master_socket, &readfds);
    max_sd = this->master_socket;

    //add child sockets to set
    for (int i = 0 ; i < MAX_USERS; i++)
    {
        //socket descriptor
        sd = this->client_socket[i];

        //if valid socket descriptor then add to read list
        if(sd > 0)
            FD_SET( sd , &readfds);

        //highest file descriptor number, need it for the select function
        if(sd > max_sd)
            max_sd = sd;
    }

    //wait for an activity on one of the sockets , timeout is NULL ,
    //so wait indefinitely
    activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

    if ((activity < 0) && (errno!=EINTR))
    {
        printf("select error");
    }

    //If something happened on the master socket ,
    //then its an incoming connection
    if (FD_ISSET(this->master_socket, &readfds))
    {
        if (!this->isRunning()) return 0;
        if ((new_socket = accept(this->master_socket,
                                (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //add new socket to array of sockets
        for (int i = 0; i < MAX_USERS; i++)
        {
            //if position is empty
            if( this->client_socket[i] == 0 )
            {
                newConnection(i, new_socket);
                break;
            }
        }

    }
    //else it's some IO operation on some other socket
    for (int i = 0; i < MAX_USERS; i++)
    {
        sd = this->client_socket[i];
        if(sd == 0) continue;

        //if data recieved
        if (FD_ISSET( sd , &readfds))
        {

            //launch read message since data is probably a message recieved

            if (readMessage(sd,currentMessage))
            {
                setSenderIndex(i);
                //if username is not initialize
                if (strlen(getUserName(i))<=0)
                {

                    //first message recieved is the message with only the username
                    //initialize client's username as the message recieved

                    setUsername(i, currentMessage.message);
                    if (isUserAlreadyConnected(usernames[i])) {
                        refuseConnection();
                    }
                    continue;

                }

                //THIS IS WHERE THE SERVERS WILL BRANCH OFF
                processData();



            //Somebody disconnected , get his details and print
            } else {

                newDisconnection();

            }
        }
    }
    return 1;
}
void Server::sendMsg(const string friend_username, const string strMessage) {
    
    setStrToSend("L'utilisateur n'est pas enligne, essayez : /sendMsg 'username' 'message'");

    string user_name;
    user_name.assign(getUserName(getSenderIndex()));
    if(!db.isFriends(user_name, friend_username)){
        setStrToSend("You are not friends with the user you are trying to message");
        return;
    }

    string formatedMessage=createMsgString(strMessage, time(nullptr));

    Message msg;
    msg.message = strdup(formatedMessage.c_str());
    msg.size = formatedMessage.size();

    for (int j=0; j<MAX_USERS; j++) {
        if (strcmp(getUserName(j), getUserName(sender_index))) {
            if (!strcmp(getUserName(j),friend_username.c_str())) {

                sendMessage(getClientSocket(j), msg);
                setToSend(false);
                break;
            }
        }
    }

    free(msg.message);
}

void Server::wrongCommand() {
    setStrToSend("Commande inconnue. Tapez /help pour voir les commandes.");
}

void Server::tryCommand() {
    setToSend(true);

    wrongCommand();//wrongCommand by default


    if (getCommandsSize()>1) {
        string cmd=getCommandAt(0);
        //besoins de "" pour mettre des espaces dans le message
        if (cmd == "sendMsg") { //"/sendMsg {username} {message} "
            if (getCommandsSize()>=3) {
                sendMsg(getCommandAt(1), getCommandAt(2));
            } else {
                setStrToSend("Format incorrect, essayez : /sendMsg {utilisateur} {message}. \
                            Ajoutez des "" pour pouvoir mettre des espaces.");
            }

        }else if(getCommandAt(0) == "changePseudo"){
            cout << "reçu" << endl;
            setUsername(getSenderIndex(), getCommandAt(1).c_str()); 
        }
    } else if(getCommandAt(0) == "disconnect"){
        newDisconnection();
    }
}

void Server::processCmd() {
    time(&messageToSend.timestamp);

    setToSend(true);
    tryCommand();

}

void Server::processData() {
    
    setToSend(true);
    if (getCurrentMessage().message[0] == '/') {
        vector<string> commandsToCopy;
        parseMsg(getCurrentMessage(), commandsToCopy);
        setCommands(commandsToCopy);

        processCmd();

    } else if (getCurrentMessage().size==0) {
        setStrToSend("Un message vide n'est pas valide.");
        
    } else {
        setStrToSend("Message non reconnu. Essayez '/' au début de votre commande.");
    }

    if (isStrToSend()) {
        messageToSend.message = strdup(strMsgToSend.c_str());
        messageToSend.size=strMsgToSend.size();
        sendMessage(getClientSocket(getSenderIndex()), messageToSend);
        free(messageToSend.message);
    }
}
