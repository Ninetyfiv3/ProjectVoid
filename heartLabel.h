
#ifndef HEARTLABEL_H
#define HEARTLABEL_H

#include <QLabel>


class HeartLabel : public QLabel
{
public:
    HeartLabel(QWidget *parent = nullptr);
    void loseHeart();
    void gainHeart();
private:
    QPixmap heart_full;
    QPixmap heart_empty;
};

#endif // HEARTLABEL_H
