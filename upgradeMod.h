#ifndef UPGRADEMOD_H
#define UPGRADEMOD_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QFontDatabase>
#include <QGraphicsScene>
#include <QRandomGenerator>

#include "qGraphicsButton.h"
#include "player.h"
#include "weaponBase.h"

class UpgradeMod : public QObject
{
    Q_OBJECT
public:
    explicit UpgradeMod(int s, int u_index, Player *player, WeaponBase *weapon, QGraphicsScene *scene, QObject *parent = nullptr);
    ~UpgradeMod();

    static void resetAll();

    static bool getU_vitality();
    static void setU_vitality(bool newU_vitality);

    static bool getU_giant();
    static void setU_giant(bool newU_giant);

    static bool getU_dodge();
    static void setU_dodge(bool newU_dodge);

    static bool getU_regeneration();
    static void setU_regeneration(bool newU_regeneration);

    static bool getU_hollowpoint();
    static void setU_hollowpoint(bool newU_hollowpoint);

    static bool getU_doubleshot();
    static void setU_doubleshot(bool newU_doubleshot);

    static bool getU_heavybarrel();
    static void setU_heavybarrel(bool newU_heavybarrel);

    static bool getU_reaper();
    static void setU_reaper(bool newU_reaper);

    static bool getU_fasthand();
    static void setU_fasthand(bool newU_fasthand);

    static bool getU_overload();
    static void setU_overload(bool newU_overload);

    static bool getU_lightbullet();
    static void setU_lightbullet(bool newU_lightbullet);

    static bool getU_barrage();
    static void setU_barrage(bool newU_barrage);

    static bool getU_streamline();
    static void setU_streamline(bool newU_streamline);

    static bool getU_sniper();
    static void setU_sniper(bool newU_sniper);

    static bool getU_takeaim();
    static void setU_takeaim(bool newU_takeaim);

    static bool getU_pierecinground();
    static void setU_pierecinground(bool newU_pierecinground);

    static bool getU_haste();
    static void setU_haste(bool newU_haste);

    static bool getU_overspeed();
    static void setU_overspeed(bool newU_overspeed);

    static bool getU_runngun();
    static void setU_runngun(bool newU_runngun);

    static bool getU_inthewind();
    static void setU_inthewind(bool newU_inthewind);

signals:
    void upgradeSelected();

private:
    int sign;
    int upgrade_index;
    Player *m_player;
    WeaponBase *m_weapon;

    QGraphicsScene *m_scene;

    QGraphicsButton *u_icon;
    QGraphicsTextItem *u_desc;

    void hoverEnter();
    void hoverLeave();
    void mouseClick();
    void setUpgrade(int u_index);

    void l_vitality();
    void l_giant();
    void l_dodge();
    void l_regeneration();

    void l_hollowpoint();
    void l_doubleshot();
    void l_heavybarrel();
    void l_reaper();

    void l_fasthand();
    void l_overload();
    void l_lightbullet();
    void l_barrage();

    void l_streamline();
    void l_sniper();
    void l_takeaim();
    void l_pierecinground();

    void l_haste();
    void l_overspeed();
    void l_runngun();
    void l_inthewind();

    static bool u_vitality; // +2HP 活力
    static bool u_giant; // +4HP +25%size 巨人化
    static bool u_dodge; // +30%dodge 闪避
    static bool u_regeneration; //+1HP/30s 自动修复

    static bool u_hollowpoint; // +25%damage 空尖子弹
    static bool u_doubleshot; // +1round, -20%damage 二重射击
    static bool u_heavybarrel; // +40%damage, -10%firerate 重型枪管
    static bool u_reaper; // +15%damage, +1penetration 死神收割

    static bool u_fasthand; // +15%firerate 快速射击
    static bool u_overload; // +25%firerate, +30%spread 超载
    static bool u_lightbullet; // +20%firerate, -10%damage 轻型子弹
    static bool u_barrage; // +35%firerate while standing still 弹雨

    static bool u_streamline; // +30%roundspeed 流线构型
    static bool u_sniper; // +15%roundspeed -20%spread 狙击
    static bool u_takeaim; // -50%spread -10%firerate 精确打击
    static bool u_pierecinground; // +2penetration 穿甲子弹

    static bool u_haste; // +15%speed 急促脚步
    static bool u_overspeed; // +25%speed +20%spread 超速
    static bool u_runngun; // +10%speed +10%firerate 跑射
    static bool u_inthewind; // +30%firerate&+20%speed when not get hurt in 30s

};

#endif // UPGRADEMOD_H
