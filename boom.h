#ifndef BOOM_H
#define BOOM_H

#include <QObject>
#include <QPainter>
#include <QVector>

#include "enemy.h"

class GameScene;

class Boom : public QObject
{
    Q_OBJECT
public:
    explicit Boom(QPoint point,int t,GameScene * g,double pBulletSize=0.45,QObject *parent = nullptr);

    void Draw(QPainter * painter);
    void InitPixmap();
    void GeneratePixArray(QString path,int num,double pixSize,int rate=150);    //确定特效类型

    virtual void timerEvent(QTimerEvent *);

    QPoint getPos(){return pos;}

protected:
    GameScene * game;
    QVector<QPixmap> pix;   //特效图片数组

    QPoint pos;             //特效生成点
    double bulletBoomSize;

    int eventId;
    int index;
    int type;//1是pagoda，2是狼人，3是子弹，4是升级黄色光效，5是青色，6是金色，7是冰冻特效，8是......

};

class IceBoom:public Boom   //冰冻特效
{
    Q_OBJECT
public:
    IceBoom(Enemy * enemy,GameScene * g,int t=7);
    virtual void timerEvent(QTimerEvent *);

private:
    bool endFlag;
    Enemy * targetEnemy;
    int frozenTime;
};

#endif // BOOM_H
