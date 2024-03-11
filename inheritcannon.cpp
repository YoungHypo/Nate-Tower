#include "cannon.h"
#include "gamescene.h"
#include "bullet.h"
#include <QVector2D>
#include <QtMath>
#include <QDebug>
#include <QTimer>

FlameCannon::FlameCannon(QPoint point,QString pName,GameScene * g):
    Cannon(point,pName,g)
{
    //重新上载图片
    pixSize=0.29;
    pix=QPixmap(pName);
    pix=pix.scaled(QSize(pix.width()*pixSize,pix.height()*pixSize),Qt::KeepAspectRatio);
    //更新部分数据成员
    type=3;
    aggressivity=8;
    curHp=maxHp=80;
    coin=curHp;
    healthWith=54;
    attackRate=3800;
    range=(int)(Cannon::range*1.6);
    delaHeight=(int)(pix.height()*0.668)+1;
    delaWidth=(int)(pix.width()*0.687);
    bulletPixSize=0.45;
}


void FlameCannon::LockEnemyWithinAttackRange()
{
    Cannon::LockEnemyWithinAttackRange();
}

void FlameCannon::AttackEnemy()
{
    if(game->isGameLose||game->isGameWin) return;
    if(lockedEmemy&&game->enemyList.indexOf(lockedEmemy)!=-1)
    {
        FlameBullet* flameBullet=new FlameBullet(this,lockedEmemy,":/image/purple_bullet.png",bulletPixSize);
        game->LoadBullet(flameBullet);
        flameBullet->Move();
    }
}


RangeCannon::RangeCannon(QPoint point,QString pName,GameScene * g):
    Cannon(point,pName,g),
    enemyNum(3)
{
    //初始化敌人数组
    enemyInRangeList.resize(enemyNum);

    pixSize=0.33;
    pix=QPixmap(pName);
    pix=pix.scaled(QSize(pix.width()*pixSize,pix.height()*pixSize),Qt::KeepAspectRatio);

    type=4;
    aggressivity=10;
    curHp=maxHp=80;
    coin=curHp;
    healthWith=54;
    attackRate=2600;
    range=(int)(Cannon::range*1.8);

    delaHeight=(int)(pix.height()*0.580)+1;
    delaWidth=pix.width()*0.5;
    bulletPixSize=0.47;
}

bool RangeCannon::CheckSpaceinEnemyInRangeList(int& indexlist)//是否有空针
{
    for(int i=indexlist;i<enemyInRangeList.size();i++)
        if(!enemyInRangeList[i]){
            indexlist=i;//返回第一个空针位置
            return true;
        }
    return false;
}

bool RangeCannon::CheckEmptyNullinEnemyInRangeList(int& indexlist)//是否全为空针
{
    for(int i=0;i<enemyInRangeList.size();i++)
        if(enemyInRangeList[i]) {
            indexlist=i;
            return false;
        }
    return true;
}

void RangeCannon::LockEnemyWithinAttackRange()
{
    if(game->isGameLose||game->isGameWin) return;
    if(curHp<=0){
        game->AudioPlay(game->cannonDeletePlayer);
        game->DeleteCannon(this);
        return;
    }
    //先检查是不是有敌人失效的情况
    for(int i=0;i<enemyInRangeList.size();i++){
        if(enemyInRangeList[i]&&game->enemyList.indexOf(enemyInRangeList[i])==-1){
            enemyInRangeList[i]=NULL;
        }
    }
    //尽可能填满敌人list
    int indexlist=0;
    if(CheckSpaceinEnemyInRangeList(indexlist)){//只要有空间就填补敌人
        foreach(Enemy *enemy,game->enemyList){
            if(CircleDistance(pos,range,enemy->getPos(),1)&&!enemy->ProtectedFlag()){
                if(enemyInRangeList.indexOf(enemy)!=-1) continue;//已经有的敌人就再不加了
                if(CheckSpaceinEnemyInRangeList(indexlist)){
                    enemyInRangeList[indexlist]=enemy;
                    indexlist++;
                }
                else{//没空间就退出循环
                    break;
                }
            }
        }
    }
    //旋转炮台准备攻击
    indexlist=0;
    if(!CheckEmptyNullinEnemyInRangeList(indexlist)){
        //炮台对向第一个敌人
        QVector2D normal(enemyInRangeList[indexlist]->getPos()-pos);
        normal.normalize();
        rotateDegree=qRadiansToDegrees(qAtan2(normal.y(),normal.x()))+90;
        //检测敌人是否跳脱
        for(int i=indexlist;i<enemyInRangeList.size();i++){
            if(enemyInRangeList[i]){
                if(!CircleDistance(pos,range,enemyInRangeList[i]->getPos(),1)){
                    enemyInRangeList[i]=NULL;
                }
                else{//开始攻击
                    if(!attackTimer->isActive()) {
                        AttackEnemy();
                        attackTimer->start(attackRate);
                    }
                }
            }
        }
    }
}

void RangeCannon::AttackEnemy()
{
    if(game->isGameLose||game->isGameWin) return;
    for(int i=0;i<enemyInRangeList.size();i++){
        if(enemyInRangeList[i]&&game->enemyList.indexOf(enemyInRangeList[i])!=-1){
            RangeBullet* rangeBullet=new RangeBullet(this,enemyInRangeList[i],":/image/Missile.png",bulletPixSize);
            game->LoadBullet(rangeBullet);
            rangeBullet->Move();
        }
    }
}

