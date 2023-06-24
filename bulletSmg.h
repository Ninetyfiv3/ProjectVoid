
#ifndef BULLETSMG_H
#define BULLETSMG_H

#include "bulletBase.h"



class BulletSmg : public BulletBase
{
    Q_OBJECT
public:
    explicit BulletSmg(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration_, QGraphicsScene *scene, QObject *parent = nullptr);
};

#endif // BULLETSMG_H
