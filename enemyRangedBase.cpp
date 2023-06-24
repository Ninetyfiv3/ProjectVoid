
#include "enemyRangedBase.h"
#include <QRandomGenerator>

EnemyRangedBase::EnemyRangedBase(Player *pl, QGraphicsScene *scene, QObject *parent)
    : EnemyBase{pl, scene, parent}
{
    connect(m_timer_static, &QTimer::timeout, this, &EnemyRangedBase::angleChange);
    angle_change_ceil = 100;
    angle_change_count = 0;
}

void EnemyRangedBase::mobMove()
{
    if (!m_player) return;
    qreal dist = QLineF(pos(), m_player->pos()).length();
    QPointF newPos;
    qreal dx;
    qreal dy;
    if (dist > 400){
        // 计算出 Enemy 到 Player 的方向
        QLineF line(pos(), m_player->pos());

        // 计算出移动的距离和方向
        dx = cos(line.angle() * M_PI / 180) * speed;
        dy = -sin(line.angle() * M_PI / 180) * speed;
        newPos = pos() + QPointF(dx, dy);
    } else {
        dx = cos(angle * M_PI / 180) * speed;
        dy = -sin(angle * M_PI / 180) * speed;
        newPos = pos() + QPointF(dx, dy);
    }
    if (pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos)) {
        if (dx > 0) { newPos = pos() + QPointF(speed, 0); }
        else { newPos = pos() + QPointF(-speed, 0); }
    }
    if (pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos)) {
        if (dy > 0) { newPos = pos() + QPointF(0, speed); }
        else { newPos = pos() + QPointF(0, -speed); }
    }
    if (pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos))
        newPos = pos() + QPointF(0, 0);
    // 移动 Enemy
    setPos(newPos);
}

void EnemyRangedBase::angleChange()
{
    angle_change_count++;
    if (angle_change_count < angle_change_ceil) return;
    angle_change_count = 0;
    angle = QRandomGenerator::global()->bounded(360);
    angle_change_ceil = QRandomGenerator::global()->bounded(200) + 100;
}

