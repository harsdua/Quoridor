#ifndef BUTTONWALL_H
#define BUTTONWALL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

#include "common.h"

class ButtonWall : public QPushButton
{
    Q_OBJECT
public:
    ButtonWall(QWidget *parent = nullptr, int i=0, int j=0, QString dir="H");
    ~ButtonWall();
    void setStateWall(bool b);
    bool getStateWall();
    bool isPressed();
    void setUnpressed();

private slots:
    void pushButton_clicked();

private:
    Point coord;
    string move, strDir;
    int posX, posY;
    QString direction;
    bool pressed = false;
    int numberWallSelected=0;

    bool isWall = false;

signals:
    void wallClicked(string move, string dir);
};

#endif // BUTTONWALL_H
