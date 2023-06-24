
#include "enemyRangedDroneElite.h"
#include "bulletEnemy.h"

EnemyRangedDroneElite::EnemyRangedDroneElite(Player *pl, QGraphicsScene *scene, QObject *parent)
    : EnemyRangedBase{pl, scene, parent}, attack_ceil(400)
{
    pix = QPixmap(":/images/images/enemy/en_float_elite.png");
    setPixmap(pix);
    setScale(6.5);
    setTransformOriginPoint(boundingRect().center());
    attack_count = 0;
    connect(m_timer_static, &QTimer::timeout, this, &EnemyRangedDroneElite::attack);

    health = 60;
    xp = 5;
    speed = 0.75;
}

void EnemyRangedDroneElite::load()
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

void EnemyRangedDroneElite::attack()
{
    attack_count++;
    qreal dist = QLineF(pos(), m_player->pos()).length();
    if (dist > 400) return;
    if (attack_count < attack_ceil) return;
    attack_count = 0;
    qreal angle = -QLineF(pos(), m_player->pos()).angle();
    angle -= 15;
    for (int i = 0; i < 3; ++i) {
    BulletEnemy *bullet = new BulletEnemy(pos(), angle + 15*i, 3, 1, 0, m_scene, m_player, m_scene);
    bullet->setScale(2);
    bullet->setParentItem(this->parentItem());
    m_scene->addItem(bullet);
    }
}

