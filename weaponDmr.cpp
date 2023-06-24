#include "weaponDmr.h"
#include "bulletDmr.h"
#include <QRandomGenerator>

WeaponDmr::WeaponDmr(Player *pl, QGraphicsScene *scene, QObject *parent)
    : WeaponBase{pl, scene, parent}
{
    pix = QPixmap(":/images/images/weapon/wp_dmr.png");
    setPixmap(pix);
    setTransformOriginPoint(boundingRect().center());
    spread = 3;
    setScale(3);
}

void WeaponDmr::fire()
{
    if (!is_fire && fire_count >= (100.0 / fire_rate_boost)) return;
    fire_count++;
    if (!is_fire) return;
    if (fire_count < (100.0 / (fire_rate_boost + 0.35 * barrage * m_player->getIs_still() + 0.3 * m_player->getIs_inthewind()))) return;
    fire_count = 0;
    sfx_fire->play();
    for (int i = 0; i < 1 + round_boost; ++i) {
        BulletDmr *bullet = new BulletDmr(pos(), rotation(), 60 * speed_boost, 30 * damage_boost, penetration, m_scene, parent());
        bullet->setRotation(bullet->rotation() + QRandomGenerator::global()->bounded((int)(-spread*10), (int)(spread*10)) * 0.1);
        bullet->setParentItem(parentItem());
        bullet->setScale(3);
        m_scene->addItem(bullet);
    }
}
