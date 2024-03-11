#include "pagoda.h"
#include "Common.h"
#include "bullet.h"
#include "gamescene.h"
#include "routepoint.h"
#include <QPoint>
#include <QTimerEvent>
#include <QDebug>
#include <QTimer>

GoldPagoda::GoldPagoda(QPoint point,QString pName,GameScene * g):
    Pagoda(point,pName,g),
    index_h(0),
    hasEnemyinRoute(false),
    toxicRate(5000)
{
    //初始化数据成员
    type=4;
    curHp=maxHp=80;
    coin=curHp;
    healthWith=60;
    aggressivity=20;
    attackRate*=1.1;
    InitPixmap(0.55);
    event_id_h=startTimer(180);
    //生成终点
    GenerateEndPos();
    //装填毒弹
    toxicTimer=new QTimer(this);
    connect(toxicTimer,SIGNAL(timeout()),this,SLOT(LoadToxicBullet()));
}

void GoldPagoda::LoadToxicBullet()
{
    if(game->isGameLose||game->isGameWin) return;
    if(hasEnemyinRoute&&!lockedEmemy){//如果有敌人锁定了，直接进入近战再不染了
        ToxicBullet * toxicBullet=new ToxicBullet(this);
        game->LoadBullet(toxicBullet);
        toxicBullet->Move();
    }
}

void GoldPagoda::InitPixmap(double pSize)
{
    Pagoda::InitPixmap(pSize);
    InitHaloPixmap(0.50);
}

void GoldPagoda::timerEvent(QTimerEvent * ev)
{
    Pagoda::timerEvent(ev);
    if(game->isGameLose||game->isGameWin) return;
    if(ev->timerId()==event_id_h)
    {
        index_h++;
        if(index_h>=pixHalo.size())
        {
            index_h=0;//从头播放
        }
    }
}

void GoldPagoda::InitHaloPixmap(double pSize)
{
    pixHalo.clear();
    for(int i=0;i<13;i++)
    {
        QString fileName=QString(":/image/greenHalo%1.png").arg(i,2,10,QLatin1Char('0'));
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*pSize,map.height()*pSize),Qt::KeepAspectRatio);
        pixHalo.push_back(map);
    }
}

void GoldPagoda::GenerateEndPos()
{
    int index=GetPointIndex(pos),t=0;
    if(game->pointFlag[index].isRoute[0]) t=0;
    else if(game->pointFlag[index].isRoute[1]) t=1;
    RoutePoint * former=game->routePointList[t].first();
    RoutePoint * latter=former->NextPoint();
    while(latter!=NULL){
        //确定该建塔点所在路径的起始点
        if(CheckPosBetweenTwoPoints(former->Pos(),latter->Pos(),pos)){
            endpos=former->Pos();
            break;
        }
        else{//继续向后遍历
            former=latter;
            latter=latter->NextPoint();
        }
    }
}

void GoldPagoda::AttackEnemy()
{
    Pagoda::AttackEnemy();
}

void GoldPagoda::Draw(QPainter * painter)
{
    Pagoda::Draw(painter);
    painter->save();
    if(hasEnemyinRoute){//画绿色光环
        const QPoint offsetPoint(-pixHalo[index_h].width()*0.5,-pixmap[index_h].height()*0.5-60);
        painter->translate(pos);
        painter->drawPixmap(offsetPoint,pixHalo[index_h]);
    }
    painter->restore();

}

void GoldPagoda::LockTargetWithinAttackRange()
{
    if(game->isGameLose||game->isGameWin) return;
    //一旦说碰到敌人走进近战范围，立马改用近战塔作战
    Pagoda::LockTargetWithinAttackRange();
    bool tmpFlag=false;
    foreach(Enemy *enemy,game->enemyList)
    {   //如果敌人进入黄金塔所在的路线
        if(CheckPosBetweenTwoPoints(pos,endpos,enemy->getPos())){
            hasEnemyinRoute=true;
            tmpFlag=true;
            break;
        }
    }
    if(!tmpFlag) hasEnemyinRoute=false;
    //确实路径有敌人，打就完了
    if(hasEnemyinRoute){
        if(!toxicTimer->isActive()) {
            LoadToxicBullet();
            toxicTimer->start(toxicRate);
        }
    }

}
