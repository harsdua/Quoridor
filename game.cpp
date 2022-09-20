/**
 * @file game.cpp
 * @author Edem Lawson & Miguel Lozano
 * @brief Fichier header de la classe Game
 */

#include "game.h"
#include "database.h"

#include <iostream>
#include <vector>


using namespace std;

// Constructeur

Game::Game(GameMode setMode, vector<string> names) : mode{setMode} {

		if (mode==standard) {
			if(names.size()==4) {mode=four;}
		}

		for(const auto& name : names){
			players.push_back(new Player(name));
		}
		board.setGameMode(mode);
}


// Getters

int Game::getNumPlayers(){return id-1;}

bool Game::isWon(){return winner;}

bool Game::isInProgress(){return playing;}

Player* Game::getNextPlayer(){
	Player * nextPlayer;
	if((int)players.size() >= actualPlayer->getId() + 1){ nextPlayer = getPlayerById(actualPlayer->getId()+1); }
	else{ nextPlayer = getPlayerById(1); }
	return nextPlayer;
}

Player* Game::getPlayerById(int id){
	for(auto p : players){
		if(p->getId() == id){
			return p;
		}
	}
	return nullptr;
}

Player* Game::getPlayerByName(string name){
	for(auto p : players){
		if(p->getName() == name){
			return p;
		}
	}
	return nullptr;
}



string Game::getWinnerName(){
	return winner->getName();
}

int Game::getActualPlayerWalls(){return actualPlayer->getRemainingWall();}

string Game::getActualPlayerTime(){return actualPlayer->getRemainingTime();}

vector<string> Game::getJumpCoords(){return CoordForJump;}

GameMode Game::getGameMode(){return mode;}


//Setters

void Game::setPlayerId(Player* player, int newId){
	player->setId(newId);
	id++;
}


void Game::setAllPlayersInfo(){
	cout << "test rem" << endl;
	for(auto &player : players){
		setPlayerId(player, id);
		if (mode == four){
			player->setWallBank(5);
		}else{
			player->setWallBank(10);
		}
		if (mode == timed){
			//player->setTimer(5);
			player->loadTimer("2:23");
		}
	}
}

void Game::setAllStarting(){
	int pos = 0;
	for(auto &player : players){
		actualPlayer = player;
		vector<string> vec{starting[pos]};
		playOnBoard(vec, player);
        board.rotateBoard();


		player->setStart_coord(player->getPoint());
        board.rotateBoard();

		++pos;
	}
}

// Others

void Game::startGame(vector<string> newStarting) {
	if (newStarting.size()>0) {
		starting.clear();
		for(size_t i=0; i<newStarting.size(); i++) {
			string str=newStarting[i];
			starting.push_back(str);
		}
	}
	playing = true;
	//startGame();	
}

void Game::startGame() {
	initBoard(mode);
}

void Game::initBoard(GameMode mode){
	setAllPlayersInfo();
	setAllStarting();

	playing = true;
	if (mode == timed && !actualPlayer->isTimerCounting()){actualPlayer->startTimer();}
}

void Game::playOnBoard(vector< string>& args, Player *p){
	// Place le coup donné par le joueur [player_id] dans le plateau
	if(args.size() == 1){
        cout << p->getName() << " a joué " << args[0] << endl;
		Point pt = board.getConvertedCoup(args[0]);
		board.clearPawn(*p);
		board.play(pt.x, pt.y, p->getId());
		p->setCoord({pt.x, pt.y});
	}else{
		board.placeWall(createWall(board.getConvertedCoup(args[0]), board.getConvertedCoup(args[1]), args[2][0]));
		p->removeWall();
	}
	if (mode == timed && playing){
		if (actualPlayer->isTimerEmpty()){
			cout<< "Fallait réfléchir plus vite " << actualPlayer->getName() << ", tu as pardu la partie par manque de temps" << endl;
			winner = getNextPlayer();
		}
		else{
			actualPlayer->stopTimer();
			getNextPlayer()->startTimer();
		}
	}
	if(!winner && isWinner(p)){
		winner = actualPlayer;
	}else{
		actualPlayer = getNextPlayer();
    }
}

bool Game::isWinner(Player *p){
	if (playing)
	{
		if (p->isWinningCoord(p->getPoint())){
			return true;
		}
	}
	return false;
}


int Game::play(string coup){

	vector<string> args;

	stringstream ss(coup);
	string item;
	while (getline(ss, item, ' ')) {
		args.push_back(item);
	}
	if(args.size() == 3 && actualPlayer->getRemainingWall() == 0){
		return 0;	//Le joueur n'as plus de murs et veux en placer un 

	}else if(board.legalCoup(args, actualPlayer, players)){
		if (args.size() == 3 || !board.isOtherPlayerOnPoint(args, actualPlayer)){ // c'est un coup legal sur une case vide / un simple pose de mur
			playOnBoard(args, actualPlayer);
			return 1;
		}else{ // c'est un coup sur une case occupée / speciale pour les pouvoirs
			if (board.isOtherPlayerOnPoint(args, actualPlayer)){
				vector<string>sautsPossibles = board.getJumpPoints(args,actualPlayer);
				cout << sautsPossibles[0] << endl;
				if (sautsPossibles.size()>1){
					CoordForJump = sautsPossibles;
					return 2;
				}else if (sautsPossibles.size() == 1){
					cout << sautsPossibles[0] << endl;
					playOnBoard(sautsPossibles, actualPlayer);
					/*
					if (mode == attack){
						appelle la fonction du mode attack
					}
					*/
					return 1;
				}
			}else{
				/*
				else if (on est sur une case avec un pouvoir){
					*donner le pouvoir au joueur*
					playOnBoard(args, actual player);
					return 1;
				}else{
					return 0;
				}
				*/
				return 0;
			}
		}
	}
	return 0;	// Le coup n'est simplement pas legal (en dehors du plateau, coup impossible,..)
}

bool Game::playJump(string coup){
	vector<string> args;
	stringstream ss(coup);
	string item;
	while (getline(ss, item, ' ')) {
		args.push_back(item);
	}
	if(args.size() == 1){
		for(auto sautPossibles : CoordForJump){
			if (args[0] == sautPossibles){
				playOnBoard(args, actualPlayer);
				/*
				if (mode == attack){
					appelle la fonction du mode attack
				}
				*/
				return true;
			}
		}
		return false;
	}else{
		return false;
	}
}

void Game::displayBoard() {
	board.displayBoard();
}

string Game::getLastPlayerPlayed(){
	return actualPlayer->getName();
}

bool Game::loadGame(string username, string name_of_game){

    Database db;
	if(db.isPartyExistForUser(username, name_of_game)){
		vector<string> data = db.getSavedParty(username, name_of_game);
		vector<string> namePlayerSaved = deserializePlayerListInString(data[6]);
		vector<int> wallRemaining = deserializeWallRemaining(data[8]);
		if(!isSamePlayerNameLoaded(namePlayerSaved))return false;
		// data[2] -> board serialized data[3] -> wall serialized
		board.deserializeBoard(data[2]);
		board.loadWallsFromPartySaved(deserialiseWallString(data[3]));
		board.setGameMode(stringToGameMode(data[4]));
		
		if(board.getGameMode()==timed){ deserializeTimer(data[9]); }

		//réorganisation joueur id
		for(size_t i = 0; i < namePlayerSaved.size(); i++){
			getPlayerByName(namePlayerSaved[i])->setId(i+1);
			getPlayerByName(namePlayerSaved[i])->setWallBank(wallRemaining[i]);
			id++;
			cout << "Name - " << namePlayerSaved[i] << " - WallRem : " << getPlayerByName(namePlayerSaved[i])->getRemainingWall() << endl;
		}

		actualPlayer = getPlayerByName(data[5]);

		deserializeStartCoord(data[7]);
		readjustPlayerCoord();
		return true;
	}else{ 
		return false;
	}
}

bool Game::saveGame(string username, string name_of_game){
	Database db;

	if(!db.isPartyExistForUser(username, name_of_game)){
		string board_serialized = board.serializeBoard();
		string wallList_serialized = serializeWallList(board.getWallList());
		if(board.getGameMode() == timed){
			db.saveParty(username, name_of_game, board_serialized, wallList_serialized, gameModeToString(board.getGameMode()), getLastPlayerPlayed(), serializePlayerListToString(), serializeStartCoord(), serializeWallRemaining(), serializeTimer());
		}else{
			db.saveParty(username,name_of_game, board_serialized, wallList_serialized, gameModeToString(board.getGameMode()), getLastPlayerPlayed(), serializePlayerListToString(), serializeStartCoord(), serializeWallRemaining());
		}
		return true;
	}else{
		return false;
	}
}

string Game::serializeTimer(){
	string serialize = "";
	for(auto p : players){
		serialize += p->getName();
		serialize += "-";
		serialize += p->getRemainingTime();
		serialize += ";";
	}
	serialize.pop_back();
	return serialize;
}

void Game::deserializeTimer(string str){
	stringstream ss(str);
	string item;

	while (getline(ss, item, ';')) {
		//item -> chaque joueur avec son timer
		stringstream ss2(item);
		string item2;

		string username, timer;
		int counter = 0;
		while (getline(ss2, item2, '-')) {
			//item2 -> nom joueur puis timer
			if(counter==0) username = item2;
			else timer = item2; 
			counter++;
		}
		for(auto p : players){
			if(p->getName() == username){ p->loadTimer(timer); }
		}
	}
}

vector<string> Game::deserializePlayerListInString(string str){
	vector<string> players_string;
	stringstream ss(str);
	string item;

	while (getline(ss, item, ';')) {
		players_string.push_back(item);
	}

	return players_string;
}

string Game::serializePlayerListToString(){
	string serialize_player = "";
	for(auto p : players){
		serialize_player += p->getName();
		serialize_player += ";";
	}
	serialize_player.pop_back();
	return serialize_player;
}

bool Game::isSamePlayerNameLoaded(vector<string> vec){
	if (vec.size() != players.size()) return false;
	for(string playerLoaded : vec){
		bool isInGame = false;
		for(Player* playerInGame : players){
			if(playerInGame->getName() == playerLoaded){
				isInGame = true;
			}
		}
		if(!isInGame) return false;
	}
	return true;
}

string Game::serializeStartCoord(){
	string serialize = "";
	for(auto p : players){
		serialize += to_string(p->getStart_Point().x);
		serialize += ":";
		serialize += to_string(p->getStart_Point().y);
		serialize += ";";
	}
	serialize.pop_back();
	cout << "serialize : " << serialize << endl;
	return serialize;
}

void Game::deserializeStartCoord(string str){
	stringstream ss(str);
	string item;

	int counter_global = 0;
	while (getline(ss, item, ';')) {
		stringstream ss2(item);
		string item2;

		int counter = 0;
		Point temp;
		while (getline(ss2, item2, ':')) {
			if(counter == 0) temp.x = stoi(item2);
			else temp.y = stoi(item2);
			counter++;
		}

		players[counter_global]->setStart_coord(temp);
		counter_global++;
	}
}

void Game::readjustPlayerCoord(){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(board.getBoard()[i][j] != 0){
				getPlayerById(board.getBoard()[i][j])->setCoord({i, j});
			}
		}
	}
}

string Game::serializeWallRemaining(){
	string serialize = "";
	for(auto p : players){
		serialize += to_string(p->getRemainingWall());
		serialize += ":";
	}
	serialize.pop_back();
	return serialize;
}

vector<int> Game::deserializeWallRemaining(string str){
	vector<int> wallRem;
	stringstream ss(str);
	string item;

	while (getline(ss, item, ':')) {
		wallRem.push_back(stoi(item));
	}

	return wallRem;
}

vector<Player*> Game::getPlayerList(){
	return players;
}

/*
2 joueurs: joueur n°1 -> plateau normal puis pour le dernier joueur -> rotation de 180°
4 joueurs: joueur n°1 -> plateau normal puis pour chaque joueur -> rotation de 90°
*/
void Game::rotateBoard(){
    if(players.size() == 2){
        board.rotateBoard();
    }
    board.rotateBoard();
}
