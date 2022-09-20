/**
 * @file timer.cpp
 * @author Edem
 * @brief Fichier .cpp de la classe Timer
 * 
 */
#include "timer.h"


// Getters
bool Timer::isCounting(){return counting;}

//Setters

void Timer::setTimer(int givenlimit){min_left = givenlimit;}

void Timer::loadTimer(string remainingTime){
	stringstream ss(remainingTime); string item;
    int counter = 0;
    while (getline(ss, item, ':')) {
        if (!counter){  //counter == 0, on est sur les minutes
			min_left = stoi(item);
		}else{    //counter !=0 on est sur les secondes
			sec_left = stoi(item);
		}
        counter++;
    }
	setNewLimit();
}

//Others 

time_t Timer::getTime(){
	time_t temp; time(&temp);
	return temp;
}

bool Timer::isEmpty(){
	return (difftime(limit, getTime())<= 0) ? true : false;
}

void Timer::start_timer(){
	counting = true;
	time(&start);
	setNewLimit();
}

void Timer::stop_timer(){
	counting = false;
	double seconds = difftime(getTime(), start); 
	min_left -= (int)seconds/60;
	sec_left -= (int)seconds%60;
}

std::string Timer::getRemainingTime(){
	double current = difftime(limit, getTime());
	int seconds = (int)current % 60;
	int minutes = (int)current / 60;
	return to_string(minutes) + ":" + to_string(seconds);
}

void Timer::setNewLimit() {
	cout << "min_left1: " << min_left << "|  sec_left1: " << sec_left << endl;
	time(&limit);
	tm * structLimit = localtime(&limit);

	structLimit->tm_min += min_left;
	structLimit->tm_sec += sec_left;

	limit = mktime(structLimit);

	cout << "min limite1: " << structLimit->tm_min << "| sec limite1: " << structLimit->tm_sec << endl;

	cout << "limit min : " << to_string(localtime(&limit)->tm_min) << "limit sec : " << to_string(localtime(&limit)->tm_min) << endl;;
}
