
#include "weaponSmg.h"
#include <QRandomGenerator>
#include "bulletSmg.h"

WeaponSmg::WeaponSmg(Player *pl, QGraphicsScene *scene, QObject *parent)
    : WeaponBase(pl, scene, parent)
{
    pix = QPixmap(":/images/images/weapon/wp_smg.png");
    spread = 15;
    setPixmap(pix);
    setTransformOriginPoint(boundingRect().left(), boundingRect().center().y());
    setScale(3);
}

void WeaponSmg::fire()
{
    if (!is_fire && fire_count >= (10.0 / fire_rate_boost)) return;
    fire_count++;
    if (!is_fire) return;
    if (fire_count < (10.0 / (fire_rate_boost + 0.35 * barrage * m_player->getIs_still() + 0.3 * m_player->getIs_inthewind()))) return;
    fire_count = 0;
    sfx_fire->play();
    for (int i = 0; i < 1 + round_boost; ++i) {
        BulletSmg *bullet = new BulletSmg(pos(), rotation(), 15 * speed_boost, 5 * damage_boost, penetration, m_scene, parent());
        bullet->setRotation(bullet->rotation() + QRandomGenerator::global()->bounded((int)(-spread*10), (int)(spread*10)) * 0.1);
        bullet->setParentItem(parentItem());
        bullet->setScale(3);
        m_scene->addItem(bullet);
    }

}

