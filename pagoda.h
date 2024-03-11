#ifndef PAGODA_H
#define PAGODA_H

#include "tower.h"

class GameScene;

class Pagoda:public Tower
{
    Q_OBJECT
public:
    Pagoda(QPoint point,QString pName,GameScene * g);
    ~Pagoda();

    virtual void Draw(QPainter * painter);      //绘画事件
    virtual void LockTargetWithinAttackRange(); //在攻击范围内锁定敌人
    virtual void timerEvent(QTimerEvent *);     //

    void InitPixmap(double pSize);              //初始化图片
    void GetDamage(int _aggressity);            //受伤
    void GetLocked(Enemy *enemy);               //被敌人锁定
    void UpdateFierce();                        //升级为狂暴塔

    QList<Enemy *> enemyAttackList;             //攻击该塔的敌人list

protected:
    int eventId;                //攻击特效开启计时器
    int index;                  //攻击特效下标
    QVector<QPixmap> pixmap;    //攻击特效图片

public slots:
    virtual void AttackEnemy();
};

class IcePagoda:public Pagoda
{
    Q_OBJECT
public:
    IcePagoda(QPoint point,QString pName,GameScene * g);
    void InitPixmap(double pSize);

    virtual void Draw(QPainter * painter);
    virtual void LockTargetWithinAttackRange();
    virtual void timerEvent(QTimerEvent *);

protected:
    bool hasFrozenEnemy;    //已经冻结敌人的标志

public slots:
    virtual void AttackEnemy();
};


class GoldPagoda:public Pagoda  //毒塔生成毒雾子弹来攻击该条路径上的所有敌人
{
    Q_OBJECT
public:
    GoldPagoda(QPoint point,QString pName,GameScene * g);
    void InitPixmap(double pSize);
    void InitHaloPixmap(double pSize);          //初始化光环毒雾
    void GenerateEndPos();                      //生成毒子弹移动的终点

    virtual void Draw(QPainter * painter);
    virtual void LockTargetWithinAttackRange();
    virtual void timerEvent(QTimerEvent *);

    QPoint getEndPos(){return endpos;}

protected:
    int index_h;
    int event_id_h;
    QPoint endpos;
    bool hasEnemyinRoute;
    int toxicRate;
    QTimer * toxicTimer;
    QVector<QPixmap> pixHalo;   //攻击特效图片

public slots:
    virtual void AttackEnemy();
    void LoadToxicBullet();
};

#endif // PAGODA_H
