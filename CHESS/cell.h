#ifndef CELL_H
#define CELL_H
#include<QPushButton>
#include<QString>
#include<QPainter>
#include<QPoint>
#include<QPaintEvent>
#include<QDebug>
#include<QWidget>
#include <QObject>

class Cell : public QPushButton
{
    Q_OBJECT
public:
    //explicit Cell(QObject *parent = nullptr);
    Cell();
    int getSide() { return side; }
    int getColor() { return color; }
    void setSide(int);
    void setColor(int);
    int getPos_x(){ return pos_x; }
    int getPos_y(){ return pos_y; }
    void fresh(int);
    void init();
    void mousePressEvent(QMouseEvent *e);
    bool isChess;
    void setPos(int,int);
    void choose(bool);
    void setJumped(bool);
    bool ifChosen(){return chosen;}
    bool ifJumped(){return jumped;}
private:
    int side;  //0:no side   1:mine  2:yours
    int color; //0:no color  1:blue  2:red
    int pos_x;
    int pos_y;
    bool chosen;
    bool jumped;
signals:
    void sendPos(int,int,int);
    void sendinfo(int,bool,bool);
};

#endif // CELL_H
