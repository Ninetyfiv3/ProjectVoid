
#ifndef BULLETBASE_H
#define BULLETBASE_H


#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QList>

#include "enemyBase.h"


class BulletBase : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit BulletBase(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration_, QGraphicsScene *scene, QObject *parent = nullptr);

    static QTimer *m_timer_static;

signals:

protected:
    void enemyHitCheck();
    void move();
    QGraphicsScene *m_scene;
    qreal m_speed;
    qreal m_damage;
    int m_penetration;
    QList<EnemyBase*> hurt_enemy_list;
    int hurt_count;
};

#endif // BULLETBASE_H
