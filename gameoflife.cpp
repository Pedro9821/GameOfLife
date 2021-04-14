#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#include <QColor>
#include <QColorDialog>
#include <QTimer>

#include "gameoflife.h"
#include "gameoflifewidgets.h"

#include "ui_gameoflife.h"

GameOfLife::GameOfLife(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameOfLife),
    currentColor(QColor(Qt::green)),
    game(new GameOfLifewidgets(this))
{
    ui->setupUi(this);
    //timer = new QTimer(this);
    //timer->start(600);



    connect(ui->startButton_5, SIGNAL(clicked()), game,SLOT(start()));
    connect(ui->stopButton_5, SIGNAL(clicked()), game,SLOT(stop()));
    connect(ui->clearButton_5, SIGNAL(clicked()), game,SLOT(clear()));

    connect(ui->iterInterval_5, SIGNAL(valueChanged(int)), game, SLOT(setInterval(int)));
    connect(ui->cellsControl_5, SIGNAL(valueChanged(int)), game, SLOT(setNumberOfCell(int)));
    connect(game,SIGNAL(changeField(bool)),ui->cellsControl_5,SLOT(setDisabled(bool)));
    connect(game,SIGNAL(gameEnds(bool)),ui->cellsControl_5,SLOT(setEnabled(bool)));
    //connect(timer, SIGNAL(timeout()),this,SLOT(iterator()));


    connect(ui->saveButton_5, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->loadButton_5, SIGNAL(clicked()), this, SLOT(load()));

    //connect(ui->howManySteps, SIGNAL(valueChanged(int)), game, SLOT(iteration()));

    ui->mainLayout_5->setStretchFactor(ui->gameLayout_5, 5);
    ui->mainLayout_5->setStretchFactor(ui->setLayout_5, 5);
    ui->gameLayout_5->addWidget(game);
}



GameOfLife::~GameOfLife(){
    delete ui;
}




void GameOfLife::save(){
    QString filename = QFileDialog::getSaveFileName(this,tr("Zapisz dane gry"),QDir::homePath(),tr("Gra w życie *.zycie Files (*.zycie)"));
    if(filename.length() < 1)
        return;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QString s = QString::number(game->numberOfCell())+"\n";
    file.write(s.toUtf8());
    file.write(game->toFile().toUtf8());

    file.close();
}

void GameOfLife::load(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Wczytaj zapisany plik"),QDir::homePath(),tr("Gra w życie (*.zycie)"));
    if(filename.length() < 1)
        return;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);

    int sv;
    in >> sv;
    ui->cellsControl_5->setValue(sv);

    game->setNumberOfCell(sv);
    QString dump="";
    for(int i=0; i != sv; i++) {
        QString t;
        in >> t;
        dump.append(t+"\n");
    }
    game->setToFile(dump);

}
