#include "enemy.h"
#include "Common.h"
#include "boom.h"
#include "pagoda.h"
#include "gamescene.h"
#include <QPoint>
#include <QVector2D>
#include <QTimerEvent>
#include <QTimer>
#include <QDebug>

FlashEnemy::FlashEnemy(RoutePoint *startPoint,GameScene * g)
    :Enemy(startPoint,g,false,false),
      flashFlag(false),
      flashInterval(5000)
{
    type=3;
    aggressivity=12;
    curHp=maxHp=120;
    coin=curHp;
    healthWith=60;
    speed=12;
    InitPixmap();
}

void FlashEnemy::LockTargetWithinAttackRange()
{
    if(game->isGameLose||game->isGameWin) return;
    if(!state) return;
    if(!lockedPagoda){
        foreach(Pagoda* pagoda,game->pagodaList){
            if(LineDistance(curPoint,pagoda->getPos(),FixedLen*2+speed)){
                if(CheckDirectionBetweenPagodaEnemy(pagoda,this)){
                    if(!flashFlag){
                        //进入闪现冷却时间
                        flashFlag=true;
                        //获得单位向量点
                        QPoint tarPoint=destRouPoint->Pos();
                        QVector2D normal(tarPoint-curPoint);
                        normal.normalize();
                        QPoint cellPoint=normal.toPoint();
                        //更新敌人坐标
                        curPoint=pagoda->getPos()+cellPoint;
                        lockedPagoda=NULL;
                        //加入闪现特效
                        Boom* boom=new Boom(curPoint+3*cellPoint,9,game);
                        game->boomList.push_back(boom);
                        game->AudioPlay(game->flashPlayer);
                        //冷却时间到达后，重新准备闪现
                        QTimer::singleShot(flashInterval, this,[=](){
                            flashFlag=false;
                        });
                    }
                    else{
                        lockedPagoda=pagoda;
                        lockedPagoda->GetLocked(this);
                    }
                    break;
                }
            }
        }
    }
    if(lockedPagoda){//锁定的塔可能不在了，要么手动撤销，要么被敌人打死了
        if(game->pagodaList.indexOf(lockedPagoda)==-1){
            lockedPagoda=NULL;
        }
        else{//直接反击
            if(!attackTimer->isActive()) {
                AttackPagoda();
                attackTimer->start(attackRate);
            }
        }
    }
}

void FlashEnemy::InitPixmap()
{   //加载行走
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            QString fileName=QString(":/image/calmWerewolf%1%2.png").arg(i).arg(j);
            QPixmap map(fileName);
            map=map.scaled(QSize(map.width()*1.2,map.height()*1.2),Qt::KeepAspectRatio);
            pixmap[i][j]=map;
        }
    }
}


BossEnemy::BossEnemy(RoutePoint *startPoint,GameScene * g)
    :Enemy(startPoint,g,false,false),
      range(UnitRange*1.1),
      invadeRate(2000),
      invadeAggressivity(15)
{
    type=5;
    aggressivity=20;
    curHp=maxHp=200;
    coin=curHp;
    healthWith=100;
    speed=8;
    InitPixmap();
    eventId=startTimer(180);
    invadeTimer=new QTimer(this);
    connect(invadeTimer,SIGNAL(timeout()),this,SLOT(AttackCannon()));
    invadeTimer->start(invadeRate);
}

void BossEnemy::AttackCannon()
{
    if(game->isGameLose||game->isGameWin) return;
    if(!state) return;
    if(frozenFlag) return;//被冰冻住就G了
    cannonInvadedList.clear();
    foreach(Cannon* cannon,game->cannonList)
    {
        if(CircleDistance(curPoint,range,cannon->getPos(),1))
            cannonInvadedList.push_back(cannon);
    }
    if(!cannonInvadedList.empty()){
        //boss自身闪电特效
        Boom* boom=new Boom(curPoint,10,game);
        game->boomList.push_back(boom);
        game->AudioPlay(game->lightPlayer);
        //攻击远程塔
        foreach(Cannon* cannon,cannonInvadedList){
            Boom* boom=new Boom(cannon->getPos(),11,game);
            game->boomList.push_back(boom);
            cannon->GetDamage(invadeAggressivity);
        }
    }
}

void BossEnemy::LockTargetWithinAttackRange()
{
    Enemy::LockTargetWithinAttackRange();
}

void BossEnemy::InitPixmap()
{    //加载行走
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            QString fileName=QString(":/image/orc%1%2.png").arg(i).arg(j);
            QPixmap map(fileName);
            map=map.scaled(QSize(map.width()*1.2,map.height()*1.2),Qt::KeepAspectRatio);
            pixmap[i][j]=map;
        }
    }
    pixEffect.clear();
    //加载抓痕
    for(int i=0;i<6;i++)
    {
        QString fileName=QString(":/image/orcClaw%1.png").arg(i);
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*0.5,map.height()*0.5),Qt::KeepAspectRatio);
        pixEffect.push_back(map);
    }
}
