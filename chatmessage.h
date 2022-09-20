#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>

using namespace std;

class ChatMessage : public QWidget
{
    Q_OBJECT
public:
    ChatMessage(QWidget *parent = nullptr, string pseudo = "");
    ~ChatMessage();
    void displayMessage(QString message, QString name);

private slots:

    void writeMessage();

private:
    QLineEdit *messageInput, *nameInput;
    QPlainTextEdit *messageOutput;
    QLabel *title;
};

#endif // CHATMESSAGE_H
