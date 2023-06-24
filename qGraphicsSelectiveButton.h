#ifndef QGRAPHICSSELECTIVEBUTTON_H
#define QGRAPHICSSELECTIVEBUTTON_H

#include "qGraphicsButton.h"
#include <QObject>

class QGraphicsSelectiveButton : public QGraphicsButton
{
    Q_OBJECT
public:
    explicit QGraphicsSelectiveButton(QString path, QPointF pos, QObject *parent = nullptr);

    void resetStatus();
signals:
    void selected();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool is_selected = false;
};

#endif // QGRAPHICSSELECTIVEBUTTON_H
