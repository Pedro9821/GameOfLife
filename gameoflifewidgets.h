#ifndef GAMEOFLIFEWIDGETS_H
#define GAMEOFLIFEWIDGETS_H

#include <QColor>
#include <QWidget>

class GameOfLifewidgets : public QWidget
{
    Q_OBJECT
public:
    explicit GameOfLifewidgets(QWidget *parent = 0);
    ~GameOfLifewidgets();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:
    void changeField(bool ok);
    void gameOver(bool ok);

public slots:
    void start(const int &number = -1);
    void stop();
    void clear();

    int numberOfCell();
    void setNumberOfCell(const int &s);

    int interval();
    void setInterval(int msec);

    QString toFile();
    void setToFile(const QString &data);

    QColor color();
    void setColor(const QColor &color);



    //void iterator();



private slots:
    void grid(QPainter &p);
    void field(QPainter &p);
    void newGeneration();


private:
    bool isAlive(int k, int j);
    void reset();
    QColor choseColor;
    QTimer* timer;
    int generations;
    bool* myField;
    bool* myNextField;
    int size;

    //int r;
};
#endif // GAMEOFLIFEWIDGETS_H
