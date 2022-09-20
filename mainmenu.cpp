#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent, string pseudonyme): QWidget(parent)
{
    title = new QLabel(this);
    title->setStyleSheet("background-color: rgb(63, 63, 63);");
    title->setText("<html><head/><body><p align=center><span style= font-size:20pt; text-decoration: underline;>Quoridor !</span></p></body></html>");
    title->setGeometry(20, 20, 1700, 100);

    ladderLabel = new QLabel(this);
    ladderLabel->setStyleSheet("background-color: rgb(63, 63, 63);");
    ladderLabel->setText("<html><head/><body><p><span style= font-size:12pt; font-weight:700;>Classement actuel :</span></p></body></html>");
    ladderLabel->setGeometry(20, 200, 800, 50);

    ladder = new QPlainTextEdit(this);
    ladder->setStyleSheet("background-color: rgb(84, 84, 84);");
    ladder->setGeometry(20, 260, 800, 650);
    ladder->setReadOnly(true);

    createLabel = new QLabel(this);
    createLabel->setStyleSheet("background-color: rgb(63, 63, 63);");
    createLabel->setText("<html><head/><body><p align=center><span style= font-size:12pt; font-weight:700;>Création d'une partie</span></p></body></html>");
    createLabel->setGeometry(1000, 250, 600, 50);

    lobbyName = new QLineEdit(this);
    lobbyName->setStyleSheet("background-color: rgb(84, 84, 84);");
    lobbyName->setGeometry(1000, 330, 600, 50);
    lobbyName->setPlaceholderText("Nom du lobby");
    connect(lobbyName, &QLineEdit::returnPressed, this, &MainMenu::startLobby);

    createLobby = new QPushButton(this);
    createLobby->setText("Démarrer le lobby");
    createLobby->setStyleSheet("background-color: rgb(255, 92, 28);");
    createLobby->setGeometry(1150, 400, 300, 50);
    connect(createLobby, SIGNAL(clicked(bool)), this, SLOT(startLobby()));

    /*
    loadingLabel = new QLabel(this);
    loadingLabel->setStyleSheet("background-color: rgb(63, 63, 63);");
    loadingLabel->setText("<html><head/><body><p align=center><span style= font-size:12pt; font-weight:700;>Chargement d'une partie</span></p></body></html>");
    loadingLabel->setGeometry(1000, 520, 600, 50);

    partyName = new QLineEdit(this);
    partyName->setStyleSheet("background-color: rgb(84, 84, 84);");
    partyName->setGeometry(1000, 570, 600, 50);
    partyName->setPlaceholderText("Nom de la partie à charger");

    loadParty = new QPushButton(this);
    loadParty->setText("Démarrer un lobby");
    loadParty->setStyleSheet("background-color: rgb(255, 92, 28);");
    loadParty->setGeometry(1150, 640, 300, 50);
    connect(loadParty, SIGNAL(clicked(bool)), this, SLOT(startLobby()));
    */

    refreshLadder = new QPushButton(this);
    refreshLadder->setText("Rafraichir le classement");
    refreshLadder->setStyleSheet("background-color: rgb(84, 84, 84);");
    refreshLadder->setGeometry(620, 200, 200, 50);
    connect(refreshLadder, SIGNAL(clicked(bool)), this, SLOT(updateRanking()));

    disconnectButton = new QPushButton(this);
    disconnectButton->setText("Se déconnecter");
    disconnectButton->setStyleSheet("background-color: rgb(255, 0, 0);");
    disconnectButton->setGeometry(1150, 800, 300, 50);
    connect(disconnectButton, SIGNAL(clicked(bool)), this, SLOT(leaveSystem()));

    displayLeaderBoard();
}

MainMenu::~MainMenu(){

}

void MainMenu::startLobby(){
    string nameLobby = lobbyName->text().toStdString();
    emit addLobbyToTab(nameLobby);
}

void MainMenu::displayLeaderBoard(){
    db.disconnect();
    db.connect();
    ladder->appendPlainText("PLACE    -    WINRATE    -    JOUEUR");
    int counter = 0;
    for(string a : db.getLeaderBoard()){
        double winrate = db.getWinrate(a);
        int winrate_ok = 0;
        if(winrate > 0) winrate_ok = floor(winrate*100);
        else winrate_ok = 0.0;
        counter++;
        string str = "N°" + to_string(counter) + "           |    " + to_string(winrate_ok) + "%    |    " + a;
        if(10 < counter < 100){
            str = "N°" + to_string(counter) + "          |    " + to_string(winrate_ok) + "%    |    " + a;
        }else{
            str = "N°" + to_string(counter) + "         |    " + to_string(winrate_ok) + "%    |    " + a;
        }
        ladder->appendPlainText(QString::fromStdString(str));
    }
}

void MainMenu::leaveSystem(){
    emit askLeaveSystem();
}

void MainMenu::updateRanking(){
    ladder->clear();
    displayLeaderBoard();
}

