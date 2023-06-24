
#ifndef WEAPONSMART_H
#define WEAPONSMART_H

#include "weaponBase.h"


class WeaponSmart : public WeaponBase
{
    Q_OBJECT
public:
    explicit WeaponSmart(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);
    void fire() override;
};

#endif // WEAPONSMART_H
