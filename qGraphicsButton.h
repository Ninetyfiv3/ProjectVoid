#ifndef QGRAPHICSBUTTON_H
#define QGRAPHICSBUTTON_H

#include <QGraphicsPixmapItem>
#include <QObject>

class QGraphicsButton : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    QGraphicsButton(QString path, QPointF pos, QObject *parent = nullptr);
signals:
    void hoverEnter();
    void hoverLeave();
    void mouseClick();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QString ic_path;
    bool is_hovered = false;
    bool is_pressed = false;
};

#endif // QGRAPHICSBUTTON_H
