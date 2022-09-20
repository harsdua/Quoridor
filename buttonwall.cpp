#include "buttonwall.h"
#include <QDebug>

ButtonWall::ButtonWall(QWidget *parent, int i, int j, QString dir): QPushButton(parent), posX(i), posY(j), direction(dir)
{
    setStyleSheet("background-color: rgb(193, 187, 19);");
    if (direction == "H")
        setGeometry( posY*80+20*posY+20, posX*80+20*posX+20+80,80, 20);
    else
        setGeometry( posY*80+20*posY+20+80, posX*80+20*posX+20,20, 80);
    this->show();
    vector<char>charlist{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
    move = charlist[posX];
    move += to_string(posY + 1);
    strDir = dir.toStdString();

}

ButtonWall::~ButtonWall(){

}

void ButtonWall::pushButton_clicked()
{
    qDebug() << posX << posY << direction;
    if (numberWallSelected == 0)
        numberWallSelected++;

    if (!pressed)
        setStyleSheet("background-color: rgb(0, 0, 0);");

    else
        setStyleSheet("background-color: rgb(193, 187, 19);");
    pressed = !pressed;
    emit wallClicked(move, strDir);
}

void ButtonWall::setStateWall(bool b){
    isWall = b;
    this->setStyleSheet("background-color: rgb(90, 50, 50);");
    this->setDisabled(b);
    this->update();
}

bool ButtonWall::getStateWall(){

    return isWall;
}

bool ButtonWall::isPressed(){
    return pressed;
}
void ButtonWall::setUnpressed(){
    pressed = false;
}
