#include "common.h"

GameMode stringToGameMode(const string& mode) {
	GameMode toSet;
	if (mode=="timed") {
		toSet=timed;
	} else if (mode=="four"){
		toSet=four;
	} else if (mode=="standard"){
		toSet=standard;
	} else if (mode=="attack"){
		toSet=attack;
	} else {
		cout << "Gamemode " << mode << " n'existe pas." << endl;
		cout << "Gamemode par défaut appliqué : standard" << endl;
		toSet = standard;
	}
	return toSet;
}

string gameModeToString(const GameMode& mode) {
	string strMode="";
	switch (mode) {
		case(timed):
			strMode="timed";
			break;
		case(four):
			strMode=four;
			break;
		case(standard):
			strMode="standard";
			break;
		case(attack):
			strMode="attack";
			break;
		default:
			cout<<"Wrong mode"<<mode<<endl;
	} 

	return strMode;
}

bool isPointNeighbor(Point pt_1, Point pt_2){
    bool res = false;
    if (pt_1.x == pt_2.x){
        if (pt_1.y - pt_2.y == 1 || pt_1.y - pt_2.y == -1){
            res = true;
        }
    }
    if (pt_1.y == pt_2.y){
        if (pt_1.x - pt_2.x == 1 || pt_1.x - pt_2.x == -1){
            res = true;
        }
    }
    return res;
}