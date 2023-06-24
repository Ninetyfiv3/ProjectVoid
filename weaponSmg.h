
#ifndef WEAPONSMG_H
#define WEAPONSMG_H

#include "weaponBase.h"


class WeaponSmg : public WeaponBase
{
    Q_OBJECT
public:
    explicit WeaponSmg(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);
    void fire() override;
};

#endif // WEAPONSMG_H
