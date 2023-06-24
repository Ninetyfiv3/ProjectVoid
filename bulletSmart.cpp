
#include "bulletSmart.h"
#include "qobject.h"

BulletSmart::BulletSmart(QPointF pos_, qreal angle_, qreal speed_, qreal damage_, int penetration_, QGraphicsScene *scene, QObject *parent)
    : BulletBase{pos_, angle_, speed_, damage_ , penetration_, scene, parent}
{
    setPixmap(QPixmap(":/images/images/weapon/wp_smart_bullet.png"));
    setScale(3);
    connect(m_timer_static, &QTimer::timeout, this, &BulletSmart::track);
}

void BulletSmart::track()
{
    QList<QGraphicsItem*> item_list = m_scene->items();
    QList<EnemyBase*> enemy_list;
    EnemyBase *target = nullptr;
    qreal min_dist = 0x3ffffff;
    for (QGraphicsItem *item : item_list) {
        EnemyBase *enemy = dynamic_cast<EnemyBase*>(item);
        if (enemy) enemy_list.append(enemy);
    }
    for (EnemyBase *enemy : enemy_list) {
        bool flag = false;
        for (auto p : hurt_enemy_list) {
            if (p == enemy) {
                flag = true;
                break;
            }
        }
        if (flag) continue;
        qreal dist = QLineF(pos(), enemy->pos()).length();
        if (dist < min_dist) {
            target = enemy;
            min_dist = dist;
        }
    }
    if (target != nullptr){

        qreal targetAngle = QLineF(pos(), target->pos()).angle();   // 顺时针角度
        qreal bulletAngle = rotation();                     // 逆时针角度

        qreal angle = targetAngle + bulletAngle;
        while (angle > 180) angle -= 360;     // 确保在-180到180度范围内
        while (angle < -180) angle += 360;
        if (angle > 4) {
            setRotation(rotation() - 4);
        } else if (angle < -4){
            setRotation(rotation() + 4);
        } else {
            setRotation(rotation() - angle);
        }
    }
}

