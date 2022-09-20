#ifndef BUTTONBOX_H
#define BUTTONBOX_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

#include "common.h"

class ButtonBox : public QPushButton
{
    Q_OBJECT
public:
    ButtonBox(QWidget *parent = nullptr, int i=0, int j=0, QString player="None");
   ~ButtonBox();
    Point getCoord();
private slots:

    void pushButton_clicked();

private:
    string move;
    int posX, posY;
    QString player;
    bool pressed = false;

    Point coord;
signals:
    void buttonPressed(string move);
};

#endif // BUTTONBOX_H
