
#ifndef ENEMYRANGEDBASE_H
#define ENEMYRANGEDBASE_H

#include "enemyBase.h"
#include <QTimer>


class EnemyRangedBase : public EnemyBase
{
    Q_OBJECT
public:
    explicit EnemyRangedBase(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);

protected:
    void mobMove() override;
    virtual void attack() = 0;
    void angleChange();
    qreal angle;
    int angle_change_count;
    int angle_change_ceil;
};

#endif // ENEMYRANGEDBASE_H
