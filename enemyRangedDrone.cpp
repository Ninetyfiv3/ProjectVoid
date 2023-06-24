
#include "enemyRangedDrone.h"
#include "bulletEnemy.h"
#include <QDebug>

EnemyRangedDrone::EnemyRangedDrone(Player *pl, QGraphicsScene *scene, QObject *parent)
    : EnemyRangedBase{pl, scene, parent}, attack_ceil(400)
{
    pix = QPixmap(":/images/images/enemy/en_float.png");
    setPixmap(pix);
    setScale(5);
    setTransformOriginPoint(boundingRect().center());
    attack_count = 0;
    connect(m_timer_static, &QTimer::timeout, this, &EnemyRangedDrone::attack);

    health = 30;
    xp = 2;
    speed = 0.5;
}

void EnemyRangedDrone::load()
{
    QImage img(pix.toImage());
    if (is_hurt) {
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
    qreal angle = QLineF(pos(), m_player->pos()).angle();
    if ( (angle > 90 && angle < 270) || (angle < -90 && angle > -270)){
        img.mirror(true, false);
    }
    setPixmap(QPixmap::fromImage(img));
}

void EnemyRangedDrone::attack()
{
    attack_count++;
    qreal dist = QLineF(pos(), m_player->pos()).length();
    if (dist > 400) return;
    if (attack_count < attack_ceil) return;
    attack_count = 0;
    qreal angle = -QLineF(pos(), m_player->pos()).angle();
    BulletEnemy *bullet = new BulletEnemy(pos(), angle, 3, 1, 0, m_scene, m_player, m_scene);
    bullet->setScale(2);
    bullet->setParentItem(this->parentItem());
    m_scene->addItem(bullet);

}

