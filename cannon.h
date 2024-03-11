#ifndef CANNON_H
#define CANNON_H

#include "tower.h"

class Cannon: public Tower
{
public:
    Cannon(QPoint point,QString pName,GameScene * g);
    ~Cannon();
    void Draw(QPainter * painter);
    void GetDamage(int _aggressity);
    void UpdateFierce();        //升级为狂暴塔

    virtual void LockEnemyWithinAttackRange();

    qreal RotatedDegree(){return rotateDegree;}
    int DelaHeight(){return delaHeight;}
    int OffsetWidth(){return delaWidth-pix.width()*0.5;}

protected:
    int range;              //攻击范围
    qreal rotateDegree;     //旋转角度
    int delaHeight;         //偏移高度
    int delaWidth;          //偏移宽度
    double bulletPixSize;   //子弹图片放缩尺寸
    bool damageFlag;        //受伤害标志

protected slots:
    virtual void AttackEnemy();
};

class FlameCannon:public Cannon
{
public:
    FlameCannon(QPoint point,QString pName,GameScene * g);
    virtual void LockEnemyWithinAttackRange();

protected slots:
    virtual void AttackEnemy();
};

class RangeCannon:public Cannon
{
public:
    RangeCannon(QPoint point,QString pName,GameScene * g);
    virtual void LockEnemyWithinAttackRange();

    bool CheckSpaceinEnemyInRangeList(int& indexlist);      //检查是否由空余弹道可以锁定敌人
    bool CheckEmptyNullinEnemyInRangeList(int& indexlist);  //检查是否所有弹道全空

protected:
    QVector<Enemy *> enemyInRangeList;      //被该远程炮锁定的敌人list
    int enemyNum;                           //允许最多被锁定的敌人数目
protected slots:
    virtual void AttackEnemy();
};

#endif // CANNON_H
