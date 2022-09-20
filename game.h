/**
 * @file game.cpp
 * @author Edem Lawson & Miguel Lozano
 * @brief Fichier .cpp de la classe Game
 */

#ifndef __GAME__H
#define __GAME__H

#include "player.h"
#include "board.h"
#include "common.h"

#include <iostream>
#include <vector>
#include <string>


using namespace std;

//!  Classe Game. 
/*!
  Classe qui représente la partie lancée dans le lobby, elle est la classe intermédiaire 
  entre le plateau de jeu et les joueurs/le lobby.
*/

class Game {
  public:
    /** GameMode enum. 
     *  énumeration de tous les modes e jeu disponibles
     */

  private:

	int id = 1; 							///< stocke le nombre de joueurs et permet de leur attribuer un id
	bool playing = false;        			///< permet de connaitre l''etat de la partie
	vector<Player*> players;	  			///< Liste des joueurs de la partie 
	GameMode mode;							///< Mode de jeu de la partie en cours
	Player* actualPlayer = nullptr;			///< Pointeur vers le joueur qui va jouer le prochain coup
	Player* winner = nullptr;				///< Pointeur vers le joueur qui a gagné la partie
	vector<string> CoordForJump;				///< Stock les coordonnées que le joueur peut atteindre en sautant
    vector<string> starting={"I5", "I5", "I5", "I5"}; ///< positions de départ (par défaut : A5, I5, E1, E9)

  public:
		Board board;							///< Plateau ou la partie est jouée

	//! Constructeur
	Game(GameMode mode, vector<string> names);


	// Getters

	/**
	 * @brief Retourne le nombre de joueurs dans la partie
	 * 
	 * @return int 
	 */
	int getNumPlayers();

	/** isWon. 
     *  @brief Permet de savoir si il y a un vainqueur a la partie 
	 * 	@return bool true si il y a un gagnant
     */
	bool isWon();

	/** isInProgress. 
     *  @brief Permet de savoir si la partie est en cours ou pas 
	 * 	@return bool true si la partie est en cours
     */
	bool isInProgress();

	/** getNextPlayer.
     *  @brief Retourne un pointeur vers le prochain joueur.
     */
	Player* getNextPlayer();

	/** getPlayerByID. 
     *  @brief Permet d'avoir le pointeur vers un joueur en ayant son Id.
	 * 	@param id Id du joueur dont on veut le pointeur
	 * 	@return pointeur vers le joueur dont l'Id est donnée en parametre
     */
	Player* getPlayerById(int id);

	/** getPlayerByName. 
     *  @brief Permet d'avoir le pointeur vers un joueur en ayant son nom.
	 * 	@param name nom du joueur dont on veut le pointeur
	 * 	@return pointeur vers le joueur dont le nom est donné en parametre
     */
	Player* getPlayerByName(string name);

	/** getWinnerName. 
     *  @brief Renvoie un message annoncant le vainqueur de la partie.
	 * 	@return string message pour annoncer le vainqueur
     */
	string getWinnerName();

	/**
	 *  getActualPlayerWalls.
	 *  @brief Retourne le nombre de murs restants du joueur actuel
	 *  @return int le nombre de joueur
	 */
	int getActualPlayerWalls();

	/**
	 *  getActualPlayerWalls.
	 *  @brief Retourne le temps qu'il reste au joueur
	 *  @return string 
	 */
	string getActualPlayerTime();

	/**
	 * @brief 
	 * 
	 */
	vector<string> getJumpCoords();

	GameMode getGameMode();

	// Setters

	/**
	 *  @brief Donne l'id donné en parametre au joueur également donné en paramètre.
	 *  @param player adresse du joueur dont l'id doit être set
	 *  @param newId int de l'id qu'on veut donner au joueur
	 */
	void setPlayerId(Player* player, int newId);

	/** setAllPlayersInfo. 
     *  @brief Initialise le nombre de mur, les Id et le timer de chaque joueur selon le mode.
     */
	void setAllPlayersInfo();

	/** setAllStarting.
	 *	@brief Initialise les pions des joueurs sur le plateau.
	 */
	void setAllStarting();


	// Others

	/** startGame. 
     *  @brief Permet de débuter une partie, elle lance l'initialisation du plateau et la premiere demande de coup.
		 * 
		 * @param starting positions de départ (par défaut : A5, I5, E1, E9)
     */
	void startGame(vector<string> newStarting);

	void startGame();

	/** initBoard. 
     *  @brief Fonction qui initialise le plateau, càd elle place les joueurs (de façon aléatoire?) sur une des positions 
	 * 	de départ et passe la variable playing à true
	 * 	@param mode Mode de jeu dans lequel la partie est lancée
     */
	void initBoard(GameMode mode);

	/** playOnBoard. 
     * @brief Permet de jouer un coup sur le plateau.
	 * @param args Vecteur de char* qui contient le coup tapé par le joueur (le coup est divisé si le coup joué est composé => un mur) 
	 * @param p Pointeur vers le joueur dont le coup est stocké dans args 
     */
	void playOnBoard(vector<string>& args, Player *p);

	/** isWinner. 
     *  @brief Verifie si un certain joueur a gagné
	 * 	@param p joueur dont la condition gagnant/pas gagnant est vérifiée
	 * 	@return true ou false, respectivement si le joueur p a gagné ou pas
     */
	bool isWinner(Player *p);

	/** play.
	 *  @brief Prend le coup donné en parametre et le traite de sorte a ce qu'il puisse être joué sur le plateau
	 * 	@param coup Tableau de char qui comprend le coup donné en paramètre
	 * 	@return 0 Le coup entré est illégal
	 *  @return 1 Le coup entré est légal et "simple"
	 * 	@return 2 Le coup entré est légal mais provoque un saut
     */
	int play(string coup);

	/**
	 * @brief 
	 * 
	 */
	bool playJump(string coup);

	/** getBoard. 
     *  @brief Retourne un string qui 
	 * 	@return bool true si la partie est en cours
     */
	string getBoard();

	void displayBoard();

	/**
	 * @brief Récupère le nom du dernier joueur qui a joué un coup
	 * 
	 * @return string : nom du joueur
	 */
	string getLastPlayerPlayed();

	/**
	 * @brief Sérialise le timer de chaque joueur présent dans la partie
	 * 
	 * @return string : tous les timers sérialisés
	 */
	string serializeTimer();

	/**
	 * @brief Désérialise le timer de chaque joueur présent dans la partie
	 * 
	 * @param str Tous les timers sérialisés
	 */
	void deserializeTimer(string str);

	/**
	 * @brief Sauvegarder une partie
	 * 
	 * @param username Nom de l'hôte du lobby
	 * @param name_of_game Nom de la partie
	 * @return true Si c'est sauvegardé
	 * @return false Si ce n'est pas sauvegardé
	 */
	bool saveGame(string username, string name_of_game);

	/**
	 * @brief 
	 * 
	 * @param username Nom de l'hôte du lobby
	 * @param name_of_game Nom de la partie
	 * @return true Si le chargement est réussi
	 * @return false Si le chargement a échoué
	 */
    bool loadGame(string username, string name_of_game);

	/**
	 * @brief Cette fonction sérialise la liste des noms des joueurs en un seul string
	 * 
	 * @return string liste des noms des joueurs sérialisée
	 */
	string serializePlayerListToString();

	/**
	 * @brief Retourne une liste de nom de joueurs qui sont présent dans la partie sauvegardée
	 * 
	 * @param str liste sérialisée
	 * @return vector<string> liste de nom des joueurs
	 */
	vector<string> deserializePlayerListInString(string str);

	/**
	 * @brief Compare si les joueurs dans le jeu sont les mêmes que ceux de la partie chargée
	 * 
	 * @param vec liste de nom des joueurs
	 * @return true si les joueurs dans la partie sauvegardée sont présent dans le lobby
	 * @return false si un ou plusieurs joueurs ne sont pas présent dans le lobby
	 */
	bool isSamePlayerNameLoaded(vector<string> vec);

	/**
	 * @brief Sérialise les coordonnées de départ des joueurs
	 * 
	 * @return string Liste sérialisée des coordonnées de départ des joueurs
	 */
	string serializeStartCoord();

	/**
	 * @brief Désérialise les coordonnées de départ des joueurs
	 * 
	 * @param str Liste des points sérialisée
	 */
	void deserializeStartCoord(string str);

	/**
	 * @brief Remplace les bonnes coordonnées actuelles à tous les joueurs présent dans la partie
	 * 
	 */
	void readjustPlayerCoord();

	/**
	 * @brief Sérialise le nombre de murs restant pour chaque joueur restant
	 * 
	 * @return string Liste sérialisée des murs restants pour chaque joueur
	 */
	string serializeWallRemaining();

	/**
	 * @brief Réapplique les murs restants pour chaque joueur
	 * 
	 * @param str Liste sérialisée des murs restants
	 * @return vector<int> Liste des murs restants
	 */
	vector<int> deserializeWallRemaining(string str);

	/**
	 * @brief Retourne la liste des joueurs de la partie
	 * 
	 * @return vector<Player*> Liste des joueurs de la partie
	 */
	vector<Player*> getPlayerList();

	/**
	 * @brief Fait une rotation du plateau en fonction du joueur
	 * 
	 */
    void rotateBoard();

};


#endif
