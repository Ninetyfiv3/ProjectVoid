
#ifndef ENEMYRANGEDDRONE_H
#define ENEMYRANGEDDRONE_H

#include "enemyRangedBase.h"


class EnemyRangedDrone : public EnemyRangedBase
{
    Q_OBJECT
public:
    explicit EnemyRangedDrone(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);

private:
    void load() override;
    void attack() override;
    int attack_count;
    const int attack_ceil;
    QPixmap pix;
};

#endif // ENEMYRANGEDDRONE_H
