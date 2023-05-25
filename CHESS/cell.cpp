#include "cell.h"

//Cell::Cell(QObject *parent)
  //  : QObject{parent}
//{

//}

Cell::Cell()
{
    side = -1;
    color = -1;
    pos_x = -1;
    pos_y = -1;
    chosen = false;
    this->isChess = true;
    jumped = false;
}

void Cell::setPos(int x, int y){
    pos_x = x;
    pos_y = y;
}

void Cell::setSide(int a){
    this->side = a;
}

void Cell::setColor(int a){

}

void Cell::fresh(int a){
    this->side = a;
}

void Cell::init(){
    if(isChess == false){
        this->hide();
    }
}

void Cell::choose(bool yn){
    this->chosen = yn;
}

void Cell::setJumped(bool yn){
    this->jumped = yn;
}

void Cell::mousePressEvent(QMouseEvent *e){
    emit sendPos(this->pos_x,this->pos_y,this->side);
    emit sendinfo(side,chosen,jumped);
}
