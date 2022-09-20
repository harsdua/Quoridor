/**
 * @file player.h
 * @author Edem & Miguel
 * @brief Fichier header de la classe Player
 */
#ifndef __PLAYER_H
#define __PLAYER_H

#include "common.h"
#include "timer.h"


using namespace std;


//!  Classe Player. 
/*!
  Classe qui représente un joueur de la partie.
*/
class Player {

	string name;				///< Nom/Pseudo du joueur

	int id;						///< Id du joueur, permet de déterminer l'ordre de jeu
	int wallRemaining;		///< Nombre de murs que le joueur peut poser

	Point coord;				///< Coordonnées actuelles du pion du joueur sur le plateau
	Point start_coord;			///< Coordonées de départ du pion du joueur sur le plateau

	Timer timer;				///< Timer personel du joueur pour le mode temps

	public:

	//! Constructeur
	/**
	 * @brief Construit un nouvel objet Player 
	 * 
	 * @param name Nom du joueur a counstruitre
	 */
	Player(string name);


	// Getters

	/** getId. 
     *  @brief Retoutne l'id du joueur.
	 * 	@return id l'id du joueur sous forme de int
     */
	int getId();

	/** getRemainingWall. 
     *  @brief Permet de savoir combien de murs le joueur peut encore placer.
	 * 	@return getRemainingWall le nombre de murs restants sous forme de int 
     */
	int getRemainingWall();

	/** getName. 
     *  @brief Permet de récupérer le nom du joueur.
	 * 	@return le nom/pseudo du joueur sous forme de std::string
     */
	string getName();

	/** isTimerCounting. 
     *  @brief Permet de savoir si le timer du joueur est actif ou non et renvoie le booléen correspondant (true = actif).
	 * 	@return bool
     */
	bool isTimerCounting();

	/** isTimerEmpty. 
     *  @brief Permet de savoir si le timer du joueur est vide ou non et renvoie le booléen correspondant (true = vide).
	 * 	@return bool
     */	
	bool isTimerEmpty();

	/** getRemainingTime. 
     *  @brief Permet de savoir combien de temps il reste sur le timer du joueur.
	 * 	@return string du temps restant dans le format "mm:ss"
     */
	string getRemainingTime();

	/** getPoint. 
     *  @brief Permet de récupérer la position actuelle du pion du joueur.
	 * 	@return Point de la position actuelle
     */
	Point getPoint();

	/** getStart_Point. 
     *  @brief Permet de récupérer la position initialle du pion du joueur.
	 * 	@return Point de la position initialle
     */
	Point getStart_Point();


	//Setters

	/** setId. 
     *  @brief Set l'id du joueur a l'id donné en parametre.
	 *  @param newId Nouvelle id à donner au joueur
     */
	void setId(int newId);

	/** setTimer. 
     *  @brief Permet de set le temps d'activité du timer.
	 * 	@param time le temps EN MINUTES pendant lequel le timer doit être actif
     */
	void setTimer(int time);

	/**
	 * @brief permet de set le Timer du joueur a un certain temps donné (ici utilisé pour charger un temps stocké dans la bdd)
	 * 
	 * @param storedTimer string sous forme "mm:ss" qui represente le temps restant a donner au joueur
	 */
	void loadTimer(string storedTimer);

	/** setCoord. 
     *  @brief Permet de set les coordonnées du pion du joueur 
	 * 	@param pt nouvelles coordonnées du pion du joueur
     */
	void setCoord(Point pt);

	/** setStart_Coord. 
     *  @brief Permet de set les coordonnées initiales du pion du joueur 
	 * 	@param pt nouvelles coordonnées initiales du pion du joueur
     */
	void setStart_coord(Point pt);

	/** setWallBank.
	 * 	@brief Permet de set le nombre de murs du joueur au nombre donné en paramètre.
	 * 	@param nbrWalls int du nombre de murs qui sont attribué au joueur
	 */
	void setWallBank(int nbrWalls);

	//Others

	/** startTimer. 
     *  @brief Permet de lancer/relancer le timer du joueur.
     */
	void startTimer();

	/** stopTimer. 
     *  @brief Permet de stoper le timer du joueur.
     */
	void stopTimer();

	/** removeWall. 
     *  @brief Permet de décrémenter le nombre de murs restants du joueur.
     */
	void removeWall();

	/**
	 * @brief Regarde si le point donné est une case gagnante pour le joueur
	 * 
	 * @param pt Point pour lequel on effectue la verification
	 * @return true le point donné est une case gagnante
	 * @return false le point donné n'est pas une case gagnante
	 */
	bool isWinningCoord(Point pt);

};





#endif