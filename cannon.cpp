#include "cannon.h"
#include "gamescene.h"
#include "Common.h"
#include "bullet.h"
#include "qvectornd.h"
#include <QVector2D>
#include <QtMath>
#include <QDebug>
#include <QTimer>

Cannon::Cannon(QPoint point,QString pName,GameScene * g)
    :Tower(point,pName,g,0.27),
      range(UnitRange),
      rotateDegree(0.0),
      delaHeight((int)(pix.height()*0.731)+1),
      delaWidth(pix.width()*0.5),
      bulletPixSize(0.45),
      damageFlag(false)
{
    type=1;
    aggressivity=10;
    curHp=60;
    maxHp=curHp;
    coin=curHp;
    healthWith=40;
    //开启攻击
    attackRate=1000;
    attackTimer=new QTimer(this);
    connect(attackTimer,SIGNAL(timeout()),this,SLOT(AttackEnemy()));
}

void Cannon::UpdateFierce()
{
    //血量增1.5倍
    maxHp=curHp=80;
    healthWith=54;
    coin=curHp;
    //伤害、攻击间隔、攻击范围加倍
    aggressivity*=1.5;
    attackRate*=1.5;
    range=(int)(1.8*range);
    //计时器重开
    if(attackTimer->isActive()) attackTimer->start(attackRate);
    //重新上载图片
    pix=QPixmap(":/image/Cannon3.png");
    double upSize=0.25;
    pix=pix.scaled(QSize(pix.width()*upSize,pix.height()*upSize),Qt::KeepAspectRatio);
    delaHeight=(int)(pix.height()*0.66)+1;
    delaWidth=pix.width()*0.5;
    bulletPixSize=0.55;
    //设置状态
    type=2;
}

void Cannon::AttackEnemy()
{
    if(game->isGameLose||game->isGameWin) return;
    if(lockedEmemy&&game->enemyList.indexOf(lockedEmemy)!=-1)//调用子弹类开打
    {
        Bullet* bullet=new Bullet(this,lockedEmemy,":/image/cannonBullet.png",bulletPixSize);
        game->LoadBullet(bullet);
        bullet->Move();
        qDebug()<<"子弹数量"<<game->bulletList.size();
    }

}

void Cannon::LockEnemyWithinAttackRange()
{
    if(game->isGameLose||game->isGameWin) return;
    if(curHp<=0) {
        game->AudioPlay(game->cannonDeletePlayer);
        game->DeleteCannon(this);
        return;
    }
    //查找敌人并且锁定
    if(!lockedEmemy){
        foreach(Enemy *enemy,game->enemyList){//敌人在攻击范围内且未加防护罩
            if(CircleDistance(pos,range,enemy->getPos(),1)&&!enemy->ProtectedFlag()){
                lockedEmemy=enemy;
                enemy->GetLocked(this);
                break;
            }
        }
    }
    //旋转炮台并且排除敌人溜了的情况
    if(lockedEmemy){
        if(game->enemyList.indexOf(lockedEmemy)==-1){//检测是否之前锁定的敌人不在了
            lockedEmemy=NULL;
        }
        else{//旋转炮台准备开顶
            QVector2D normal(lockedEmemy->getPos()-pos);
            normal.normalize();
            rotateDegree=qRadiansToDegrees(qAtan2(normal.y(),normal.x()))+90;
            //检测锁定敌人是否逃脱
            if(!CircleDistance(pos,range,lockedEmemy->getPos(),1)){
                lockedEmemy->GetEscape(this);
                lockedEmemy=NULL;//敌人溜了
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

void Cannon::GetDamage(int _aggressity)
{
    damageFlag=true;
    QTimer::singleShot(1000, this,[=](){
        damageFlag=false;
    });
    curHp-=_aggressity;
    if(curHp<=0){
        curHp=0;
    }
}

void Cannon::Draw(QPainter * painter)
{
    painter->save();
    const QPoint offsetPoint(-delaWidth,-delaHeight);
    painter->translate(pos);
    painter->rotate(rotateDegree);
    painter->drawPixmap(offsetPoint,pix);
    if(damageFlag){
        //血条特效
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawRect(QRect(QPoint(-healthWith/2,-FixedLen),QSize(healthWith,5)));

        painter->setBrush(Qt::green);
        painter->drawRect(QRect(QPoint(-healthWith/2,-FixedLen),QSize((double)curHp/maxHp*healthWith,5)));
    }
    painter->restore();
}

Cannon::~Cannon()
{
    if(attackTimer->isActive()) attackTimer->stop();
    delete attackTimer;
}
