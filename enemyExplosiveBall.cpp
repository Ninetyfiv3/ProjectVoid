
#include "enemyExplosiveBall.h"

EnemyExplosiveBall::EnemyExplosiveBall(Player *pl, QGraphicsScene *scene, QObject *parent)
    : EnemyExplosiveBase{pl, scene, parent}
{
    // 基本数据
    explosion_range = 32;
    explosion_damage = 50;
    speed = 1;
    health = 50;

    pix = QPixmap(":/images/images/enemy/en_explosive.png");
    setPixmap(pix);
    setScale(4);
    setTransformOriginPoint(boundingRect().center());
    fx_explosion = new QMovie(":/images/images/fx/fx_explosion.gif");
    fx_explosion->setParent(this);
}

void EnemyExplosiveBall::load()
{
    qreal angle = QLineF(pos(), m_player->pos()).angle();
    setRotation(-angle);
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
    setPixmap(QPixmap::fromImage(img));
}

void EnemyExplosiveBall::explode()
{
    disconnect(m_timer_static, &QTimer::timeout, this, &EnemyExplosiveBall::load);
    disconnect(m_timer_static, &QTimer::timeout, this, &EnemyExplosiveBall::mobMove);
    connect(m_timer_static, &QTimer::timeout, this, &EnemyExplosiveBall::enemyHitCheck);
    disconnect(this, &EnemyBase::enemyDeath, this, &EnemyExplosiveBall::death);
    fx_explosion->start();
    moveBy(-16, -16);
    setPixmap(fx_explosion->currentPixmap());
    setTransformOriginPoint(boundingRect().center());
    connect(m_timer_static, &QTimer::timeout, this, [=]{
        disconnect(m_timer_static, &QTimer::timeout, this, &EnemyExplosiveBall::hitBoxCheck);
        disconnect(m_timer_static, &QTimer::timeout, this, &EnemyExplosiveBall::enemyHitCheck);
    });
    connect(fx_explosion, &QMovie::frameChanged, this, [=](){
        setPixmap(fx_explosion->currentPixmap());
    });
    connect(fx_explosion, &QMovie::finished, this, [=](){
        m_scene->removeItem(this);
        deleteLater();
    });
}

