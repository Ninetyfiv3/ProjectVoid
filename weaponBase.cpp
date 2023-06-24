
#include "weaponBase.h"


QTimer *m_timer = nullptr;

WeaponBase::WeaponBase(Player *pl, QGraphicsScene *scene, QObject *parent)
    : QObject{parent}, m_scene(scene), m_player(pl)
{
    fire_rate_boost = 1;
    damage_boost = 1;
    speed_boost = 1;
    fire_count = 0;
    is_fire = 0;
    round_boost = 0;
    penetration = 0;
    barrage = false;

    sfx_fire = new QSoundEffect(this);
    sfx_fire->setVolume(0.5);
    sfx_fire->setSource(QUrl::fromLocalFile(":/sfx/sfx/single_shot.wav"));

}



void WeaponBase::updatePos()
{
    setPos(m_player->pos());
}

void WeaponBase::rotateToCursor(const QPointF &target)
{
    QPointF weaponPos = mapToScene(transformOriginPoint());
    double dx = target.x() - weaponPos.x();
    double dy = target.y() - weaponPos.y();
    setRotation(-qRadiansToDegrees(qAtan2(dx, dy))+90);
}

void WeaponBase::setFireRateBoost(qreal fr)
{
    fire_rate_boost = fr;
}

void WeaponBase::setDamageBoost(qreal dmg)
{
    damage_boost = dmg;
}

void WeaponBase::setSpeedBoost(qreal sp)
{
    speed_boost = sp;
}

void WeaponBase::load()
{
    QImage img(pix.toImage());
    if ( (rotation() > 90 && rotation() < 270) || (rotation() < -90 && rotation() > -270)){
        img.mirror(false, true);
    }
    setPixmap(QPixmap::fromImage(img));
}

void WeaponBase::fire_on()
{
    is_fire = true;
}

void WeaponBase::fire_off()
{
    is_fire = false;
}

qreal WeaponBase::getFireRateBoost()
{
    return fire_rate_boost;
}

qreal WeaponBase::getDamageBoost()
{
    return damage_boost;
}

qreal WeaponBase::getSpeedBoost()
{
    return speed_boost;
}

int WeaponBase::getRoundBoost() const
{
    return round_boost;
}

void WeaponBase::setRoundBoost(int newRound_boost)
{
    round_boost = newRound_boost;
}

qreal WeaponBase::getSpread() const
{
    return spread;
}

void WeaponBase::setSpread(qreal newSpread)
{
    spread = newSpread;
}

int WeaponBase::getPenetration() const
{
    return penetration;
}

void WeaponBase::setPenetration(int newPenetration)
{
    penetration = newPenetration;
}

bool WeaponBase::getBarrage() const
{
    return barrage;
}

void WeaponBase::setBarrage(bool newBarrage)
{
    barrage = newBarrage;
}



