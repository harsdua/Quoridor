#ifndef __BOARD__H
#define __BOARD__H

#include "common.h"
#include "player.h"
#include "wall.h"

#include <string.h>

#include <charconv>
#include <set>
#include <vector>


class Board {

	vector<vector<int>> board;
	vector<Wall> wallList;
	int taillePlateau = 9;
	vector<vector<Point>> zones;

	GameMode gameMode;

	public:
	
	Board();

	Board(GameMode GameMode) : gameMode{gameMode} {
		Board();
	}

    // Setters

    void setNewWallList(vector<Wall> New_WL);


    // Others

	///Utilité

	/**
	 * @brief Affiche le plateau de jeu et les éléments qui s'y trouve en cout
	 */
	void displayBoard();

	/**
	 * @brief Convertit le coup donné, le transforme en la position qu'il represente dans la matrice plateau
	 * 
	 * @param coup string qui représente une case du plateau affiché
	 * @return Point conversion de la case donné en point de la matrice
	 */
	Point getConvertedCoup(string coup);

	/**
	 * @brief Prends un vecteur contenant potentiellement plusieurs coups/cases et le convertit en vecteur de point de la matrcie
	 * 
	 * @param args vecteur de string a traduire en vecteur de Points de la matrice
	 * @return vector<Point> Contient les cases fournies converties en Points
	 */
	vector<Point> charstrToPoint(vector<string>args);


	///Manipulation de variable

	/**
	 * @brief efface le pion du joueur donné, en paramètre, de la matrice
	 * 
	 * @param p Pointeur vers le joueur dont ont veut effacer le pion
	 */
	void clearPawn(Player p);

	/**
	 * @brief Ajoute le mur donné en paramètre dans la liste des murs du plateau
	 * 
	 * @param w Wall qu'on veut ajouter au plateau 
	 */
	void placeWall(Wall w);

	/**
	 * @brief Ajoute le pion du joueur, dont l'id est donné en paramètre, dans le plateau à la position
	 * 		  board[int x][int y]
	 * 
	 * @param x Abcisse de la coordonnée à laquelle on veut placer le pion
	 * @param y Ordonnée de la coordonnée à laquelle on veut placer le pion
	 * @param id Id du joueur dont on veut placer le pion
	 */
	void play(int x, int y, int id);


	///Verification de coup

	/**
	 * @brief Regarde si la poisition donné en paramètre et a une case (non diagonale) 
	 * du joueur, pointé par le pointeur donné en paramètre
	 * @param pt Position dont on veut savoir si elle est voisine du joueur
	 * @param p Pointeur vers le joueur voulu pour la verification mentionnée
	 * @return true La case est une voisine (non diagonale) du joueur 
	 * @return false La case n'est pas une voisine (non diagonale) du joueur 
	 */
	bool isOneStepAway(Point pt, Player *p);

	/**
	 * @brief Regarde si le point donné n'est occupé par aucun joueur/pion
	 * 
	 * @param pt Point/Position dans la matrice pour laquelle on va verfier l'etat d'occupation
	 * @return true La case donnée est libre
	 * @return false La case donnée est occupée
	 */
	bool isFreePointInBoard(Point pt);

	/**
	 * @brief Regarde si,lorsqu'on le place, le mur donné en paramètre bloque l'accès a l'arrivée d'un des joueurs.
	 * 
	 * @param w Le mur a placer dans le plateau
	 * @return true si l'accès d'un des joueurs est bloqué
	 * @return false aucun joueurs n'est bloqué
	 */
	bool isPlayerBlocked(Wall w, vector<Player*>players);

	/**
	 * @brief Regarde si un Point donné est dans une zone (en général)
	 * 
	 * @param pt Point dont on vérifie si il se trouve dans un zone
	 * @return true Le point est déja classé dans une zone
	 * @return false Le point n'est pas encore classé dans une zone
	 */
	bool isInZone(Point pt);

	/**
	 * @brief Nous permet de trouver dans quelle zone se trouve un certain point donné
	 * @param pt Point dont on veut savoir dans quelle zone il se trouve
	 * @return int Index de la zone, dans laquelle se trouve le point pt, dans le vecteur zones
	 */
	int inWhichZoneIs(Point pt);

	/**
	 * @brief Regarde si il y a un mur entre deux points donnés (ils ne doivent pas être collés)
	 * @param p1 premier point pour la vérification
	 * @param p2 deuxieme point pour la vérification
	 * @return true il y a un mur entre les deux points donnés
	 * @return false les deux points ne sont pas séparés par un mur
	 */
	bool isWallBetween(Point p1, Point p2);

	
	/**
	 * @brief Regarde si un certain point est interne a la matrice de jeu
	 * 
	 * @param pt Point pour lequel on veut faire la verification
	 * @return true Le point est interne a la matrice de jeu
	 * @return false Le point est externe a la matrice de jeu
	 */
	bool isPointInBoard(Point pt);

	/**
	 * @brief Regarde si chaque joueur à encore au moins un chemin vers une case gagnante
	 * 
	 * @return true Tous les joueurs peuvent encore gagner
	 * @return false il y a au moins un joueur qui ne peut plus gagner
	 */
	bool isWayToWin(vector<Player*>players);

	/**
	 * @brief Regarde si le mur qu'on veut placer se trouve dans les limites de le matrice de jeu
	 * 
	 * @param argsInPoint vecteur de Points avoisinant le mur 
	 * @param args Coup donné transformé en vecteur de string
	 * @return true le mur est à l'intérieur du plateau
	 * @return false le mur est à l'exterieur du plateau
	 */
	bool isWallInBoard(vector<Point> argsInPoint, vector<string> args);

	/**
	 * @brief Regarde si il y a un autre joueur sur la case concerné par le coup donné par le joueur pointé
	 * 			(peut être remplacée par isFreePointInBoard mais je l'ai gardé car les noms permettent de mieux comprendre
	 * 			les fonctions dans lequelles elles apparaissent)
	 * @param args Coup donné transformé en vecteur de string
	 * @param p Pointeur vers le joueur qui a donné le coup
	 * @return true il y a un autre joueur sur la case donné
	 * @return false la case donnée est libre
	 */
	bool isOtherPlayerOnPoint(vector<string> args, Player *p);

	/**
	 * @brief Regarde si le joueur donné peut, dans un cas normal/basique (il est seul, pas de murs) se déplacer sur la case donnéé
	 * 
	 * @param pt Case/position dont on veut savoir si le joueur peut l'atteindre
	 * @param p joueur utilisé pour la verification
	 * @return true le joueur peut su déplacer sur la case
	 * @return false le joueur ne peut pas se déplacer sur la case donnée
	 */
	bool legalPointForPlayer(Point pt, Player *p);

	/**
	 * @brief Regarde si un certain coup donné est legal/jouable ou pas 
	 * 
	 * @param args Coup donné transformé en vecteur de string
	 * @param p Pointeur vers le joueur jouant le coup
	 * @return true Le coup est legal
	 * @return false Le coup est illégal
	 */
	bool legalCoup(vector<string> args, Player *p, vector<Player*>players);

	/**
	 * @brief Regarde si le mur est légal pour les abcisses (il ne dépasse pas)
	 * 
	 * @param argsInPoint Coup donné avec les cases converties en vecteur points
	 * @param args Coup donné transformé en vecteur de string
	 * @return true Le mur est légal pour les abcisses
	 * @return false Le mur n'est pas legal pour les abcisses
	 */
	bool legalWallX(vector<Point> argsInPoint, vector<string> args);

	/**
	 * @brief Regarde si le mur est légal pour les ordonnées (il ne dépasse pas)
	 * 
	 * @param argsInPoint Coup donné avec les cases converties en vecteur points
	 * @param args Coup donné transformé en vecteur de string
	 * @return true Le mur est légal pour les ordonnées
	 * @return false Le mur n'est pas legal pour les ordonnées
	 */
	bool legalWallY(vector<Point> argsInPoint, vector<string> args);
	
	/**
	 * @brief Regarde si la structure du mur a placer est bonne (syntaxe, point légaux)
	 * 
	 * @param argsInPoints Coup donné avec les cases converties en vecteur points
	 * @param args Coup donné transformé en vecteur de string
	 * @return true la structure est bonne
	 * @return false la structure n'est pas bonne
	 */
	bool checkWallStruct(vector<Point> argsInPoints, vector<string> args);

	/**
	 * @brief Regarde si la logique du mur a placer est bonne (pas de doublon/croix, personne n'est enfermé, mur interne au plateu) 
	 * 
	 * @param argsInPoints Coup donné avec les cases converties en vecteur points
	 * @param args Coup donné transformé en vecteur de string
	 * @return true La logique du mur est bonne
	 * @return false La logique du mur n'est pas bonne
	 */
	bool checkWallLogic(vector<Point> argsInPoint, vector<string> args, vector<Player*>players);
	
	/**
	 * @brief Regarde si un il y a deja un mur qui sépare les même cases que celui passé en paramètre
	 * 
	 * @param w Mur pour lequel on veut effectuer la verification
	 * @return true Le mur existe déja / celui donné en paramètre en croise un autre
	 * @return false Le mur donné n'existe pas (encore) et il ne croise aucun autre mur
	 */
	bool checkDoubleWall(Wall w);
	

	/**
	 * @brief Trouve les cases sur lequelles on peut aterrir si on saute par dessus la case donnée
	 * 	sous forme de vecteur de char*
	 * 
	 * @param args Position de la case a sauter sous forme de vecteur de string
	 * @param p Pointeur vers le joueur qui saute
	 * @return vector<char*> position de la/des case(s) d'attérisage(s)
	 */
	vector<string> getJumpPoints(vector<string> args, Player *p);
	
	/**
	 * @brief stocke dans zones, les différentes zones du plateau (séparées par des murs)
	 */
	void makeZones();

	/**
	 * @brief Ajoute les voisins de la case donnée dans la même zone que cette dernière
	 * 
	 * @param pt case dont on veut ajouter les voisins dans la zone ou elle se trouve
	 */
	void addNeighborsToZone(Point pt);

	/**
	 * @brief Ajoute un case donné dans une zone donnée en evitant le fait d'ajouter un doublon
	 * 
	 * @param pt Point de la case a ajouter a la zone
	 * @param zone int index de la zone, dans laquelle on veut ajouter pt, dans zones
	 * @return true la case à été ajoutée
	 * @return false la case n'as pas été ajoutée
	 */
	bool addToZone(Point pt, int zone);


	///Sauvegarde de partie

    bool saveGame(string username, string name_of_game);
    bool loadGame(string username, string name_of_game);
	void loadWallsFromPartySaved(vector<Wall>wallsaved);
    void deserializeBoard(string board_ser);
    string serializeBoard();
	vector<string> getUsersIndexes();
	GameMode getGameMode();
	void setGameMode(const GameMode newMode);
	vector<Wall> getWallList();

	/**
	 * @brief Retourne le board
	 * 
	 * @return vector<vector<int>> Le board de la game 
	 */
	vector<vector<int>> getBoard();
	void setBoard(vector<vector<int>> bo);
    void clearWallList();
    void setWallList(vector<Wall> lst);
    void rotateBoard();
	
};

#endif //__BOARD__H
