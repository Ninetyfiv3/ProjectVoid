
#include "weaponSmart.h"
#include "bulletSmart.h"
#include <QRandomGenerator>

WeaponSmart::WeaponSmart(Player *pl, QGraphicsScene *scene, QObject *parent)
    : WeaponBase{pl, scene, parent}
{
    pix = QPixmap(":/images/images/weapon/wp_smart.png");
    setPixmap(pix);
    setTransformOriginPoint(boundingRect().center());
    spread = 20;
    setScale(3);
}

void WeaponSmart::fire()
{
    if (!is_fire && fire_count >= (20.0 / fire_rate_boost)) return;
    fire_count++;
    if (!is_fire) return;
    if (fire_count < (20.0 / (fire_rate_boost + 0.35 * barrage * m_player->getIs_still() + 0.3 * m_player->getIs_inthewind()))) return;
    fire_count = 0;
    sfx_fire->play();
    for (int i = 0; i < 1 + round_boost; ++i) {
        BulletSmart *bullet = new BulletSmart(pos(), rotation(), 15 * speed_boost, 4 * damage_boost, penetration, m_scene, parent());
        bullet->setRotation(bullet->rotation() + QRandomGenerator::global()->bounded((int)(-spread*10), (int)(spread*10)) * 0.1);
        bullet->setParentItem(parentItem());
        bullet->setScale(3);
        m_scene->addItem(bullet);
    }
}

