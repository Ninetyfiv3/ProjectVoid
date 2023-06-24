
#include "exp.h"

QTimer *Exp::m_timer = nullptr;

Exp::Exp(Player *pl, QPointF pos, QGraphicsScene *sc, int xp) : m_player(pl), m_scene(sc), m_xp(xp)
{
    setPos(pos);
    setPixmap(QPixmap(":/images/images/ic_energyunit.png"));
    setScale(3);


    m_scene->addItem(this);
    setParent(m_scene);
    connect(m_timer, &QTimer::timeout, this, &Exp::checkPickup);
}

void Exp::checkPickup()
{
    QLineF dist = QLineF(pos(), m_player->pos());
    if (dist.length() <= m_player->getPickupRange()){
        qreal radian = dist.angle() * M_PI / 180;
        qreal dx = cos(radian) * 5;
        qreal dy = -sin(radian) * 5;
        moveBy(dx, dy);
    }
    if (collidesWithItem(m_player)){
        m_player->pickupExp(m_xp);
        m_scene->removeItem(this);
        delete this;
    }
}

