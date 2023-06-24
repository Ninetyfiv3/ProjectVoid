
#include "bulletSmg.h"

BulletSmg::BulletSmg(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration_, QGraphicsScene *scene, QObject *parent)
    : BulletBase{pos_, angle_, speed_, damage_, penetration_, scene, parent}
{
    setPixmap(QPixmap(":/images/images/weapon/wp_smg_bullet.png"));
}

