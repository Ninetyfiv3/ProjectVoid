
#ifndef EXP_H
#define EXP_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QObject>

#include "player.h"

class Exp : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Exp(Player *pl, QPointF pos, QGraphicsScene *sc, int xp);
    void checkPickup();

    static QTimer *m_timer;

private:
    Player *m_player;
    QGraphicsScene *m_scene;
    int m_xp;
};

#endif // EXP_H
