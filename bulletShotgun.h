#ifndef BULLETSHOTGUN_H
#define BULLETSHOTGUN_H

#include "bulletBase.h"

class BulletShotgun : public BulletBase
{
    Q_OBJECT
public:
    explicit BulletShotgun(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration_, QGraphicsScene *scene, QObject *parent = nullptr);
};

#endif // BULLETSHOTGUN_H
