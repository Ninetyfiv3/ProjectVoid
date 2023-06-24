#include "upgradeMod.h"

bool UpgradeMod::u_vitality = false; // +2HP 活力
bool UpgradeMod::u_giant = false; // +4HP +25%size 巨人化
bool UpgradeMod::u_dodge = false; // +30%dodge 闪避
bool UpgradeMod::u_regeneration = false; //+1HP/30s 自动修复

bool UpgradeMod::u_hollowpoint = false; // +25%damage 空尖子弹
bool UpgradeMod::u_doubleshot = false; // +1round, -20%damage 二重射击
bool UpgradeMod::u_heavybarrel = false; // +40%damage, -10%firerate 重型枪管
bool UpgradeMod::u_reaper = false; // +15%damage, +1penetration 死神收割

bool UpgradeMod::u_fasthand = false; // +15%firerate 快速射击
bool UpgradeMod::u_overload = false; // +25%firerate, +30%spread 超载
bool UpgradeMod::u_lightbullet = false; // +20%firerate, -10%damage 轻型子弹
bool UpgradeMod::u_barrage = false; // +35%firerate while standing still 弹雨

bool UpgradeMod::u_streamline = false; // +30%roundspeed 流线构型
bool UpgradeMod::u_sniper = false; // +15%roundspeed -20%spread 狙击
bool UpgradeMod::u_takeaim = false; // -50%spread -10%firerate 精确打击
bool UpgradeMod::u_pierecinground = false; // +2penetration 穿甲子弹

bool UpgradeMod::u_haste = false; // +15%speed 急促脚步
bool UpgradeMod::u_overspeed = false; // +25%speed +20%spread 超速
bool UpgradeMod::u_runngun = false; // +10%speed +10%firerate 跑射
bool UpgradeMod::u_inthewind = false; // +30%firerate&+20%speed when not get hurt in 30s

UpgradeMod::UpgradeMod(int s, int u_index, Player *player, WeaponBase *weapon, QGraphicsScene *scene, QObject *parent)
    : QObject{parent}, sign(s), upgrade_index(u_index), m_player(player), m_weapon(weapon), m_scene(scene)
{
    QFontDatabase::addApplicationFont(":/fonts/fonts/fusion-pixel-proportional.ttf");

    u_desc = new QGraphicsTextItem();
    u_desc->setDefaultTextColor(Qt::white);
    u_desc->setFont(QFont("fusion pixel proportional", 16));
    setUpgrade(upgrade_index);
    m_scene->addItem(u_icon);
    connect(u_icon, &QGraphicsButton::hoverEnter, this, &UpgradeMod::hoverEnter);
    connect(u_icon, &QGraphicsButton::hoverLeave, this, &UpgradeMod::hoverLeave);
    connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::mouseClick);
}

UpgradeMod::~UpgradeMod()
{
    m_scene->removeItem(u_icon);
    m_scene->removeItem(u_desc);
    u_icon->deleteLater();
    delete u_desc;
}

void UpgradeMod::resetAll()
{
    u_vitality = false;
    u_giant = false;
    u_dodge = false;
    u_regeneration = false;

    u_hollowpoint = false;
    u_doubleshot = false;
    u_heavybarrel = false;
    u_reaper = false;

    u_fasthand = false;
    u_overload = false;
    u_lightbullet = false;
    u_barrage = false;

    u_streamline = false;
    u_sniper = false;
    u_takeaim = false;
    u_pierecinground = false;

    u_haste = false;
    u_overspeed = false;
    u_runngun = false;
    u_inthewind = false;
}

void UpgradeMod::hoverEnter()
{
    m_scene->addItem(u_desc);
}

void UpgradeMod::hoverLeave()
{
    m_scene->removeItem(u_desc);
}

void UpgradeMod::mouseClick()
{
    emit upgradeSelected();
    deleteLater();
}

void UpgradeMod::setUpgrade(int u_index)
{
    switch (u_index) {
    case 0:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_void", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("???: 无效果");
        break;
    case 1:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_vitality", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("活力: +2HP");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_vitality);
        break;
    case 2:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_giant", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("巨人化: +4HP, +25%角色大小");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_giant);
        break;
    case 3:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_dodge", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("规避动作: +30%闪避");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_dodge);
        break;
    case 4:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_regeneration", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("自我修理: 每30s回复1HP");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_regeneration);
        break;
    case 5:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_hollowpoint", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("空尖子弹: +25%伤害");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_hollowpoint);
        break;
    case 6:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_doubleshot", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("二重射击: +1子弹数量, -10%伤害");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_doubleshot);
        break;
    case 7:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_heavybarrel", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("重型枪管: +40%伤害, -10%射速");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_heavybarrel);
        break;
    case 8:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_reaper", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("死神收割: +15%伤害, +1穿透");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_reaper);
        break;
    case 9:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_fasthand", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("快速射击: +15%射速");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_fasthand);
        break;
    case 10:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_overload", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("超载: +25%射速, +30%扩散");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_overload);
        break;
    case 11:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_lightbullet", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("轻型子弹: +20%射速, -10%伤害");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_lightbullet);
        break;
    case 12:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_barrage", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("弹雨: 站立不动时+35%射速");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_barrage);
        break;
    case 13:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_streamline", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("流线构型: +30%子弹速度");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_streamline);
        break;
    case 14:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_sniper", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("狙击: +15%子弹速度, -20%扩散");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_sniper);
        break;
    case 15:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_takeaim", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("精确打击: -50%扩散, -10%射速");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_takeaim);
        break;
    case 16:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_pierecinground", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("穿甲子弹: +2穿透");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_pierecinground);
        break;
    case 17:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_haste", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("急促脚步: +20%移动速度");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_haste);
        break;
    case 18:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_overspeed", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("超速驾驶: +25%移动速度, +20%扩散");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_overspeed);
        break;
    case 19:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_runngun", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("RunN'Gun: +15%移动速度, +10%射速");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_runngun);
        break;
    case 20:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_inthewind", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("风之行者: 若在15s内未受到伤害, +30%移动速度, +30%射速");
        connect(u_icon, &QGraphicsButton::mouseClick, this, &UpgradeMod::l_inthewind);
        break;
    default:
        u_icon = new QGraphicsButton(":/images/images/icons/upgrades/u_void", QPointF(320 * sign + 280, 160), this);
        u_desc->setPlainText("???: 无效果");
        break;
    }
    u_desc->setPos(640 - u_desc->boundingRect().width() / 2, 400);
    u_icon->setScale(2.5);
}


void UpgradeMod::l_vitality()
{
    m_player->setMax_hp(m_player->getMax_hp() + 2);
    m_player->setHealth(m_player->getHealth() + 2);
    setU_vitality(true);
}

void UpgradeMod::l_giant()
{
    m_player->setMax_hp(m_player->getMax_hp() + 4);
    m_player->setHealth(m_player->getHealth() + 4);
    m_player->setScale(m_player->scale() * 1.25);
    setU_giant(true);
}

void UpgradeMod::l_dodge()
{
    m_player->setDodge(m_player->getDodge() + 30);
    setU_dodge(true);
}

void UpgradeMod::l_regeneration()
{
    m_player->setIs_regen(true);
    setU_regeneration(true);
}

void UpgradeMod::l_hollowpoint()
{
    m_weapon->setDamageBoost(m_weapon->getDamageBoost() + 0.25);
    setU_hollowpoint(true);
}

void UpgradeMod::l_doubleshot()
{
    m_weapon->setRoundBoost(m_weapon->getRoundBoost() + 1);
    setU_doubleshot(true);
}

void UpgradeMod::l_heavybarrel()
{
    m_weapon->setDamageBoost(m_weapon->getDamageBoost() + 0.40);
    m_weapon->setFireRateBoost(m_weapon->getFireRateBoost() - 0.1);
    setU_heavybarrel(true);
}

void UpgradeMod::l_reaper()
{
    m_weapon->setPenetration(m_weapon->getPenetration() + 1);
    m_weapon->setDamageBoost(m_weapon->getDamageBoost() + 0.15);
    setU_reaper(true);
}

void UpgradeMod::l_fasthand()
{
    m_weapon->setFireRateBoost(m_weapon->getFireRateBoost() + 0.15);
    setU_fasthand(true);
}

void UpgradeMod::l_overload()
{
    m_weapon->setFireRateBoost(m_weapon->getFireRateBoost() + 0.25);
    m_weapon->setSpread(m_weapon->getSpread() * 1.3);
    setU_overload(true);
}

void UpgradeMod::l_lightbullet()
{
    m_weapon->setFireRateBoost(m_weapon->getFireRateBoost() + 0.2);
    m_weapon->setDamageBoost(m_weapon->getDamageBoost() - 0.1);
    setU_lightbullet(true);
}

void UpgradeMod::l_barrage()
{
    m_weapon->setBarrage(true);
    setU_barrage(true);
}

void UpgradeMod::l_streamline()
{
    m_weapon->setSpeedBoost(m_weapon->getSpeedBoost() + 0.3);
    setU_streamline(true);
}

void UpgradeMod::l_sniper()
{
    m_weapon->setSpeedBoost(m_weapon->getSpeedBoost() + 0.15);
    m_weapon->setSpread(m_weapon->getSpread() * 0.8);
    setU_sniper(true);
}

void UpgradeMod::l_takeaim()
{
    m_weapon->setSpread(m_weapon->getSpread() * 0.5);
    m_weapon->setFireRateBoost(m_weapon->getFireRateBoost() - 0.1);
    setU_takeaim(true);
}

void UpgradeMod::l_pierecinground()
{
    m_weapon->setPenetration(m_weapon->getPenetration() + 2);
    setU_pierecinground(true);
}

void UpgradeMod::l_haste()
{
    m_player->setSpeed_boost(m_player->getSpeed_boost() + 0.2);
    setU_haste(true);
}

void UpgradeMod::l_overspeed()
{
    m_player->setSpeed_boost(m_player->getSpeed_boost() + 0.25);
    m_weapon->setSpread(m_weapon->getSpread() * 1.2);
    setU_overspeed(true);
}

void UpgradeMod::l_runngun()
{
    m_player->setSpeed_boost(m_player->getSpeed_boost() + 0.15);
    m_weapon->setFireRateBoost(m_weapon->getFireRateBoost() + 0.1);
    setU_runngun(true);
}

void UpgradeMod::l_inthewind()
{
    m_player->setU_inthewind(true);
    setU_inthewind(true);
}

bool UpgradeMod::getU_inthewind()
{
    return u_inthewind;
}

void UpgradeMod::setU_inthewind(bool newU_inthewind)
{
    u_inthewind = newU_inthewind;
}

bool UpgradeMod::getU_runngun()
{
    return u_runngun;
}

void UpgradeMod::setU_runngun(bool newU_runngun)
{
    u_runngun = newU_runngun;
}

bool UpgradeMod::getU_overspeed()
{
    return u_overspeed;
}

void UpgradeMod::setU_overspeed(bool newU_overspeed)
{
    u_overspeed = newU_overspeed;
}

bool UpgradeMod::getU_haste()
{
    return u_haste;
}

void UpgradeMod::setU_haste(bool newU_haste)
{
    u_haste = newU_haste;
}

bool UpgradeMod::getU_pierecinground()
{
    return u_pierecinground;
}

void UpgradeMod::setU_pierecinground(bool newU_pierecinground)
{
    u_pierecinground = newU_pierecinground;
}

bool UpgradeMod::getU_takeaim()
{
    return u_takeaim;
}

void UpgradeMod::setU_takeaim(bool newU_takeaim)
{
    u_takeaim = newU_takeaim;
}

bool UpgradeMod::getU_sniper()
{
    return u_sniper;
}

void UpgradeMod::setU_sniper(bool newU_sniper)
{
    u_sniper = newU_sniper;
}

bool UpgradeMod::getU_streamline()
{
    return u_streamline;
}

void UpgradeMod::setU_streamline(bool newU_streamline)
{
    u_streamline = newU_streamline;
}

bool UpgradeMod::getU_barrage()
{
    return u_barrage;
}

void UpgradeMod::setU_barrage(bool newU_barrage)
{
    u_barrage = newU_barrage;
}

bool UpgradeMod::getU_lightbullet()
{
    return u_lightbullet;
}

void UpgradeMod::setU_lightbullet(bool newU_lightbullet)
{
    u_lightbullet = newU_lightbullet;
}

bool UpgradeMod::getU_overload()
{
    return u_overload;
}

void UpgradeMod::setU_overload(bool newU_overload)
{
    u_overload = newU_overload;
}

bool UpgradeMod::getU_fasthand()
{
    return u_fasthand;
}

void UpgradeMod::setU_fasthand(bool newU_fasthand)
{
    u_fasthand = newU_fasthand;
}

bool UpgradeMod::getU_reaper()
{
    return u_reaper;
}

void UpgradeMod::setU_reaper(bool newU_reaper)
{
    u_reaper = newU_reaper;
}

bool UpgradeMod::getU_heavybarrel()
{
    return u_heavybarrel;
}

void UpgradeMod::setU_heavybarrel(bool newU_heavybarrel)
{
    u_heavybarrel = newU_heavybarrel;
}

bool UpgradeMod::getU_doubleshot()
{
    return u_doubleshot;
}

void UpgradeMod::setU_doubleshot(bool newU_doubleshot)
{
    u_doubleshot = newU_doubleshot;
}

bool UpgradeMod::getU_hollowpoint()
{
    return u_hollowpoint;
}

void UpgradeMod::setU_hollowpoint(bool newU_hollowpoint)
{
    u_hollowpoint = newU_hollowpoint;
}

bool UpgradeMod::getU_regeneration()
{
    return u_regeneration;
}

void UpgradeMod::setU_regeneration(bool newU_regeneration)
{
    u_regeneration = newU_regeneration;
}

bool UpgradeMod::getU_dodge()
{
    return u_dodge;
}

void UpgradeMod::setU_dodge(bool newU_dodge)
{
    u_dodge = newU_dodge;
}

bool UpgradeMod::getU_giant()
{
    return u_giant;
}

void UpgradeMod::setU_giant(bool newU_giant)
{
    u_giant = newU_giant;
}

bool UpgradeMod::getU_vitality()
{
    return u_vitality;
}

void UpgradeMod::setU_vitality(bool newU_vitality)
{
    u_vitality = newU_vitality;
}
