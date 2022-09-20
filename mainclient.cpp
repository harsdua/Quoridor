#include "client.h"
#include "database.h"
#include "utils.h"

#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>


using namespace std;

void loadingScreen(){
    cout << "Chargement..." << endl;
    sleep(3);
    system("clear");

}

vector<string> splitString(string s){
    vector<string> v;
    string temp = "";
    for(size_t i=0;i<s.length();++i){
        if(s[i]==' '){
            v.push_back(temp);
            temp = "";
        }
        else{
            temp.push_back(s[i]);
        }
    }
    v.push_back(temp);
    return v;
}



string mainPrompt(Database& database) {
    cout << "Quoridor : terminal client" << endl;


    while (true) {
        loadingScreen();

        string userInput = "default";

        vector <string> vUserInput;
        cout << "Menu principal: " << endl;
        cout << "Tapez '/connect username password' si vous avez un compte." << endl;
        cout << "Sinon, tapez '/register username password password' pour créer un compte." << endl;



        getline(cin, userInput);
        vUserInput = splitString(userInput);

        if (vUserInput[0] == "/connect") {
            if (vUserInput.size()!=3) {cout << "Erreur : mauvaise syntaxe" << endl; continue;}
                string userName = vUserInput[1];
                string pass = vUserInput[2];
                if (database.isUserRegistered(userName) && database.isPasswordCorrect(userName, pass)) {
                    cout << "Connexion établie" << endl;
                    return userName;
                }

            cout << "Erreur : mot de passe ou pseudo invalide" << endl;
        }
        else if (vUserInput[0] == "/register") {
            if (vUserInput.size()!=4) {cout << "Erreur : mauvaise syntaxe" << endl; continue;}
            string userName = vUserInput[1];
            string pass = vUserInput[2];
            string pass2 = vUserInput[3];
            if (database.isUserRegistered(userName)) {
                cout << "Pseudo non disponible" << endl;
            }
            else if (pass == pass2) {
                database.createUser(userName, pass);
                cout << "Votre compte a été créé avec succès."
                     << endl
                     << "Vous pouvez maintenant vous connecter avec vos informations d'identification nouvellement enregistrées."
                     << endl;

            }
            else {
                cout << "Erreur : les deux mots de passe ne correspondent pas" << endl;
            }
        }
        else {
            cout << "Entrée invalide. Veuillez faire attention à la syntaxe. N'ajoutez pas d'espaces dans les arguments" << endl;
            continue;
        }

        cout << "Vous allez être redirigé vers le menu principal" << endl;

    }
}






void gestionDuCompte(Database& database, string username){

    while (true){

        loadingScreen();

        cout << "Responsable des comptes :" << endl;
        cout << endl;
        cout << "Pour changer votre mot de passe, tapez /changePass newPassword newPassword" << endl;
        cout << "Pour changer votre nom d'utilisateur, tapez /changeName newName newName" << endl;
        cout << "Pour supprimer votre compte, tapez /deleteAcc" << endl;
        cout << endl;
        cout << "Pour se connecter au masterServer afin de jouer à Quoridor, tapez /start" << endl;
        string userInput = "default";
        getline(cin , userInput);

        vector<string> vUserInput = splitString(userInput);
        if(vUserInput[0]=="/start"){
            return;
        }
        else if(vUserInput[0]=="/deleteAcc"){
            database.unregisterUser(username);
        }
        else if (vUserInput.size()!=3 ){
            cout << "Erreur : Entrée non valide. Veuillez faire attention à la syntaxe. N'ajoutez pas d'espaces à la fin ou dans les mots de passe." << endl;
            continue;
        }

        else if(vUserInput[0]=="/changeName"){
            if(vUserInput[1]==vUserInput[2]){
                database.changeUsername(username,vUserInput[1]); //fails here
                cout << "Changement de nom réussi" << endl;

            }
            else{
                cout<< "Erreur : Les deux noms saisis ne correspondent pas"<< endl;
            }
        }
        else if(vUserInput[0]=="/changePass"){
            string newPass = vUserInput[1];
            string newPass2 = vUserInput[2];
            if(newPass==newPass2){
                database.changePassword(username,newPass); //fails here
                cout << "Changement de mot de passe réussi"<< endl<<endl;
            }

            else{
                cout<< "Erreur : les deux mots de passe ne correspondent pas" <<endl;
            }
        }
        else{
            cout << "Erreur : entrée invalide" << endl;
        }

    }
}



int main(int argc, const char* argv[]){
    const char *UserNameConstChar = "defaultVal";
    int port = 8080;
    if ((argv[1]) && (argv[2]) && (atoi(argv[2]))) {
        UserNameConstChar = argv[1];
        port = atoi(argv[2]);
    }
    else{
        Database database;
        database.connect();
        string userName = mainPrompt(database);
        UserNameConstChar = userName.c_str();
        gestionDuCompte(database, userName);
    }
    loadingScreen();
        
    Client client(UserNameConstChar,port, false);

}

/*
 * TODO
 * remove database debugging couts
 * extras:
 * delete user db problem?
 * changing usernames/passwords dont work db problem?
 * prevent user from typing when loadingScreen
 */
