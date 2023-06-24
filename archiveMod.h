#ifndef ARCHIVEMOD_H
#define ARCHIVEMOD_H

#include "qGraphicsButton.h"
#include <QObject>
#include <QGraphicsScene>
#include <QSettings>

class ArchiveMod : public QObject
{
    Q_OBJECT
public:
    explicit ArchiveMod(int index, QGraphicsScene *scene, QObject *parent = nullptr);
    void hideAndDestroy();

signals:
    void saveArchive(int sign);
    void loadArchive(int sign);

private:
    int sign;
    QGraphicsButton *btn;
    QGraphicsTextItem *desc;
    QGraphicsScene *m_scene;
    QSettings *archive;
    void mouseClick();
};

#endif // ARCHIVEMOD_H
