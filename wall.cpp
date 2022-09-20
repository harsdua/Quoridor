
#include "wall.h"

#include <vector>
#include <iostream>

using namespace std;


bool Wall::operator== (const Wall &w)  const {
		// check si les murs sont les même ET/OU si les murs se croisent 
		vector<Point>c1{pt_1, pt_2, adj_1, adj_2};
		vector<Point>c2{w.pt_1, w.pt_2, w.adj_1, w.adj_2};
		int tester = 0;
		for (auto point : c1)
			for (auto other_point : c2)
				if (point == other_point){
					tester++;
				}
		if(tester == 4){
			//les quatres cases concernées par les deux murs sont les mêmes => les deux murs sont égaux/ se croisent
			return true;
		}else{
			return false;
		}
	}

bool isInWallPoint(Point pt, Wall w){
	if (pt == w.pt_1 || pt == w.pt_2){
		return true;
	}
	return false;
}

bool isInWallAdj(Point pt, Wall w){
	if (pt == w.adj_1 || pt == w.adj_2){
		return true;
	}
	return false;
}


bool possibleWall(Point pt_1, Point pt_2){
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

Wall createWall(Point pt_1, Point pt_2, char direction){

	Wall wall;
	wall.pt_1 = pt_1;
	wall.pt_2 = pt_2;

	int dir = 1;
	if (direction == '-'){
		dir = -1;
	}
	if (pt_1.x == pt_2.x){
		Point ad_1 = {pt_1.x + dir, pt_1.y};
		Point ad_2 = {pt_2.x + dir, pt_2.y};
		wall.adj_1 = ad_1;
		wall.adj_2 = ad_2;
	}else{
		Point ad_1 = {pt_1.x, pt_1.y + dir};
		Point ad_2 = {pt_2.x, pt_2.y + dir};
		wall.adj_1 = ad_1;
		wall.adj_2 = ad_2;
	};

	//wallList.push_back(wall);
	return wall;
}

string serializeWallList(vector<Wall> wallList){
    string wallToString = "";

    int counter = 0;
    for(auto w : wallList){
        if(counter > 0)wallToString += "-";
        wallToString += to_string(w.pt_1.x);
        wallToString += ":";
        wallToString += to_string(w.pt_1.y);
        wallToString += ";";
        wallToString += to_string(w.pt_2.x);
        wallToString += ":";
        wallToString += to_string(w.pt_2.y);
        wallToString += ";";
        wallToString += to_string(w.adj_1.x);
        wallToString += ":";
        wallToString += to_string(w.adj_1.y);
        wallToString += ";";
        wallToString += to_string(w.adj_2.x);
        wallToString += ":";
        wallToString += to_string(w.adj_2.y);
        counter++;
    }

    return wallToString;
}

vector<Wall> deserialiseWallString(string wall_str){
    vector<Wall> wall_list;
    stringstream ss(wall_str);
    string item;

    while(getline(ss, item, '-')){
        stringstream ss2(item);
        string item2;
        Point pt_1, pt_2, adj_1, adj_2;
        int counter = 0;
        while(getline(ss2, item2, ';')){
            stringstream ss3(item2);
            string item3;
            int counter_bis = 0;
            Point p;
            while(getline(ss3, item3, ':')){
                if(counter_bis == 0){ p.x = stoi(item3); }
                else{ p.y = stoi(item3); }
                counter_bis++;
            }
            if(counter == 0){pt_1 = p;}
            else if(counter == 1){pt_2 = p;}
            else if(counter == 2){adj_1 = p;}
            else if(counter == 3){adj_2 = p;}
            counter++;
            //cout << "item2 - " << item2 << endl;
        }
        Wall temp = {pt_1, pt_2, adj_1, adj_2};
        wall_list.push_back(temp);
    }
    return wall_list;
}