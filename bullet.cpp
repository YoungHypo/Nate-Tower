#include "bullet.h"
#include "cannon.h"
#include "Common.h"
#include "gamescene.h"
#include <QPropertyAnimation>
#include <QVector2D>

Bullet::Bullet(Cannon* can,Enemy* enem,QString pName,double pSize,QObject *parent)
    : QObject{parent},
      srcCannon(can),
      targetEnemy(enem),
      game(can->getGameScene()),
      animation(NULL),
      startPos(GetPointRotate(QPoint(srcCannon->OffsetWidth(),-srcCannon->DelaHeight()),srcCannon->RotatedDegree())+srcCannon->getPos()),
      targetPos(enem->getPos()),
      curPos(startPos),
      rotateDegree(srcCannon->RotatedDegree()),
      aggressivity(srcCannon->Aggressivity()),
      pixSize(pSize)
{
    pix=QPixmap(pName);
    pix=pix.scaled(QSize(pix.width()*pixSize,pix.height()*pixSize),Qt::KeepAspectRatio);
    double posDist=PointDistance(srcCannon->getPos(),targetPos);
    duratTime=(int)(UintDurat*(posDist/90));
}

Bullet::Bullet(QPoint spoint,QPoint epoint,GameScene* g,QObject *parent)
  : QObject{parent},
    srcCannon(NULL),
    targetEnemy(NULL),
    game(g),
    animation(NULL),
    startPos(spoint),
    targetPos(epoint),
    curPos(startPos),
    rotateDegree(0.0)
{

}

void Bullet::Draw(QPainter * painter)
{
    painter->save();
    QPoint offsetPoint(-pix.width()*0.5,-pix.height()*0.5);
    //转到炮口的位置
    painter->translate(curPos);
    painter->rotate(rotateDegree);
    painter->drawPixmap(offsetPoint,pix);
    painter->restore();
}

void Bullet::Move()
{
    animation = new QPropertyAnimation(this, "curPos");
    animation->setDuration(duratTime);
    animation->setStartValue(startPos);
    animation->setEndValue(targetPos);
    connect(animation, SIGNAL(finished()),this,SLOT(HitTarget()));
    animation->start();
}

void Bullet::SetAnimationEndValue()
{
    if(targetEnemy&&game->enemyList.indexOf(targetEnemy)!=-1)
        animation->setEndValue(targetEnemy->getPos());
}

void Bullet::HitTarget()
{
    if(targetEnemy&&game->enemyList.indexOf(targetEnemy)!=-1)
        targetEnemy->GetDamage(aggressivity,true);
    game->DeleteBullet(this);
}
