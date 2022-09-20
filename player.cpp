/**
 * @file player.h
 * @author Edem & Miguel
 * @brief Fichier .cpp de la classe Player
 */

#include "player.h"

//Constructeur

Player::Player(string name):name{name} {}

// Getters

int Player::getId(){return id;}

int Player::getRemainingWall(){return wallRemaining;}

string Player::getName(){return name;}

bool Player::isTimerCounting(){return timer.isCounting();}

bool Player::isTimerEmpty(){return timer.isEmpty();}

string Player::getRemainingTime(){return timer.getRemainingTime();}

Point Player::getPoint(){return coord;}

Point Player::getStart_Point(){return start_coord;}

// Setters

void Player::setId(int newId){id = newId;}

void Player::setTimer(int time){timer.setTimer(time);}

void Player::loadTimer(string storedTimer){timer.loadTimer(storedTimer);}

void Player::setCoord(Point pt){coord = pt;}

void Player::setStart_coord(Point pt){start_coord = pt;}

void Player::setWallBank(int nbrWalls){wallRemaining = nbrWalls;}

// Others

void Player::startTimer(){timer.start_timer();}

void Player::stopTimer(){timer.stop_timer();}

void Player::removeWall(){wallRemaining--;}

bool Player::isWinningCoord(Point pt){
	if (start_coord.x == 0 && pt.x == 8){
		return true;
	}else if (start_coord.x == 8 && pt.x == 0){
		return true;
	}else if (start_coord.y == 0 && pt.y == 8){
		return true;
	}else if (start_coord.y == 8 && pt.y == 0){
		return true;
	}else{
		return false;
	}
}
