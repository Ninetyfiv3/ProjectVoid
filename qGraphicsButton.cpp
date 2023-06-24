#include "qGraphicsButton.h"

QGraphicsButton::QGraphicsButton(QString path, QPointF pos, QObject *parent)
    : QObject{parent}, ic_path(path)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    QString pt = ic_path + "_idle";
    setPixmap((QPixmap(pt)));
    setPos(pos);
}

void QGraphicsButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QString path = ic_path + "_hover.png";
    setPixmap((QPixmap(path)));
    is_hovered = true;
    emit hoverEnter();
}

void QGraphicsButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QString path = ic_path + "_idle.png";
    setPixmap((QPixmap(path)));
    is_hovered = false;
    emit hoverLeave();
}

void QGraphicsButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    is_pressed = true;
}

void QGraphicsButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_pressed && is_hovered) emit mouseClick();
    is_pressed = false;
}
