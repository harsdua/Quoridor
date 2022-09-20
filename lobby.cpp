#include "lobby.h"
#include "database.h"
#include "wall.h"

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <signal.h>

#include <vector>
#include <iostream>



using namespace std;

Lobby::Lobby(const int& port, bool inUI) : Server{inUI} {
    createLobby(port);

    if (isTerminal()) {
        while (isRunning()) {
            listening();
        }
    }
}
Lobby::~Lobby() {
    db.removeLobbyInDB(db.getNameLobbyFromPort(port));
}

bool Lobby::isInJump(){
    return inJump;
}

void Lobby::signal_handler(int sig) {
    //prévient les clients de la fermeture du lobby
    Message exit;
    time(&exit.timestamp);
    string strExit="/ENDLOBBY";
    exit.message = strdup(strExit.c_str());
    exit.size=strExit.size();
    for(int j=0; j<MAX_USERS; j++) {
        sendMessage(getClientSocket(j), exit);
    }
    
    free(exit.message);
    if(db.isLobbyNameInDB(db.getNameLobbyFromPort(getPort()))){ db.removeLobbyInDB(db.getNameLobbyFromPort(getPort())); }
    Server::signal_handler(sig);
    
}

void Lobby::newDisconnection() {
    if (getSenderIndex()==0) {
        signal_handler(SIGINT);
    }

    for (size_t i=0; i<players.size(); i++) {

        if (getSenderIndex()== (int) i) {
            for(size_t j=i; j<players.size()-1; j++) {
                players[i+1] = players[i];
            }
            players.pop_back();
            break;
        }
    }
    Server::newDisconnection();
}

void Lobby::newConnection(const int index, int new_socket) {
    if(getNumberConnectedPlayer() < 4){
        Server::newConnection(index, new_socket);
        if (!host_socket) {
            host_socket=&new_socket;
        }
    }
}

void Lobby::setUsername(const int index, const char* name) {
    Server::setUsername(index, name);

    if (players.size()<4) {
        string name(getUserName(index));
        cout<<"nom :"<<name<<endl;
        players.push_back(name);
    }
}

int Lobby::createLobby(int port) {
    setPort(port);
    Server::createSocket();
    return 1;
}

void Lobby::handleWin() {
    db.disconnect();
    db.connect();
    db.removeLobbyInDB(db.getNameLobbyFromPort(port));
    if(!isTerminal()){
        string strWincrash="Partie terminé veuillez relancer le serveur pour rejouer ";
        allchat(strWincrash);
    }
    Message winMessage;
    string nameWinner = game->getWinnerName();
                
    string strWin="Gagnant : ";
    strWin.append(nameWinner);

    winMessage.message=strdup(strWin.c_str());
    winMessage.size=strWin.size();

    for (size_t j=0; j<MAX_USERS;j++) {
        sendMessage(getClientSocket(j), winMessage);
    }
    free(winMessage.message);

    db.addVictory(nameWinner);
    
    //For pour les loses
    for(auto p : players){
        if(p != nameWinner) db.addLose(p);
    }

    delete game;
    game=nullptr;
    index_waiting=0;

    if(isTerminal()){
        sleep(8);
    }
    signal_handler(SIGINT);
}

void Lobby::playGame(const string move) {
    played=false;

    //jouer le coup

    cout << getUserName(index_waiting) << " essaye " << move << endl;
        
    int move_return = game->play(move);

    if(move_return == 1) { // coup a été joué
        if(isTerminal()){
            cout << getUserName(index_waiting) << " joue " << move <<endl;
            played=true;
            index_waiting=(index_waiting+1)%game->getNumPlayers();
            getGame()->rotateBoard();
            display(index_waiting);
        }else{
            display(index_waiting);
            cout << getUserName(index_waiting) << " joue " << move <<endl;
            played=true;
            index_waiting=(index_waiting+1)%game->getNumPlayers();
            getGame()->rotateBoard();
            display(index_waiting);
        }

                
    } else if (move_return==2) { // si la partie attend un saut
		inJump = true;
        string strJump="Vous passez par-dessus un joueur, choisissez entre: ";
		for (size_t i = 0 ; i < game->getJumpCoords().size() ; i++){
			strJump.append(" ");
            strJump.append(game->getJumpCoords()[i]);
			}   
        strJump += "\nPour cela tapez /jump {indice(s)}";
        askMove(strJump);
    }

    if (!played && move_return == 0) { //si le coup n'était pas valid et que le joueur ne tentais pas un saut
        askMove("Coup non valid, Réessayez. /move {indice(s)}");
    }
    if (game->isWon()) {
        handleWin();

    } else {
        if (played) {
            askMove();
        }
    }
}



void Lobby::askMove(string strMove) {
	strMove += " | Mur(s) restant(s): " +  to_string(game->getActualPlayerWalls());
	if (game->getGameMode() == timed){
		strMove += "\nTemps restant: " + game->getActualPlayerTime();
	}
    Message askMove;
    askMove.message = strdup(strMove.c_str());
    askMove.size=strMove.size();
    sendMessage(getClientSocket(index_waiting), askMove);

    free(askMove.message);
}
void Lobby::showHelp(){
    cout << "This is the lobby. You can start a game with /startGame gameMode" << endl;
    cout << "Only the host can start the game" << endl;
}

void Lobby::saveGame(const string game_name) {
    setToSend(true);

    if(getSocketIndex(getUserName(getSenderIndex()))==0) {//si c'est l'hôte
        if (game) { // si il y a une game en cours
            if (game->saveGame(getUserName(getSenderIndex()), game_name)) {
                cout<<"game sauvegardée"<<endl;
                delete game;
                game=nullptr;
                setToSend(false);
                signal_handler(SIGINT);

            } else {
                setStrToSend("La game n'a pas pu être sauvegardée. Essayez un autre nom.");

            }
        } else {
            setStrToSend("Pas de game en cours.");
        }
    } else {
        setStrToSend("Vous n'êtes pas l'hôte.");
    }
}

void Lobby::loadGame(const string game_name) {

    setToSend(true);

    if(getSocketIndex(getUserName(getSenderIndex()))==0) {//si c'est l'hôte
        if (!game) {
            if (db.isPartyExistForUser(getUserName(getSenderIndex()), game_name)) {
                vector<string> info_party = db.getSavedParty(getUserName(getSenderIndex()), game_name);
                    
                if (info_party[0]==db.getUUIDFromUsername(getUserName(getSenderIndex()))) {
                        
                    game = new Game(stringToGameMode(info_party[4]), players);
                    if(!game->loadGame(getUserName(getSenderIndex()), game_name)){
						setStrToSend("Erreur dans la liste des joueurs");
						return;
					}

                    vector<string> pos=game->board.getUsersIndexes();

                    if ((pos.size()>0) && (pos.size()==players.size())) { // si nb positions == nb joueurs

                        game->startGame(pos);
                               
                    } else if ((pos.size()>0) && (pos.size()>players.size())) { //si + de postions que de joueurs
                        vector<string> newPos(pos.begin(), pos.begin()+players.size()-1 );
                        game->startGame(newPos);
                    }else {
                        game->startGame();
                        
                    }

                    display(-1);
                    setToSend(false);
                    askMove();
                    
                } else {
                    setStrToSend("Vous n'êtes pas l'auteur de cette game.");
                }
            } else {
                setStrToSend("La partie n'existe pas ou vous n'en êtes pas l'hôte.");
            }
        } else {
            setStrToSend("Partie déjà en cours.");
        }
    } else {
        setStrToSend("Vous n'êtes pas l'hôte.");
    }
}

void Lobby::tryStartGame(const string mode) {
    setToSend(true);
    setStrToSend("La partie n'a pas pu être créée");

    if (getSocketIndex(getUserName(getSenderIndex()))==0) {// si c'est l'hôte

        if (!game) {game = new Game(stringToGameMode(mode), players);}

        game->startGame();
        display(-1);
        setToSend(false);
        askMove();

    } else {
        setStrToSend("Vous n'êtes pas l'hôte.");
    }
}

void Lobby::tryMove(const string move) {
    setToSend(true);

    if (game) { // si un message est envoyé alors qu'il y a une game
		if (!inJump)
		{
			if (getSocketIndex(getUserName(getSenderIndex()))==index_waiting) { // si c'est au tour de l'envoyeur du message
				if (game->isInProgress()) {
					
					playGame(move);
					setToSend(false);
				} else {
					setStrToSend("La partie est en pause.");
				}
			} else {
				setStrToSend("Ce n'est pas votre tour.");
			}
		}else{
			setStrToSend("Le joueur actuel est en saut. Essayez /jump {indexe(s)}.");
		}
    } else {
        setStrToSend("Pas de partie en cours.");
    }
}

void Lobby::tryJump(const string jump) {
	setStrToSend("le saut n'est pas valid.");
    setToSend(true);

    if (game) { // si un message est envoyé alors qu'il y a une partie
		if (inJump){
			if (getSocketIndex(getUserName(getSenderIndex()))==index_waiting) {
				if (game->isInProgress()) {
					if(!game->playJump(jump)){
						setStrToSend("Votre saut n'est pas valide. Veuiller réessayer");
					}else{
                        display(-1);
                        game->rotateBoard();
						cout << getUserName(index_waiting) <<"saute et attérit sur " << jump << endl;
						played=true;
						inJump = false;
						index_waiting=(index_waiting+1)%game->getNumPlayers();
						setToSend(false);
                        askMove();
                        if (game->isWon()) {
                            handleWin();
                        } else {
                            if (played) {
                                askMove();
                            }
                        }
					}
				} else {
					setStrToSend("La partie est en pause.");
				}
			} else {
				setStrToSend("Ce n'est pas votre tour.");
			}
		}else{
			setStrToSend("Vous n'êtes pas en saut. Essayez /move {indexe(s)}.");
        }
        
    } else {
        setStrToSend("Pas de partie en cours.");
    }
}

void Lobby::tryExitLobby() {
    if (getSocketIndex(getUserName(getSenderIndex()))==0) {
        signal_handler(SIGINT); //fin du processus
    } else {
        setToSend(true);
        setStrToSend("Vous n'êtes pas l'hôte.");
    }
}

void Lobby::allchat(string messageTxt) {
    Message allchatMsg;
    string strAllChatMsg= "/allChat ";
    strAllChatMsg += "\"";
    strAllChatMsg.append(getUserName(getSenderIndex()));
    strAllChatMsg += ": ";
    strAllChatMsg.append(messageTxt);
    strAllChatMsg += "\"";

    allchatMsg.message=strdup(strAllChatMsg.c_str());
    allchatMsg.size=strAllChatMsg.size();
    for (size_t j=0; j<MAX_USERS;j++){
        sendMessage(getClientSocket(j), allchatMsg);
    }
    free(allchatMsg.message);
    setToSend(false);
    return;
}

void Lobby::tryCommand() {
    
    Server::tryCommand(); 
    
    if (getCommandsSize()>0) {
        string cmd=getCommandAt(0);
        //besoins de "" pour mettre des espaces dans le message
        if (cmd == "saveGame") { //"/saveParty {name}"
            if (getCommandsSize()>1) {
                saveGame(getCommandAt(1));
            } else {
                setStrToSend("Commande incorrect. Essayez /saveGame {nom}");
            }
        } else if (cmd == "abandon") { // "/abandon"
            if (getCommandsSize()>0) {
                abandon();
            } else {
                setStrToSend("Commande incorrect. Essayez /abandon");
        }
        }
        else if (cmd == "loadGame") { //"/loadParty {nom}"
            if (getCommandsSize()>1) {
                loadGame(getCommandAt(1));
            } else {
                setStrToSend("Commande incorrect. Essayez /loadGame {nom}");
            }
            
        }else if (cmd == "allChat") { //"/allchat {msg}"
            if (getCommandsSize()>1) {
                allchat(getCommandAt(1));
            } else {
                setStrToSend("Commande incorrect. Essayez /allchat \"msg\", utiliser des guillemets pour borner le message");
            }
        }else if (cmd == "launchGame") { //"/allchat {msg}"
            if (getCommandsSize()>0) {
                launchGame();
            } else {
                setStrToSend("Commande incorrect. Essayez /allchat \"msg\", utiliser des guillemets pour borner le message");
            }
        } else if (cmd == "startGame") { //"/startGame {mode_de_jeu}"
            if(getCommandsSize()>1) {
                tryStartGame(getCommandAt(1));   
            } else {
                setStrToSend("Commande incorrect. Essayez /startGame {mode_de_jeu}");
            } 
        } else if (cmd == "move") { //"/move {indexe(s)}"
            if (getCommandsSize()>1) {
                tryMove(getCommandAt(1));
            } else {
                setStrToSend("Commande incorrecte. Essayez /move {indexe(s)}");
            }
        } else if (cmd == "jump") { //"/move {indexe(s)}"
            if (getCommandsSize()>1) {
                tryJump(getCommandAt(1));
            } else {
                setStrToSend("Commande incorrecte. Essayez /jump {indexe(s)}");
            }
        } else if (cmd == "exitLobby") { // "/exitLobby"
            tryExitLobby();
            
        }
    }
}

void Lobby::launchGame(){
    Message launchMsg;
    string strlaunchMsg= "/launchGame";

    launchMsg.message=strdup(strlaunchMsg.c_str());
    launchMsg.size=strlaunchMsg.size();
    for (size_t j=0; j<MAX_USERS;j++){
        sendMessage(getClientSocket(j), launchMsg);
    }
    free(launchMsg.message);
    setToSend(false);
    return;
}

void Lobby::abandon() {
    setStrToSend("Vous avez abandonné la partie !");
    for (int i = 0; i < 5; ++i) {
        if(db.getLose(getUserName(getSenderIndex())) > 1) db.addLose(getUserName(getSenderIndex()));
    }

    if(game->getNumPlayers() == 2){
        for(Player* p : game->getPlayerList()) if(p->getName() != getUserName(getSenderIndex())){ db.addVictory(p->getName()); }
    }
    
    delete game;
    Message abandonMsg;
    string strAbandon= "";
    strAbandon.append(getUserName(getSenderIndex()));
    strAbandon.append(" a choisi d'abandonner la partie, il a eu une pénalité de 5 défaites\n"
                      "Les autres joueurs ne seront ni perdant, ni gagnant !\n"
                      "La partie & le lobby se fermeront dans 5 secondes...");
    //cout << " Partie abandonnée par: " << getUserName(getSenderIndex()) << endl;
    abandonMsg.message=strdup(strAbandon.c_str());
    abandonMsg.size=strAbandon.size();
    for (size_t j=0; j<MAX_USERS;j++) {
        sendMessage(getClientSocket(j), abandonMsg);
    }
    free(abandonMsg.message);

    
    sleep(5);
    signal_handler(SIGINT);
}

int Lobby::getNumberConnectedPlayer(){
    return players.size();
}

void Lobby::display(int i){
    if(isTerminal()){
        game->displayBoard();

    }else if(i == -1){

        string displayRequest = "/displayBoard ";
        Message allchatMsg;

        string serBoard = getGame()->board.serializeBoard();
        displayRequest += serBoard + " ";
        string serWall = serializeWallList(getGame()->board.getWallList());
        if (serWall.size() == 0){
            serWall = "noWalls";
        }
        displayRequest += serWall;


        allchatMsg.message=strdup(displayRequest.c_str());
        allchatMsg.size=displayRequest.size();

        for (size_t j=0; j<players.size();j++){

            sendMessage(getClientSocket(j), allchatMsg);

        }

        free(allchatMsg.message);

        setToSend(false);
    }else{
        string displayRequest = "/displayBoard ";
        Message allchatMsg;

        string serBoard = getGame()->board.serializeBoard();
        displayRequest += serBoard + " ";
        string serWall = serializeWallList(getGame()->board.getWallList());
        if (serWall.size() == 0){
            serWall = "noWalls";
        }
        displayRequest += serWall;


        allchatMsg.message=strdup(displayRequest.c_str());
        allchatMsg.size=displayRequest.size();

        sendMessage(getClientSocket(i), allchatMsg);

        free(allchatMsg.message);

        setToSend(false);
    }
}

Game* Lobby::getGame(){
    return game;
}
