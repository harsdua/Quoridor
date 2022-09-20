////////////
//
//  auteur: Louis Devroye (000523920)
//  
//  date: 03-03-2022
//
//  description : Server capable de recevoir des connexions et des messages de plusieurs utilisateurs. 
//                Il traite les requetes et communique avec une base de donnée.
//
//  cours: info-f-209 groupe 7
//
////////////

#ifndef MASTERSERVER_H
#define MASTERSERVER_H

#include "server.h"
#include "utils.h"


#include <vector>


using namespace std;

/**
 * @brief server pincipal capable de gérer des requetes
 * 
 */
class MasterServer: public Server{

    public:

    MasterServer(const int& port=8080, bool inUI=true);
    
    /**
     * @brief Envoie une demande d'amis au client passer par la commande
     * 
     * @param user_username pseudo de l'utilisateur à qui envoyer la demande
     */
    void sendFriendRequest(const string user_username);

    /**
     * @brief enlève l'amis de la liste d'amis de la base de donnée
     * 
     * @param user_username pseudo de l'utilisateur qu'il faut retirer
     */
    void removeFriend(const string user_username);
    
    /**
     * @brief invite l'utilisateur passé par la commande au lobby
     * 
     * @param user_username pseudo de l'utilisateur qu'il faut retirer
     * @param port port sur lequel inviter l'amis
     * 
     */
    void inviteUser(const string friend_socket, const string port);

    /**
     * @brief Essaie d'accepter une demande d'ami du joueur user_username
     * 
     * @param user_username pseudo de l'utilisateur de qui on accepte la demande d'ami
     */
    void acceptFriend(const string user_username);

    /**
     * @brief Essaie de refuser une demande d'ami du joueur user_username
     * 
     * @param user_username pseudo de l'utilisateur de qui on refuse la demande d'ami
     */
    void declineFriend(const string user_username);
        
    /**
     * @brief tente d'exécuter la commande reçue
     * 
     */
    void tryCommand() override;

    
};

#endif //MASTERSERVER_H
