////////////
//
//  auteur: Louis Devroye (000523920)
//  
//  date: 03-03-2022
//
//  description : Lobby, server capable de gérer des connexions et des requetes sur un port via des sockets.
//                le lobby possède un hote, capable de lancer, de mettre en pause et de relancer une partie.
//
//  cours: info-f-209 groupe 7
//
////////////

#ifndef LOBBY_SERVER_H
#define LOBBY_SERVER_H

#define TRUE 1
#define FALSE 0
#define MAX_USERS 1000

#include "server.h"
#include "utils.h"
#include "game.h"
#include "observer_subject.h"


/**
 * @brief Lobby capable de lancer et de gérer un partie et des clients. 
 */
class Lobby : public Server{
    int port, index_waiting=0;
    bool played=true, inJump = false;
    int *host_socket=nullptr;
    Game* game=nullptr;

    vector<string> players;

public:

    Lobby(const int& port=9090, bool inUI=true);
    virtual ~Lobby();

    bool isInJump();
    /**
     * @brief override de la réception d'un signal
     * 
     * @param sig signal à envoyer lors de l'exit
     */
    void signal_handler(int sig);

    /**
     * @brief Met le nom d'utilisateur NAME dans la table des pseudo à l'INDEX 
     * et dans la table des joueurs si elle n'est pas pleine
     * 
     * @param index index de la table
     * @param name nom à placer
     */
    void setUsername(const int index, const char* name) override;

    /**
     * @brief création d'un lobby d'une partie de Quoridor
     * 
     * @param port port sur lequel le lobby doit écouter
     * 
     */
    int createLobby(int port);

    /**
     * @brief joue un coup de la partie
     * 
     * @param move coup à essayer par le plateau de jeu
     */
    void playGame(const string move);
    
    /**
     * @brief envoie une message à tous les utilisateurs dul lobby
     * 
     * @param messageTxt message à envoyer
     */
    void allchat(string messageTxt);

    /**
     * @brief essaie d'exécuté la commande dans le vecteur
     * 
     */
    void tryCommand() override;

    /**
     * @brief ajoute le nouveau client dans la liste des clients
     * 
     * @param index indexe de la nouvelle connexion
     * @param new_socket socket de la nouvelle connexion
     */
    void newConnection(const int index, const int new_socket) override;

    /**
     * @brief test la deconnexion d'un client pour voir si c'est l'hote et ferme le server si oui
     */
    void newDisconnection() override;

    /**
     * @brief demande un coup au joueur pour qui c'est le tour de jouer
     * 
     */
    void askMove(string strMove="Entrez un coup");


    /**
     * @brief essaie de lance la partie
     * 
     * @param mode mode de jeu pour la partie
     */
    void tryStartGame(const string mode);

    /**
     * @brief traite la commande associéer à jouer un coup
     * 
     * @param move coup à essayer 
     */
    void tryMove(const string move);

    /**
     * @brief essaie de suter au dessu d'un joueur
     * 
     * @param jump sur lequel le joueur essaie de sauter
     */
    void tryJump(const string jump);

    /**
     * @brief regarde si command est une instruction pour éteindre le lobby 
     */
    void tryExitLobby();

    /**
     * @brief essaie de sauvegarder la partie en cours
     * 
     * @param game_name nom de la partie à sauvegarder
     */
    void saveGame(const string game_name);

    /**
     * @brief essaie de charger une partie avec le nom d'utilisateur donné dans command[1]
     * 
     * @param game_name nom de la partie à charger
     */
    void loadGame(const string game_name);


    /**
     * @brief gère la partie lorsqu'un joueur gagne et les joueurs
     * 
     */
    void handleWin();

    /**
     * @brief Imprime l'aide pour des commandes disponible
     *
     */
    void showHelp();

    /**
     * @brief abandonne une partie
     */
    void abandon();

    int getNumberConnectedPlayer();

    void display(int i);

    Game* getGame();

    void launchGame();
};



#endif //LOBBY_SERVER_H
