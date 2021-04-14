#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <QMainWindow>
#include <QColor>
#include <QTimer>

#include "gameoflifewidgets.h"

namespace Ui {
class GameOfLife;
}

class GameOfLife : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameOfLife(QWidget *parent = 0);
    ~GameOfLife();

public slots:

    void save();
    void load();


private:
    Ui::GameOfLife *ui;
    QColor currentColor;
    GameOfLifewidgets* game;
    //QTimer *timer;

};
#endif // GAMEOFLIFE_H
