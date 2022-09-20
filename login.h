#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

using namespace std;

class Login : public QWidget
{
    Q_OBJECT
public:
    Login(QWidget *parent = nullptr, QString *pseudonyme = nullptr);
    ~Login();

private slots:

    void on_connectButton_clicked();

    void on_createAccountButton_clicked();

signals:

    void succesLogin(string pseudo);

private:
    QLineEdit *pseudoInput, *pseudoInputC, *passwordInput, *passwordInputC, *confirmPasswordInput;
    QPushButton *createAccountButton, *connectButton;
    QLabel *labelWelcome, *loginLabel, *createLabel;

    QString *pseudonyme;
};

#endif // LOGIN_H
