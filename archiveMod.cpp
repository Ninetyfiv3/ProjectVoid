#include "archiveMod.h"
#include "QFontDatabase"

ArchiveMod::ArchiveMod(int index, QGraphicsScene *scene, QObject *parent)
    : QObject{parent}, sign(index), m_scene(scene)
{
    QFontDatabase::addApplicationFont(":/fonts/fonts/fusion-pixel-proportional.ttf");
    btn = new QGraphicsButton(":/images/images/icons/buttons/ic_blank", QPointF(640 - 16, 180 * sign + 180), this);
    btn->setTransformOriginPoint(btn->boundingRect().center());
    btn->setScale(6);
    m_scene->addItem(btn);

    QString fileName = QString("./archive_%1.ini").arg(sign);
    archive = new QSettings(fileName, QSettings::IniFormat, this);
    bool is_saved = archive->value("is_saved", false).toBool();

    desc = new QGraphicsTextItem();
    desc->setDefaultTextColor(Qt::white);
    desc->setFont(QFont("fusion pixel proportional", 16));
    if (is_saved) {
        desc->setPlainText("Saved");
    } else {
        desc->setPlainText("Empty");
    }
    desc->setPos(640 - desc->boundingRect().width() / 2, 180 * sign + 180 + desc->boundingRect().height() / 2);
    m_scene->addItem(desc);

    connect(btn, &QGraphicsButton::mouseClick, this, &ArchiveMod::mouseClick);
}

void ArchiveMod::hideAndDestroy()
{
    m_scene->removeItem(btn);
    m_scene->removeItem(desc);
    delete desc;
    deleteLater();
}

void ArchiveMod::mouseClick()
{
    emit saveArchive(sign);
    emit loadArchive(sign);
    bool is_saved = archive->value("is_saved", false).toBool();
    if (is_saved) {
        desc->setPlainText("Saved");
    } else {
        desc->setPlainText("Empty");
    }
    desc->setPos(640 - desc->boundingRect().width() / 2, 180 * sign + 180 + desc->boundingRect().height() / 2);
    m_scene->addItem(desc);
}
