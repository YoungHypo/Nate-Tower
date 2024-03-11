#include "pagoda.h"
#include "Common.h"
#include "gamescene.h"
#include <QPoint>
#include <QTimerEvent>
#include <QDebug>
#include <QTimer>


Pagoda::Pagoda(QPoint point,QString pName,GameScene * g)
    : Tower(point,pName,g,0.9),
      index(0)
{
    type=1;
    curHp=60;
    maxHp=curHp;
    coin=curHp;
    healthWith=40;
    aggressivity=10;
    InitPixmap(0.4);
    eventId=startTimer(180);
    attackRate=1000;
    attackTimer=new QTimer(this);
    connect(attackTimer,SIGNAL(timeout()),this,SLOT(AttackEnemy()));
}

Pagoda::~Pagoda()
{
    if(attackTimer->isActive()) attackTimer->stop();
    delete attackTimer;
}

/*升级为狂暴塔，直接调用该函数修改当前塔的属性，不用继承*/
void Pagoda::UpdateFierce()
{
    //血量加1.5
    maxHp*=1.5;
    healthWith*=1.5;
    //回血
    curHp=maxHp;
    //伤害和攻击间隔加倍
    aggressivity*=2;
    attackRate*=1.2;
    if(attackTimer->isActive()) attackTimer->start(attackRate);
    //重置图片，攻击特效变大
    pix=QPixmap(":/image/fiercePagoda.png");
    pix=pix.scaled(QSize(pix.width()*pixSize,pix.height()*pixSize),Qt::KeepAspectRatio);
    InitPixmap(0.68);
    //设置状态
    type=2;
}

void Pagoda::AttackEnemy()
{
    if(game->isGameLose||game->isGameWin) return;
    if(lockedEmemy&&game->enemyList.indexOf(lockedEmemy)!=-1){
        lockedEmemy->GetDamage(aggressivity);
    }
}

void Pagoda::LockTargetWithinAttackRange()
{
    if(game->isGameLose||game->isGameWin) return;
    if(!lockedEmemy){
        foreach(Enemy *enemy,game->enemyList){
            if(LineDistance(pos,enemy->getPos(),FixedLen*2)){
                if(CheckDirectionBetweenPagodaEnemy(this,enemy)){
                    lockedEmemy=enemy;
                    enemy->GetLocked(this);
                    break;
                }
            }
        }
    }
    if(lockedEmemy){//看锁定的敌人还在不在，是不是被别的塔给打死了
        if(game->enemyList.indexOf(lockedEmemy)==-1){
            lockedEmemy=NULL;
        }
        else{
            if(!attackTimer->isActive()) {
                AttackEnemy();
                attackTimer->start(attackRate);
            }
        }
    }
}

void Pagoda::Draw(QPainter * painter)
{
    Tower::Draw(painter);
    painter->save();
    if(lockedEmemy)
    {
        const QPoint offsetPoint(-pixmap[index].width()*0.5,-pixmap[index].height()*0.5);
        painter->translate(pos);
        painter->drawPixmap(offsetPoint,pixmap[index]);
    }
    if(!enemyAttackList.isEmpty())
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawRect(QRect(QPoint(-healthWith/2,-FixedLen),QSize(healthWith,5)));

        painter->setBrush(Qt::green);
        painter->drawRect(QRect(QPoint(-healthWith/2,-FixedLen),QSize((double)curHp/maxHp*healthWith,5)));


    }
    painter->restore();

}

void Pagoda::timerEvent(QTimerEvent * ev)
{
    if(game->isGameLose||game->isGameWin) return;
    if(ev->timerId()==eventId)
    {
        index++;
        if(index>=pixmap.size())
        {
            index=0;//从头播放
        }
    }
}

void Pagoda::GetDamage(int _aggressity)
{
    curHp-=_aggressity;
    if(curHp<=0){
        game->AudioPlay(game->pagodaDeletePlayer);
        game->DeletePagoda(this);
    }
}

void Pagoda::GetLocked(Enemy *enemy)
{
    enemyAttackList.push_back(enemy);
}

void Pagoda::InitPixmap(double pSize)
{
    pixmap.clear();
    for(int i=0;i<19;i++)
    {
        QString fileName=QString(":/image/magicSymbol%1.png").arg(i,2,10,QLatin1Char('0'));
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*pSize,map.height()*pSize),Qt::KeepAspectRatio);
        pixmap.push_back(map);
    }
}

