#include "pagoda.h"
#include "Common.h"
#include "gamescene.h"
#include <QPoint>
#include <QTimerEvent>
#include <QDebug>
#include <QTimer>

IcePagoda::IcePagoda(QPoint point,QString pName,GameScene * g):
    Pagoda(point,pName,g),
    hasFrozenEnemy(false)
{
    //初始化
    type=3;
    curHp=maxHp=80;
    coin=curHp;
    healthWith=60;
    aggressivity=15;
    attackRate=7000;
    InitPixmap(0.38);
    eventId=startTimer(180);
}

void IcePagoda::timerEvent(QTimerEvent * ev)
{
    Pagoda::timerEvent(ev);
}

void IcePagoda::InitPixmap(double pSize)
{
    pixmap.clear();
    for(int i=0;i<13;i++)
    {
        QString fileName=QString(":/image/iceFog%1.png").arg(i,2,10,QLatin1Char('0'));
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*pSize,map.height()*pSize),Qt::KeepAspectRatio);
        pixmap.push_back(map);
    }
}

void IcePagoda::AttackEnemy()
{
    if(game->isGameLose||game->isGameWin) return;
    //敌人在，而且还得没被冻住
    if(lockedEmemy&&game->enemyList.indexOf(lockedEmemy)!=-1&&!lockedEmemy->FrozenFlag()){
        lockedEmemy->StartFrozen();
        lockedEmemy->GetDamage(aggressivity);
        //生成冰冻特效
        IceBoom * iceBoom=new IceBoom(lockedEmemy,game);
        game->boomList.push_back(iceBoom);
        game->AudioPlay(game->frozenPlayer);
    }
}

void IcePagoda::Draw(QPainter * painter)
{
    Tower::Draw(painter);
    painter->save();
    if(hasFrozenEnemy)
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

void IcePagoda::LockTargetWithinAttackRange()
{
    if(game->isGameLose||game->isGameWin) return;
    if(lockedEmemy){//看锁定的敌人还在不在，是不是被别的塔给打死了or敌人给冻住了
        if(game->enemyList.indexOf(lockedEmemy)==-1||lockedEmemy->FrozenFlag()){
            lockedEmemy=NULL;
        }
        else{//攻击敌人
            if(!attackTimer->isActive()){
                AttackEnemy();
                attackTimer->start(attackRate);
            }
        }
    }
    if(!lockedEmemy){
        foreach(Enemy *enemy,game->enemyList){//锁定靠近范围的
            if(LineDistance(pos,enemy->getPos(),FixedLen*2-5)){//锁定同向的
                if(CheckDirectionBetweenPagodaEnemy(this,enemy)){//锁定没被冻住的
                    hasFrozenEnemy=true;
                    if(!enemy->FrozenFlag()){
                        //先冻住敌人
                        lockedEmemy=enemy;
                        //先攻击一发
                        //开启计时器
                        if(!attackTimer->isActive()){
                            AttackEnemy();
                            attackTimer->start(attackRate);
                        }
                        return;
                    }
                }
            }
        }
        hasFrozenEnemy=false;
    }
}
