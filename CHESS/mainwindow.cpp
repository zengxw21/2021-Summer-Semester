#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->setInterval(1000);
    ui->lcdNumber->display(QString::number(20));
    ui->label->setText("round 0");
   // this->setStyleSheet("background-color:black");
    for(int i = 0 ; i <= 16 ; i += 2){
        this->chess[i] = new Cell[15];
        for(int j = 0 ; j <= 14 ; j ++ ){
            chess[i][j].setParent(this);
            chess[i][j].resize(30,30);
            chess[i][j].setStyleSheet(QString("QPushButton{border-radius:15px;background-color:grey;}"));
            chess[i][j].move(j*40+30,i*34.6+50);

        }
    }
    for(int i = 1; i <= 15; i+= 2 ){
        this->chess[i] = new Cell[14];
        for(int j = 0; j <= 13 ; j ++){
            chess[i][j].setParent(this);
            chess[i][j].resize(30,30);
            chess[i][j].setStyleSheet(QString("QPushButton{border-radius:15px;background-color:grey;}"));
            chess[i][j].move(j*40 +50,i*34.6 + 50);
        }
    }
    for(int i = 0;i<=14;i++){
        if(i!= 7){
            chess[0][i].isChess = false;
            chess[16][i].isChess = false;
        }
        if(i!=6&&i!=7&&i!=8){
            chess[2][i].isChess = false;
            chess[14][i].isChess = false;
        }
        if((i<1)||(i>13)){
            chess[4][i].isChess = false;
            chess[12][i].isChess = false;
        }
        if((i<2)||(i>12)){
            chess[6][i].isChess = false;
            chess[10][i].isChess = false;
        }
        if((i<3)||(i>11)){

            chess[8][i].isChess = false;
        }
    }
    for(int i = 0;i<=13;i++){
        if(i!=6 &&i !=7){
            chess[1][i].isChess = false;
            chess[15][i].isChess = false;
        }
        if((i<5)||(i>8)){
            chess[3][i].isChess = false;
            chess[13][i].isChess = false;
        }

        if((i<1)||(i>12)){
            chess[5][i].isChess = false;
            chess[11][i].isChess = false;
        }
        if((i<2)||(i>11)){
            chess[7][i].isChess = false;
            chess[9][i].isChess = false;
        }
    }
    for(int i = 0;i<=16;i++){
        for(int j =0 ; j <= 14-(i%2) ; j ++){
            if(chess[i][j].isChess == false){
                chess[i][j].hide();
            }
            else{
                chess[i][j].setPos(i,j);
                //connect(&chess[i][j],SIGNAL(sendPos(int,int,int)),this,SLOT(test(int,int,int)));
                //connect(&chess[i][j],SIGNAL(sendinfo(int,bool,bool)),this,SLOT(test_1(int,bool,bool)));
                connect(&chess[i][j],SIGNAL(sendPos(int,int,int)),this,SLOT(Legal(int,int,int)));
               // connect(&chess[i][j],SIGNAL(sendPos(int,int,int)),this,SLOT(JumpJudge(int,int,int)));
                chess[i][j].setSide(0);
            }
        }
    }
    QMenuBar *menu_bar = menuBar();
    QMenu *menu_0 = menu_bar->addMenu("connect");
    QMenu *menu_1 = menu_bar->addMenu("play");
    QAction *AdmitDefeat = menu_1->addAction("admit defeat");
    QAction *start = menu_1->addAction("start");
    QAction *CreateConnect = menu_0->addAction("create the connect");
    QAction *ConnectToServer = menu_0->addAction("connect to server");

    connect(timer,SIGNAL(timeout()),this,SLOT(TimeUpdate()));
    connect(CreateConnect,SIGNAL(triggered(bool)),this,SLOT(InitServer()));
    connect(ConnectToServer,SIGNAL(triggered(bool)),this,SLOT(ConnectHost()));
    connect(AdmitDefeat,SIGNAL(triggered(bool)),this,SLOT(givein()));
    connect(start,SIGNAL(triggered(bool)),this,SLOT(AskForStart()));
}





void MainWindow::TimeUpdate(){
    this->TimeLimit--;
    ui->lcdNumber->display(QString::number(TimeLimit));
    if(TimeLimit == 0){
        timer->stop();
        TimeLimit = 20;
        ui->lcdNumber->display(QString::number(TimeLimit));
        if(from.getPos_x() != -1){//选中了棋子
            chess[from.getPos_x()][from.getPos_y()].choose(false);
            chess[from.getPos_x()][from.getPos_y()].setSide(0);
            chess[from.getPos_x()][from.getPos_y()].setJumped(false);
            chess[from.getPos_x()][from.getPos_y()].setStyleSheet("QPushButton{border-radius:15px;background-color:grey;}");
            from.setPos(-1,-1);
            if(!path.empty()){
                int init_x = path[0];
                int init_y = path[1];
                if(this->mySide == 1){
                    chess[init_x][init_y].setStyleSheet("QPushButton{border-radius:15px;background-color:black;}");
                    chess[init_x][init_y].setSide(1);
                }
                if(this->mySide == 2){
                    chess[init_x][init_y].setStyleSheet("QPushButton{border-radius:15px;background-color:blue;}");
                    chess[init_x][init_y].setSide(2);
                }
                chess[init_x][init_y].setJumped(false);
                chess[init_x][init_y].choose(false);
                path.clear();
            }
        }
        OutofTime ++;
        GameRound ++;
        QString r = "round " + QString::number(GameRound);
        ui->label->setText(r);
        JudgeWin();
        ui->label_4->setText("your time out:" + QString::number(OutofTime));
        QByteArray *array = new QByteArray;
        array->clear();
        QString s = "o";
        array->append(s.toUtf8());
        ReadWriteSocket->write(array->data());
        myTurn = false;
        ui->label_2->clear();
    }
}

void MainWindow::InitServer(){
    WaitingDialog = new QDialog;
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    QPushButton *button = new QPushButton;
    edit = new QLineEdit;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        qDebug()<<ipAddressesList.at(i).toString();
         if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
             ipAddressesList.at(i).toIPv4Address()) {
             ipAddress = ipAddressesList.at(i).toString();
             break;
       }
    }
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    address = ipAddress;
    QString sentence_1;
    sentence_1.clear();
    sentence_1.append(tr("your ip address:"));
    sentence_1.append(address);
    label_1->setText(sentence_1);
    label_2->setText(tr("set your port:"));
    button->setText(tr("OK"));
    layout_1->addWidget(label_2);
    layout_1->addWidget(edit);
    layout->addWidget(label_1);
    layout->addLayout(layout_1);
    layout->addWidget(button);
    WaitingDialog->setLayout(layout);
    connect(button,SIGNAL(clicked(bool)),this,SLOT(InitServer_2()));
    this->ListenSocket =new QTcpServer;
    WaitingDialog->show();
    WaitingDialog->exec();
}

void MainWindow::InitServer_2(){
    WaitingDialog->close();
    newWaitingDialog = new QDialog;
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    QLabel *label_3 = new QLabel;
    QString sentence_1;
    QString sentence_2;
    sentence_1.append("your ip address is:");
    sentence_1.append(address);
    label_2->setText(sentence_1);
    sentence_2.append("your port is:");
    sentence_2.append(edit->text());
    label_3->setText(sentence_2);
    label_1->setText(tr("waiting for connection"));
    QVBoxLayout *layout_1 = new QVBoxLayout;
    layout_1->addWidget(label_2);
    layout_1->addWidget(label_3);
    layout_1->addWidget(label_1);
    newWaitingDialog->setLayout(layout_1);
    QString port = edit->text();
    int portInt = port.toInt();
    this->ListenSocket->listen(QHostAddress::Any,portInt);
    QObject::connect(this->ListenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    newWaitingDialog->show();
    newWaitingDialog->exec();
}


void MainWindow::ConnectHost(){
    ConnectingDialog = new QDialog;
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    line_1 = new QLineEdit;
    line_2 = new QLineEdit;
    QPushButton *button = new QPushButton;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QHBoxLayout *layout_2 = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    label_1->setText(tr("host ip address:"));
    label_2->setText(tr("host port:"));
    button->setText(tr("OK"));
    layout_1->addWidget(label_1);
    layout_1->addWidget(line_1);
    layout_2->addWidget(label_2);
    layout_2->addWidget(line_2);
    layout->addLayout(layout_1);
    layout->addLayout(layout_2);
    layout->addWidget(button);
    ConnectingDialog->setLayout(layout);
    this->ReadWriteSocket = new QTcpSocket;
    connect(button,SIGNAL(clicked(bool)),this,SLOT(ConnectHost_2()));
    ConnectingDialog->show();
    ConnectingDialog->exec();
}

void MainWindow::ConnectHost_2(){
    QString address = line_1->text();
    QString portstring = line_2->text();
    int port = portstring.toInt();
    this->ReadWriteSocket->connectToHost(QHostAddress(address),port);
    connect(ReadWriteSocket,SIGNAL(connected()),this,SLOT(connected()));
    if(!ReadWriteSocket->waitForConnected()){
        connectfailed();
    }
}

void MainWindow::connected(){
    finished = false;
    GameRound = 0;
    OutofTime = 0;
    ConnectingDialog->close();
    connect(this->ReadWriteSocket,SIGNAL(readyRead()),
            this,SLOT(recvmessage()));

}

void MainWindow::acceptConnection(){
    finished = false;
    GameRound = 0;
    OutofTime = 0;
    newWaitingDialog->close();
    this->ReadWriteSocket = this->ListenSocket->nextPendingConnection();
    connect(this->ReadWriteSocket,SIGNAL(readyRead()),
            this,SLOT(recvmessage()));
}
void MainWindow::connectfailed(){
    QMessageBox::about(NULL,tr("error"),tr("please check your ip and port"));
}

void MainWindow::givein(){
    if(GameRound < 20){
        QMessageBox::about(NULL,tr("forbidden"),tr("you can only give in "
                                                   "after 20 rounds"));
        return;
    }
    QMessageBox WhetherGiveUp(QMessageBox::NoIcon,tr("admit defeat"),
    tr("do you want to give in?"),QMessageBox::Yes | QMessageBox::No,NULL);
    if(WhetherGiveUp.exec() == QMessageBox::Yes){
        QByteArray *bytearray = new QByteArray;
        bytearray->clear();
        bytearray->append('x');
        ReadWriteSocket->write(bytearray->data());
        QMessageBox::about(NULL,tr("result"),tr("you lose"));
        finished = true;
    }
}

void MainWindow::AskForStart(){
    QMessageBox::about(NULL,tr("start"),tr("waiting for answer"));
    QByteArray *array = new QByteArray;
    array->clear();
    array->append('q');
    ReadWriteSocket->write(array->data());

}

void MainWindow::recvmessage(){
    QString info;
    info.clear();
    info += this->ReadWriteSocket->readAll();

    if(info[0] == 'x'){
        EnemyGiveUp();
    }
    if(info[0] == 'q'){
        QMessageBox AskStart(QMessageBox::NoIcon,tr("start"),
        tr("opposite wants to start"),QMessageBox::Yes | QMessageBox::No,NULL);
        if(AskStart.exec() == QMessageBox::Yes){
            QByteArray *array = new QByteArray;
            array->clear();
            array->append('y');
            ReadWriteSocket->write(array->data());
            QMessageBox::about(NULL,tr("start"),tr("game start"));
            init_1();
        }
    }
    if(info[0] == 'y'){
        QMessageBox::about(NULL,tr("start"),tr("game start"));
        init_2();
    }
    if(info[0] == 'm'){
        QStringList list = info.split(",");
        QString s1 = list[1];  int a = s1.toInt();
        QString s2 = list[2];  int b = s2.toInt();
        QString s3 = list[3];  int x = s3.toInt();
        QString s4 = list[4];  int y = s4.toInt();
        this->chess[a][b].setStyleSheet(QString("QPushButton{border-radius:15px;"
                                                "background-color:grey;}"));
        chess[a][b].setSide(0);
        if(this->mySide == 1){//my chess is yellow
            this->chess[x][y].setStyleSheet(QString("QPushButton{border-radius:15px;"
                                                    "background-color:blue;}"));
            chess[x][y].setSide(2);
        }
        if(this->mySide == 2){
            this->chess[x][y].setStyleSheet(QString("QPushButton{border-radius:15px;"
                                                    "background-color:black;}"));
            chess[x][y].setSide(1);
        }
        myTurn = true;
        QString name;
        if(mySide == 1){
            name = "black";
        }
        if(mySide == 2){
            name = "blue";
        }
        ui->label_2->setText("your turn:"+name);
        timer->start();
    }
    if(info[0] == 'j'){
        int num = 0;//comma
        QStringList list = info.split(",");
        for(int i = 0;i<info.length();i++){
            if(info[i] == ','){
                num ++;
            }
        }

        for(int i = 1; i < num ; i+=4){
            int index[4];
            for(int j = 0;j<4;j++){
                QString ss = list[i+j];
                index[j] = ss.toInt();
            }
            this->chess[index[0]][index[1]].setStyleSheet(QString("QPushButton{border-radius:15px;"
                                                    "background-color:grey;}"));
            chess[index[0]][index[1]].setSide(0);
            if(this->mySide == 1){//my chess is yellow
                this->chess[index[2]][index[3]].setStyleSheet(QString("QPushButton{border-radius:15px;"
                                                        "background-color:blue;}"));
                chess[index[2]][index[3]].setSide(2);
            }
            if(this->mySide == 2){
                this->chess[index[2]][index[3]].setStyleSheet(QString("QPushButton{border-radius:15px;"
                                                        "background-color:black;}"));
                chess[index[2]][index[3]].setSide(1);
            }
            QEventLoop loop;
            QTimer::singleShot(300, &loop, SLOT(quit()));
            loop.exec();
        }
        this->myTurn = true;
        QString name;
        if(mySide == 1){
            name = "black";
        }
        if(mySide == 2){
            name = "blue";
        }
        ui->label_2->setText("your turn:"+name);
        timer->start();
    }
    if(info[0] == 'w'){
        QMessageBox::about(NULL,tr("lose"),tr("you lose"));
        finished = true;
        timer->stop();
    }
    if(info[0] == 'l'){
        QMessageBox::about(NULL,tr("win"),tr("you win"));
        finished = true;
    }
    if(info[0] == 'o'){
        this->myTurn = true;
        QString name;
        if(mySide == 1){
            name = "black";
        }
        if(mySide == 2){
            name = "blue";
        }
        ui->label_2->setText("your turn:"+name);
        timer->start();
    }
}

void MainWindow::EnemyGiveUp(){
    QMessageBox::about(NULL,tr("win"),tr("enemy give up"));
    finished = true;
}

void MainWindow::init_1(){
    LeaveMyCamp = 0;
    ArriveYourCamp = 0;
    myTurn = true;
    timer->start();
    mySide = 1;
    QString name;
    if(mySide == 1){
        name = "black";
    }
    if(mySide == 2){
        name = "blue";
    }
    ui->label_2->setText("your turn:"+name);
    for(int i = 0;i<=3;i++){
        for(int j = 0 ;j<=14- (i%2);j++){
            if(this->chess[i][j].isChess == true){
                chess[i][j].setStyleSheet("border-radius:15px;background-color:black");
                chess[i][j].setSide(1);

            }
        }
    }
    for(int i = 13;i<=16;i++){
        for(int j = 0 ;j<=14- (i%2);j++){
            if(this->chess[i][j].isChess == true){
                chess[i][j].setStyleSheet("border-radius:15px;background-color:blue");
                chess[i][j].setSide(2);
            }
        }
    }

}

void MainWindow::init_2(){
    LeaveMyCamp = 0;
    ArriveYourCamp = 0;
    myTurn = false;
    mySide = 2;
    for(int i = 13;i<=16;i++){
        for(int j = 0 ;j<=14- (i%2);j++){
            if(this->chess[i][j].isChess == true){
                chess[i][j].setStyleSheet("border-radius:15px;background-color:blue");
                chess[i][j].setSide(2);

            }
        }
    }
    for(int i = 0;i<=3;i++){
        for(int j = 0 ;j<=14- (i%2);j++){
            if(this->chess[i][j].isChess == true){
                chess[i][j].setStyleSheet("border-radius:15px;background-color:black");
                chess[i][j].setSide(1);
            }
        }
    }
}

void MainWindow::Legal(int x, int y,int side_){
    if(myTurn){
        if(!finished){
            if(side_ == this->mySide){
                if(from.getPos_x() == -1){//若起点为空
                    this->from.setPos(x,y);
                    QString ss = "from:";
                    ss += QString::number(x);
                    ss += ",";
                    ss += QString::number(y);
                    ui->label_4->setText(ss);
                }
                if(from.getPos_x() != -1){
                    if(chess[from.getPos_x()][from.getPos_y()].ifJumped() == false){
                        this->from.setPos(x,y);//占位
                        QString ss = "from:";
                        ss += QString::number(x);
                        ss += ",";
                        ss += QString::number(y);
                        ui->label_4->setText(ss);
                    }
                }
                if(chess[x][y].ifJumped() == true){
                    ///////TODO
                    sendJump(x,y);
                }
            }
            if(side_ == 0){
                if(from.getPos_x() != -1){//非空
                    if(chess[from.getPos_x()][from.getPos_y()].ifJumped() == false){
                        this->dest.setPos(x,y);

                        LegalMove();
                    }
                    LegalJump(x,y);
                    //Legal(x,y,side_);
                }
            }
        }
    }
}



void MainWindow::sendJump(int x,int y){


    timer->stop();
    this->TimeLimit = 20;
    ui->lcdNumber->display(QString::number(TimeLimit));

    QString ss = "from:";
    ss += QString::number(-1);
    ss += ",";
    ss += QString::number(-1);
    ui->label_4->setText(ss);

    QByteArray *array = new QByteArray;
    array->clear();
    QString s = "j,";
    for(auto i = 0;i < (int)(this->path.size()-1);i++){
        s += QString::number(this->path[i]);
        s += ",";
    }
    s += QString::number(this->path[(int)path.size()-1]);
    int count = 0;
    for(int i = 0;i < s.length();i++){
        if(s[i] == ','){
            count++;
        }
    }
    if(count < 3){return;}//防止出错
    chess[x][y].choose(false);
    chess[x][y].setJumped(false);
    this->from.setPos(-1,-1);
    GameRound++;
    QString r = "round " + QString::number(GameRound);
    ui->label->setText(r);
    myTurn = false;
    ui->label_2->clear();
    path.clear();

    array->append(s.toUtf8());
    ReadWriteSocket->write(array->data());
    JudgeWin();
}

void MainWindow::LegalJump(int x,int y){
    int a = from.getPos_x();
    int b = from.getPos_y();
    bool isLegal = false;
    if(a == x){
        if(b - y == 2){
            if(chess[a][b-1].getSide()!=0){
                isLegal = true;
            }
        }
        if(y - b == 2){
            if(chess[a][b+1].getSide()!=0){
                isLegal = true;
            }
        }
    }
    if(a - x == 2){
        if(a%2 == 0){//偶数
            if(b - y == 1){
                if(chess[a-1][b-1].getSide()!=0){
                    isLegal = true;
                }
            }
            if(y - b == 1){
                if(chess[a-1][b].getSide()!=0){
                    isLegal = true;
                }
            }
        }
        if(a%2 == 1){//奇数
            if(b - y == 1){
                if(chess[a-1][b].getSide()!=0){
                    isLegal = true;
                }
            }
            if(y - b == 1){
                if(chess[a-1][b+1].getSide()!=0){
                    isLegal = true;
                }
            }
        }
    }
    if(x - a == 2){
        if(a%2 == 0){
            if(b - y == 1){
                if(chess[a+1][b-1].getSide()!=0){
                    isLegal = true;
                }
            }
            if(y - b == 1){
                if(chess[a+1][b].getSide()!=0){
                    isLegal = true;
                }
            }
        }
        if(a%2 == 1){
            if(b - y ==1){
                if(chess[a+1][b].getSide()!=0){
                    isLegal = true;
                }
            }
            if(y - b == 1){
                if(chess[a+1][b+1].getSide()!=0){
                    isLegal = true;
                }
            }
        }
    }
    if(isLegal){
        JumpUpdate(a,b,x,y);
    }
}

void MainWindow::JumpUpdate(int a, int b, int x, int y){
    path.push_back(a);
    path.push_back(b);
    path.push_back(x);
    path.push_back(y);
    chess[a][b].setStyleSheet(QString("QPushButton{border-radius:15px;background-color:grey;}"));
    chess[a][b].fresh(0);
    if(mySide == 1){
        chess[x][y].setStyleSheet("QPushButton{border-radius:15px;background-color:black;}");
    }
    if(mySide == 2){
        chess[x][y].setStyleSheet("QPushButton{border-radius:15px;background-color:blue;}");
    }
    chess[x][y].fresh(mySide);
    chess[a][b].choose(false);
    chess[x][y].choose(true);
    from.setPos(x,y);
    QString ss = "from:";
    ss += QString::number(x);
    ss += ",";
    ss += QString::number(y);
    ui->label_4->setText(ss);
    chess[x][y].setJumped(true);
    chess[a][b].setJumped(false);
}

void MainWindow::LegalMove(){
    //bool isLegal = false;
    int a = from.getPos_x();
    int b = from.getPos_y();
    int x = dest.getPos_x();
    int y = dest.getPos_y();
    if(this->from.getPos_x()%2 == 1){//奇数行
        if(from.getPos_x() == dest.getPos_x()){//同一行
            if(from.getPos_y() - dest.getPos_y() == 1 ||
               from.getPos_y() - dest.getPos_y() == -1){
                chess[a][b].fresh(0);
                chess[x][y].fresh(mySide);
                PaintUpdate();
            }
        }
        if(from.getPos_y() == dest.getPos_y()){//同一列
            if(from.getPos_x() - dest.getPos_x() == 1 ||
               from.getPos_x() - dest.getPos_x() == -1){
                chess[a][b].fresh(0);
                chess[x][y].fresh(mySide);
                PaintUpdate();
            }
        }
        if(dest.getPos_y() - from.getPos_y() == 1){
            if(from.getPos_x() - dest.getPos_x() == 1 ||
               from.getPos_x() - dest.getPos_x() == -1){
                chess[a][b].fresh(0);
                chess[x][y].fresh(mySide);
                PaintUpdate();
            }
        }
    }
    if(this->from.getPos_x()%2 == 0){//偶数行
        if(from.getPos_x() == dest.getPos_x()){//同一行
            if(from.getPos_y() - dest.getPos_y() == 1 ||
               from.getPos_y() - dest.getPos_y() == -1){
                chess[a][b].fresh(0);
                chess[x][y].fresh(mySide);
                PaintUpdate();
            }
        }
        if(dest.getPos_y() - from.getPos_y() == -1){//
            if(from.getPos_x() - dest.getPos_x() == 1 ||
               from.getPos_x() - dest.getPos_x() == -1){
                chess[a][b].fresh(0);
                chess[x][y].fresh(mySide);
                PaintUpdate();
            }
        }
        if(dest.getPos_y() == from.getPos_y() ){
            if(from.getPos_x() - dest.getPos_x() == 1 ||
               from.getPos_x() - dest.getPos_x() == -1){
                chess[a][b].fresh(0);
                chess[x][y].fresh(mySide);
                PaintUpdate();
            }
        }
    }
}

void MainWindow::PaintUpdate(){
    GameRound++;

    timer->stop();
    this->TimeLimit = 20;
    ui->lcdNumber->display(QString::number(TimeLimit));

    QString r = "round " + QString::number(GameRound);
    ui->label->setText(r);
    int a = this->from.getPos_x();
    int b = this->from.getPos_y();
    int x = this->dest.getPos_x();
    int y = this->dest.getPos_y();
    this->chess[a][b].setStyleSheet(QString("QPushButton{border-radius:15px;background-color:grey;}"));
    if(chess[x][y].getSide() == 1)
    this->chess[x][y].setStyleSheet("QPushButton{border-radius:15px;background-color:black;}");
    if(chess[x][y].getSide() == 2)
    this->chess[x][y].setStyleSheet("QPushButton{border-radius:15px;background-color:blue;}");
    this->from.setPos(-1,-1);
    QString ss = "from:";
    ss += QString::number(-1);
    ss += ",";
    ss += QString::number(-1);
    ui->label_4->setText(ss);
    this->dest.setPos(-1,-1);

    QByteArray *array = new QByteArray;
    array->clear();
    QString s = "m,";
    s += QString::number(a);
    s += ",";
    s += QString::number(b);
    s += ",";
    s += QString::number(x);
    s += ",";
    s += QString::number(y);

    array->append(s.toUtf8());
    ReadWriteSocket->write(array->data());
    JudgeWin();
    this->myTurn = false;
    ui->label_2->clear();
}

void MainWindow::JudgeWin(){
    if(GameRound>=30){
        int count = 0;
        if(this->mySide == 1){
            for(int i = 13;i <= 16; i ++){
                for(int j = 0 ;j<=14- (i%2);j++){
                    if(chess[i][j].isChess){
                        if(chess[i][j].getSide() == 1){
                            count++;
                        }
                    }
                }
            }
        }
        if(this->mySide == 2){
            for(int i = 0;i <= 3; i ++){
                for(int j = 0 ;j<=14- (i%2);j++){
                    if(chess[i][j].isChess){
                        if(chess[i][j].getSide() == 2){
                            count++;
                        }
                    }
                }
            }
        }
        if(count == 10){
            QMessageBox::about(NULL,tr("win"),tr("you win"));
            finished = true;
            QByteArray *array = new QByteArray;
            array->clear();
            array->append('w');
            ReadWriteSocket->write(array->data());
        }
    }
    bool failed = false;
    if(GameRound == 20){
        if(noleaveCamp() > 5){
            failed = true;
        }
    }
    if(GameRound == 25){
        if(noleaveCamp() > 2){
            failed = true;
        }
    }
    if(GameRound == 30){
        if(noleaveCamp() > 0){
            failed = true;
        }
    }
    if(OutofTime ==3 ){
        failed = true;
    }
    if(failed){
        QMessageBox::about(NULL,tr("lose"),tr("you lose"));
        finished = true;
        QByteArray *array = new QByteArray;
        array->clear();
        array->append('l');
        ReadWriteSocket->write(array->data());
    }
}

int MainWindow::noleaveCamp(){
    int count = 0;
    if(this->mySide == 1){
        for(int i = 0;i<=3;i++){
            for(int j = 0 ;j<=14- (i%2);j++){
                if(this->chess[i][j].isChess == true){
                    if(chess[i][j].getSide() == 1){
                        count ++;
                    }
                }
            }
        }
    }
    if(this->mySide == 2){
        for(int i = 13;i<=16;i++){
            for(int j = 0 ;j<=14- (i%2);j++){
                if(this->chess[i][j].isChess == true){
                    if(chess[i][j].getSide() == 2){
                        count ++;
                    }
                }
            }
        }
    }
    return count;
}

MainWindow::~MainWindow()
{
    delete ui;
}

