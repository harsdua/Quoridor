/**
 * @file timer.cpp
 * @author Edem
 * @brief Fichier header de la classe Timer
 * 
 */
#ifndef __TIMER__H
#define __TIMER__H

#include <string> //string
#include <time.h> //time_t, time(), localtime(), difftime(), mktime()
#include <sstream> // stringstream(), getline()
#include <iostream>
using namespace std;

//!  Classe Timer. 
/*!
  Classe qui représente le timer d'un joueur pour une partie en mode temps.
*/
class Timer{
	
	bool counting = false;			///< Permet de savoir si le timer est actif ou pas 

	int min_left = 0;				///< le nombre de minutes que le timer reste actif LORS de son/sa activation/réactivation 

	int sec_left = 0;				///< le nombre de secondes que le timer reste actif LORS de son/sa activation/réactivation 

	time_t limit;

	time_t start;
	//struct tm * limit;				///< Structure de temps qui stocke le temps auquel le timer va être totalement écoulé

	//struct tm * start;				///< Structure de temps qui stocke le temps auquel le timer est activé/réactivé

  public:

	// Getters

	/** isCounting. 
     *  Getter de la variable d'instance counting, qui indique si le timer est actif ou pas.
	 * 	@return bool
     */
	bool isCounting();

	//Setters

	/** setTimer. 
     *  Permet de donner une limite de temps au timer
	 *  @param givenlimit int qui donne le temps limite EN MINUTES
     */
	void setTimer(int givenlimit);

	/**
	 * @brief Permet de charger un temps, stocké dans la base de données, comme temps restant du timer
	 * 
	 * @param RemainingTime string sous forme "mm:ss" du temps restant
	 */
	void loadTimer(string RemainingTime);

	//Others 

	/** getTime. 
     *  Permet d'avoir un élément qui contient les données temporelles au moment de l'appel.
	 * @return Renvoie un élément time_t qui contient le temps actuel (dont l'heure/minutes/secondes qui nous interesse ici)
     */
	time_t getTime();

	/** isTimerEmpty. 
     *  Retourne vrai ou faux, respectivement si il reste du temps dans le timer ou pas.
	 * 	@return bool
     */
	bool isEmpty();

	/** start_timer. 
     *  Débute ou reprends le décompte du timer.
     */
	void start_timer();

	/** stop_timer. 
     *  Interrompt le décompte du timer.
     */
	void stop_timer();

	/** setNewLimit. 
     *  Permet de replacer le temps d'arret du timer lors de la reprise de ce dernier.
	 * 
     */
	string getRemainingTime();

	/** setNewLimit. 
     *  Permet de replacer le temps d'arret du timer lors de la reprise de ce dernier.
     */
	void setNewLimit();

};



#endif
