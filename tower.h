#ifndef TOWER_H
#define TOWER_H

#include <QObject>
#include <QPainter>
#include <QDebug>
#include "enemy.h"

class Tower : public QObject
{
    Q_OBJECT
public:
    Tower(QPoint point,QString pName,GameScene * g,double pixSize=0.3,QObject *parent = nullptr);   
    void Draw(QPainter * painter);

    virtual void LockEnemyWithinAttackRange(){};

    QPoint getPos(){return pos;}
    Enemy* LockedEnemy(){return lockedEmemy;}
    GameScene* getGameScene(){return game;}
    int Aggressivity(){return aggressivity;}
    int Coin(){return coin;}
    int Type(){return type;}
    /*近战塔：1是普通塔，2是升级塔，3是冰塔，4是黄金毒塔
      远程塔：1是普通，2是升级，3是持续，4是范围*/

protected:
    QPoint pos;
    QPixmap pix;
    QTimer * attackTimer;
    Enemy* lockedEmemy;
    GameScene* game;
    double pixSize;         //图片尺寸
    int attackRate;         //攻击范围
    int curHp;              //当前血量
    int maxHp;              //最大血量
    int healthWith;         //血条
    int aggressivity;       //攻击力
    int type;               //塔台类型
    int coin;               //资源数

protected slots:
    virtual void AttackEnemy(){};
};

#endif // TOWER_H
