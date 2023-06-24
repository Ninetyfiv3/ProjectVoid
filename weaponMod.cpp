#include "weaponMod.h"

WeaponMod::WeaponMod(int s, int index, QGraphicsScene *scene, QObject *parent)
    : QObject{parent}, sign(s), w_index(index), m_scene(scene)
{
    desc = new QGraphicsTextItem();
    desc->setDefaultTextColor(Qt::white);
    desc->setFont(QFont("fusion pixel proportional", 16));
    setWeapon(w_index);
    m_scene->addItem(icon);
    connect(icon, &QGraphicsButton::hoverEnter, this, &WeaponMod::hoverEnter);
    connect(icon, &QGraphicsButton::hoverLeave, this, &WeaponMod::hoverLeave);
    connect(icon, &QGraphicsButton::mouseClick, this, &WeaponMod::mouseClick);
}

void WeaponMod::resetButton()
{
    icon->resetStatus();
}

void WeaponMod::hoverEnter()
{
    m_scene->addItem(desc);
}

void WeaponMod::hoverLeave()
{
    m_scene->removeItem(desc);
}

void WeaponMod::mouseClick()
{
    emit weaponSelected(m_weapon);
}

void WeaponMod::setWeapon(int index)
{
    switch (index) {
    case 1:
        icon = new QGraphicsSelectiveButton(":/images/images/icons/weapons/ic_smg", QPointF(102.4 + 294.4 * sign, 200));
        m_weapon = "smg";
        desc->setPlainText("冲锋枪: 高射速, 突突突突突突就完事了");
        break;
    case 2:
        icon = new QGraphicsSelectiveButton(":/images/images/icons/weapons/ic_smart", QPointF(102.4 + 294.4 * sign, 200));
        m_weapon = "smart";
        desc->setPlainText("智慧手枪MKⅡ: 自动索敌. 才不是在致敬隔壁ttf2呢!");
        break;
    case 3:
        icon = new QGraphicsSelectiveButton(":/images/images/icons/weapons/ic_shotgun", QPointF(102.4 + 294.4 * sign, 200));
        m_weapon = "shotgun";
        desc->setPlainText("霰弹枪: 打出一片弹丸雨, 低射速, 高伤害");
        break;
    case 4:
        icon = new QGraphicsSelectiveButton(":/images/images/icons/weapons/ic_dmr", QPointF(102.4 + 294.4 * sign, 200));
        m_weapon = "dmr";
        desc->setPlainText("射手步枪: 高精度, 高伤害. 一枪一个!");
        break;
    default:
        break;
    }
    icon->setScale(6);
    desc->setPos(640 - desc->boundingRect().width() / 2, 360);
}
