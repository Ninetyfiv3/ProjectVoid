#ifndef BULLETDMR_H
#define BULLETDMR_H

#include "bulletBase.h"

class BulletDmr : public BulletBase
{
    Q_OBJECT
public:
    explicit BulletDmr(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration, QGraphicsScene *scene, QObject *parent = nullptr);
};

#endif // BULLETDMR_H
