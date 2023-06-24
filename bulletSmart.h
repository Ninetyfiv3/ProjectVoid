
#ifndef BULLETSMART_H
#define BULLETSMART_H

#include "bulletBase.h"
#include "enemyBase.h"

class BulletSmart : public BulletBase
{
    Q_OBJECT
public:
    explicit BulletSmart(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration_, QGraphicsScene *scene, QObject *parent = nullptr);
private:
    void track();
};

#endif // BULLETSMART_H
