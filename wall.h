#ifndef __WALL__H
#define __WALL__H

#include "common.h"
#include <vector>
#include <iostream>
#include <sstream> 

using namespace std;


struct Wall {
	Point pt_1;
	Point pt_2;
	Point adj_1;
	Point adj_2;

    char direction;

	bool operator== (const Wall &w)  const;

    Wall &operator=(Wall &w){
        pt_1 = w.pt_1;
        pt_2 = w.pt_2;
        adj_1 = w.adj_1;
        adj_2 = w.adj_2;
        return *this;
    }

};



bool possibleWall(Point pt_1, Point pt_2);

bool isInWallPoint(Point pt, Wall w);

bool isInWallAdj(Point pt, Wall w);

Wall createWall(Point pt_1, Point pt_2, char direction);

string serializeWallList(vector<Wall> wallList);
vector<Wall> deserialiseWallString(string wall_str);
#endif
