#ifndef DATABASE_H
#define DATABASE_H

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <vector>

class Database {

	/*
	Class Database sert de lien entre le jeu et la base de donnée, dont l'authentification & la liste d'amis
	*/

	sqlite3 *db;
	sqlite3_stmt *stmt;
	bool isConnected = false;

	public:
	/**
	 * @brief Méthode servant à se connecter à la base de donnée
	 * 
	 */
	void connect();

	/**
	 * @brief Méthode servant à se déconnecter de la base de donnée
	 * 
	 */
	void disconnect();

	/**
	 * @brief Méthode servant à créer les tables si elles ne le sont pas
	 * 
	 */
	void init();

	/**
	 * @brief Méthode servant à envoyer une requête ne nécessitant pas de retour
	 * 
	 * @param str - Requête sous-forme de string
	 * @return true - Si exécutée dans la BDD
	 * @return false - Si non exécutée dans la BDD
	 */
	bool sendRequest(string str);

	/**
	 * @brief Fonction servant à savoir si l'utilisateur est enregistré ou non
	 * 
	 * @param username - Nom d'utilisateur
	 * @return true - Si l'utilisateur est déjà enregistré
	 * @return false - Si l'utilsiateur n'est pas enregistré
	 */
	bool isUserRegistered(string username);

	/**
	 * @brief Méthode servant à savoir si le mot de passe renseigné de l'utilsateur est correct
	 * 
	 * @param username - Nom d'utilisateur
	 * @param passwd - Mot de passe
	 * @return true - Si le mot de passe est bon
	 * @return false - Si le mot de passe est faux
	 */
	bool isPasswordCorrect(string username, string passwd);

	/**
	 * @brief Méthode servant à savoir si une demande d'ami de username vers username2 a déjà été faite
	 * 
	 * @param username - Nom d'utilisateur / Envoyeur de la demande
	 * @param username2 - Nom d'utilisateur / Destinataire de la demande
	 * @return true - Si la demande existe déjà
	 * @return false - Si la demande n'existe pas
	 */
	bool isFriendRequestExist(string username, string username2);

	/**
	 * @brief Méthode servant à savoir si username & username2 sont amis
	 * 
	 * @param username - Nom d'utilisateur
	 * @param username2 - Nom d'utilisateur
	 * @return true - Si ils sont amis
	 * @return false - Si ils ne sont pas amis
	 */
	bool isFriends(string username, string username2);

	/**
	 * @brief Connaître l'état de réponse de la demande d'ami
	 * 
	 * @param username - Nom d'utilisateur
	 * @param username2 - Nom d'utilisateur
	 * @return true - Si répondu oui à la demande
	 * @return false - Si répondu non à la demande
	 */
	bool getStateFriendRequest(string username, string username2);

	/**
	 * @brief Récuperer la liste d'amis sous-forme de vector<string>
	 * 
	 * @param username - Nom d'utilisateur concerné par la demande
	 * @return vector<string> - Liste d'amis du nom d'utilisateur concerné
	 */
	vector<string> getFriendsList(string username);

    /**
     * @brief getFriendRequestsList
     * @param username
     * @return
     */
    vector<string> getFriendRequestsList(string username);

	/**
	 * @brief Méthode servant à créer la table USER
	 * 
	 */
	void createTableUser();

	/**
	 * @brief Méthode servant à créer la table FRIEND_REQUEST
	 * 
	 */
	void createTableFriendRequest();

	/**
	 * @brief Méthode servant à créer la table FRIENDS_LIST
	 * 
	 */
	void createTableFriends();

	/**
	 * @brief Méthode servant à créer la table PARTY_SAVED
	 * 
	 */
	void createTableSaveParty();


	/**
	 * @brief Méthode servant à créer la table LOBBY_LIST
	 * 
	 */
	void createTableLobbyList();

	/**
	 * @brief Méthode servant à supprimer la table LOBBY_LIST
	 * 
	 */
	void deleteTableLobbyList();
	
	/**
	 * @brief Méthode servant à créer un utilsateur à l'aide de son nom d'utilisateur et de son mot de passe choisi
	 * 
	 * @param username - Nom d'utilisateur
	 * @param password - Mot de passe
	 */
	void createUser(string username, string password);

	/**
	 * @brief Méthode servant à créer une demande d'ami de username vers username2
	 * 
	 * @param username - Nom d'utilisateur
	 * @param username2 - Nom d'utilisateur
	 */
	void createFriendRequest(string username, string username2);

	/**
	 * @brief Méthode servant à username et username2 d'être ami
	 * 
	 * @param username - Nom d'utilisateur
	 * @param username2 - Nom d'utilisateur
	 */
	void addFriends(string username, string username2);

	/**
	 * @brief Méthode servant à username et username2 de ne plus être ami
	 * 
	 * @param username - Nom d'utilisateur
	 * @param username2 - Nom d'utilisateur
	 */
	void removeFriends(string username, string username2);

	/**
	 * @brief Méthode servant à répondre à la demande d'ami de username vers username2
	 * 
	 * @param username - Nom d'utilisateur
	 * @param username2 - Nom d'utilisateur
	 * @param b - true si username2 accepte la demande d'ami / false si username2 refuse la demande d'ami
	 */
	void respToFriendRequestFrom(string username, string username2, bool b);

	/**
	 * @brief Méthode servant à supprimer la demande d'ami de username vers username2
	 * 
	 * @param username - Nom d'utilisateur
	 * @param username2 - Nom d'utilisateur
	 */
	void removeFriendRequest(string username, string username2);

	/**
	 * @brief Méthode servant à sauvegarder une partie dans la BDD
	 * 
	 * @param username - Nom d'utilisateur
	 * @param name_of_party - Nom de la partie
	 * @param board_serialize - String du plateau sérialisée
	 * @param wall_serialize - String de la liste des murs sérialisée
	 * @param gameType - Nom du type de partie
	 * @param lastPlayedIndex - Index du dernier utilisateur qui a joué
	 * @param playerList - liste des joueurs présent dans la partie
	 * @param playerStartCoord - liste des points de départ des joueurs
	 * @param timeRemaining - à remplir que si c'est pas une partie avec un temps imparti
	 */
	void saveParty(string username, string name_of_party, string board_serialize, string wall_serialize, string gameType, string lastPlayed, string playerList, string listPointStart, string wallRemaining, string timeRemaining = "NULL");

	/**
	 * @brief Méthode servant à récupérer de la base de donnée une partie sauvegardée par un utilisateur
	 * 
	 * @param username - Nom d'utilisateur
	 * @param name_of_party - Nom de la partie
	 * @return vector<string> - Renvoi une vecteur de string dont les indexs :
	 * 
	 * [0] = Nom d'utilisateur du créateur 
	 * [1] = Nom de la partie 
	 * [2] = String du plateau sérialisé
	 * [3] = String de la liste des murs sérialisé
	 * [4] = String du type de la partie
	 * [5] = String de l'id du dernier joueur qui a joué
	 * [6] = String contenant la liste des joueurs de la partie
	 * [7] = String contenant la liste des positions de départ des joueurs
	 * [8] = String contenant les murs restants de chaque joueur présent dans la partie
	 * [9] = String du temps restant des compteurs
	 */
	vector<string> getSavedParty(string username, string name_of_party);

	/**
	 * @brief Méthode servant à supprimer une partie d'un utilisateur
	 * 
	 * @param username - Nom d'utilisateur
	 * @param name_of_party - Nom de la partie
	 */
	void deleteSaveParty(string username, string name_of_party);

	/**
	 * @brief Méthode servant à savoir si une partie de ce nom existe pour l'utilisateur
	 * 
	 * @param username - Nom d'utilisateur 
	 * @param name_of_party - Nom de la partie
	 * @return true - Si la partie existe
	 * @return false - Si la partie n'existe pas
	 */
	bool isPartyExistForUser(string username, string name_of_party);
	 
	 /**
	  * @brief Méthode servant à récupérer les données complètes d'un utilisateur
	  * 
	  * @param username - Nom d'utilisateur
	  * @return vector<string> - Renvoi un vecteur de string dont les indexs :
	  * 
      * [0] = UUID
	  * [1] = Nom d'utilisateur
	  * [2] = Mot de passe
	  * [3] = Nombre de victoires
	  * [4] = Nombre de défaites
	  * [5] = Winrate
	  */
	vector<string> getUserData(string username);

	/**
	 * @brief Méthode servant à connaitre le nombre de victoire de l'utilisateur renseigné
	 * 
	 * @param username - Nom d'utilisateur
	 * @return int - Nombre de victoire
	 */
	int getVictory(string username);

	/**
	 * @brief Méthode servant à connaitre le nombre de défaite de l'utilisateur renseigné
	 * 
	 * @param username - Nom d'utilisateur
	 * @return int - Nombre de défaites
	 */
	int getLose(string username);

	/**
	 * @brief Méthode servant à connaitre le winrate de l'utilisateur renseigné
	 * 
	 * @param username - Nom d'utilisateur
	 * @return double - Winrate de l'utilisateur
	 */
	double getWinrate(string username);

	/**
	 * @brief Méthode servant à ajouter une victoire à l'utilisateur renseigné
	 * 
	 * @param username - Nom d'utilisateur
	 */
	void addVictory(string username);

	/**
	 * @brief Méthode servant à ajouter une défaite à l'utilisateur renseigné
	 * 
	 * @param username - Nom d'utilisateur
	 */
	void addLose(string username);

	/**
	 * @brief Méthode servant à calculer le winrate de l'utilisateur concerné
	 * 
	 * @param username - Nom d'utilisateur
	 */
	void calculateWinrate(string username);


	/**
	 * @brief Méthode servant à changer le nom d'utilisateur par un nouveau renseigné
	 * 
	 * @param old_username - Ancien nom d'utilisateur
	 * @param new_username - Nouveau nom d'utilisateur
	 */
	void changeUsername(string old_username, string new_username);

	/**
	 * @brief Méthode servant à changer le mot de passe de l'utilisateur par un nouveau souhaité
	 * 
	 * @param username - Nom d'utilisateur
	 * @param new_password - Nouveau mot de passe souhaité
	 */
	void changePassword(string username, string new_password);

	/**
	 * @brief Méthode servant à désinscrire un utilisateur
	 * 
	 * @param username - Nom d'utilisateur
	 */
	void unregisterUser(string username);

	/**
	 * @brief Méthode servant à récupérer le classement en fonction du winrate de tous les joueurs
	 * 
	 * @return vector<string> - Vecteur contenant les 10 meilleurs joueurs en fonction du winrate
	 */
	vector<string> getLeaderBoard();

	// Partie chiffrement

	/**
	 * @brief Méthode servant à chiffrer le mot de passe de l'utilisateur | PAS ENCORE UTILISEE
	 * 
	 * @param passwd - Mot de passe non chiffré
	 * @return string - Mot de passe chiffré
	 */
	string encryptPassword(string passwd);

	/**
	 * @brief Création d'un lien entre un port et un nom de partie
	 * 
	 * @param name_of_game Nom de la partie
	 * @param port_used Port utilisé
	 * @param isPrivate True si le lobby doit être privé, False si le lobby doit être public
	 */
	void createLobbyInDB(string name_of_game, int port_used, bool isPrivate);

	/**
	 * @brief Suppression du lien entre un nom de partie et le port utilisé
	 * 
	 * @param name_of_game Nom de la partie
	 */
	void removeLobbyInDB(string name_of_game);

	/**
	 * @brief Méthode qui vérifie si le nom de la partie est déjà relié à un port
	 * 
	 * @param name_of_game Nom de la partie
	 * @return true Présence de la partie dans la DB
	 * @return false Non-présence de la partie dans la DB
	 */
	bool isLobbyNameInDB(string name_of_game);

	/**
	 * @brief Méthode qui vérifie si le nom de la partie est déjà relié à un port
	 * 
	 * @param name_of_game Nom de la partie
	 * @return true Présence de la partie dans la DB
	 * @return false Non-présence de la partie dans la DB
	 */
	bool isLobbyPortInDB(int port_used);

	/**
	 * @brief Méthode qui vérifie si la partie est privée / publique
	 * 
	 * @param name_of_game Nom de la partie
	 * @return true Partie est privée
	 * @return false Partie est publique
	 */
	bool isLobbyIsPrivate(string name_of_game);

	/**
	 * @brief Récupère le nom de la partie en relation avec le port donné et utilisé
	 * 
	 * @param port Port utilisé
	 * @return string Nom de la partie
	 */
	string getNameLobbyFromPort(int port);

	/**
	 * @brief Récupère le port donné grâce au nom du lobby
	 * 
	 * @param string Nom du lobby
	 * @return int Port utilisé par le lobby
	 */
	int getPortFromNameLobby(string nameLobby);

	/**
	 * @brief Récupère la liste de toutes les parties présentes dans le lobby
	 * 
	 * @return vector<string> Liste de toutes les parties présentes dans le lobby
	 */
	vector<string> getListLobby();

    void addInLobby(string username, string lobbyName);
    bool isInLobby(string username);
    void removeInLobby(string username);
    string getLobbyNameFromPlayer(string username);

    void deleteAccount(string username);

    string getUUIDFromUsername(string username);
    string getUsernameFromUUID(string UUID);

	int getFreePort();

	Database(){
		connect();
		// Requête servant à avoir plusieurs instances pouvant modifier la base de donnée en même temps
		sendRequest("PRAGMA journal_mode=WAL;");
		init();
	}

};

#endif
