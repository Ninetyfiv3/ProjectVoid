#ifndef ENEMYRANGEDDRONEELITE_H
#define ENEMYRANGEDDRONEELITE_H



#include "enemyRangedBase.h"


class EnemyRangedDroneElite : public EnemyRangedBase
{
    Q_OBJECT
public:
    explicit EnemyRangedDroneElite(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);

private:
    void load() override;
    void attack() override;
    int attack_count;
    const int attack_ceil;
    QPixmap pix;
};
#endif // ENEMYRANGEDDRONEELITE_H
