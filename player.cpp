
#include "player.h"
#include <iostream>
#include <QCursor>
#include <QRandomGenerator>
#include <QDebug> // debug

Player::Player(QGraphicsView *view, QObject *parent)
    :QObject{parent}, m_velocity(0, 0), max_hp(3), m_health(3), speed(2.25), exp(0), pickup_range(100), speed_boost(1), dodge(0), is_invincible(false), invincible_time(1), m_view(view)
{
    // 生成player贴图
    mv_idle = new QMovie(this);
    mv_idle->setFileName(":/images/images/pl_idle.gif");
    mv_idle->setSpeed(33);
    mv_idle->start();
    mv_run = new QMovie(this);
    mv_run->setFileName(":/images/images/pl_run.gif");
    mv_run->setSpeed(67);
    mv_run->start();

    // 基础模型设定
    load();
    setTransformOriginPoint(boundingRect().right(), boundingRect().center().y());
    setPos(640 - boundingRect().width(), 360 - boundingRect().height());
    setScale(3);

    // 无敌帧计时器
    m_invincibleTimer = new QTimer(this);
    connect(m_invincibleTimer, &QTimer::timeout, this, &Player::resetInvincible);

    // 地图边界
    mapbounds = QRectF(680, 603, 3464, 1405);
    pilebounds1 = QRectF(878, 721, 172, 308);
    pilebounds2 = QRectF(884, 1513, 172, 308);
    pilebounds3 = QRectF(2320, 1097, 172, 308);
    pilebounds4 = QRectF(3780, 731, 172, 308);
    pilebounds5 = QRectF(3764, 1509, 172, 308);

    // 受伤音效初始化
    sfx_hurt = new QSoundEffect(this);
    sfx_hurt->setSource(QUrl("qrc:/sfx/sfx/Blood_Splash_Quick_01.wav"));
}



void Player::addVelocity(int dx, int dy)
{
    // if 语句用来防止速度叠加
    if (m_velocity.x() != dx)
    m_velocity.setX(m_velocity.x() + dx);
    if (m_velocity.y() != dy)
    m_velocity.setY(m_velocity.y() + dy);
}

void Player::setVelocityX(int dx)
{
    m_velocity.setX(dx);
}

void Player::setVelocityY(int dy)
{
    m_velocity.setY(dy);
}

void Player::updatePos()
{
    qreal speed_reduce = 1;
    if (m_velocity.length() != 0) speed_reduce = m_velocity.length();
    QPointF newPos = this->pos() + QPointF(m_velocity.x() * speed * (speed_boost + 0.3 * is_inthewind) / speed_reduce, m_velocity.y() * speed * speed_boost / speed_reduce);
    if (!mapbounds.contains(newPos)
        || pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos))
    newPos = this->pos() + QPointF(m_velocity.x() * speed * (speed_boost + 0.3 * is_inthewind) / speed_reduce, 0);
    if (!mapbounds.contains(newPos)
        || pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos))
    newPos = this->pos() + QPointF(0, m_velocity.y() * speed * (speed_boost + 0.3 * is_inthewind) / speed_reduce);
    if (!mapbounds.contains(newPos)
        || pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos))
    newPos = this->pos() + QPointF(0, 0);
    setPos(newPos);
}

QPointF Player::posUpdate()
{
    qreal speed_reduce = 1;
    if (m_velocity.length() != 0) speed_reduce = m_velocity.length();
    QPointF newPos = this->pos() + QPointF(m_velocity.x() * speed * (speed_boost + 0.3 * is_inthewind) / speed_reduce, m_velocity.y() * speed * speed_boost / speed_reduce);
    if (!mapbounds.contains(newPos)
        || pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos))
    newPos = this->pos() + QPointF(m_velocity.x() * speed * (speed_boost + 0.3 * is_inthewind) / speed_reduce, 0);
    if (!mapbounds.contains(newPos)
        || pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos))
    newPos = this->pos() + QPointF(0, m_velocity.y() * speed * (speed_boost + 0.3 * is_inthewind) / speed_reduce);
    if (!mapbounds.contains(newPos)
        || pilebounds1.contains(newPos)
        || pilebounds2.contains(newPos)
        || pilebounds3.contains(newPos)
        || pilebounds4.contains(newPos)
        || pilebounds5.contains(newPos))
    newPos = this->pos() + QPointF(0, 0);
    return pos() - newPos;
}

void Player::hurt()
{
    if (is_invincible){
        return;
    }
    sfx_hurt->play();
    int dodge_chance = QRandomGenerator::global()->bounded(0, 100);
    if (dodge_chance > dodge) {
        inthewind_count = 0;
        m_health--;
        emit playerHurt();
    }
    is_invincible = true;
    m_invincibleTimer->start(1000 * invincible_time);
    if (m_health <= 0){
        emit playerDeath();
    }
}
void Player::resetInvincible()
{
    is_invincible = false;
    m_invincibleTimer->stop();
}

int Player::getHealth()
{
    return m_health;
}

void Player::setHealth(int hp)
{
    m_health = hp;
}

double Player::getPickupRange()
{
    return pickup_range;
}

int Player::getExp()
{
    return exp;
}

void Player::setExp(int xp)
{
    exp = xp;
}

qreal Player::getSpeed()
{
    return speed_boost;
}

void Player::setSpeed(qreal sp)
{
    speed_boost = sp;
}

void Player::pickupExp(int xp)
{
    exp += xp;
}

void Player::load()
{
    if (m_velocity.x() == 0 && m_velocity.y() == 0){
        QImage img(mv_idle->currentPixmap().toImage());
        if (m_view->mapFromGlobal(QCursor::pos()).x() < 640){
            img.mirror(true, false);
        }
        if (is_invincible) {
            uchar *pixels = img.bits();
            int width = img.width();
            int height = img.height();
            int bytesPerLine = img.bytesPerLine();
            for (int y = 0; y < height; ++y) {
                uchar *line = pixels + y * bytesPerLine;
                for (int x = 0; x < width; ++x) {
                    uchar alpha = line[x * 4 + 3];
                    if (alpha != 0) {
                        line[x * 4] = 255;
                        line[x * 4 + 1] = 255;
                        line[x * 4 + 2] = 255;
                    }
                }
            }
        }
        QPixmap pix(QPixmap::fromImage(img));
        setPixmap(pix);
    } else {
        QImage img(mv_run->currentPixmap().toImage());
        if (m_velocity.x() < 0){
            img.mirror(true, false);
        }
        if (is_invincible) {
            uchar *pixels = img.bits();
            int width = img.width();
            int height = img.height();
            int bytesPerLine = img.bytesPerLine();
            for (int y = 0; y < height; ++y) {
                uchar *line = pixels + y * bytesPerLine;
                for (int x = 0; x < width; ++x) {
                    uchar alpha = line[x * 4 + 3];
                    if (alpha != 0) {
                        line[x * 4] = 255;
                        line[x * 4 + 1] = 255;
                        line[x * 4 + 2] = 255;
                    }
                }
            }
        }
        QPixmap pix(QPixmap::fromImage(img));
        setPixmap(pix);
    }
}

qreal Player::getDodge() const
{
    return dodge;
}

void Player::setDodge(qreal newDodge)
{
    dodge = newDodge;
}

int Player::getMax_hp() const
{
    return max_hp;
}

void Player::setMax_hp(int newMax_hp)
{
    max_hp = newMax_hp;
}

void Player::setIs_regen(bool newIs_regen)
{
    is_regen = newIs_regen;
}

void Player::regen()
{
    if (!is_regen || m_health >= max_hp) return;
    regen_count++;
    if (regen_count < 3000) return;
    regen_count = 0;
    m_health++;
    emit playerRegen();
}

bool Player::getIs_inthewind() const
{
    return is_inthewind;
}

bool Player::getU_inthewind() const
{
    return u_inthewind;
}

void Player::setU_inthewind(bool newU_inthewind)
{
    u_inthewind = newU_inthewind;
}

void Player::inTheWind()
{
    if (u_inthewind == false) return;
    if (inthewind_count < 1500) {
        is_inthewind = false;
        inthewind_count++;
        return;
    } else {
        is_inthewind = true;
        return;
    }
}

qreal Player::getInvincible_time() const
{
    return invincible_time;
}

void Player::setInvincible_time(qreal newInvincible_time)
{
    invincible_time = newInvincible_time;
}

qreal Player::getSpeed_boost() const
{
    return speed_boost;
}

void Player::setSpeed_boost(qreal newSpeed_boost)
{
    speed_boost = newSpeed_boost;
}

bool Player::getIs_still() const
{
    if (m_velocity.x() == 0 && m_velocity.y() == 0) return true;
    else return false;
}

