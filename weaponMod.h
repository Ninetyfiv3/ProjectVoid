#ifndef WEAPONMOD_H
#define WEAPONMOD_H

#include <QObject>
#include <QFontDatabase>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#include "qGraphicsSelectiveButton.h"

class WeaponMod : public QObject
{
    Q_OBJECT
public:
    explicit WeaponMod(int s, int index, QGraphicsScene *scene, QObject *parent = nullptr);

    void resetButton();

signals:
    void weaponSelected(QString weapon);
private:
    int sign;
    int w_index;
    QString m_weapon;
    QGraphicsSelectiveButton *icon;
    QGraphicsTextItem *desc;
    QGraphicsScene *m_scene;

    void hoverEnter();
    void hoverLeave();
    void mouseClick();

    void setWeapon(int index);
};

#endif // WEAPONMOD_H
