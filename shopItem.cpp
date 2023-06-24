#include "shopItem.h"

ShopItem::ShopItem(QString name_, int price_, int level_, int max_level_, QPixmap icon_, QObject *parent)
    : QObject{parent}, name(name_), price(price_), level(level_), max_level(max_level_), icon(icon_)
{

}

int ShopItem::getPrice() const
{
    return price;
}

void ShopItem::setPrice(int newPrice)
{
    price = newPrice;
}

int ShopItem::getLevel() const
{
    return level;
}

void ShopItem::setLevel(int newLevel)
{
    level = newLevel;
}

int ShopItem::getMax_level() const
{
    return max_level;
}

QPixmap ShopItem::getIcon() const
{
    return icon;
}

QString ShopItem::getName() const
{
    return name;
}
