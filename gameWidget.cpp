
#include "gameWidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
    setFixedSize(1280, 720);
    setWindowIcon(QIcon(":/images/images/icons/Myicon.ico"));
    setWindowTitle("Project Void");
    weapon = "smg";
    init_startMenu();
    m_startMenu->show();
    QPixmap cursor_pic(":/images/images/ic_cursor.png");
    QCursor cursor(cursor_pic, -cursor_pic.width()/2, -cursor_pic.height()/2);
    setCursor(cursor);
}

void GameWidget::init_startMenu()
{
    m_startMenu = new StartMenu();
    m_startMenu->setParent(this);
    connect(m_startMenu, &StartMenu::startGame, this, &GameWidget::start_gameEngine);
    connect(m_startMenu, &StartMenu::weaponSelect, this, [=](QString w){
        weapon = w;
    });
    connect(m_startMenu, &StartMenu::loadArchive, this, [=](QString arc){
        archive = arc;
    });
}

void GameWidget::init_gameEngine()
{
    m_gameEngine = new GameEngine(weapon, archive);
    qInfo() << archive;
    m_gameEngine->setParent(this);
    weapon = "smg";
    connect(m_gameEngine, &GameEngine::Endgame, this, &GameWidget::end_game);
}

void GameWidget::start_gameEngine()
{
    m_startMenu->hide();
    init_gameEngine();
    m_gameEngine->show();
    m_gameEngine->setFocus();
}

void GameWidget::end_game()
{
    m_gameEngine->hide();
    disconnect(m_gameEngine, &GameEngine::Endgame, this, &GameWidget::end_game);
    init_startMenu();
    m_startMenu->show();
    m_startMenu->setFocus();
}


