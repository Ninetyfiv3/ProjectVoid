
#include "bulletEnemy.h"

BulletEnemy::BulletEnemy(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration_, QGraphicsScene *scene, Player *pl, QObject *parent)
    : BulletBase{pos_, angle_, speed_, damage_ ,penetration_, scene, parent}
{
    setPixmap(QPixmap(":/images/images/enemy/en_bullet.png"));
    setTransformOriginPoint(boundingRect().center());
    m_player = pl;
    disconnect(m_timer_static, &QTimer::timeout, this, &BulletEnemy::enemyHitCheck);
    connect(m_timer_static, &QTimer::timeout, this, &BulletEnemy::playerHitCheck);
    setZValue(114);
}

void BulletEnemy::playerHitCheck()
{
    if (collidesWithItem(m_player)) {
        m_player->hurt();
        m_scene->removeItem(this);
        deleteLater();
    }
    if (x() < 0 || x() > 4800 || y() < 0 || y() > 2700) deleteLater();
}

