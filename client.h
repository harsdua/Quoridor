////////////
//
//  auteur: Louis Devroye (000523920)
//
//  date: 03-03-2022
//
//  description : Client capable de se connecter à un server via un port (8080 par défaut).
//                Il doit avoir un pseudo. le client gère l'envoi et la réception de messages via le multithreading
//
//  cours: info-f-209 groupe 7
//
////////////

#ifndef CLIENT_H
#define CLIENT_H

#include "utils.h"
#include "lobby.h"
#include "database.h"
#include "observer_subject.h"

#include <pthread.h>


#include <vector>
#include <iostream>

#define TRUE 1
#define FALSE 0

using namespace std;

/**
 * @brief Un client se réfère à un utilisateur.
 * L'utilisateur peut chatter, créer un lobby, lancer une partie (s'il est l'hôte) et voir ses amis (en ligne ou non).
 */
class Client : public Subject {
  string ip="127.0.0.1"; 
  Lobby* lobby=nullptr;
  int port_server=8080, sock=0, port_lobby=0;
  bool isConnect = false;

  string pseudo;
  pthread_t tids[1];
  bool host, inUi;
  static inline Client* me;


public:

  Database database;

  Client(string pseudo, const int& port, bool inUi=true);
  ~Client()=default;

  // getters & setters
  string getPseudo();
  string getIP();

  pthread_t getTid();

  int getSocket();
  int getPortLobby();

  void setSocket(const int &newSocket);
  void setHost();
  void removeHost();
  void setLobbyPort(const int& port);

  bool isHost();
  bool isConnected();
  bool isTerminal();


  /**
   * @brief Créer un terminal et connect un client du même nom au lobby à port
   *
   * @param port port sur lequel se connecter au lobby
   */
  void connectClientToLobby(const string &port);

  /**
   * @brief Créer un terminal et lance un lobby dedans
   *
   */
  void createLobby();

  /**
   * @brief Créer un lobby et le lie au client en tant qu'hôte de ce lobby (pour le client) puis connect ce client au lobby
   *
   * @param newPort port du lobby sur lequel le client est hote
   * @return true si tout s'est bien déroulé
   * @return false si le client est déjà hôte d'un lobby;
   */
  int setLobby(const string& nameLobby);

  /**
   * @brief Délie le client du lobby pour lequel il est l'hôte et ferme ce lobby
   *
   * @return true le lobby s'est bien fermé
   * @return false si le client n'est pas l'hôte d'un lobby
   */
  void killLobby();

  /**
   * @brief fonction de thread pour recevoir des messages
   *
   * @param param pointeur inutilisé
   * @return void* nécessairement void * pour pthread_create
   */
  static void *userThreadFun(void *param);

  /**
   * @brief Envoie le pseudo de l'utilisateur au server
   *
   */
  void sendUserName();

  /**
   * @brief Essaie de se connecter au server
   *
   * @return -1 = erreur, 0 = pas réussi, 1 = réussi
   */
  int tryConnect();

  /**
   * @brief Envoie un message à un autre utilisateur grâce à son ID_USER
   *
   * @param id_user identifiant unique de l'utilisateur auquel envoyer le message
   * @param line message à envoyer
   *
  */
  void TrySendMessage(const string& line);

  /**
  * @brief Montre le leaderboard
  */
  void consultLeaderBoard();

  /**
   * @brief Construct a new consult Friends List object
   * 
   */
  void consultFriendsList();

  /**
  * @brief Imprime pseudonyme
  */
  void whoAmI();

  /**
  * @brief Imprime les listes des commandes et son syntax/fonctionnements.
  */
  void showHelp();

  /**
    * @brief gère la commande reçue
    *
    * @param msg message contenant la taille, le txt et la taille du txt
    */
  void processCmd(Message& msg);

  /**
   * @brief ajoute des informations nécessaires au server et test si la commande est permise dans l'état actuel
   *
   * @param msg message qui va être modifié
   *
   * @return true si la commande peut être envoyée
   */
  bool HandleCmdSend(Message &msg);

  /**
   * @brief change l'état de la connexion et envoie un message si NEWCO est false
   * 
   * @param newCo nouvel état de la connexion
   */
  void setConnection(bool newCo);

  void changePseudo(string &str);

};
#endif //CLIENT_H
