#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <QRectF>
#include <QPainter>
#include <qmath.h>
#include "gameoflifewidgets.h"

GameOfLifewidgets::GameOfLifewidgets(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    generations(-1),
    size(30)
{


    timer->setInterval(500);
    choseColor = "#35c921";
    //int x = 0;
    myField = new bool[(size + 2) * (size + 2)];
    myNextField = new bool[(size + 2) * (size + 2)];



    connect(timer, SIGNAL(timeout()), this, SLOT(newGeneration()));
    memset(myField, false, sizeof(bool)*(size + 2) * (size + 2));
    memset(myNextField, false, sizeof(bool)*(size + 2) * (size + 2));
}

GameOfLifewidgets::~GameOfLifewidgets(){
    delete [] myField;
    delete [] myNextField;
}

void GameOfLifewidgets::start(const int &number){
    generations = number;
    timer->start();
}

void GameOfLifewidgets::stop(){
    timer->stop();
}

void GameOfLifewidgets::clear(){

    for(int i = 1; i <= size; i++) {

        for(int j = 1; j <= size; j++) {

            myField[i*size + j] = false;
        }
    }

    gameOver(true);
    update();
}


int GameOfLifewidgets::numberOfCell(){
    return size;
}

void GameOfLifewidgets::setNumberOfCell(const int &s){
    size = s;
    reset();
    update();
}

void GameOfLifewidgets::reset(){
    delete [] myField;
    delete [] myNextField;
    myField = new bool[(size + 2) * (size + 2)];
    myNextField = new bool[(size + 2) * (size + 2)];
    memset(myField, false, sizeof(bool)*(size + 2) * (size + 2));
    memset(myNextField, false, sizeof(bool)*(size + 2) * (size + 2));
}

QString GameOfLifewidgets::toFile(){
    char temp;
    QString master = "";
    for(int i = 1; i <= size; i++) {
        for(int j = 1; j <= size; j++) {
            if(myField[i*size + j] == true) {
                temp = '*';
            } else {
                temp = 'o';
            }
            master.append(temp);
        }
        master.append("\n");
    }
    return master;
}

void GameOfLifewidgets::setToFile(const QString &data){
    int current = 0;
    for(int i = 1; i <= size; i++) {
        for(int j = 1; j <= size; j++) {
            myField[i*size + j] = data[current] == '*';
            current++;
        }
        current++;
    }
    update();
}

int GameOfLifewidgets::interval(){
    return timer->interval();
}

void GameOfLifewidgets::setInterval(int msec){
    timer->setInterval(msec);
}

bool GameOfLifewidgets::isAlive(int i, int j){
    int x = 0;

    x += myField[(i-1)*size + (j+1)];
    x += myField[(i-1)*size + (j-1)];
    x += myField[(i+1)*size +  (j+1)];
    x += myField[(i+1)*size +  j];
    x += myField[(i-1)*size + j];
    x += myField[i*size + (j+1)];
    x += myField[i*size + (j-1)];
    x += myField[(i+1)*size + ( j-1)];
    if (((myField[i*size + j] == true) && (x == 2)) || (x == 3))
           return true;
    return false;
}

void GameOfLifewidgets::newGeneration(){
    if(generations < 0)
        generations++;
    int notChanged=0;
    for(int i=1; i <= size; i++) {
        for(int j=1; j <= size; j++) {
            myNextField[i*size + j] = isAlive(i, j);
            if(myNextField[i*size + j] == myField[i*size + j])
                notChanged++;
        }
    }
    //int i =0;
    if(notChanged == size*size) {
        QMessageBox::information(this,tr("Koniec gry"),tr("Dalsze rozwijanie życia nie jest już możliwe"),QMessageBox::Ok);
        stop();
        gameOver(true);
        return;
    }

    for(int i=1; i <= size; i++) {
        for(int j=1; j <= size; j++) {

            myField[i*size + j] = myNextField[i*size + j];

        }

    }

    update();
    generations--;
    if(generations == 0) {
        stop();
        gameOver(true);

    }
}



void GameOfLifewidgets::mousePressEvent(QMouseEvent *e){
    emit changeField(true);
    double cellWidth = (double)width()/size;
    double cellHeight = (double)height()/size;
    int i = floor(e->y()/cellHeight)+1;
    int j = floor(e->x()/cellWidth)+1;
    myField[i*size + j] = !myField[i*size + j];
    update();
}

void GameOfLifewidgets::mouseMoveEvent(QMouseEvent *e){
    double cellWidth = (double)width()/size;
    double cellHeight = (double)height()/size;
    int i = floor(e->y()/cellHeight)+1;
    int j = floor(e->x()/cellWidth)+1;
    int currentLocation = i*size + j;
    if(!myField[currentLocation]){
        myField [currentLocation]= !myField[currentLocation];
        update();
    }
}

void GameOfLifewidgets::paintEvent(QPaintEvent *){
    QPainter p(this);
    grid(p);
    field(p);
}


void GameOfLifewidgets::grid(QPainter &p){
    QRect borders(0, 0, width()-1, height()-1);
    QColor gridColor = choseColor;
    gridColor.setAlpha(10);
    p.setPen(gridColor);

    double cellWidth = (double)width()/size;

    for(double i = cellWidth; i <= width(); i += cellWidth)
        p.drawLine(i, 0, i, height());

    double cellHeight = (double)height()/size;
    for(double i = cellHeight; i <= height(); i += cellHeight)
        p.drawLine(0, i, width(), i);
    p.drawRect(borders);
}

void GameOfLifewidgets::field(QPainter &p){
    double cellWidth = (double)width()/size;
    double cellHeight = (double)height()/size;
    for(int i=1; i <= size; i++) {
        for(int j=1; j <= size; j++) {
            if(myField[i*size + j] == true) {
                qreal left = (qreal)(cellWidth*j-cellWidth);
                qreal top  = (qreal)(cellHeight*i-cellHeight);
                QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight);
                p.fillRect(r, QBrush(choseColor));
            }
        }
    }
}

QColor GameOfLifewidgets::color(){
    return choseColor;
}

void GameOfLifewidgets::setColor(const QColor &color){
    choseColor = color;
    update();
}

//void GameOfLifewidgets::iterator(){
//    int i = 0;
//    i++;
//    qDebug(i);
//}
