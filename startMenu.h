
#ifndef STARTMENU_H
#define STARTMENU_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPushButton>
#include <QToolButton>
#include <QGraphicsEffect>
#include <QFontDatabase>
#include <QSettings>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSoundEffect>
#include <QEvent>
#include <QFile>
#include <QUrl>


class StartMenu : public QGraphicsView
{
    Q_OBJECT
public:
    StartMenu();
    void open_prepare();
    void open_main();

signals:
    void startGame();
    void weaponSelect(QString weapon);
    void loadArchive(QString archive);

protected:

private:
    QGraphicsScene *s_main;
    QGraphicsScene *s_prepare;
    QGraphicsScene *s_shop;
    QGraphicsScene *s_archive;


    QSettings *settings;
    int data_amount;


};

#endif // STARTMENU_H
