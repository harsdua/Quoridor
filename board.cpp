#include "board.h"
#include "database.h"
#include "common.h"


void Board::setGameMode(const GameMode newMode) {
	gameMode=newMode;
}

GameMode Board::getGameMode() {
	return gameMode;
}

Board::Board(){
	// Constructeur 
	//initialise board la matrice repésentant le plateau de jeu
		for(int i = 0; i < taillePlateau; i++){
			vector<int> v;
			for(int ii = 0; ii < taillePlateau; ii++){
				v.push_back(0);
			}
			board.push_back(v);
		}
}

// Setters

void Board::setNewWallList(vector<Wall> New_WL){
    wallList.clear();
    for(auto wall : New_WL){
        wallList.push_back(wall);
    }
}


// Others

void Board::play(int x, int y, int id){
	board[x][y] = id;
}

void Board::placeWall(Wall w){
	wallList.push_back(w); 
}

void Board::clearPawn(Player p){
	for(int i = 0; i < taillePlateau; i++)
		for(int ii = 0; ii < taillePlateau; ii++)
			if(board[i][ii] == p.getId())
				board[i][ii] = 0;
}



void Board::displayBoard(){
	// affiche le plateau
	//system("clear");
	vector<char> abs{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
	cout << "     ";
	for(int i = 0; i < taillePlateau; i++){
		cout << i + 1 << "       ";
	}
	cout << endl;
	cout << endl;
	for(int i = 0; i < taillePlateau; i++){
		cout << abs[i] << "   ";
		for(int ii = 0; ii < taillePlateau; ii++){
			if(board[i][ii] == 0){
				cout << "[ ]";
			}else if(board[i][ii] > 0){
				cout << "[" + to_string(board[i][ii]) +"]";
			}
			cout << "  ";
			//partie wall vertical
			if(isWallBetween(Point{i, ii}, Point{i, ii+1})){ 
				cout << "|";
			}else{
				cout << " ";
			}
			//fin espace wall
			cout << "  ";
		}
		cout << endl;
		cout << endl;
		cout << "    ";
		//partie wall horiz
		for(int ii = 0; ii < taillePlateau; ii++){
			if(isWallBetween(Point{i, ii}, Point{i+1, ii})){ 
				cout << "―――";
			}else{
				cout << "   ";
			}
			cout << "     ";
		}
		cout << endl;
		//fin partie wall
		cout << endl;
	}
	cout << endl;
}

Point Board::getConvertedCoup(string coup){
	int x;
	int y = (int)coup[1] - 49;
	vector<char>charlist{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
	for (size_t i = 0; i < charlist.size(); i++){
		if (charlist[i] == coup[0]){
			x = i;
			return {x, y};
		}
	}
	return {-1, -1};
}

bool Board::isOneStepAway(Point pt, Player * p){
	Point steps(pt.x - p->getPoint().x, pt.y - p->getPoint().y);
	for (auto &shift : vector<Point>{{0, 1}, {1, 0}, {-1, 0}, {0, -1}}){
		if (steps == shift){
			return true;
		}
	}
	return false;
}

bool Board::legalPointForPlayer(Point pt, Player *p){
	if(isPointInBoard(pt) && !isWallBetween(pt, p->getPoint()) && isOneStepAway(pt, p)){ return true; }
	return false;
}

bool Board::legalWallX(vector<Point> argsInPoint, vector<string> args){
	if (argsInPoint[0].x == argsInPoint[1].x){
		if ((argsInPoint[0].x == 0 && args[2][0] == '-') || (argsInPoint[0].x == 8 && args[2][0] == '+')){
			return false;
		}
		return true;
	}
	return true;
}

bool Board::legalWallY(vector<Point> argsInPoint, vector<string> args){
	if (argsInPoint[0].y == argsInPoint[1].y){
		if ((argsInPoint[0].y == 0 && args[2][0] == '-') || (argsInPoint[0].y == 8 && args[2][0] == '+')){
			return false;
		}
		return true;
	}
	return true;
	
}

bool Board::checkDoubleWall(Wall w){
	for (auto wall : wallList){
		if (wall == w){
			return true;
		}
	}
	return false;
}

vector<Point> Board::charstrToPoint(vector<string> args){
	vector<Point> argsInPoints;
	for (size_t i = 0; i < args.size(); i++){
		argsInPoints.push_back(getConvertedCoup(args[i]));
	}
	return argsInPoints;
}

bool Board::checkWallStruct(vector<Point> argsInPoints, vector<string> args){
	if (args[2][0] != '+' && args[2][0] != '-')
		return false;
	if (!isPointInBoard(argsInPoints[0]) || !isPointInBoard(argsInPoints[1]))
		return false;
	if(!isPointNeighbor(argsInPoints[0], argsInPoints[1]))
		return false;
	return true;
}

bool Board::isWallInBoard(vector<Point> argsInPoint, vector<string> args){
	if (legalWallX(argsInPoint, args) && legalWallY(argsInPoint, args))
		return true;

	return false;
}

bool Board::checkWallLogic(vector<Point> argsInPoint, vector<string> args, vector<Player*>players){
	if (!isWallInBoard(argsInPoint, args))
		return false;
	Wall temp = createWall(argsInPoint[0], argsInPoint[1], args[2][0]);
	if (checkDoubleWall(temp) || isPlayerBlocked(temp, players))
		return false;
	return true;

}

bool Board::legalCoup(vector<string> args, Player *p,vector<Player*>players){
	vector<Point> argsInPoint = charstrToPoint(args);
	if(args.size() == 1){
		return legalPointForPlayer(argsInPoint[0], p);

	}else if(args.size() == 3){
		if (checkWallStruct(argsInPoint, args) && checkWallLogic(argsInPoint, args, players))
			return true;
		return false;
		
	}else{
		return false;
	}
}


bool Board::isPlayerBlocked(Wall w, vector<Player*>players){
	wallList.push_back(w);
	makeZones();
	if (isWayToWin(players)){
		wallList.pop_back();
		return false;
	}else{
		wallList.pop_back();
		return true;;
	}
}

void Board::makeZones(){
	zones.clear();
	for (int ligne = 0; ligne < taillePlateau; ligne++){
		for (int colonne = 0; colonne < taillePlateau; colonne++){
			if(!isInZone(Point(ligne, colonne))){
				addNeighborsToZone(Point(ligne, colonne));
			}
		}
	}
}

void Board::addNeighborsToZone(Point pt){
	int inZone;
	if (!isInZone(pt)){
		zones.push_back({});
		inZone = zones.size() - 1;
		zones[inZone].push_back(pt);
	}
	inZone = inWhichZoneIs(pt);
	for (auto &shift : vector<Point>{{0, 1}, {1, 0}, {-1, 0}, {0, -1}}){
		Point neighbor{pt.x + shift.x, pt.y + shift.y};
		if (isPointInBoard(neighbor)){
			if (!isWallBetween(pt, neighbor)){	
				if(addToZone(neighbor, inZone)){
					addNeighborsToZone(neighbor);
				}
			}
		}
	}
}

bool Board::isInZone(Point pt){
	for (size_t i = 0; i < zones.size(); i++){
		for (size_t j = 0; j < zones[i].size(); j++){
			if (pt == zones[i][j]){
				return true;
			}
		}
	}
	return false;
}

int Board::inWhichZoneIs(Point pt){
	for (size_t i = 0; i < zones.size(); i++){
		for (size_t j = 0; j < zones[i].size(); j++){
			if (pt == zones[i][j]){
				return i;
			}
		}
	}
	return -1;
}

bool Board::isWallBetween(Point p1, Point p2){
	for (size_t i = 0; i < wallList.size(); i++){
		if ((isInWallPoint(p1, wallList[i]) && isInWallAdj(p2, wallList[i])) || (isInWallPoint(p2, wallList[i]) && isInWallAdj(p1, wallList[i])) ){
			// si les deux point sont séparés par un mur
			return true;
		}
	}
	return false;
}

bool Board::addToZone(Point pt, int zone){
	if (isInZone(pt)){
		return false;
	}else{
		zones[zone].push_back(pt);
		return true;
	}
}


bool Board::isPointInBoard(Point pt){
	if (((0 <= pt.x) && (pt.x <= 8)) && ((0 <= pt.y)  && (pt.y <= 8))){
		return true;
	}else{
		return false;
	}	
}

bool Board::isWayToWin(vector<Player*>players){
	for(auto player : players){
		bool canWin = false;
		for (size_t i = 0; i < zones.size(); i++){
			bool playerInZone = false;
			bool winInZone = false;
			for (size_t j = 0; j < zones[i].size(); j++){
				if (player->getPoint() == zones[i][j])
					playerInZone = true;
				if (player->isWinningCoord(zones[i][j]))
					winInZone = true;
			}
			if (playerInZone && winInZone)
				canWin = true;
		}
		if (!canWin)		// Il y a un joueur qui ne peut plus gagner dans sa zone
			return false;
	}
	return true;  //tous les joueurs peuvent encore gagner leur zone
}

bool Board::isOtherPlayerOnPoint(vector<string>args, Player *p){
	Point pt(getConvertedCoup(args[0]));
	if (board[pt.x][pt.y] != 0 && board[pt.x][pt.y] != p->getId()){
		return true;
	}else{
		return false;
	}
}

bool Board::isFreePointInBoard(Point pt){
	if (board[pt.x][pt.y] == 0){
		return true;
	}else{
		return false;
	}
}

vector<string> Board::getJumpPoints(vector<string> args, Player *p){
	Point pt(getConvertedCoup(args[0]));
	vector <string> targets;
	Point dir(pt.x - p->getPoint().x, pt.y - p->getPoint().y);
	vector<string> charlist{"A", "B", "C", "D", "E", "F", "G", "H", "I"};
	for (auto &shift: vector<Point>({{-1, 0},{ 0, 1},{ 1, 0},{ 0, -1}}))
		if (shift != Point(-dir.x, -dir.y)){ 			// on ne peut pas ressauter en arrière
			Point shiftedPoint(pt.x + shift.x, pt.y + shift.y);
			if (shift == dir){ 								// saut direct derrière dans la même direction
				if (isPointInBoard(shiftedPoint) && !isWallBetween(pt, shiftedPoint) && isFreePointInBoard(shiftedPoint)){
					targets.clear();
					targets.push_back(charlist[shiftedPoint.x]+ to_string(shiftedPoint.y + 1));
					return targets;
				}
			}else if (isPointInBoard(shiftedPoint) && !isWallBetween(pt, shiftedPoint) && isFreePointInBoard(shiftedPoint)){
				targets.push_back(charlist[shiftedPoint.x] + to_string(shiftedPoint.y + 1));
			}
		}	
	return targets;
}

//Loading game
void Board::loadWallsFromPartySaved(vector<Wall> wallsaved){
    wallList.clear();
    if(wallsaved.size() > 0){
        for(auto data : wallsaved){
            wallList.push_back(data);
        }
    }
}
string Board::serializeBoard(){
    Database db;
    string board_serialized = "";
    for(int i = 0; i < taillePlateau; i++){
        for(int ii = 0; ii < taillePlateau; ii++){
            board_serialized += to_string(board[i][ii]);
        }
        if(i != 8)board_serialized += ";";
    }
    return board_serialized;
}

void Board::deserializeBoard(string board_ser) {
    Database db;

    stringstream ss(board_ser);
    string item;

    int counter = 0;
    while (getline(ss, item, ';')) {
        //test.push_back(item);
        for (int i = 0; i < taillePlateau; i++) {
            board[counter][i] = (int) item[i] - 48;
            //cout << "item- " << item[i] << " - counter - " << counter << endl;
        }
        counter++;
    }
}

vector<string> Board::getUsersIndexes() {
	vector<string>indexes;
	vector<string> charlist{"A", "B", "C", "D", "E", "F", "G", "H", "I"};

	for (int y=0; y<taillePlateau; y++) {
		for (int x=0; x<taillePlateau; x++) {
			if (board[y][x] > 0) {
				string index= (charlist[y] + to_string(x+1).c_str()).c_str();
				//cout<<"index "<<index<<endl;
				indexes.push_back(index);
			}
		}
	}

	return indexes;
}

vector<Wall> Board::getWallList(){
	return wallList;
}

vector<vector<int>> Board::getBoard(){
	return board;
}

void Board::setBoard(vector<vector<int>> new_board){
	for(int x = 0; x < 9; x++){
		for(int y = 0; y < 9; y++){
			board[x][y] = new_board[x][y];
		}
	}
}

void Board::clearWallList(){
    wallList.clear();
}

void Board::setWallList(vector<Wall> lst){
    for(auto w : lst){
        wallList.push_back(w);
    }
}


void Board::rotateBoard(){
    //Rotation 90°
    vector<vector<int>> new_board;
    vector<Wall> temp_wall;

    for(int i = 0; i < 9; i++){
        vector<int> v;
        for(int ii = 0; ii < 9; ii++){
            v.push_back(0);
        }
        new_board.push_back(v);
    }

    for(int x = 0; x < 9; x++){
        for(int y = 0; y < 9; y++){
            new_board[y][8 - x] = getBoard()[x][y];
        }
    }

    for(auto w : getWallList()){
        Wall ww;

        Point p_temp = Point(w.pt_1.y, 8 - w.pt_1.x);
        ww.pt_1 = p_temp;

        p_temp.x = w.pt_2.y; p_temp.y = 8 - w.pt_2.x;
        ww.pt_2 = p_temp;

        p_temp.x = w.adj_1.y; p_temp.y = 8 - w.adj_1.x;
        ww.adj_1 = p_temp;

        p_temp.x = w.adj_2.y; p_temp.y = 8 - w.adj_2.x;
        ww.adj_2 = p_temp;

        temp_wall.push_back(ww);
    }

    clearWallList();

    for(auto w : getWallList()){
        Point p_temp = Point(w.pt_1.y, 8 - w.pt_1.x);
        w.pt_1 = p_temp;

        p_temp.x = w.pt_2.y; p_temp.y = 8 - w.pt_2.x;
        w.pt_2 = p_temp;

        p_temp.x = w.adj_1.y; p_temp.y = 8 - w.adj_1.x;
        w.adj_1 = p_temp;

        p_temp.x = w.adj_2.y; p_temp.y = 8 - w.adj_2.x;
        w.adj_2 = p_temp;
    }

    setWallList(temp_wall);

    setBoard(new_board);
}
