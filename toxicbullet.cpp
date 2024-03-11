#include "bullet.h"
#include "pagoda.h"
#include "gamescene.h"
#include "qcoreevent.h"
#include "Common.h"
#include <QPropertyAnimation>
#include <QVector2D>

ToxicBullet::ToxicBullet(GoldPagoda * pag)
    : Bullet(pag->getPos(),pag->getEndPos(),pag->getGameScene()),
      srcPagoda(pag),
      index(0)
{
    aggressivity=srcPagoda->Aggressivity();
    double posDist=PointDistance(srcPagoda->getPos(),targetPos);
    duratTime=(int)(2*UintDurat*(posDist/90));
    pixSize=0.22;
    InitPixmap();
    eventId=startTimer(180);
}

void ToxicBullet::Draw(QPainter * painter)
{
    painter->save();
    QPoint offsetPoint(-pixmap[index].width()*0.5,-pixmap[index].height()*0.5);
    painter->translate(curPos);
    painter->drawPixmap(offsetPoint,pixmap[index]);
    painter->restore();
}

void ToxicBullet::SetAnimationEndValue()
{
    //遍历该条路径上的所有敌人
    foreach(Enemy *enemy,game->enemyList){//如果敌人进入黄金塔所在的路线
        if(CheckPosBetweenTwoPoints(startPos,targetPos,enemy->getPos())){
            //如果二者相遇了，敌人直接受伤
            if(LineDistance(curPos,enemy->getPos(),20)){
                enemy->GetDamage(aggressivity,true);
            }
        }
    }
}

void ToxicBullet::InitPixmap()
{
    pixmap.clear();
    for(int i=0;i<9;i++)
    {
        QString fileName=QString(":/image/toxicFog%1.png").arg(i);
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*pixSize,map.height()*pixSize),Qt::KeepAspectRatio);
        pixmap.push_back(map);
    }
}

void ToxicBullet::timerEvent(QTimerEvent * ev)
{
    if(ev->timerId()==eventId)
    {
        index++;
        if(index>=pixmap.size())
        {
            index=0;
        }
    }
}

void ToxicBullet::HitTarget()
{
    //到达终点，毒雾消失
    game->DeleteBullet(this);
}

