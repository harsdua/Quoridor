#ifndef __COMMON__H
#define __COMMON__H

#include <string.h>
#include <iostream>

using namespace std;

struct Point {
	int x, y;

	Point &operator=(Point &p){
		x = p.x;
		y = p.y;
		return *this;
	}
	bool operator== (const Point &p)  const { 
		if(p.x == x && p.y == y){
			return true;
		}
		return false;
	}
	bool operator!= (const Point &p)  const { 
		if(p.x != x || p.y != y){
			return true;
		}
		return false;
	}
	bool operator> (const Point &p)  const { 
		if(x > p.x){
			return true;
		}else if (x == p.x){
			if (y > p.y){
				return true;
			}
			return false;
		}
		return false;
	}
	Point() {}
	Point(int x, int y): x{x}, y{y}{}
	};

enum GameMode{standard, timed, four, attack};

GameMode stringToGameMode(const string& mode);

string gameModeToString(const GameMode& mode);

bool isPointNeighbor(Point pt_1, Point pt_2);

#endif