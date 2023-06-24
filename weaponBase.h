
#ifndef WEAPONBASE_H
#define WEAPONBASE_H


#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QSoundEffect>
#include <QUrl>

#include "player.h"

class WeaponBase : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit WeaponBase(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);

    void updatePos();
    void rotateToCursor(const QPointF& target);
    void setFireRateBoost(qreal fr);
    void setDamageBoost(qreal dmg);
    void setSpeedBoost(qreal sp);

    virtual void fire() = 0;
    void load();
    void fire_on();
    void fire_off();

    // 数据接口
    qreal getFireRateBoost();
    qreal getDamageBoost();
    qreal getSpeedBoost();

    static QTimer *m_timer;

    int getRoundBoost() const;
    void setRoundBoost(int newRound_boost);

    qreal getSpread() const;
    void setSpread(qreal newSpread);

    int getPenetration() const;
    void setPenetration(int newPenetration);

    bool getBarrage() const;
    void setBarrage(bool newBarrage);

signals:

protected:
    QGraphicsScene *m_scene;
    Player *m_player;
    qreal fire_rate_boost;
    qreal damage_boost;
    qreal speed_boost;
    int round_boost;
    qreal spread;
    int penetration;
    bool barrage;

    QPixmap pix;
    int fire_count;
    bool is_fire;

    // 音效
    QSoundEffect *sfx_fire;
};

#endif // WEAPONBASE_H
