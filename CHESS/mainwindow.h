#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QWidget>
#include <QtNetwork>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <vector>
#include <algorithm>
#include <QStringList>
#include <QEventLoop>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <stdlib.h>
#include <QPalette>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include<QEventLoop>
#include<QTimer>
#include "cell.h"
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void EnemyGiveUp();
    void init_1();
    void init_2();
    //void Play();
    void LegalMove();
    void PaintUpdate();
    void LegalJump(int,int);
    void JumpUpdate(int,int,int,int);
    void sendJump(int ,int);
    void JudgeWin();
    int noleaveCamp();
private:
    Ui::MainWindow *ui;
    QTcpServer *ListenSocket;
    QTcpSocket *ReadWriteSocket;
    QDialog *WaitingDialog;
    QDialog *newWaitingDialog;
    QLineEdit *edit;
    QString address;
    QDialog *ConnectingDialog;
    QLineEdit *line_1;
    QLineEdit *line_2;
    bool finished;
    bool myTurn;
    int LeaveMyCamp;
    int ArriveYourCamp;
    int GameRound;
    int OutofTime;
    int mySide;
    int TimeLimit = 20;
    Cell from;
    Cell dest;
    Cell *chess[17];
    Cell ChosenChess;
    vector<int> path;
    QTimer *timer;
public slots:
    void InitServer();
    void ConnectHost();
    void InitServer_2();
    void ConnectHost_2();
    void acceptConnection();
    void connected();
    void connectfailed();
    void givein();
    void recvmessage();
    void AskForStart();
    void Legal(int,int,int);
    //void JumpJudge(int,int,int);
    void TimeUpdate();
    //void test(int,int,int);
    //void test_1(int,bool,bool);
};
#endif // MAINWINDOW_H
