////////////
//
//  auteur: Louis Devroye (000523920)
//  
//  date: 03-03-2022
//
//  description : Server, classe abstraite utilisée pour créer un server sur un port. 
//                Il doit etre capable de recevoir des connexions et des requetes via des sockets et
//                de traiter les requetes.
//
//  cours: info-f-209 groupe 7
//
////////////

#ifndef SERVER_H
#define SERVER_H


#define TRUE 1
#define FALSE 0
#define MAX_USERS 1000

#include "utils.h"
#include "database.h"

#include <arpa/inet.h> //close
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <vector>
#include <string>


using namespace std;

/**
 * @brief Classe abstraite utilisée pour démarer un gérer un server. 
 * Il reçoit des connexions et des requetes sur un socket et les traites.
 * 
 */
class Server{
    int master_socket, port, addrlen , new_socket , activity, sd, max_sd;
    int client_socket[MAX_USERS];
    const char* usernames[MAX_USERS];
    Message messageToSend, currentMessage;
    struct sockaddr_in address;
    //set of socket descriptors
    fd_set readfds;

    bool volatile keepRunning = true;
    bool inUi = true;
    vector<string> commands={};
    int sender_index=0;
    bool send_to_sender=true;
    string strMsgToSend="Error";
    int opt = TRUE;

    //utilisé par la fonction static
    static inline Server* me;

public:
    Database db;

    Server(bool inUi=true);

    // getters & setters
    int getSenderIndex();
    int getPort();
    int getSocketIndex(const char* username);
    int getClientSocket(const int& sock_index);

    const char* getUserName(const int& sock_index);

    void setPort(const int& newPort);
    void setToSend(bool toSet);
    void setStrToSend(string newStr);
    void setSenderIndex(int newIndex);
    void setCommands(vector<string> newVector);

    /**
     * @brief Met le nom d'utilisateur NAME dans la table des pseudo à l'INDEX
     * 
     * @param index index de la table
     * @param name nom à placer
     */
    virtual void setUsername(const int index, const char* name);


    string getCommandAt(int index);
    size_t getCommandsSize();
    Message getCurrentMessage();

    bool isRunning();
    bool isStrToSend();
    bool isTerminal();


    /**
     * @brief Fonction permettant de gérer un signal venant du terminal 
     * (static est utile pour appeler cette fonction dans une instance)
     * 
     * @param sig signal reçu du terminal
     */
    static void signal_handler(int sig);


    /**
     * @brief Créer un message formaté à partir de MESSAGE à envoyer à RECEIVER_SOCKET
     * 
     * @param message message à envoyer
     * @param time_stamp temps local de la machine
     * 
     * @return string Le message formaté
     */
    string createMsgString(const string message, const time_t time_stamp);

    /**
     * @brief Se met sur un file descriptor et écoute les messages entrants
     * 
     * @return int > 0 si tout s'est bien déroulé
     */
    int createSocket();

    /**
     * @brief fonction d'écoute du socket entre le client et le server
     * 
     * @return int 
     */
    int listening();

    /**
     * @brief Fonction appelée lors d'une nouvelle connexion sur le socket
     * 
     * @param index index de la nouvelle connexion
     * @param new_socket socket à attribuer
     */
    virtual void newConnection(const int index, int new_socket);

    /**
     * @brief gère une nouvelle déconnexion
     */
    virtual void newDisconnection();

    /**
     * @brief regarde si le nom d'utilisateur est déjà présent dans la table des pseudo
     * 
     * @param username nom d'utilisateur à tester
     * 
     * @return true si le client est déjà connecté
     */
    bool isUserAlreadyConnected(const char* username);

    /**
     * @brief envoie un message à l'utilisateur pour lui indiquer qu'il faut se déconnecter.
     */
    void refuseConnection();

    /**
     * @brief envoie un message au client passer par la commande
     * 
     * @param friend_username nom de l'utilisateur auquel envoyer le message
     * @param strMessage string à envoyer
     */
    void sendMsg(const string friend_username, const string strMessage);

    /**
     * @brief Modifie le string pour y placer un message d'erreur
     * 
     */
    virtual void wrongCommand();

    /**
     * @brief test la signification de la commande et essaie de l'exécuter
     */
    virtual void tryCommand();
    /**
     * @brief gère la commande reçue
     */
    virtual void processCmd();

    /**
     * @brief gère un nouveau message reçu
     * 
     */
    virtual void processData();

    


};

#endif //SERVER_H
