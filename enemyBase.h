
#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include "player.h"

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSoundEffect>
#include <QUrl>

class EnemyBase : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit EnemyBase(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);

    void hurt(qreal damage_);

    static QTimer *m_timer_static;
    static QSoundEffect *sfx_enemy_hurt;

signals:
    void enemyDeath();

protected:
    Player *m_player;
    qreal speed;
    qreal health;
    int xp;
    QGraphicsScene *m_scene;
    QTimer hurt_timer;
    bool is_hurt;

    virtual void death();
    virtual void mobMove();
    void hitBoxCheck();
    virtual void load() = 0;
    void hurt_reload();

    static QRectF pilebounds1;
    static QRectF pilebounds2;
    static QRectF pilebounds3;
    static QRectF pilebounds4;
    static QRectF pilebounds5;

};



#endif // ENEMYBASE_H
