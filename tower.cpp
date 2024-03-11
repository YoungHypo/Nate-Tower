#include "tower.h"
#include <QVector2D>

Tower::Tower(QPoint point, QString pName, GameScene * g,double pSize, QObject *parent)
    : QObject{parent},
      pos(point),
      attackTimer(NULL),
      lockedEmemy(NULL),
      game(g),
      pixSize(pSize),
      attackRate(0),
      coin(40)
{
    pix=QPixmap(pName);
    pix=pix.scaled(QSize(pix.width()*pixSize,pix.height()*pixSize),Qt::KeepAspectRatio);
}

void Tower::Draw(QPainter * painter)
{
    painter->save();
    const QPoint offsetPoint(-pix.width()*0.5,-pix.height()*0.5);
    painter->translate(pos);
    painter->drawPixmap(offsetPoint,pix);
    painter->restore();
}
