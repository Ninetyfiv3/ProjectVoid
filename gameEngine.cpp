
#include <QFile>
#include "gameEngine.h"
#include "qapplication.h"
#include "exp.h"

#include "bulletBase.h"

#include "enemyRangedDrone.h"
#include "enemyBot.h"
#include "enemyExplosiveBall.h"
#include "enemyBotElite.h"
#include "enemyRangedDroneElite.h"

#include "weaponSmg.h"
#include "weaponSmart.h"
#include "weaponShotgun.h"
#include "weaponDmr.h"

#include "upgradeMod.h"




GameEngine::GameEngine(const QString &weapon, const QString &load_archive): weapon_type(weapon), enemy_gen_count(0), bullet_gen_count(0), phase(0), phase_count(0), countdown_count(0), level(0), data_count(0)
{
    QFontDatabase::addApplicationFont(":/fonts/fonts/fusion-pixel-proportional.ttf");
    // 生成场景
    setFixedSize(1280, 720);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_scene = new QGraphicsScene(this);

    // 背景生成
    QPixmap background_pic(":/images/images/background_gameplay.png");
    m_background = new QGraphicsPixmapItem;
    m_background->setPixmap(background_pic);
    m_background->setZValue(-0x3ffffff);
    m_scene->addItem(m_background);

    // 角色生成
    m_player = new Player(this, this);
    m_player->setParentItem(m_background);

    connect(m_player, &Player::playerDeath, this, &GameEngine::endgame_lose);
    // 武器生成
    if (weapon == "smg") m_weapon = new WeaponSmg(m_player, m_scene, this);
    else if (weapon == "smart") m_weapon = new WeaponSmart(m_player, m_scene, this);
    else if (weapon == "shotgun") m_weapon = new WeaponShotgun(m_player, m_scene, this);
    else if (weapon == "dmr") m_weapon = new WeaponDmr(m_player, m_scene, this);
    m_weapon->setParentItem(m_background);

    m_scene->addItem(m_player);
    m_scene->addItem(m_weapon);
    m_scene->setSceneRect(0, 0, 1280, 720);

    setScene(m_scene);

    // 局外升级
    settings = new QSettings("./game.ini", QSettings::IniFormat, this);
    data_amount = settings->value("data", 0).toInt();
    // 读取样式表
    QFile file_sqr_pb(":/style/stylesheet/sqr_pb.qss");
    file_sqr_pb.open(QFile::ReadOnly);
    QString style_sqr_pb;
    if (file_sqr_pb.isOpen()) {
        style_sqr_pb = QLatin1String(file_sqr_pb.readAll());
        file_sqr_pb.close();
    }
    // Data显示文本生成
    QString str("Data: ");
    str += QString::number(data_amount);
    data_text = new QGraphicsTextItem(str);
    data_text->setDefaultTextColor(Qt::white);
    data_text->setFont(QFont("fusion pixel proportional", 24));
    data_text->setPos(100, 20);
    m_scene->addItem(data_text);
    // 角色血量升级
    int hp_increment = settings->value("HP Increment", 0).toInt();
    m_player->setMax_hp(m_player->getMax_hp() + hp_increment);
    m_player->setHealth(m_player->getHealth() + hp_increment);

    qreal muzzle_choke = settings->value("Muzzle Choke", 0).toInt() * qreal(0.2);
    m_weapon->setSpread(m_weapon->getSpread() * (qreal(1) - muzzle_choke));

    data_collector = settings->value("Data Collector", 0).toInt();

    // 计时器
    // 移动检测计时器
    main_timer = new QTimer(this);
    // player移动和贴图加载
    connect(main_timer, &QTimer::timeout, m_player, &Player::updatePos);
    connect(main_timer, &QTimer::timeout, m_player, &Player::load);
    connect(main_timer, &QTimer::timeout, m_player, &Player::inTheWind);
    connect(main_timer, &QTimer::timeout, m_player, &Player::regen);
    // 武器移动，加载贴图，开火
    connect(main_timer, &QTimer::timeout, m_weapon, &WeaponBase::updatePos);
    connect(main_timer, &QTimer::timeout, m_weapon, &WeaponBase::load);
    connect(main_timer, &QTimer::timeout, m_weapon, &WeaponBase::fire);
    // 背景移动
    connect(main_timer, &QTimer::timeout, this, &GameEngine::updateBackground);
    // 敌人生成计时器
    connect(main_timer, &QTimer::timeout, this, &GameEngine::enemyGenerator);
    // Enemy移动攻击计时器
    EnemyBase::m_timer_static = main_timer;
    // Bullet计时器
    BulletBase::m_timer_static = main_timer;
    // 经验计时器
    Exp::m_timer = main_timer;
    // 升级检测
    connect(main_timer, &QTimer::timeout, this, &GameEngine::levelCheck);
    // 局外升级
    connect(main_timer, &QTimer::timeout, this, &GameEngine::dataIncrement);
    connect(qApp, &QApplication::aboutToQuit, this, [=](){
        settings->setValue("data", data_amount);
    });
    // 倒计时刷新
    connect(main_timer, &QTimer::timeout, this, &GameEngine::updateCountdown);
    // 开始计时
    main_timer->start(10);

    // 暂停按键生成
    pb_pause = new QPushButton(this);
    connect(pb_pause, &QPushButton::clicked, this, [=](){
        gamePause();
        showArchive();
    });
    m_scene->addWidget(pb_pause);


    // 血条生成
    setHealthBar();
    connect(m_player, &Player::playerHurt, this, &GameEngine::setHealthBar);
    connect(m_player, &Player::playerRegen, this, &GameEngine::setHealthBar);

    // 移动player至地图中央
    m_player->moveBy(2400, 1350);
    m_background->moveBy(-2400, -1350);

    // 游戏阶段初始化
    // 数据初始化
    enemy_generate_rate = 7.5;
    melee_enemy_rate = 100;
    ranged_enemy_rate = 0;
    charge_enemy_rate = 0;
    explosive_enemy_rate = 0;
    elite_enemy_rate = 0;
    // 计时器初始化
    connect(main_timer, &QTimer::timeout, this, &GameEngine::phaseUp);
    // 倒计时初始化
    countdown_text = new QGraphicsTextItem();
    m_scene->addItem(countdown_text);
    countdown_text->setFont(QFont("fusion pixel proportional", 32));
    countdown_text->setPos(100, 80);
    countdown_text->setDefaultTextColor(Qt::white);

    countdown_time = QTime(0, 10, 0);
    // 经验条生成
    // 经验条背景
    exp_bar_bg = new QGraphicsRectItem(0, 0, 400, 20);
    exp_bar_bg->setBrush(QColor(100, 100, 100));
    exp_bar_bg->setPen(QPen(QColor(0, 0, 0), 4));
    exp_bar_bg->setPos(440, 20);
    m_scene->addItem(exp_bar_bg);

    // 经验条
    exp_bar = new QGraphicsRectItem(0, 0, 0, 20);
    exp_bar->setBrush(QColor(0, 200, 255));
    exp_bar->setPos(440, 22);
    exp_bar->setRect(0, 0, 0, 16); // 初始化宽度为0
    m_scene->addItem(exp_bar);

    // 等级文本
    level_text = new QGraphicsTextItem("Level 1");
    level_text->setDefaultTextColor(Qt::white);
    level_text->setFont(QFont("fusion pixel proportional", 16));
    level_text->setPos(600, 30);
    m_scene->addItem(level_text);


    // 背景音乐
    bgm_player = new QSoundEffect(this);
    bgm_player->setSource(QUrl::fromLocalFile(":/bgm/bgm/Horizon Star - DDRKirby(ISQ).wav"));
    bgm_player->setLoopCount(0x3ffffff);
    bgm_player->setVolume(0.1);
    bgm_player->play();

    // 受击音效
    EnemyBase::sfx_enemy_hurt = new QSoundEffect(this);
    EnemyBase::sfx_enemy_hurt->setSource(QUrl("qrc:/sfx/sfx/sfx_sounds_impact1.wav"));
    EnemyBase::sfx_enemy_hurt->setVolume(0.15);

    // 复活音效
    sfx_respawn = new QSoundEffect(this);
    sfx_respawn->setSource(QUrl::fromLocalFile(":/sfx/sfx/Buff_Intelligence.wav"));
    sfx_respawn->setVolume(0.4);

    // 载入存档
    for (int i = 0; i < 3; ++i) {
        QString fileName = QString("./archive_%1.ini").arg(i);
        archive[i] = new QSettings(fileName, QSettings::IniFormat, this);
    }
    if (load_archive == "none") return;
    else if (load_archive == "archive0") loadArchive(0);
    else if (load_archive == "archive1") loadArchive(1);
    else if (load_archive == "archive2") loadArchive(2);
}


void GameEngine::enemyGenerator()
{
    enemy_gen_count++;
    if (enemy_gen_count < 1000 / enemy_generate_rate) return;
    enemy_gen_count = 0;
    // 随机生成敌人位置,直到满足生成条件
    QPointF enemyPos;
    qreal dist;
    do {
        enemyPos.setX(QRandomGenerator::global()->bounded(0, 4800));
        enemyPos.setY(QRandomGenerator::global()->bounded(0, 2700));
        dist = QLineF(m_player->pos(), enemyPos).length();

    } while (dist < 550
             || dist > 650
             || pilebounds1.contains(enemyPos)
             || pilebounds2.contains(enemyPos)
             || pilebounds3.contains(enemyPos)
             || pilebounds4.contains(enemyPos)
             || pilebounds5.contains(enemyPos));   // 不在视野范围或过近时重新生成

    EnemyBase *enemy = nullptr;
    qreal random = QRandomGenerator::global()->bounded(0, 100);
    qreal ran_elite = QRandomGenerator::global()->bounded(0, 100);
    if (random <= melee_enemy_rate){
        if (ran_elite >= elite_enemy_rate) enemy = new EnemyBot(m_player, m_scene, this); // 生成近战敌人
        else enemy = new EnemyBotElite(m_player, m_scene, this);
    } else if (random > melee_enemy_rate && random <= melee_enemy_rate + ranged_enemy_rate){
        if (ran_elite >= elite_enemy_rate) enemy = new EnemyRangedDrone(m_player, m_scene, this); // 生成远程敌人
        else enemy = new EnemyRangedDroneElite(m_player, m_scene, this);
    } else if (random > melee_enemy_rate + ranged_enemy_rate && random <= melee_enemy_rate + ranged_enemy_rate + explosive_enemy_rate){
        enemy = new EnemyExplosiveBall(m_player, m_scene, this);// 生成自爆敌人
    }
    if (!enemy) return;
    enemy->setParentItem(m_background);
    enemy->setPos(enemyPos);
    enemy->setZValue(1);
    // 将敌人添加到场景中
    m_scene->addItem(enemy);
}

void GameEngine::setHealthBar()
{
    int health = m_player->getHealth();
    int max_hp = m_player->getMax_hp();
    for (auto *p : heart_list) {
        if (p) p->deleteLater();
    }
    heart_list.clear();
    for (int i = 0; i < max_hp; ++i) {
        HeartLabel *heart = new HeartLabel(this);
        if (i >= health) heart->loseHeart();
        heart->move(heart->x() - i*36, heart->y());
        heart_list.append(heart);
    }
}

void GameEngine::updateBackground()
{
    QPointF dis = m_player->posUpdate();
    m_background->moveBy(dis.x(), dis.y());
}

void GameEngine::gamePause()
{
    main_timer->stop();
    pb_pause->disconnect();
    connect(pb_pause, &QPushButton::clicked, this, [=](){
        hideArchive();
        gameContinue();
    });
}

void GameEngine::gameContinue()
{
    main_timer->start();
    pb_pause->disconnect();
    connect(pb_pause, &QPushButton::clicked, this, [=](){
        gamePause();
        showArchive();
    });
}

void GameEngine::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        m_player->addVelocity(0, -1);
        break;
    case Qt::Key_S:
        m_player->addVelocity(0, 1);
        break;
    case Qt::Key_A:
        m_player->addVelocity(-1, 0);
        break;
    case Qt::Key_D:
        m_player->addVelocity(1, 0);
    default:
        break;
    }
}

void GameEngine::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        m_player->addVelocity(0, 1);
        break;
    case Qt::Key_S:
        m_player->addVelocity(0, -1);
        break;
    case Qt::Key_A:
        m_player->addVelocity(1, 0);
        break;
    case Qt::Key_D:
        m_player->addVelocity(-1, 0);
    default:
        break;
    }
}



void GameEngine::mouseMoveEvent(QMouseEvent *event)
{
    m_weapon->rotateToCursor(event->pos());
    QGraphicsView::mouseMoveEvent(event);
}

void GameEngine::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        m_weapon->fire_on();
        break;
    default:
        break;
    }
    // 将事件传递给视图
    QGraphicsView::mousePressEvent(event);
}

void GameEngine::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        m_weapon->fire_off();
        break;
    default:
        break;
    }
    // 将事件传递给视图
    QGraphicsView::mouseReleaseEvent(event);
}

void GameEngine::phaseUp() // 渐进式游戏阶段
{
    phase_count++;
    if (phase_count < 6000) return; // 60s
    phase_count = 0;
    phase++;
    switch (phase) {
    case 1:
        enemy_generate_rate = 10;
        melee_enemy_rate = 90;
        ranged_enemy_rate = 0;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 10;
        elite_enemy_rate = 0;
        break;
    case 2:
        enemy_generate_rate = 10;
        melee_enemy_rate = 70;
        ranged_enemy_rate = 10;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 0;
        break;
    case 3:
        enemy_generate_rate = 12.5;
        melee_enemy_rate = 70;
        ranged_enemy_rate = 10;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 0;
        break;
    case 4:
        enemy_generate_rate = 15;
        melee_enemy_rate = 70;
        ranged_enemy_rate = 10;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 0;
        break;
    case 5:
        enemy_generate_rate = 15;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 30;
        break;
    case 6:
        enemy_generate_rate = 20;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 30;
        break;
    case 7:
        enemy_generate_rate = 20;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 65;
        break;
    case 8:
        enemy_generate_rate = 30;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 65;
        break;
    case 9:
        enemy_generate_rate = 50;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 100;
        break;
    case 10:
        endgame_win();
        break;
    default:
        break;
    }
}

void GameEngine::updateCountdown()
{
    countdown_count++;
    if (countdown_count < 100) return;
    countdown_count = 0;
    countdown_time = countdown_time.addSecs(-1);
    countdown_text->setPlainText(countdown_time.toString("mm:ss"));
}

void GameEngine::levelCheck()
{
    int exp = m_player->getExp();
    setExpBar();
    if (exp < 6 + 4 * level) return;
    m_player->setExp(0);
    if (level > 17) return;
    level++;
    gamePause();
    levelUp();
}

QList<int> GameEngine::upgradeGenerator()
{
    QList<int> list;
    const int MAX_COUNT = 100;
    int count = 0;
    for (int i = 0; i < 3; ++i) {
        int type_index = QRandomGenerator::global()->bounded(1, 21);
        count++;
        if (count > MAX_COUNT) {
            list.append(0);
            continue;
        }
        bool flag = false;
        for (int p : list) {
            if (type_index == p) {
                flag = true;
                break;
            }
        }
        if (flag) {
            i--;
            continue;
        }
        switch (type_index) {
        case 1:
            if (UpgradeMod::getU_vitality() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 2:
            if (UpgradeMod::getU_vitality() == true && UpgradeMod::getU_giant() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 3:
            if (UpgradeMod::getU_vitality() == true && UpgradeMod::getU_dodge() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 4:
            if ((UpgradeMod::getU_giant() == true || UpgradeMod::getU_dodge() == true) && UpgradeMod::getU_regeneration() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 5:
            if (UpgradeMod::getU_hollowpoint() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 6:
            if (UpgradeMod::getU_hollowpoint() == true && UpgradeMod::getU_doubleshot() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 7:
            if (UpgradeMod::getU_hollowpoint() == true && UpgradeMod::getU_heavybarrel() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 8:
            if ((UpgradeMod::getU_doubleshot() == true || UpgradeMod::getU_heavybarrel() == true) && UpgradeMod::getU_reaper() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 9:
            if (UpgradeMod::getU_fasthand() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 10:
            if (UpgradeMod::getU_fasthand() == true && UpgradeMod::getU_overload() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 11:
            if (UpgradeMod::getU_fasthand() == true && UpgradeMod::getU_lightbullet() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 12:
            if ((UpgradeMod::getU_overload() == true || UpgradeMod::getU_lightbullet() == true) && UpgradeMod::getU_barrage() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 13:
            if (UpgradeMod::getU_streamline() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 14:
            if (UpgradeMod::getU_streamline() == true && UpgradeMod::getU_sniper() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 15:
            if (UpgradeMod::getU_streamline() == true && UpgradeMod::getU_takeaim() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 16:
            if ((UpgradeMod::getU_sniper() == true || UpgradeMod::getU_takeaim() == true) && UpgradeMod::getU_pierecinground() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 17:
            if (UpgradeMod::getU_haste() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 18:
            if (UpgradeMod::getU_haste() == true && UpgradeMod::getU_overspeed() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 19:
            if (UpgradeMod::getU_haste() == true && UpgradeMod::getU_runngun() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        case 20:
            if ((UpgradeMod::getU_overspeed() == true || UpgradeMod::getU_runngun() == true) && UpgradeMod::getU_inthewind() == false) {
                list.append(type_index);
            } else {
                i--;
                continue;
            }
            break;
        default:
            break;
        }
    }
    return list;
}

void GameEngine::levelUp()
{
    QList<int> u_list = upgradeGenerator();
    QList<UpgradeMod*> mod_list;
    for (int i = 0; i < 3; ++i) {
        UpgradeMod *mod = new UpgradeMod(i, u_list[i], m_player, m_weapon, m_scene, this);
        mod_list.append(mod);
    }
    for (auto p : mod_list) {
        for (int i = 0; i < 3; ++i) {
            connect(p, &UpgradeMod::upgradeSelected, mod_list[i], &UpgradeMod::deleteLater);
        }
        connect(p, &UpgradeMod::upgradeSelected, this, &GameEngine::gameContinue);
        connect(p, &UpgradeMod::upgradeSelected, this, &GameEngine::setHealthBar);
    }
}

void GameEngine::setExpBar()
{
    int exp = m_player->getExp();
    float ratio = exp / (float)(6 + 4 * level);
    exp_bar->setRect(0, 0, 400 * ratio, 16);
    level_text->setPlainText(QString("Level %1").arg(level));
}

void GameEngine::dataIncrement()
{
    data_count++;
    if (data_count < (100 * (1 - 0.15 * data_collector))) return;
    data_count = 0;
    data_amount++;
    QString str("Data: ");
    str += QString::number(data_amount);
    data_text->setPlainText(str);
}

void GameEngine::loadArchive(int sign)
{
    bool is_saved = archive[sign]->value("is_saved", false).toBool();
    if (!is_saved) return;
    // 角色属性
    archive[sign]->beginGroup("player");
    int max_hp = archive[sign]->value("max_hp").toInt();
    int health = archive[sign]->value("health").toInt();
    qreal dodge = archive[sign]->value("dodge").toInt();
    qreal speed_boost = archive[sign]->value("speed_boost").toInt();
    QPointF player_pos = archive[sign]->value("player_pos").toPointF();
    qreal player_scale = archive[sign]->value("player_scale").toReal();
    bool inthewind = archive[sign]->value("inthewind").toInt();
    int exp = archive[sign]->value("exp").toInt();
    archive[sign]->endGroup();
    m_player->setMax_hp(max_hp);
    m_player->setHealth(health);
    m_player->setDodge(dodge);
    m_player->setSpeed_boost(speed_boost);
    m_player->setScale(player_scale);
    m_player->setU_inthewind(inthewind);
    m_player->setExp(exp);
    QPointF delta_pos = player_pos - m_player->pos();
    m_background->moveBy(-delta_pos.x(), -delta_pos.y());
    m_player->setPos(player_pos);
    setHealthBar();

    // 武器属性
    archive[sign]->beginGroup("weapon");
    weapon_type = archive[sign]->value("weapon_type").toString();
    qreal damage_boost = archive[sign]->value("damage_boost").toReal();
    qreal firerate_boost = archive[sign]->value("firerate_boost").toReal();
    qreal bullet_speed_boost = archive[sign]->value("bullet_speed_boost").toReal();
    qreal spread = archive[sign]->value("spread").toReal();
    int round_boost = archive[sign]->value("round_boost").toInt();
    int barrage = archive[sign]->value("barrage").toInt();
    int penetration = archive[sign]->value("penetration").toInt();
    archive[sign]->endGroup();
    m_scene->removeItem(m_weapon);
    delete m_weapon;
    if (weapon_type == "smg") m_weapon = new WeaponSmg(m_player, m_scene, this);
    else if (weapon_type == "smart") m_weapon = new WeaponSmart(m_player, m_scene, this);
    else if (weapon_type == "shotgun") m_weapon = new WeaponShotgun(m_player, m_scene, this);
    else if (weapon_type == "dmr") m_weapon = new WeaponDmr(m_player, m_scene, this);
    m_weapon->setParentItem(m_background);
    m_weapon->setDamageBoost(damage_boost);
    m_weapon->setFireRateBoost(firerate_boost);
    m_weapon->setSpeedBoost(bullet_speed_boost);
    m_weapon->setSpread(spread);
    m_weapon->setRoundBoost(round_boost);
    m_weapon->setBarrage(barrage);
    m_weapon->setPenetration(penetration);
    m_scene->addItem(m_weapon);
    // 武器移动，加载贴图，开火
    connect(main_timer, &QTimer::timeout, m_weapon, &WeaponBase::updatePos);
    connect(main_timer, &QTimer::timeout, m_weapon, &WeaponBase::load);
    connect(main_timer, &QTimer::timeout, m_weapon, &WeaponBase::fire);

    // 局内升级
    archive[sign]->beginGroup("upgrade");
    bool u_vitality = archive[sign]->value("u_vitality").toBool();
    bool u_giant = archive[sign]->value("u_giant").toBool();
    bool u_dodge = archive[sign]->value("u_dodge").toBool();
    bool u_regeneration = archive[sign]->value("u_regeneration").toBool();

    bool u_hollowpoint = archive[sign]->value("u_hollowpoint").toBool();
    bool u_doubleshot = archive[sign]->value("u_doubleshot").toBool();
    bool u_heavybarrel = archive[sign]->value("u_heavybarrel").toBool();
    bool u_reaper = archive[sign]->value("u_reaper").toBool();

    bool u_fasthand = archive[sign]->value("u_fasthand").toBool();
    bool u_overload = archive[sign]->value("u_overload").toBool();
    bool u_lightbullet = archive[sign]->value("u_lightbullet").toBool();
    bool u_barrage = archive[sign]->value("u_barrage").toBool();

    bool u_streamline = archive[sign]->value("u_streamline").toBool();
    bool u_sniper = archive[sign]->value("u_sniper").toBool();
    bool u_takeaim = archive[sign]->value("u_takeaim").toBool();
    bool u_pierecinground = archive[sign]->value("u_pierecinground").toBool();

    bool u_haste = archive[sign]->value("u_haste").toBool();
    bool u_overspeed = archive[sign]->value("u_overspeed").toBool();
    bool u_runngun = archive[sign]->value("u_runngun").toBool();
    bool u_inthewind = archive[sign]->value("u_inthewind").toBool();

    archive[sign]->endGroup();

    UpgradeMod::setU_vitality(u_vitality);
    UpgradeMod::setU_giant(u_giant);
    UpgradeMod::setU_dodge(u_dodge);
    UpgradeMod::setU_regeneration(u_regeneration);
    UpgradeMod::setU_hollowpoint(u_hollowpoint);
    UpgradeMod::setU_doubleshot(u_doubleshot);
    UpgradeMod::setU_heavybarrel(u_heavybarrel);
    UpgradeMod::setU_reaper(u_reaper);
    UpgradeMod::setU_fasthand(u_fasthand);
    UpgradeMod::setU_overload(u_overload);
    UpgradeMod::setU_lightbullet(u_lightbullet);
    UpgradeMod::setU_barrage(u_barrage);
    UpgradeMod::setU_streamline(u_streamline);
    UpgradeMod::setU_sniper(u_sniper);
    UpgradeMod::setU_takeaim(u_takeaim);
    UpgradeMod::setU_pierecinground(u_pierecinground);
    UpgradeMod::setU_haste(u_haste);
    UpgradeMod::setU_overspeed(u_overspeed);
    UpgradeMod::setU_runngun(u_runngun);
    UpgradeMod::setU_inthewind(u_inthewind);

    // 载入游戏阶段
    archive[sign]->beginGroup("gameStage");
    level = archive[sign]->value("level").toInt();
    phase = archive[sign]->value("phase").toInt();
    phase_count = archive[sign]->value("phase_count").toInt();
    countdown_time = archive[sign]->value("countdown_time").toTime();
    countdown_count = archive[sign]->value("countdown_count").toInt();
    archive[sign]->endGroup();
    switch (phase) {
    case 1:
        enemy_generate_rate = 10;
        melee_enemy_rate = 90;
        ranged_enemy_rate = 0;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 10;
        elite_enemy_rate = 0;
        break;
    case 2:
        enemy_generate_rate = 10;
        melee_enemy_rate = 70;
        ranged_enemy_rate = 10;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 0;
        break;
    case 3:
        enemy_generate_rate = 12.5;
        melee_enemy_rate = 70;
        ranged_enemy_rate = 10;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 0;
        break;
    case 4:
        enemy_generate_rate = 15;
        melee_enemy_rate = 70;
        ranged_enemy_rate = 10;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 0;
        break;
    case 5:
        enemy_generate_rate = 15;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 30;
        break;
    case 6:
        enemy_generate_rate = 20;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 30;
        break;
    case 7:
        enemy_generate_rate = 20;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 65;
        break;
    case 8:
        enemy_generate_rate = 30;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 65;
        break;
    case 9:
        enemy_generate_rate = 50;
        melee_enemy_rate = 60;
        ranged_enemy_rate = 20;
        charge_enemy_rate = 0;
        explosive_enemy_rate = 20;
        elite_enemy_rate = 100;
        break;
    case 10:
        endgame_win();
        break;
    default:
        break;
    }

    // 敌人重新生成
    QVariantList enemyList = archive[sign]->value("enemyList").toList();
    for (auto p : enemyList) {
        QPointF enemyPos;
        qreal dist;
        EnemyBase *enemy = nullptr;
        do {
            enemyPos.setX(QRandomGenerator::global()->bounded(0, 4800));
            enemyPos.setY(QRandomGenerator::global()->bounded(0, 2700));
            dist = QLineF(m_player->pos(), enemyPos).length();

        } while (dist < 550
                 || dist > 650
                 || pilebounds1.contains(enemyPos)
                 || pilebounds2.contains(enemyPos)
                 || pilebounds3.contains(enemyPos)
                 || pilebounds4.contains(enemyPos)
                 || pilebounds5.contains(enemyPos));   // 不在视野范围或过近时重新生成

        if (p.toMap().value("type") == "bot") {
            enemy = new EnemyBot(m_player, m_scene, this);
        } else if (p.toMap().value("type") == "drone") {
            enemy = new EnemyRangedDrone(m_player, m_scene, this);
        } else if (p.toMap().value("type") == "ball") {
            enemy = new EnemyExplosiveBall(m_player, m_scene, this);
        } else if (p.toMap().value("type") == "bot_elite") {
            enemy = new EnemyBotElite(m_player, m_scene, this);
        } else if (p.toMap().value("type") == "drone_elite") {
            enemy = new EnemyRangedDroneElite(m_player, m_scene, this);
        }
        if (!enemy) return;
        enemy->setParentItem(m_background);
        enemy->setPos(enemyPos);
        enemy->setZValue(1);
        // 将敌人添加到场景中
        m_scene->addItem(enemy);
    }
}

void GameEngine::saveArchive(int sign)
{
    // 是否存在存档
    bool is_saved = true;
    archive[sign]->setValue("is_saved", is_saved);
    // 角色属性
    int max_hp = m_player->getMax_hp();
    int health = m_player->getHealth();
    qreal dodge = m_player->getDodge();
    qreal speed_boost = m_player->getSpeed_boost();
    QPointF player_pos = m_player->pos();
    qreal player_scale = m_player->scale();
    bool inthewind = m_player->getU_inthewind();
    int exp = m_player->getExp();
    archive[sign]->beginGroup("player");
    archive[sign]->setValue("max_hp", max_hp);
    archive[sign]->setValue("health", health);
    archive[sign]->setValue("dodge", dodge);
    archive[sign]->setValue("speed_boost", speed_boost);
    archive[sign]->setValue("player_pos", player_pos);
    archive[sign]->setValue("player_scale", player_scale);
    archive[sign]->setValue("exp", exp);
    archive[sign]->setValue("inthewind", inthewind);
    archive[sign]->endGroup();
    // 武器属性
    qreal damage_boost = m_weapon->getDamageBoost();
    qreal firerate_boost = m_weapon->getFireRateBoost();
    qreal bullet_speed_boost = m_weapon->getSpeedBoost();
    qreal spread = m_weapon->getSpread();
    int round_boost = m_weapon->getRoundBoost();
    int barrage = m_weapon->getBarrage();
    int penetration = m_weapon->getPenetration();
    archive[sign]->beginGroup("weapon");
    archive[sign]->setValue("weapon_type", weapon_type);
    archive[sign]->setValue("damage_boost", damage_boost);
    archive[sign]->setValue("firerate_boost", firerate_boost);
    archive[sign]->setValue("bullet_speed_boost", bullet_speed_boost);
    archive[sign]->setValue("spread", spread);
    archive[sign]->setValue("round_boost", round_boost);
    archive[sign]->setValue("barrage", barrage);
    archive[sign]->setValue("penetration", penetration);
    archive[sign]->endGroup();
    // 局内升级
    bool u_vitality = UpgradeMod::getU_vitality(); // +2HP 活力
    bool u_giant = UpgradeMod::getU_giant(); // +4HP +25%size 巨人化
    bool u_dodge = UpgradeMod::getU_dodge(); // +30%dodge 闪避
    bool u_regeneration = UpgradeMod::getU_dodge(); //+1HP/30s 自动修复

    bool u_hollowpoint = UpgradeMod::getU_hollowpoint(); // +25%damage 空尖子弹
    bool u_doubleshot = UpgradeMod::getU_doubleshot(); // +1round, -20%damage 二重射击
    bool u_heavybarrel = UpgradeMod::getU_heavybarrel(); // +40%damage, -10%firerate 重型枪管
    bool u_reaper = UpgradeMod::getU_reaper(); // +15%damage, +1penetration 死神收割

    bool u_fasthand = UpgradeMod::getU_fasthand(); // +15%firerate 快速射击
    bool u_overload = UpgradeMod::getU_overload(); // +25%firerate, +30%spread 超载
    bool u_lightbullet = UpgradeMod::getU_lightbullet(); // +20%firerate, -10%damage 轻型子弹
    bool u_barrage = UpgradeMod::getU_barrage(); // +35%firerate while standing still 弹雨

    bool u_streamline = UpgradeMod::getU_streamline(); // +30%roundspeed 流线构型
    bool u_sniper = UpgradeMod::getU_sniper(); // +15%roundspeed -20%spread 狙击
    bool u_takeaim = UpgradeMod::getU_takeaim(); // -50%spread -10%firerate 精确打击
    bool u_pierecinground = UpgradeMod::getU_pierecinground(); // +2penetration 穿甲子弹

    bool u_haste = UpgradeMod::getU_haste(); // +15%speed 急促脚步
    bool u_overspeed = UpgradeMod::getU_overspeed(); // +25%speed +20%spread 超速
    bool u_runngun = UpgradeMod::getU_runngun(); // +10%speed +10%firerate 跑射
    bool u_inthewind = UpgradeMod::getU_inthewind(); // +30%firerate&+20%speed when not get hurt in 30s

    archive[sign]->beginGroup("upgrade");

    archive[sign]->setValue("u_vitality", u_vitality);
    archive[sign]->setValue("u_giant", u_giant);
    archive[sign]->setValue("u_dodge", u_dodge);
    archive[sign]->setValue("u_regeneration", u_regeneration);

    archive[sign]->setValue("u_hollowpoint", u_hollowpoint);
    archive[sign]->setValue("u_doubleshot", u_doubleshot);
    archive[sign]->setValue("u_heavybarrel", u_heavybarrel);
    archive[sign]->setValue("u_reaper", u_reaper);

    archive[sign]->setValue("u_fasthand", u_fasthand);
    archive[sign]->setValue("u_overload", u_overload);
    archive[sign]->setValue("u_lightbullet", u_lightbullet);
    archive[sign]->setValue("u_barrage", u_barrage);

    archive[sign]->setValue("u_streamline", u_streamline);
    archive[sign]->setValue("u_sniper", u_sniper);
    archive[sign]->setValue("u_takeaim", u_takeaim);
    archive[sign]->setValue("u_pierecinground", u_pierecinground);

    archive[sign]->setValue("u_haste", u_haste);
    archive[sign]->setValue("u_overspeed", u_overspeed);
    archive[sign]->setValue("u_runngun", u_runngun);
    archive[sign]->setValue("u_inthewind", u_inthewind);

    archive[sign]->endGroup();

    // 敌人存储
    QVariantList enemyList;
    QList<QGraphicsItem*> itemlist = m_scene->items();
    for (auto item : itemlist) {
        QVariantMap enemyMap;
        EnemyBot *enemybot = dynamic_cast<EnemyBot*>(item);
        if (enemybot) {
            enemyMap.insert("type", "bot");
            enemyMap.insert("pos", enemybot->pos());
            continue;
        }
        EnemyRangedDrone *enemydrone = dynamic_cast<EnemyRangedDrone*>(item);
        if (enemydrone) {
            enemyMap.insert("type", "drone");
            enemyMap.insert("pos", enemydrone->pos());
            continue;
        }
        EnemyExplosiveBall *enemyball = dynamic_cast<EnemyExplosiveBall*>(item);
        if (enemyball) {
            enemyMap.insert("type", "ball");
            enemyMap.insert("pos", enemyball->pos());
            continue;
        }
        EnemyBotElite *enemybotelite = dynamic_cast<EnemyBotElite*>(item);
        if (enemybotelite) {
            enemyMap.insert("type", "bot_elite");
            enemyMap.insert("pos", enemybotelite->pos());
            continue;
        }
        EnemyRangedDroneElite *enemydroneelite = dynamic_cast<EnemyRangedDroneElite*>(item);
        if (enemydroneelite) {
            enemyMap.insert("type", "drone_elite");
            enemyMap.insert("pos", enemydroneelite->pos());
            continue;
        }
        enemyList.append(enemyMap);
    }
    archive[sign]->beginGroup("enemy");
    archive[sign]->setValue("enemyList", enemyList);
    archive[sign]->endGroup();

    // 游戏阶段
    archive[sign]->beginGroup("gameStage");
    archive[sign]->setValue("phase", phase);
    archive[sign]->setValue("phase_count", phase_count);
    archive[sign]->setValue("countdown_time", countdown_time);
    archive[sign]->setValue("countdown_count", countdown_count);
    archive[sign]->setValue("level", level);
    archive[sign]->endGroup();
}

void GameEngine::showArchive()
{
    for (int i = 0; i < 3; ++i) {
        archive_mod[i] = new ArchiveMod(i, m_scene, this);
        connect(archive_mod[i], &ArchiveMod::saveArchive, this, [=](int index){
            saveArchive(index);
        });
    }
}

void GameEngine::hideArchive()
{
    for (int i = 0; i < 3; ++i) {
        archive_mod[i]->disconnect();
        archive_mod[i]->hideAndDestroy();
    }
}

void GameEngine::endgame_lose() {
    int respawn_udisk = settings->value("Respawn Udisk", 0).toInt();
    if (respawn_udisk > 0) {
        m_player->setHealth(m_player->getMax_hp());
        setHealthBar();
        respawn_udisk--;
        settings->setValue("Respawn Udisk", respawn_udisk);
        sfx_respawn->play();
        return;
    }
    // 读取样式表
    QFile file_sqr_pb(":/style/stylesheet/sqr_pb.qss");
    file_sqr_pb.open(QFile::ReadOnly);
    QString style_sqr_pb;
    if (file_sqr_pb.isOpen()) {
        style_sqr_pb = QLatin1String(file_sqr_pb.readAll());
        file_sqr_pb.close();
    }
    // 重置所有局内升级
    UpgradeMod::resetAll();
    // 停止所有计时器
    main_timer->stop();
    // 停止背景音乐
    bgm_player->stop();
    // 删除血条
    for (auto *p : heart_list) {
        if (p) p->deleteLater();
    }
    // 保存
    settings->setValue("data", data_amount);
    // 显示游戏结束界面
    end_scene = new QGraphicsScene(this);
    end_scene->setSceneRect(0, 0, 1280, 720);
    setScene(end_scene);
    end_scene->setFocus();
    // 背景
    end_scene->addPixmap(QPixmap(":/images/images/ABackground.png"));
    // 文字
    QGraphicsTextItem *end_text = new QGraphicsTextItem("You Lose!");
    end_text->setFont(QFont("fusion pixel proportional", 72));
    end_text->setDefaultTextColor(Qt::red);
    end_text->setPos(420, 260);
    end_scene->addItem(end_text);
    // 返回按钮
    QPushButton *return_btn = new QPushButton("back");
    end_scene->addWidget(return_btn);
    return_btn->move(120, 540);
    return_btn->setStyleSheet(style_sqr_pb);
    return_btn->resize(110, 110);
    connect(return_btn, &QPushButton::clicked, this, [=](){
        emit Endgame();
        deleteLater();
    });
}

void GameEngine::endgame_win() {
    // 读取样式表
    QFile file_sqr_pb(":/style/stylesheet/sqr_pb.qss");
    file_sqr_pb.open(QFile::ReadOnly);
    QString style_sqr_pb;
    if (file_sqr_pb.isOpen()) {
        style_sqr_pb = QLatin1String(file_sqr_pb.readAll());
        file_sqr_pb.close();
    }
    // 重置所有局内升级
    UpgradeMod::resetAll();
    // 停止所有计时器
    main_timer->stop();
    // 停止背景音乐
    bgm_player->stop();
    // 删除血条
    for (auto *p : heart_list) {
        if (p) p->deleteLater();
    }
    // 保存
    settings->setValue("data", data_amount);
    // 显示游戏结束界面
    end_scene = new QGraphicsScene(this);
    end_scene->setSceneRect(0, 0, 1280, 720);
    setScene(end_scene);
    // 背景
    end_scene->addPixmap(QPixmap(":/images/images/ABackground.png"));
    // 文字
    QGraphicsTextItem *end_text = new QGraphicsTextItem("You Win!");
    end_text->setFont(QFont("fusion pixel proportional", 72));
    end_text->setDefaultTextColor(Qt::green);
    end_text->setPos(420, 260);
    end_scene->addItem(end_text);
    // 返回按钮
    QPushButton *return_btn = new QPushButton("back");
    end_scene->addWidget(return_btn);
    return_btn->move(120, 540);
    return_btn->setStyleSheet(style_sqr_pb);
    return_btn->resize(110, 110);
    connect(return_btn, &QPushButton::clicked, this, [=](){
        emit Endgame();
        deleteLater();
    });
}
