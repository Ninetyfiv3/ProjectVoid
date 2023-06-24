
#include "heartLabel.h"

HeartLabel::HeartLabel(QWidget *parent) : QLabel{parent}
{
    heart_full = QPixmap(":/images/images/icons/heart_full.png");
    heart_empty = QPixmap(":/images/images/icons/heart_empty.png");
    setPixmap(heart_full);
    setGeometry(1220, 44, 32, 32);
    show();
}

void HeartLabel::loseHeart()
{
    setPixmap(heart_empty);
}

void HeartLabel::gainHeart()
{
    setPixmap(heart_full);
}
