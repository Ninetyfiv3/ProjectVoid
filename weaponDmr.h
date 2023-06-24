#ifndef WEAPONDMR_H
#define WEAPONDMR_H

#include "weaponBase.h"

class WeaponDmr : public WeaponBase
{
    Q_OBJECT
public:
    explicit WeaponDmr(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);
    void fire() override;
};

#endif // WEAPONDMR_H
