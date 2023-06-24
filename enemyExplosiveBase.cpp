
#include "enemyExplosiveBase.h"
#include "exp.h"


EnemyExplosiveBase::EnemyExplosiveBase(Player *pl, QGraphicsScene *scene, QObject *parent)
    : EnemyBase{pl, scene, parent}
{
    sfx_explosion = new QSoundEffect(this);
    sfx_explosion->setSource(QUrl("qrc:/sfx/sfx/explosion-sfx.wav"));
}

void EnemyExplosiveBase::death()
{
    Exp *exp = new Exp(m_player, pos(), m_scene, 1);
    exp->setParentItem(this->parentItem());
    sfx_explosion->play();
    explode();
}

void EnemyExplosiveBase::enemyHitCheck()
{
    QList<QGraphicsItem*> itemList = collidingItems();
    for (QGraphicsItem *item : itemList) {
        EnemyBase* enemy = dynamic_cast<EnemyBase*>(item);
        if (enemy) {
            enemy->hurt(explosion_damage);
        }
    }
}


