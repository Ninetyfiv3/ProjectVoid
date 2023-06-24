QT       += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    archiveMod.cpp \
    bulletBase.cpp \
    bulletDmr.cpp \
    bulletEnemy.cpp \
    bulletShotgun.cpp \
    bulletSmart.cpp \
    bulletSmg.cpp \
    enemyBase.cpp \
    enemyBot.cpp \
    enemyBotElite.cpp \
    enemyExplosiveBall.cpp \
    enemyExplosiveBase.cpp \
    enemyRangedBase.cpp \
    enemyRangedDrone.cpp \
    enemyRangedDroneElite.cpp \
    exp.cpp \
    gameEngine.cpp \
    gameWidget.cpp \
    heartLabel.cpp \
    main.cpp \
    player.cpp \
    qGraphicsButton.cpp \
    qGraphicsSelectiveButton.cpp \
    shopItem.cpp \
    startMenu.cpp \
    upgradeMod.cpp \
    weaponBase.cpp \
    weaponDmr.cpp \
    weaponMod.cpp \
    weaponShotgun.cpp \
    weaponSmart.cpp \
    weaponSmg.cpp

HEADERS += \
    archiveMod.h \
    bulletBase.h \
    bulletDmr.h \
    bulletEnemy.h \
    bulletShotgun.h \
    bulletSmart.h \
    bulletSmg.h \
    enemyBase.h \
    enemyBot.h \
    enemyBotElite.h \
    enemyExplosiveBall.h \
    enemyExplosiveBase.h \
    enemyRangedBase.h \
    enemyRangedDrone.h \
    enemyRangedDroneElite.h \
    exp.h \
    gameEngine.h \
    gameWidget.h \
    heartLabel.h \
    player.h \
    qGraphicsButton.h \
    qGraphicsSelectiveButton.h \
    shopItem.h \
    startMenu.h \
    upgradeMod.h \
    weaponBase.h \
    weaponDmr.h \
    weaponMod.h \
    weaponShotgun.h \
    weaponSmart.h \
    weaponSmg.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
