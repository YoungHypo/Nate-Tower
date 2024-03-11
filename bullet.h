#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QPoint>
#include <QPainter>
#include <QPropertyAnimation>

class Cannon;
class Enemy;
class GameScene;
class GoldPagoda;

class Bullet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint curPos READ ReadCurPos WRITE SetCurPos)
public:
    Bullet(Cannon* can,Enemy* enem,QString pName,double pSize,QObject *parent = nullptr);
    Bullet(QPoint spoint,QPoint epoint,GameScene* g,QObject *parent = nullptr);

    void Move();
    virtual void Draw(QPainter * painter);
    virtual void SetAnimationEndValue();   //确定动画终点坐标

    QPoint ReadCurPos() const {return curPos;}
    void SetCurPos(const QPoint pos) {curPos=pos;}
    Enemy* TargetEnemy(){return targetEnemy;}
    double PixSize(){return pixSize;}

protected:
    Cannon* srcCannon;                  //源远程塔
    Enemy* targetEnemy;                 //目标敌人
    GameScene* game;                    //游戏场景
    QPropertyAnimation * animation;     //动画指针
    const QPoint startPos;              //起始点
    const QPoint targetPos;             //目标点
    QPoint curPos;                      //当前坐标
    QPixmap pix;                        //子弹图片
    qreal rotateDegree;                 //旋转角度
    int aggressivity;                   //攻击力
    int duratTime;                      //持续时间
    double pixSize;                     //图片尺寸

protected slots:
    virtual void HitTarget();
};


class ToxicBullet : public Bullet
{
    Q_OBJECT
public:
    ToxicBullet(GoldPagoda * pag);
    virtual void Draw(QPainter * painter);
    virtual void SetAnimationEndValue();

    void InitPixmap();
    void timerEvent(QTimerEvent *);

protected:
    GoldPagoda * srcPagoda;     //源近战塔
    QVector<QPixmap> pixmap;    //子弹图片数组
    int eventId;                //特效开启计时器
    int index;                  //特效下标

protected slots:
    virtual void HitTarget();
};

class FlameBullet :public Bullet
{
    Q_OBJECT
public:
    FlameBullet(Cannon* can,Enemy* enem,QString pName,double pSize);

    virtual void Draw(QPainter * painter);
    virtual void SetAnimationEndValue();

protected:
    QTimer* flameTimer;
    int flameRate;
    int flameAttackNum;
    bool isFinish;

protected slots:
    virtual void HitTarget();
    virtual void AttackEnemy();
};

class RangeBullet:public Bullet
{
    Q_OBJECT
public:
    RangeBullet(Cannon* can,Enemy* enem,QString pName,double pSize);
    virtual void Draw(QPainter * painter);
    virtual void SetAnimationEndValue();

    void InitPixmap();
    void timerEvent(QTimerEvent *);

protected:
    QVector<QPixmap> pixmap;//子弹图片数组
    int eventId;            //特效开启计时器
    int index;              //特效下标

protected slots:
    virtual void HitTarget();
};





#endif // BULLET_H
