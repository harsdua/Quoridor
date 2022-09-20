#include "login.h"
#include "database.h"

Database db;

Login::Login(QWidget *parent, QString *pseudonyme)
    : QWidget(parent)
{

    pseudoInput = new QLineEdit(this);
    pseudoInput->setStyleSheet("background-color: rgb(84, 84, 84);");
    pseudoInput->setPlaceholderText("Pseudo");
    pseudoInput->setGeometry(1100, 400, 300, 50);
    connect(pseudoInput, &QLineEdit::returnPressed, this, &Login::on_connectButton_clicked);

    passwordInput = new QLineEdit(this);
    passwordInput->setStyleSheet("background-color: rgb(84, 84, 84);");
    passwordInput->setPlaceholderText("Mot de passe");
    passwordInput->setGeometry(1100, 500, 300, 50);
    passwordInput->setEchoMode(QLineEdit::Password);
    connect(passwordInput, &QLineEdit::returnPressed, this, &Login::on_connectButton_clicked);

    connectButton = new QPushButton(this);
    connectButton->setText("Se connecter");
    connectButton->setStyleSheet("background-color: rgb(255, 1, 81);");
    connectButton->setGeometry(1200, 700, 100, 50);
    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(on_connectButton_clicked()));


    pseudoInputC = new QLineEdit(this);
    pseudoInputC->setStyleSheet("background-color: rgb(84, 84, 84);");
    pseudoInputC->setPlaceholderText("Pseudo");
    pseudoInputC->setGeometry(300, 400, 300, 50);
    connect(pseudoInputC, &QLineEdit::returnPressed, this, &Login::on_createAccountButton_clicked);

    passwordInputC = new QLineEdit(this);
    passwordInputC->setStyleSheet("background-color: rgb(84, 84, 84);");
    passwordInputC->setPlaceholderText("Mot de passe");
    passwordInputC->setGeometry(300, 500, 300, 50);
    passwordInputC->setEchoMode(QLineEdit::Password);
    connect(passwordInputC, &QLineEdit::returnPressed, this, &Login::on_createAccountButton_clicked);

    confirmPasswordInput = new QLineEdit(this);
    confirmPasswordInput->setStyleSheet("background-color: rgb(84, 84, 84);");
    confirmPasswordInput->setPlaceholderText("Confirmation du mot de passe");
    confirmPasswordInput->setGeometry(300, 600, 300, 50);
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    connect(confirmPasswordInput, &QLineEdit::returnPressed, this, &Login::on_createAccountButton_clicked);

    createAccountButton = new QPushButton(this);
    createAccountButton->setText("Créer un compte");
    createAccountButton->setStyleSheet("background-color: rgb(255, 1, 81);");
    createAccountButton->setGeometry(375, 700, 150, 50);
    connect(createAccountButton, SIGNAL(clicked(bool)), this, SLOT(on_createAccountButton_clicked()));


    labelWelcome = new QLabel(this);
    labelWelcome->setText("<html><head/><body><p align=center><span style= font-size:20pt;>Bienvenue sur le système de jeu Quoridor !</p></body></html>");
    labelWelcome->setStyleSheet("background-color: rgb(63, 63, 63);");
    labelWelcome->setGeometry(20, 20, 1700, 100);

    loginLabel = new QLabel(this);
    loginLabel->setText("<html><head/><body><p align=center><span style= font-size:12pt;>Connexion à votre compte !</p></body></html>");
    loginLabel->setStyleSheet("background-color: rgb(63, 63, 63);");
    loginLabel->setGeometry(1100, 300, 300, 50);

    createLabel = new QLabel(this);
    createLabel->setText("<html><head/><body><p align=center><span style= font-size:12pt;>Créer un nouveau compte !</p></body></html>");
    createLabel->setStyleSheet("background-color: rgb(63, 63, 63);");
    createLabel->setGeometry(300, 300, 300, 50);
}

Login::~Login()
{

}


void Login::on_connectButton_clicked()
{
    QString pseudo, password;
    pseudo = pseudoInput->text();
    password = passwordInput->text();
    db.disconnect();
    db.connect();
    if(!db.isUserRegistered(pseudo.toStdString())){
        QMessageBox::information(this, "Infos", "Ce compte n'est pas enregisté !");
    } else if (db.isPasswordCorrect(pseudo.toStdString(), password.toStdString())){
        passwordInput->clear();
        pseudoInput->clear();
        emit succesLogin(pseudo.toStdString());
    }else{
        QMessageBox::information(this, "Infos", "Votre mot de passe est incorrect !");
    }
}


void Login::on_createAccountButton_clicked()
{
    db.disconnect();
    db.connect();
    QString pseudo, password, confirmPassword;
    pseudo = pseudoInputC->text();
    password = passwordInputC->text();
    confirmPassword = confirmPasswordInput->text();
    if(!db.isUserRegistered(pseudo.toStdString())){
        if(password != confirmPassword){
            QMessageBox::information(this, "Inscription", "Votre mot de passe de vérification n'est pas correct !");
        }else{
            if(password == ""){
                QMessageBox::information(this, "Inscription", "Veuillez entrer un mot de passe valide! !");
            } else{
                db.createUser(pseudo.toStdString(), password.toStdString());
                QMessageBox::information(this, "Inscription", "Votre compte a été créé avec succès !");
                confirmPasswordInput->clear();
                passwordInputC->clear();
                pseudoInputC->clear();
            }
        }
    }else{
        QMessageBox::information(this, "Inscription", "Ce compte existe déjà !");
    }
}


