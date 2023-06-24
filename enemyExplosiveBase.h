
#ifndef ENEMYEXPLOSIVEBASE_H
#define ENEMYEXPLOSIVEBASE_H

#include "enemyBase.h"

class EnemyExplosiveBase : public EnemyBase
{
    Q_OBJECT
public:
    explicit EnemyExplosiveBase(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);

protected:
    qreal explosion_range;
    qreal explosion_damage;
    virtual void explode() = 0;
    void death() override;
    void enemyHitCheck();
    QSoundEffect *sfx_explosion;
};

#endif // ENEMYEXPLOSIVEBASE_H
