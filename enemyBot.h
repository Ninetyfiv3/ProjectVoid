
#ifndef ENEMYBOT_H
#define ENEMYBOT_H


#include "enemyBase.h"

class EnemyBot : public EnemyBase
{
    Q_OBJECT
public:
    explicit EnemyBot(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);
private:
    void load() override;
    QMovie *m_movie;

};

#endif // ENEMYBOT_H
