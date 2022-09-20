#include "buttonbox.h"

#include <QDebug>

ButtonBox::ButtonBox(QWidget *parent, int i, int j, QString player): QPushButton(parent), posX(i), posY(j), player(player), coord({i,j})
{
    if (player == "1")
        setStyleSheet("background-color: rgb(255, 0, 0);");
    else if(player == "2")
        setStyleSheet("background-color: rgb(0, 255, 0);");
    else if(player == "3")
        setStyleSheet("background-color: rgb(0, 0, 255);");
    else if(player == "4")
        setStyleSheet("background-color: rgb(140, 0, 255);");
    else
        setStyleSheet("background-color: rgb(132, 97, 8);");
    setGeometry( posY*80+20*posY+20, posX*80+20*posX+20,80, 80);
    this->show();
    vector<char>charlist{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
    move = charlist[posX];
    move += to_string(posY + 1);
}

ButtonBox::~ButtonBox(){

}

void ButtonBox::pushButton_clicked(){
    qDebug() << posX << posY;
    emit buttonPressed(move);
}

Point ButtonBox::getCoord(){
    return coord;
}
