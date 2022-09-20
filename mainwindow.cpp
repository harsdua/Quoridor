#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setStyleSheet("background-color: rgb(42, 42, 42);");
    setWindowTitle("Quoridor");
    ui->tabWidget->setStyleSheet("background-color: rgb(63, 63, 63);");
    loginTab = new Login(this, pseudo);
    ui->tabWidget->addTab(loginTab, "Connexion");
    connect(loginTab, SIGNAL(succesLogin(string)), this, SLOT(setupTabs(string)));


}


MainWindow::~MainWindow()
{
    client_p->setConnection(false);
    if(client_p->isHost()){
        delete lobby_p;
        delete client_lobby;
    } else {
        //il n'était pas hôte mais il doit quitter le lobby, si il se trouvait dans un lobby
        client_p->database.removeInLobby(pp);
    }
    delete client_p;
    delete ui;
}

pthread_t MainWindow::getThreadClient(){
    return thread_client;
}

pthread_t MainWindow::getThreadLobby(){
    return thread_lobby;
}

void MainWindow::setupTabs(string pseudo){
    pp = pseudo;
    createThreadClient();
    if(!client_p->isConnected()){
        QMessageBox::information(this, "Infos", "Le MasterServeur n'est pas en ligne !");
        return;
    }
    updateWindowTitle(pseudo);
    ui->tabWidget->removeTab(0);
    // menu principal
    mainMenuTab = new MainMenu(this, pseudo);
    ui->tabWidget->addTab(mainMenuTab, "Menu principal");
    connect(mainMenuTab, SIGNAL(addLobbyToTab(string)), this, SLOT(addLobbyTab(string)));
    connect(mainMenuTab, SIGNAL(askLeaveSystem()), this, SLOT(returnLoginTab()));
    //profil
    profilTab = new Profil(this, pseudo);
    ui->tabWidget->addTab(profilTab, "Profil");
    connect(profilTab, SIGNAL(deleteAccountSignal()), this, SLOT(returnLoginTab()));
    connect(profilTab, SIGNAL(pseudoChanged(string)), this, SLOT(updateWindowTitle(string)));
    // message
    //chatMessageTab = new ChatMessage(this, pseudo);
    //ui->tabWidget->addTab(chatMessageTab, "Salon de discussion");
    //amis
    friendsTab = new FriendsMenu(this, pseudo, client_p);
    friendsTab->registerObserver(this);
    ui->tabWidget->addTab(friendsTab, "Amis");
    connect(friendsTab, SIGNAL(connectToLobby(string)), this, SLOT(connectToThreadLobby(string)));
}

void MainWindow::addLobbyTab(string nameLobby){
    //lobby
    createThreadLobby(nameLobby);
}

void MainWindow::showProfile(){
    ui->tabWidget->removeTab(2);
    ui->tabWidget->insertTab(1, profilTab, "Profil");
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::returnLoginTab(){
    client_p->setConnection(false);
    //pthread_cancel(thread_client);
    delete client_p;
    client_p = nullptr;
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    //ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(loginTab, "Connexion");
}

void MainWindow::updateWindowTitle(string pseudo){
    setWindowTitle("Quoridor - " + QString::fromStdString(pseudo));
    if(pseudo != getPseudo()) {
        client_p->changePseudo(pseudo);
        setPseudo(pseudo);
    }
}

string MainWindow::getPseudo(){
    return pp;
}

void MainWindow::createThreadClient(){
    if(!client_p){
        client_p = new Client(getPseudo(), 8080);
        client_p->registerObserver(this);
        pthread_create(&thread_client, NULL, handle_thread_client, (void *) client_p);
        //pthread_join(thread_client, NULL);
    }
}

void MainWindow::createThreadLobby(std::string nameOfLobby){
    if(!lobby_p){
        int lobbyport = client_p->setLobby(nameOfLobby);
        if (lobbyport != 0){
            lobby_p = new Lobby(lobbyport);
            pthread_create(&thread_lobby, NULL,handle_thread_lobby,(void *) lobby_p);
            connectToThreadLobby(nameOfLobby, true);
        }else{
            QMessageBox::information(this, "Infos", "Ce nom de lobby est déjà utilisé ! \nVeuillez en utiliser un autre.");
        }
    }
}

void MainWindow::connectToThreadLobby(string nameOfLobby, bool isHost){
    //connection au lobby
    client_p->connectClientToLobby(nameOfLobby);
    int lobbyport = client_p->getPortLobby();
    client_lobby = new Client(getPseudo(), lobbyport);
    pthread_create(&thread_client_lobby, NULL, handle_thread_client, (void*)client_lobby);
    client_lobby->connectClientToLobby(nameOfLobby);

    client_lobby->registerObserver(this);

    ui->tabWidget->removeTab(1);
    lobbyTab = new LobbyGame(this, pseudo, nameOfLobby, client_lobby, lobby_p, isHost);
    QString nameTab = "Lobby - " + QString::fromStdString(nameOfLobby);
    ui->tabWidget->addTab(lobbyTab, nameTab);
    connect(lobbyTab, SIGNAL(returnMainClient()), this, SLOT(showProfile()));
    ui->tabWidget->setCurrentIndex(2);
}
void* MainWindow::handle_thread_client(void* client){
    Client * currentClient = (Client*) client;
    while(currentClient->isConnected()){ }
    return nullptr;
}

void* MainWindow::handle_thread_lobby(void* lobby){
    Lobby * currentLobby = (Lobby*) lobby;
    while(currentLobby->isRunning()){
        currentLobby->listening();
    }
    return nullptr;
}

void MainWindow::subjectChat(string message){
     friendsTab->displayMessage(QString::fromStdString(message));
     profilTab->displayChatNotif();
}

void MainWindow::subjectNotifFR(string name){
    friendsTab->addRequestToList(name);
    profilTab->displayNewFRNotif();

}

void MainWindow::subjectAccept(string name){
    friendsTab->updateFriendsMenu();
    profilTab->displayAcceptedFRNotif(name);
}

void MainWindow::subjectDecline(string name){
    friendsTab->updateFriendsMenu();
    profilTab->displayDeclinedFRNotif(name);
}

void MainWindow::subjectAllChat(string message){
    lobbyTab->displayMessage(message);
}

void MainWindow::subjectMove(string board){
    //lobbyTab->displayBoard(board);
}

void MainWindow::subjectDeleteFriend(string name){
    friendsTab->updateFriendsMenu();
    profilTab->displayDeleteFriendNotif(name);
}

void MainWindow::subjectDisplayBoard(string board, string wall){
    lobbyTab->displayBoard(board, wall);
}

void MainWindow::subjectTurnPlayer(int playerId){

}

void MainWindow::subjectGameLaunch(){
    lobbyTab->initGame();
}

void MainWindow::setPseudo(string str){
    pp = str;
}
