#include "qGraphicsSelectiveButton.h"

QGraphicsSelectiveButton::QGraphicsSelectiveButton(QString path, QPointF pos, QObject *parent)
    : QGraphicsButton{path, pos, parent}
{

}

void QGraphicsSelectiveButton::resetStatus()
{
    is_selected = false;
    QString path = ic_path + "_idle.png";
    setPixmap((QPixmap(path)));
}
void QGraphicsSelectiveButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverEnter();
    if (is_selected) return;
    QString path = ic_path + "_hover.png";
    setPixmap((QPixmap(path)));
    is_hovered = true;
}

void QGraphicsSelectiveButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverLeave();
    if (is_selected) return;
    QString path = ic_path + "_idle.png";
    setPixmap((QPixmap(path)));
    is_hovered = false;
}

void QGraphicsSelectiveButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    is_pressed = true;
}

void QGraphicsSelectiveButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_pressed && is_hovered) {
        emit mouseClick();
        QString path = ic_path + "_selected.png";
        setPixmap((QPixmap(path)));
    }
    is_pressed = false;
    is_selected = true;

}
