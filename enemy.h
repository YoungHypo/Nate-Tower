#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPainter>
#include "routepoint.h"

class Pagoda;
class GameScene;
class Cannon;

class Enemy : public QObject
{
    Q_OBJECT
public:
    explicit Enemy(RoutePoint *startPoint,GameScene * g,bool fFlag=false,bool pFlag=false,QObject *parent = nullptr);
    ~Enemy();


    void Move();                //敌人移动
    void DoActivate();          //激活敌人
    void InitPixmap();          //初始化图像
    void InitSpeedPixmap();     //初始化神速敌人图像
    void UpdateSpeed();         //升级敌人为神速敌人
    void UpdateProtected();     //升级敌人无视远程

    void Draw(QPainter * painter);                      //绘图事件
    void timerEvent(QTimerEvent *);                     //时间事件
    void UpdateDirection(QPoint cur,QPoint next);       //更新敌人行走方向
    virtual void LockTargetWithinAttackRange();         //锁定塔
    void GetDamage(int _aggressity,bool tFlag=false);   //敌人受伤

    void GetLocked(Pagoda *pagoda);         //敌人被塔锁定
    void GetLocked(Cannon *cannon);         //敌人被炮锁定
    void GetEscape(Cannon *cannon);         //敌人逃脱炮的锁定范围

    //返回敌人数据成员
    Pagoda* LockedPagoda(){return lockedPagoda;}
    QPoint getPos(){return curPoint;}
    int Direction(){return direction;}
    int Type(){return type;}
    bool ProtectedFlag(){return protectedFlag;} //忽视远程塔

    //冰冻
    bool FrozenFlag(){return frozenFlag;}
    void StartFrozen(){frozenFlag=true;}
    void EndFrozen(){frozenFlag=false;}

    //持续受伤
    bool FlameFlag(){return flameFlag;}
    void StartFlame(){flameFlag=true;}
    void EndFlame(){flameFlag=false;}    

    //容器
    QList<Pagoda *> pagodaAttackList;   //攻击敌人的塔列表
    QList<Cannon *> cannonAttackList;   //攻击敌人的炮列表

    int overlapNum;         //当前敌人重叠数量

protected:
    //敌人数据
    int type;               //敌人类型：1是普通怪，2是速度怪，3是闪现怪，4是无视远程，5是boss
    int coin;               //敌人价值资源
    bool state;             //敌人状态
    int curHp;              //当前血量
    int maxHp;              //最大血量
    int speed;              //速度
    int direction;          //前进方向：0表向上，1表向下，2表向左，3表示向右
    int healthWith;         //血条宽度
    int attackRate;         //攻击速率
    int aggressivity;       //攻击力
    int eventId,eventIdEffect,eventIdFlame;     //图像时间ID
    int index,indexEffect,indexFlame;           //图像遍历下标



    GameScene * game;           //游戏场景指针
    RoutePoint * destRouPoint;  //目标航点指针
    QTimer * attackTimer;       //攻击计时器
    QPixmap pixmap[4][4];       //敌人行走图像
    QPixmap pixProtect;         //防护罩图像
    QVector<QPixmap> pixEffect; //攻击图像
    QVector<QPixmap> pixFlame;  //持续被攻击图像(旋风)
    QPoint curPoint;            //敌人当前坐标
    Pagoda* lockedPagoda;       //锁定塔指针

    bool frozenFlag;            //冰冻标志
    bool toxicFlag;             //中毒标志
    bool flameFlag;             //火焰持续图片标志
    bool fierceFlag;            //升级为狂暴塔标志
    bool protectedFlag;         //升级为忽视远程塔

protected slots:
    void AttackPagoda();
};

class FlashEnemy:public Enemy
{
    Q_OBJECT
public:
    FlashEnemy(RoutePoint *startPoint,GameScene * g);

    virtual void LockTargetWithinAttackRange();
    void InitPixmap();

protected:
    bool flashFlag;         //闪现标志
    int flashInterval;      //闪现时间间隔

};

class BossEnemy:public Enemy
{
    Q_OBJECT
public:
    BossEnemy(RoutePoint *startPoint,GameScene * g);

    virtual void LockTargetWithinAttackRange();

    void InitPixmap();

protected:
    int range;                  //攻击范围
    int invadeRate;             //攻击频率
    int invadeAggressivity;     //攻击力(目标为炮)

    QTimer * invadeTimer;               //攻击计时器
    QList<Cannon *> cannonInvadedList;  //被攻击的炮列表

protected slots:
    void AttackCannon();

};

#endif // ENEMY_H
