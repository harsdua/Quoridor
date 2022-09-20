#include "lobbygame.h"

LobbyGame::LobbyGame(QWidget *parent, QString *pseudonyme, string nameLobby, Client * client, Lobby * lobby, bool isHost) :
    QWidget(parent), pseudonyme(pseudonyme), client(client), lobby(lobby), isHost(isHost)
{

    tchatGame = new QPlainTextEdit(this);
    tchatGame->setStyleSheet("background-color: rgb(134, 135, 127);");
    tchatGame->setGeometry(1000, 100, 700, 450);
    tchatGame->setReadOnly(true);

    inputMessage = new QLineEdit(this);
    inputMessage->setStyleSheet("background-color: rgb(134, 135, 127);");
    inputMessage->setGeometry(1000, 600, 700, 100);
    inputMessage->setPlaceholderText("Ecrivez votre message");
    connect(inputMessage, SIGNAL(returnPressed()), this, SLOT(writeMessage()));

    gamemodeBox = new QComboBox(this);
    gamemodeBox->setStyleSheet("background-color: rgb(84, 84, 84);");
    gamemodeBox->setGeometry(1000, 750, 300, 50);
    gamemodeBox->addItem("Normal");

    startGame = new QPushButton(this);
    startGame->setText("Démarrer la partie");
    startGame->setStyleSheet("background-color: rgb(84, 84, 84);");
    startGame->setGeometry(1000, 850, 300, 50);
    connect(startGame, SIGNAL(clicked(bool)), this, SLOT(startGamePressed()));
    startGame->setEnabled(isHost);

    rulesButton = new QPushButton(this);
    rulesButton->setText("Règles du jeu: ");
    rulesButton->setStyleSheet("background-color: rgb(84, 84, 84);");
    rulesButton->setGeometry(1000, 900, 700, 50);

    rulesWindow = new QMainWindow(this);
    connect(rulesButton, SIGNAL(clicked(bool)), this, SLOT(displayRules()));

    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            QString player;
            //vérifie si le pion est sur tel case pour envoyer la couleur du pion
            if (i == 0 && j == 4)
                player = "1";
            else if ( i == 8 && j == 4)
                player = "2";
            else
                player = "None";
            ButtonBox *button = new ButtonBox(this, i, j, player);
            connect(button, SIGNAL(clicked(bool)), button, SLOT(pushButton_clicked()));
            connect(button, SIGNAL(buttonPressed(string)), this, SLOT(sendMove(string)));

            buttonBoxList.append(button);
        }
    }
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            ButtonWall *button = new ButtonWall(this, i, j, "H");
            connect(button, SIGNAL(clicked(bool)), button, SLOT(pushButton_clicked()));
            connect(button, SIGNAL(wallClicked(string, string)), this, SLOT(sendWall(string, string)));
            buttonWallH.append(button);
        }
    }
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            ButtonWall *button = new ButtonWall(this, i, j, "V");
            connect(button, SIGNAL(clicked(bool)), button, SLOT(pushButton_clicked()));
            connect(button, SIGNAL(wallClicked(string, string)), this, SLOT(sendWall(string, string)));
            buttonWallV.append(button);
        }
    }
    setStateButton(false);
    border1 = new QPushButton(this);
    border1->setStyleSheet("background-color: rgb(0, 0, 0);");
    border1->setGeometry( 0,0,900, 20);
    border1->show();

    border2 = new QPushButton(this);
    border2->setStyleSheet("background-color: rgb(0, 0, 0);");
    border2->setGeometry( 0,900,920, 20);
    border2->show();
    QPushButton *border3 = new QPushButton(this);
    border3->setStyleSheet("background-color: rgb(0, 0, 0);");
    border3->setGeometry( 0,0,20, 900);
    border3->show();
    QPushButton *border4 = new QPushButton(this);
    border4->setStyleSheet("background-color: rgb(0, 0, 0);");
    border4->setGeometry( 900,0,20, 900);
    border4->show();

    saveGame = new QPushButton(this);
    saveGame->setText("Interrompre et sauvegarder la partie");
    saveGame->setStyleSheet("background-color: rgb(84, 84, 84);");
    saveGame->setGeometry(1000, 850, 300, 50);
    //connect(saveGame, SIGNAL(clicked(bool)), this, SLOT(on_createAccountButton_clicked()));
    saveGame->hide();

    surrendGame = new QPushButton(this);
    surrendGame->setText("Abandonner la partie");
    surrendGame->setStyleSheet("background-color: rgb(84, 84, 84);");
    surrendGame->setGeometry(1400, 850, 300, 50);
    connect(surrendGame, SIGNAL(clicked(bool)), this, SLOT(exitLobby()));
    surrendGame->hide();

    validMove = new QPushButton(this);
    validMove->setText("Valider le déplacement");
    validMove->setStyleSheet("background-color: rgb(84, 84, 84);");
    validMove->setGeometry(1000, 750, 300, 50);
    //connect(validMove, SIGNAL(clicked(bool)), this, SLOT(on_createAccountButton_clicked()));
    validMove->hide();

    validWall = new QPushButton(this);
    validWall->setText("Valider le mur");
    validWall->setStyleSheet("background-color: rgb(84, 84, 84);");
    validWall->setGeometry(1400, 750, 300, 50);
    //connect(validWall, SIGNAL(clicked(bool)), this, SLOT(on_createAccountButton_clicked()));
    validWall->hide();
}

LobbyGame::~LobbyGame(){

}

void LobbyGame::exitLobby(){
    if(!rulesWindow->isHidden()) rulesWindow->hide();
    emit returnMainClient();
}

void LobbyGame::startGamePressed(){
    string msg = "/launchGame";
    client->TrySendMessage(msg);
    qDebug()<<"post launchGame";
    int connectedPlayer = lobby->getNumberConnectedPlayer();
    qDebug()<<"start Game";
    if(connectedPlayer == 2 || connectedPlayer == 4){
        qDebug()<<"start Game";
        string start = "/startGame standard";
        client->TrySendMessage(start);
        qDebug()<<"start Game";

    }else{
        return;
    }


}

QPushButton* LobbyGame::getBorderHost(){
    return border2;
}

QPushButton* LobbyGame::getBorderGuest(){
    return border1;
}

void LobbyGame::writeMessage(){
    QString message, name;
    message = inputMessage->text();
    string msg = "/allChat \"" + message.toStdString()+ '"';
    client->TrySendMessage(msg);
    inputMessage->clear();
}

void LobbyGame::displayMessage(string message){
  tchatGame->appendPlainText(QString::fromStdString(message));
  tchatGame->update();

}

void LobbyGame::sendMove(string move){
    string moveToSend = "/move ";
    string jumpToSend = "/jump ";

    moveToSend += move;
    jumpToSend += move;

    qDebug()<<"move sent: " + QString::fromStdString(moveToSend);
    client->TrySendMessage(moveToSend);
    client->TrySendMessage(jumpToSend);

}


void LobbyGame::resetWallClicked(){
    for (auto h : buttonWallH){
        if(h->isPressed()){
            h->setChecked(false);
            h->setStyleSheet("background-color: rgb(193, 187, 19);");
            h->update();
            h->setUnpressed();
        }

    }
    for (auto v : buttonWallV){
        if(v->isPressed()){
            v->setChecked(false);
            v->setStyleSheet("background-color: rgb(193, 187, 19);");
            v->update();
            v->setUnpressed();
        }
    }
}

void LobbyGame::sendWall(string move, string dir){

    wallCount++;
    if(wallCount == 1){
        wallToSend += '"';
        wallToSend += move;
        wallToSend += " ";
        dirWall = dir;
    }
    if(wallCount == 2){

        resetWallClicked();
        string finalWall = "/move ";
        wallToSend += move;
        wallToSend += " +";
        wallToSend += '"';
        finalWall += wallToSend;
        qDebug()<< "move wall: " +QString::fromStdString(finalWall);
        if(dirWall == dir){
            client->TrySendMessage(finalWall);
        }

        wallCount = 0;
        wallToSend = "";
    }
}

void LobbyGame::displayBoard(string brd, string wall) {
    Board board;
    board.deserializeBoard(brd);
    vector<Wall> temp_wall;
    if(wall != "noWalls"){
         temp_wall= deserialiseWallString(wall);
         board.setNewWallList(temp_wall);

    }
    int x = 0;
    for(int i = 0; i < buttonBoxList.size(); i++){
        int y = i % 9;

        if(i != 0 && i % 9 == 0) x++;

        int idPlayer = board.getBoard()[x][y];
        if(idPlayer == 1){
            buttonBoxList[i]->setStyleSheet("background-color: rgb(255, 0, 0);");
        }else if(idPlayer == 2){
            buttonBoxList[i]->setStyleSheet("background-color: rgb(0, 255, 0);");
        }else if(idPlayer == 3){
            buttonBoxList[i]->setStyleSheet("background-color: rgb(0, 0, 255);");
        }else if(idPlayer == 4){
            buttonBoxList[i]->setStyleSheet("background-color: rgb(140, 0, 255);");
        }else{
            buttonBoxList[i]->setStyleSheet("background-color: rgb(132, 97, 8);");
        }
        if ( i < buttonBoxList.size()-1 && ( i == 0 || i % 9 != 0)){
            if(board.isWallBetween(buttonBoxList[i]->getCoord(), buttonBoxList[i+1]->getCoord())){
                buttonWallV[i]->setStateWall(true);
            }
        }
        if(i < 72){
            if(board.isWallBetween(buttonBoxList[i]->getCoord(), buttonBoxList[i+9]->getCoord())){
                buttonWallH[i]->setStateWall(true);
            }
        }
        buttonBoxList[i]->update();
    }
}

void LobbyGame::displayRules(){
    QPlainTextEdit *a = new QPlainTextEdit(this);
    a->setReadOnly(true);
    a->appendHtml("<html><head/><body><p align=center><span style= font-size:14pt;>Règles de base:</span></p></body></html>");
    //a->appendHtml("<html><head/><body><p><a href=https://www.gigamic.com/files/catalog/products/rules/quoridor-classic-fr.pdf%3E<span style= text-decoration: underline; color:#007af4;>Lien vers les règles de base</span></a></p></body></html>");
    a->appendHtml("https://www.gigamic.com/files/catalog/products/rules/quoridor-classic-fr.pdf%22");
    a->appendPlainText(" ");
    a->appendHtml("<html><head/><body><p align=center><span style= font-size:14pt;>Mode Contre-la-montre:</span></p></body></html>");
    a->appendPlainText("Disponible uniquement en mode deux joueurs. Les joueurs disposent chacun d'un timer de 10 minutes et la partie se termine soit si un gagnant est élu avant la fin des timers soit si un timer arrive à bout et dans ce cas le joueur qui n'a plus de temps perd la partie.");
    a->appendPlainText(" ");
    a->appendHtml("<html><head/><body><p align=center><span style= font-size:14pt;>Mode Attaque:</span></p></body></html>");
    a->appendPlainText("Disponible uniquement en mode deux joueurs. Lorsqu'un joueur passe par dessus l'autre joueur, il envoie ce dernier à sa case de départ ou à une autre case de départ si sa case est bloqué par les murs.");
    a->appendPlainText(" ");
    a->appendHtml("<html><head/><body><p align=center><span style= font-size:14pt;>Mode Pouvoirs:</span></p></body></html>");
    a->appendPlainText("Disponible uniquement en mode deux joueurs.");
    rulesWindow->setCentralWidget(a);
    rulesWindow->setWindowTitle("Règles du jeu");
    rulesWindow->setGeometry(500, 500, 700, 300);
    rulesWindow->show();
}

void LobbyGame::setStateButton(bool b){
    for(auto btn : buttonBoxList){
        btn->setDisabled(!b);
    }

    for(auto btn : buttonWallH){
        btn->setDisabled(!b);
    }

    for(auto btn : buttonWallV){
        btn->setDisabled(!b);
    }
}

void LobbyGame::initGame(){

    delete gamemodeBox;
    delete startGame;
    setStateButton(true);

    saveGame->show();
    surrendGame->show();
    validMove->show();
    validWall->show();

}




