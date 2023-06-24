
#include "startMenu.h"
#include "archiveMod.h"
#include "shopItem.h"
#include "weaponMod.h"

StartMenu::StartMenu()
{
    // 加载字体
    QFontDatabase::addApplicationFont(":/fonts/fonts/fusion-pixel-proportional.ttf");
    // 初始化view
    setFixedSize(1280, 720);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 初始化s_main
    s_main = new QGraphicsScene(this);
    s_main->setSceneRect(0, 0, 1280, 720);
    s_main->addPixmap(QPixmap(":/images/images/ABackground.png"));
    // 背景音乐
    QSoundEffect *bgm = new QSoundEffect(this);
    bgm->setSource(QUrl::fromLocalFile(":/bgm/bgm/infinite night sky - DDRKirby(ISQ).wav"));
    bgm->setVolume(0.1);
    bgm->setLoopCount(0x3ffffff);
    bgm->play();
    // Logo
    QGraphicsPixmapItem *logo = new QGraphicsPixmapItem(QPixmap(":/images/images/icons/ic_logo.png"));
    logo->setTransformOriginPoint(logo->boundingRect().center());
    logo->setScale(2);
    logo->setPos(480, 120);
    s_main->addItem(logo);
    // 装饰
    QGraphicsPixmapItem *leaf_l = new QGraphicsPixmapItem(QPixmap(":/images/images/T_TitleLeaves.png"));
    QGraphicsPixmapItem *leaf_r = new QGraphicsPixmapItem(QPixmap::fromImage(QImage(":/images/images/T_TitleLeaves.png").mirrored(true, false)));
    leaf_r->setTransformOriginPoint(leaf_r->boundingRect().right(), leaf_r->boundingRect().top());
    leaf_r->setPos(1034, 0);
    leaf_l->setScale(1.6);
    leaf_r->setScale(1.6);
    s_main->addItem(leaf_l);
    s_main->addItem(leaf_r);
    // 读取设置
    settings = new QSettings("./game.ini", QSettings::IniFormat, this);
    data_amount = settings->value("data").toInt();
    // 读取样式表
    QFile file_main_pb(":/style/stylesheet/main_pb.qss");
    file_main_pb.open(QFile::ReadOnly);
    QString style_main_pb;
    if (file_main_pb.isOpen()) {
        style_main_pb = QLatin1String(file_main_pb.readAll());
        file_main_pb.close();
    }
    QFile file_sqr_pb(":/style/stylesheet/sqr_pb.qss");
    file_sqr_pb.open(QFile::ReadOnly);
    QString style_sqr_pb;
    if (file_sqr_pb.isOpen()) {
        style_sqr_pb = QLatin1String(file_sqr_pb.readAll());
        file_sqr_pb.close();
    }
    // 开始按钮
    QPushButton *main_start_button = new QPushButton("start");
    main_start_button->move(530, 480);
    main_start_button->resize(220, 110);
    main_start_button->setStyleSheet(style_main_pb);

    connect(main_start_button, &QPushButton::clicked, this, &StartMenu::open_prepare);

    s_main->addWidget(main_start_button);

    // 初始化s_prepare
    s_prepare = new QGraphicsScene(this);
    s_prepare->setSceneRect(0, 0, 1280, 720);
    // 背景图片
    s_prepare->addPixmap(QPixmap(":/images/images/ABackground.png"));

    QPushButton *prepare_backButton = new QPushButton("back");
    s_prepare->addWidget(prepare_backButton);
    prepare_backButton->move(100, 60);
    prepare_backButton->resize(110, 110);
    prepare_backButton->setStyleSheet(style_sqr_pb);
    connect(prepare_backButton, &QPushButton::clicked, this, &StartMenu::open_main);

    QPushButton *prp_archive_btn = new QPushButton("archive");
    s_prepare->addWidget(prp_archive_btn);
    prp_archive_btn->setStyleSheet(style_sqr_pb);
    prp_archive_btn->setFixedSize(110, 110);
    prp_archive_btn->move(260, 480);
    connect(prp_archive_btn, &QPushButton::clicked, this, [=](){
        setScene(s_archive);
    });

    QPushButton *prp_start_btn = new QPushButton("Start Game");
    s_prepare->addWidget(prp_start_btn);
    prp_start_btn->setStyleSheet(style_main_pb);
    prp_start_btn->resize(220, 110);
    prp_start_btn->move(530, 480);
    connect(prp_start_btn, &QPushButton::clicked, this, [=](){
        emit startGame();
        deleteLater();
    });

    QPushButton *prp_shop_btn = new QPushButton("Shop");
    s_prepare->addWidget(prp_shop_btn);
    prp_shop_btn->setStyleSheet(style_sqr_pb);
    prp_shop_btn->setFixedSize(110, 110);
    prp_shop_btn->move(960, 480);
    connect(prp_shop_btn, &QPushButton::clicked, this, [=](){
        setScene(s_shop);
    });

    QList<WeaponMod*> weapon_list;
    for (int i = 0; i < 4; ++i) {
        WeaponMod *wp_btn = new WeaponMod(i, i + 1, s_prepare, this);
        connect(wp_btn, &WeaponMod::weaponSelected, this, [=](QString w){
            emit weaponSelect(w);
        });
        weapon_list.append(wp_btn);
    }
    for (auto p : weapon_list) {
        for (auto q : weapon_list) {
            if (p == q) continue;
            connect(p, &WeaponMod::weaponSelected, q, &WeaponMod::resetButton);
        }
    }
    // 初始化s_shop
    s_shop = new QGraphicsScene(this);
    s_shop->setSceneRect(0, 0, 1280, 720);

    QGraphicsTextItem *data_text = new QGraphicsTextItem("Data: " + QString::number(data_amount));
    s_shop->addItem(data_text);
    data_text->setFont(QFont("fusion pixel proportional", 24));
    data_text->setDefaultTextColor(Qt::black);
    data_text->setPos(24, 0);

    QPushButton *sp_back_btn = new QPushButton("back");
    s_shop->addWidget(sp_back_btn);
    sp_back_btn->setFont(QFont("fusion pixel proportional", 12));
    sp_back_btn->move(100, 100);
    connect(sp_back_btn, &QPushButton::clicked, this, [=](){
        setScene(s_prepare);
    });

    // 添加商品
    QList<ShopItem*> items;
    int hp_increment_lvl = settings->value("HP Increment", 0).toInt();
    int muzzle_ckoke_lvl = settings->value("Data Collector", 0).toInt();
    int data_collector_lvl = settings->value("Muzzle Choke", 0).toInt();
    int respawn_udisk_amount = settings->value("Respawn Udisk", 0).toInt();
    items.append(new ShopItem("HP Increment", 500, hp_increment_lvl, 5, QPixmap(":/images/images/icons/ic_hp_increment.png")));
    items.append(new ShopItem("Data Collector", 1000, data_collector_lvl, 5, QPixmap(":/images/images/icons/ic_data_collector.png")));
    items.append(new ShopItem("Muzzle Choke", 750, muzzle_ckoke_lvl, 3, QPixmap(":/images/images/icons/ic_muzzle_choke.png")));
    items.append(new ShopItem("Respawn Udisk", 400, respawn_udisk_amount, 1, QPixmap(":/images/images/icons/ic_respawn_udisk.png")));
    int index = 0;
    for (ShopItem *item : items) {
//        // 图标
        QGraphicsPixmapItem *icon = new QGraphicsPixmapItem(item->getIcon());
        s_shop->addItem(icon);
        icon->setScale(5);
        icon->setPos(200 * (index+1) + 40, 200);
        // 购买按钮
        QPushButton *itemBtn = new QPushButton;
        itemBtn->setText(item->getName() + "\nPrice: " + QString::number(item->getPrice()));
        itemBtn->setFont(QFont("fusion pixel proportional", 16));
        itemBtn->move(200 * (index+1), 400);
        s_shop->addWidget(itemBtn);
        // 检测是否可购买
        if (data_amount >= item->getPrice() && item->getLevel() < item->getMax_level()) {
            itemBtn->setEnabled(true);
        } else {
            itemBtn->setEnabled(false);
        }
        connect(itemBtn, &QPushButton::clicked, this, [=]() {
            data_amount -= item->getPrice();
            item->setLevel(item->getLevel() + 1);
            data_text->setPlainText("Data: " + QString::number(data_amount));
            // 存储数据
            settings->setValue("data", data_amount);
            settings->setValue(item->getName(), item->getLevel());
            // 刷新按钮
            if (data_amount >= item->getPrice() && item->getLevel() < item->getMax_level()) {
                itemBtn->setEnabled(true);
            } else {
                itemBtn->setEnabled(false);
            }
        });
        index++;
    }
    // 初始化s_archive
    s_archive = new QGraphicsScene(this);
    s_archive->addPixmap(QPixmap(":/images/images/ABackground.png"));
    for (int i = 0; i < 3; ++i) {
        ArchiveMod *archive_mod = new ArchiveMod(i, s_archive, this);
        connect(archive_mod, &ArchiveMod::loadArchive, this, [=](int sign){
            emit loadArchive(QString("archive%1").arg(sign));
            emit startGame();
        });
    }
    QPushButton *archive_back_btn = new QPushButton("back");
    s_archive->addWidget(archive_back_btn);
    archive_back_btn->move(100, 60);
    archive_back_btn->resize(110, 110);
    archive_back_btn->setStyleSheet(style_sqr_pb);
    connect(archive_back_btn, &QPushButton::clicked, this, &StartMenu::open_prepare);

    setScene(s_main);

}

void StartMenu::open_prepare()
{
    setScene(s_prepare);
}

void StartMenu::open_main()
{
    setScene(s_main);
}



