#ifndef ENEMYBOTELITE_H
#define ENEMYBOTELITE_H

#include "enemyBase.h"

class EnemyBotElite : public EnemyBase
{
    Q_OBJECT
public:
    explicit EnemyBotElite(Player *pl, QGraphicsScene *scene, QObject *parent = nullptr);
private:
    void load() override;
    QMovie *m_movie;

};

#endif // ENEMYBOTELITE_H
