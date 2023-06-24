
#ifndef BULLETENEMY_H
#define BULLETENEMY_H

#include "bulletBase.h"
#include "player.h"

class BulletEnemy : public BulletBase
{
    Q_OBJECT
public:
    explicit BulletEnemy(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration_, QGraphicsScene *scene, Player *pl, QObject *parent = nullptr);

private:
    void playerHitCheck();
    Player *m_player;
};

#endif // BULLETENEMY_H
