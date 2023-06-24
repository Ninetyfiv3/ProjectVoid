
#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H


#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "startMenu.h"
#include "gameEngine.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);

private:
    StartMenu *m_startMenu;
    GameEngine *m_gameEngine;
    QString weapon;
    QString archive = "none";

    void init_startMenu();
    void init_gameEngine();
    void start_gameEngine();
    void end_game();
};

#endif // GAMEWIDGET_H
