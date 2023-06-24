
#ifndef ENEMYEXPLOSIVEBALL_H
#define ENEMYEXPLOSIVEBALL_H

#include "enemyExplosiveBase.h"

class EnemyExplosiveBall : public EnemyExplosiveBase
{
    Q_OBJECT
public:
    explicit EnemyExplosiveBall(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);
private:
    void load() override;
    void explode() override;

    QPixmap pix;
    QMovie *fx_explosion;
};

#endif // ENEMYEXPLOSIVEBALL_H
