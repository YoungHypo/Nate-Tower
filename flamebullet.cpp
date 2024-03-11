#include "cannon.h"
#include "gamescene.h"
#include "qcoreevent.h"
#include <QTimer>
#include <QPropertyAnimation>
#include <QVector2D>

/******FlameBullet******/
FlameBullet::FlameBullet(Cannon* can,Enemy* enem,QString pName,double pSize)
    :Bullet(can,enem,pName,pSize),
      flameRate(1200),
      flameAttackNum(0),
      isFinish(false)
{
    flameTimer=new QTimer(this);
    connect(flameTimer,SIGNAL(timeout()),this,SLOT(AttackEnemy()));
}

void FlameBullet::Draw(QPainter * painter)
{
    if(!isFinish){
        Bullet::Draw(painter);
    }
}

void FlameBullet::SetAnimationEndValue()
{
    if(!isFinish){
        Bullet::SetAnimationEndValue();
    }
}

void FlameBullet::AttackEnemy()
{
    if(targetEnemy&&game->enemyList.indexOf(targetEnemy)!=-1&&targetEnemy->FlameFlag()){
        if(flameAttackNum<=3){
            flameAttackNum++;
            targetEnemy->GetDamage(aggressivity);
            return;
        }
        else{//到次数了
            targetEnemy->EndFlame();
        }
    }
    game->DeleteBullet(this,true);//敌人不存在or三次持续机会用光了
}

void FlameBullet::HitTarget()
{
    isFinish=true;//动画播放结束
    if(targetEnemy&&game->enemyList.indexOf(targetEnemy)!=-1){
        targetEnemy->StartFlame();
    }
    else{
        game->DeleteBullet(this,true);
        return;
    }
    AttackEnemy();
    flameTimer->start(flameRate);
}


/******RnageBullet******/
RangeBullet::RangeBullet(Cannon* can,Enemy* enem,QString pName,double pSize)
    :Bullet(can,enem,pName,pSize),
      index(0)
{
    InitPixmap();
    eventId=startTimer(120);
}

void RangeBullet::Draw(QPainter * painter)
{
    painter->save();
    QPoint offsetPoint(-pixmap[index].width()*0.5,-pixmap[index].height()*0.5);
    //转到炮口的位置
    painter->translate(curPos);
    painter->drawPixmap(offsetPoint,pixmap[index]);
    painter->restore();

}

void RangeBullet::SetAnimationEndValue()
{
    Bullet::SetAnimationEndValue();
}

void RangeBullet::InitPixmap()
{
    pixmap.clear();
    for(int i=0;i<4;i++)
    {
        QString fileName=QString(":/image/fireBullet%1.png").arg(i);
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*pixSize,map.height()*pixSize),Qt::KeepAspectRatio);
        pixmap.push_back(map);
    }
}

void RangeBullet::timerEvent(QTimerEvent * ev)
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

void RangeBullet::HitTarget()
{
    Bullet::HitTarget();
}
