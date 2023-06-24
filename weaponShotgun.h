#ifndef WEAPONSHOTGUN_H
#define WEAPONSHOTGUN_H

#include "weaponBase.h"

class WeaponShotgun : public WeaponBase
{
    Q_OBJECT
public:
    WeaponShotgun(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);
    void fire() override;
};

#endif // WEAPONSHOTGUN_H
