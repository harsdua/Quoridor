#include "database.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace uuid;

string Database::encryptPassword(string passwd){
	return "";
}

void Database::init(){
	createTableFriendRequest();
	createTableFriends();
	createTableUser();
	createTableSaveParty();
	//Afin de la remettre à 0, car pas de fonction TRUNCATE en sqlite3
	//deleteTableLobbyList();
	createTableLobbyList();
}

void Database::removeFriendRequest(string username, string username2){
    if(isFriendRequestExist(username, username2)){
        string req = "DELETE FROM FRIEND_REQUEST WHERE FROM_USERNAME_UUID = '" + getUUIDFromUsername(username) + "' AND TO_USERNAME_UUID = '" + getUUIDFromUsername(username2) + "';";
		if(!sendRequest(req)) cerr << "Impossible d'exécuter cette requête !" << endl;
	}
}

void Database::removeFriends(string username, string username2){
    if(isFriends(username, username2)){
        string req = "DELETE FROM FRIENDS_LIST WHERE FROM_USERNAME_UUID = '" + getUUIDFromUsername(username) + "' AND TO_USERNAME_UUID = '" + getUUIDFromUsername(username2) +
                    "' OR FROM_USERNAME_UUID = '" + getUUIDFromUsername(username2) + "' AND TO_USERNAME_UUID = '" + getUUIDFromUsername(username) + "';";
		if(!sendRequest(req)) cerr << "Impossible d'exécuter cette requête !" << endl;
	}
}

bool Database::isFriends(string username, string username2){
    for(auto a : getFriendsList(username)){
        if(a == getUUIDFromUsername(username2)) return true;
	}
	return false;
}

vector<string> Database::getFriendsList(string username){
	vector<string> data;
    string req = "SELECT TO_USERNAME_UUID FROM FRIENDS_LIST WHERE FROM_USERNAME_UUID = '" + getUUIDFromUsername(username) + "';";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
        data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
	}
	sqlite3_finalize(stmt);
	return data;
}

vector<string> Database::getFriendRequestsList(string username){
    vector<string> data;
    string req = "SELECT FROM_USERNAME_UUID FROM FRIEND_REQUEST WHERE TO_USERNAME_UUID = '" + getUUIDFromUsername(username) + "';";
    int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
    if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
    sqlite3_bind_int(stmt, 1, 1);
    while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
        data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
    }
    sqlite3_finalize(stmt);
    return data;
}

void Database::addFriends(string username, string username2){
    if(isFriendRequestExist(username, username2)
            && getStateFriendRequest(username, username2)
            && !isFriends(username, username2)){
        string req = "INSERT INTO FRIENDS_LIST (FROM_USERNAME_UUID, TO_USERNAME_UUID) VALUES ('" + getUUIDFromUsername(username) + "', '" + getUUIDFromUsername(username2) + "');";
        string req2 = "INSERT INTO FRIENDS_LIST (FROM_USERNAME_UUID, TO_USERNAME_UUID) VALUES ('" + getUUIDFromUsername(username2) + "', '" + getUUIDFromUsername(username) + "');";
		if(!sendRequest(req) || !sendRequest(req2)) cerr << "Impossible d'exécuter cette requête !" << endl;
	}
}

vector<string> Database::getUserData(string name){
	vector<string> data;
	if(isUserRegistered(name)){
		string req = "SELECT * FROM USER WHERE USERNAME = '" + name + "';";
		int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
		if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
		sqlite3_bind_int(stmt, 1, 1);
		while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
            data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
			data.push_back(to_string(sqlite3_column_int(stmt, 3)));
			data.push_back(to_string(sqlite3_column_int(stmt, 4)));
			data.push_back(to_string(sqlite3_column_double(stmt, 5)));
		}
		sqlite3_finalize(stmt);
	}
	return data;
}

bool Database::isUserRegistered(string username){
	string req = "SELECT USERNAME FROM USER WHERE USERNAME = '" + username + "';";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db); return false;}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		return true;
	}
	sqlite3_finalize(stmt);
	return false;
}

bool Database::isPasswordCorrect(string username, string passwd){
	string password = getUserData(username)[2];
	if(password == passwd) return true;
	return false;
}

bool Database::isFriendRequestExist(string username, string username2){
    string uuid_1 = getUUIDFromUsername(username);
    string uuid_2 = getUUIDFromUsername(username2);
    string req = "SELECT * FROM FRIEND_REQUEST WHERE FROM_USERNAME_UUID = '" + uuid_1 + "' and TO_USERNAME_UUID = '" + uuid_2 + "';";
    int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db); return false;}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
        return true;
	}
	sqlite3_finalize(stmt);
	return false;
}

bool Database::getStateFriendRequest(string username, string username2){
    string req = "SELECT * FROM FRIEND_REQUEST WHERE FROM_USERNAME_UUID = '" + getUUIDFromUsername(username) + "' and TO_USERNAME_UUID = '" + getUUIDFromUsername(username2) + "';";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db); return false;}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		return (bool)sqlite3_column_int(stmt, 2);
	}
	sqlite3_finalize(stmt);
	return false;
}

void Database::respToFriendRequestFrom(string username, string username2, bool b){
	string value_b = to_string((bool) b);
    string req = "UPDATE FRIEND_REQUEST SET IS_ACCEPTED = " + value_b + " WHERE FROM_USERNAME_UUID = '" + getUUIDFromUsername(username) + "' AND TO_USERNAME_UUID = '" + getUUIDFromUsername(username2) + "';";
	if(!sendRequest(req)) cerr << "Impossible de répondre à la demande d'ami !" << endl;
	if(b){ addFriends(username, username2); removeFriendRequest(username, username2); }
	else{ removeFriendRequest(username, username2); }
}

void Database::connect(){
	int responseConnection = sqlite3_open("database.db", &db);
	if(!responseConnection) isConnected = true;
}

void Database::disconnect(){
	sqlite3_close(db);
}

void Database::createFriendRequest(string username, string username2){
    string uuid_1 = getUUIDFromUsername(username);
    string uuid_2 = getUUIDFromUsername(username2);
    if(!isFriendRequestExist(username, username2)){
        string req = "INSERT INTO FRIEND_REQUEST (FROM_USERNAME_UUID, TO_USERNAME_UUID) VALUES ('" + uuid_1 + "', '"  + uuid_2 + "');";
		if(!sendRequest(req)) cerr << "Impossible d'envoyer la demande d'ami !" << endl;
	}else{
		cout << "Demande d'ami déjà envoyée !" << endl;
	}

}

void Database::createUser(string name, string password){
    string req = ("INSERT INTO USER (UUID, USERNAME, PASSWORD) VALUES('" + generate_uuid_v4() + "', '" + name + "', '" + password + "');");
	if(!sendRequest(req)) cerr << "Impossible de créer l'User." << endl;
}

bool Database::sendRequest(string str){
	char* msgError;
	int resp = sqlite3_exec(db, str.c_str(), NULL, NULL, &msgError);
	if(resp != SQLITE_OK) return false;
	else return true;
}

void Database::createTableUser(){
    string req = "CREATE TABLE IF NOT EXISTS USER( UUID TEXT PRIMARY KEY, "
					"USERNAME TEXT NOT NULL, "
					"PASSWORD TEXT NOT NULL, "
					"WIN INTEGER DEFAULT 0 NOT NULL, "
					"LOSE INTEGER DEFAULT 0 NOT NULL, "
                    "WINRATE DOUBLE DEFAULT 0 NOT NULL, "
                    "LOBBY_NAME TEXT);";
	if(!sendRequest(req)) cerr << "Impossible de créer la table." << endl;
	
}

void Database::createTableFriends(){
    string req = "CREATE TABLE IF NOT EXISTS FRIENDS_LIST (FROM_USERNAME_UUID TEXT, TO_USERNAME_UUID TEXT, PRIMARY KEY(FROM_USERNAME_UUID, TO_USERNAME_UUID));";
	if(!sendRequest(req)) cerr << "Impossible de créer la table." << endl;
}

void Database::createTableFriendRequest(){
    string req = "CREATE TABLE IF NOT EXISTS FRIEND_REQUEST(FROM_USERNAME_UUID TEXT ,TO_USERNAME_UUID TEXT ,IS_ACCEPTED INTEGER);";
	if(!sendRequest(req)) cerr << "Impossible de créer la table." << endl;
}

void Database::createTableSaveParty(){
    string req = "CREATE TABLE IF NOT EXISTS PARTY_SAVED(USERNAME_CREATOR_UUID TEXT, NAME_OF_PARTY TEXT, BOARD_SERIALIZED TEXT, WALL_SERIALIZED TEXT, GAMETYPE TEXT, LAST_PLAYED TEXT, PLAYERS_LIST TEXT, LIST_POINT_START TEXT, WALL_REMAINING TEXT, TIME_REMAINING TEXT);";
	if(!sendRequest(req)) cerr << "Impossible de créer la table." << endl;
}

void Database::createTableLobbyList(){
	string req = "CREATE TABLE IF NOT EXISTS LOBBY_LIST(NAME_OF_GAME TEXT, PORT_USED INTEGER, IS_PRIVATE INTEGER);";
	if(!sendRequest(req)) cerr << "Impossible de créer la table." << endl;
}

void Database::deleteTableLobbyList(){
	string req = "DROP TABLE IF EXISTS LOBBY_LIST;";
	if(!sendRequest(req)) cerr << "Impossible de supprimer la table." << endl;
}

void Database::addVictory(string username){
	string victory = to_string(getVictory(username) + 1);
	string req = "UPDATE USER SET WIN = " + victory + " WHERE USERNAME = '" + username + "';";
	if(!sendRequest(req)) cerr << "Impossible d'ajouter la victoire au joueur : " << username << endl;
	calculateWinrate(username);
}

void Database::addLose(string username){
	string lose = to_string(getLose(username) + 1);
	string req = "UPDATE USER SET LOSE = " + lose + " WHERE USERNAME = '" + username + "';";
	if(!sendRequest(req)) cerr << "Impossible d'ajouter une défaite au joueur : " << username << endl;
	calculateWinrate(username);
}

int Database::getVictory(string username){
	return stoi(getUserData(username)[3]);
}

int Database::getLose(string username){
	return stoi(getUserData(username)[4]);
}

double Database::getWinrate(string username){
	return stod(getUserData(username)[5]);
}

void Database::calculateWinrate(string username){
	double victory = getVictory(username);
	double lose = getLose(username);
	if(lose < 1) lose = 1;
	string winrate = to_string((double) victory / (victory + getLose(username)));
    replace(winrate.begin(), winrate.end(), ',', '.');
	string req = "UPDATE USER SET WINRATE = " + winrate + " WHERE USERNAME = '" + username + "';";
    cout << req << endl;
	if(!sendRequest(req)) cerr << "Impossible de calculer le winrate du joueur : " << username << endl;
}

vector<string> Database::getLeaderBoard(){
	vector<string> leaderboard;
	string req = "SELECT USERNAME FROM USER ORDER BY WINRATE DESC LIMIT 10";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		leaderboard.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
	}
	sqlite3_finalize(stmt);
	return leaderboard;
}

void Database::changeUsername(string old_username, string new_username){
	string req = "UPDATE USER SET USERNAME = '" + new_username + "' WHERE USERNAME = '" + old_username + "';";
	if(!sendRequest(req)) cerr << "Impossible de changer le username de : " << old_username << endl;
    disconnect();
    connect();
}

void Database::changePassword(string username, string newPassword){
	string req = "UPDATE USER SET PASSWORD = '" + newPassword + "' WHERE USERNAME = '" + username + "';";
	if(!sendRequest(req)) cerr << "Impossible de changer le mot de passe de : " << username << endl;
}

void Database::unregisterUser(string username){
	if(isUserRegistered(username)){
		string req = "DELETE FROM USER WHERE USERNAME = '" + username + "';";
		if(!sendRequest(req)) cerr << "Impossible de supprimer le user : " << username << endl;
	}
}

void Database::saveParty(string username, string name_of_party, string board_serialize, string wall_serialize, string gameType, string lastPlayed, string playerList, string listPointStart, string wallRemaining, string timeRemaining){
	if(!isPartyExistForUser(username, name_of_party)){
        string req = "INSERT INTO PARTY_SAVED (USERNAME_CREATOR_UUID, NAME_OF_PARTY, BOARD_SERIALIZED, WALL_SERIALIZED, GAMETYPE, LAST_PLAYED, PLAYERS_LIST, LIST_POINT_START, WALL_REMAINING, TIME_REMAINING) VALUES "
                "('" + getUUIDFromUsername(username) + "', '" + name_of_party + "', '" + board_serialize + "', '" + wall_serialize + "', '" + gameType + "', '" + lastPlayed + "', '" + playerList + "', '" + listPointStart + "', '" + wallRemaining + "', '" + timeRemaining + "');";
		cout << req << endl;
		if(!sendRequest(req)) cerr << "Impossible de sauvegarder la partie de : " << username << endl;
	}else{
		cerr << "La partie " << name_of_party << " existe déjà pour " << username << " !" << endl;
	}
}

bool Database::isPartyExistForUser(string username, string name_of_party){
    string req = "SELECT * FROM PARTY_SAVED WHERE USERNAME_CREATOR_UUID = '" + getUUIDFromUsername(username) + "' and NAME_OF_PARTY = '" + name_of_party + "';";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db); return false;}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		return true;
	}
	sqlite3_finalize(stmt);
	return false;
}

vector<string> Database::getSavedParty(string username, string name_of_party) {
	vector<string> data;
	if(isPartyExistForUser(username, name_of_party)){
        string req = "SELECT * FROM PARTY_SAVED WHERE USERNAME_CREATOR_UUID = '" + getUUIDFromUsername(username) + "' AND NAME_OF_PARTY = '" + name_of_party + "';";
		int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
		if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
		sqlite3_bind_int(stmt, 1, 1);
		while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8))));
			data.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9))));
		}
		sqlite3_finalize(stmt);
	}else{
		cerr << "Il n'existe pas de party : " << name_of_party << " pour le joueur : " << username << " !" << endl;
	}
	return data;
}

void Database::deleteSaveParty(string username, string name_of_party){
	if(isPartyExistForUser(username, name_of_party)){
        string req = "DELETE FROM USER WHERE USERNAME_CREATOR_UUID = '" + getUUIDFromUsername(username) + "' AND NAME_OF_PARTY = '" + name_of_party + "';";
		if(!sendRequest(req)) cerr << "Impossible de supprimer la game de l'utilisateur: " << username << endl;
	}
}

void Database::createLobbyInDB(string name_of_game, int port_used, bool isPrivate){
	if(!isLobbyNameInDB(name_of_game) && !isLobbyPortInDB(port_used)){
		string req = "INSERT INTO LOBBY_LIST(NAME_OF_GAME, PORT_USED, IS_PRIVATE) VALUES ('" + name_of_game + "', " + to_string(port_used) + ", " + to_string(int(isPrivate)) + ");";
		if(!sendRequest(req)) cerr << "Impossible d'ajouter cette partie dans la liste de lobby" << endl;
	}
}

void Database::removeLobbyInDB(string name_of_game){
	if(isLobbyNameInDB(name_of_game)){
        string req1 = "UPDATE USER SET LOBBY_NAME = NULL WHERE LOBBY_NAME = '" + name_of_game + "';";
        if(!sendRequest(req1)) cerr << "Impossible de supprimer la partie de la liste de lobby";
        string req2 = "DELETE FROM LOBBY_LIST WHERE NAME_OF_GAME = '" + name_of_game + "';";
        if(!sendRequest(req2)) cerr << "Impossible de supprimer la partie de la liste de lobby";
	}
}

string Database::getLobbyNameFromPlayer(string username){
    string lobbyName;
    string req = "SELECT LOBBY_NAME FROM USER WHERE USERNAME = '" + username + "';";
    int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
    if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
    sqlite3_bind_int(stmt, 1, 1);
    while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
        lobbyName = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);
    return lobbyName;
}

bool Database::isLobbyNameInDB(string name_of_game){
	string req = "SELECT * FROM LOBBY_LIST WHERE NAME_OF_GAME = '" + name_of_game + "';";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db); return false;}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		return true;
	}
	sqlite3_finalize(stmt);
	return false;
}

bool Database::isLobbyPortInDB(int port_used){
	string req = "SELECT * FROM LOBBY_LIST WHERE PORT_USED = " + to_string(port_used) + ";";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db); return false;}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		return true;
	}
	sqlite3_finalize(stmt);
	return false;
}

string Database::getNameLobbyFromPort(int port){
	string name = "";
	string req = "SELECT NAME_OF_GAME FROM LOBBY_LIST WHERE PORT_USED = " + to_string(port) + ";";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
	}
	sqlite3_finalize(stmt);
	return name;
}

int Database::getPortFromNameLobby(string nameLobby){
	int port_used;
	string req = "SELECT PORT_USED FROM LOBBY_LIST WHERE NAME_OF_GAME = '" + nameLobby + "';";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		port_used = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return port_used;
}

vector<string> Database::getListLobby(){
	vector<string> vec_name;
	//string req = "SELECT NAME_OF_GAME FROM LOBBY_LIST WHERE IS_PRIVATE = 0;"; -> si on fait système privé / public
	string req = "SELECT NAME_OF_GAME FROM LOBBY_LIST;";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		vec_name.push_back(string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
	}
	sqlite3_finalize(stmt);
	return vec_name;
}

bool Database::isInLobby(string username){
    string req = "SELECT LOBBY_NAME FROM USER WHERE USERNAME = '" + username + "';";
    int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
    if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db); return false;}
    sqlite3_bind_int(stmt, 1, 1);
    while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
        const char *column_text = (const char *)sqlite3_column_text(stmt, 0);
        if(column_text != NULL)return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

void Database::addInLobby(string username, string lobbyName){
    if(!isInLobby(username)){
        string req = "UPDATE USER SET LOBBY_NAME = '" + lobbyName + "' WHERE USERNAME = '" + username + "';";
        if(!sendRequest(req)) cerr << "Enregistrement de lobby dans la bdd impossible pour : " << username << endl;
    }
}

void Database::removeInLobby(string username){
    if(isInLobby(username)){
        string req = "UPDATE USER SET LOBBY_NAME = NULL WHERE USERNAME = '" + username + "';";
        if(!sendRequest(req)) cerr << "Suppression de lobby dans la bdd impossible pour : " << username << endl;
    }
}

void Database::deleteAccount(string username){
    string req = "DELETE FROM USER WHERE USERNAME = '" + username + "';";
    if(!sendRequest(req)) cerr << "Impossible de supprimer le compte : " + username;
}

string Database::getUUIDFromUsername(string username){
    disconnect();
    connect();
    string name = "";
    string req = "SELECT UUID FROM USER WHERE USERNAME = '" + username + "';";
    int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
    if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
    sqlite3_bind_int(stmt, 1, 1);
    while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
        name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);
    return name;
}

string Database::getUsernameFromUUID(string UUID){
    disconnect();
    connect();
    string name = "";
    string req = "SELECT USERNAME FROM USER WHERE UUID = '" + UUID + "';";
    int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
    if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
    sqlite3_bind_int(stmt, 1, 1);
    while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
        name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);
    return name;
}

int Database::getFreePort(){
	vector<int> portUsed;
	string req = "SELECT PORT_USED FROM LOBBY_LIST;";
	int respCode = sqlite3_prepare_v2(db, req.c_str(), -1, &stmt, NULL);
	if(respCode != SQLITE_OK){ cerr << "Erreur : " << sqlite3_errmsg(db);}
	sqlite3_bind_int(stmt, 1, 1);
	while((respCode = sqlite3_step(stmt)) == SQLITE_ROW){
		portUsed.push_back(sqlite3_column_int(stmt, 0));
	}
	sqlite3_finalize(stmt);

	if(portUsed.size() == 0){
        return 1025;
	}
	
	int biggerPort = portUsed[0];

	for(auto a : portUsed){
		if(a > biggerPort) biggerPort = a;
	}

	return biggerPort + 1;
}
