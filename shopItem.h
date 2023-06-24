#ifndef SHOPITEM_H
#define SHOPITEM_H

#include <QObject>
#include <QPixmap>

class ShopItem : public QObject
{
public:
    ShopItem(QString name_, int price_, int level_, int max_level_, QPixmap icon_, QObject *parent = nullptr);

    int getPrice() const;
    void setPrice(int newPrice);

    int getLevel() const;
    void setLevel(int newLevel);

    int getMax_level() const;

    QPixmap getIcon() const;

    QString getName() const;

private:
    QString name;
    int price;
    int level;
    int max_level;
    QPixmap icon;
};

#endif // SHOPITEM_H
