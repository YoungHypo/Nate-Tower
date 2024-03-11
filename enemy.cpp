#include "enemy.h"
#include "Common.h"
#include "pagoda.h"
#include "gamescene.h"
#include <QPoint>
#include <QVector2D>
#include <QTimerEvent>
#include <QTimer>
#include <QDebug>

Enemy::Enemy(RoutePoint *startPoint,GameScene * g,bool fFlag,bool pFlag,QObject *parent)
    : QObject{parent},
      overlapNum(0),
      type(1),
      coin(100),
      state(false),
      curHp(100),
      maxHp(curHp),
      speed(10),
      direction(0),
      healthWith(50),
      attackRate(1000),
      aggressivity(10),
      index(0),
      indexEffect(0),
      indexFlame(0),
      game(g),
      destRouPoint(startPoint->NextPoint()),
      attackTimer(NULL),
      curPoint(startPoint->Pos()),
      lockedPagoda(NULL),
      frozenFlag(false),
      toxicFlag(false),
      flameFlag(false),
      fierceFlag(fFlag),
      protectedFlag(pFlag)
{
    UpdateDirection(curPoint,destRouPoint->Pos());
    InitPixmap();
    if(!fierceFlag){//初始化动画计时下标
        eventId=startTimer(150);
    }
    else{
        UpdateSpeed();
    }
    if(protectedFlag){
        UpdateProtected();
    }
    eventIdEffect=startTimer(150);
    eventIdFlame=startTimer(150);
    //设定攻击计时器
    attackTimer=new QTimer(this);
    connect(attackTimer,SIGNAL(timeout()),this,SLOT(AttackPagoda()));
}

Enemy::~Enemy()
{
    if(attackTimer->isActive()) attackTimer->stop();
    delete attackTimer;
}

void Enemy::DoActivate(){
    state = true;
}

void Enemy::UpdateSpeed()
{
    type=2;
    aggressivity=12;
    speed=18;               //提高速度
    maxHp=120;              //提升血量
    curHp=maxHp;
    coin=curHp;
    healthWith=60;
    InitSpeedPixmap();      //重新上载行走动画
    eventId=startTimer(110);//频率加快
}

void Enemy::UpdateProtected()
{
    type=4;
    aggressivity=12;
    curHp=maxHp=120;
    coin=curHp;
    healthWith=60;
    speed=12;
}

void Enemy::AttackPagoda()
{
    if(game->isGameLose||game->isGameWin) return;
    if(!state) return;
    if(!frozenFlag&&lockedPagoda&&game->pagodaList.indexOf(lockedPagoda)!=-1){//判断当前塔是否被锁定且真实存在
        lockedPagoda->GetDamage(aggressivity);
    }
}

void Enemy::GetDamage(int _aggressity,bool tFlag)
{
    if(!state) return;
    if(tFlag){
        toxicFlag=true;//击中后显示一秒血条
        QTimer::singleShot(1000, this,[=](){
            toxicFlag=false;
        });
    }
    curHp-=_aggressity;
    if(curHp<=0)
    {
        curHp=0;
        game->AudioPlay(game->deadPlayer);
        game->gameMoney+=coin;
        //词缀掉落
        if(!tFlag){
            game->DeleteEnemy(this);
        }
    }
}

void Enemy::GetLocked(Pagoda *pagoda)
{
    pagodaAttackList.push_back(pagoda);
}

void Enemy::GetLocked(Cannon *cannon)
{
    cannonAttackList.push_back(cannon);
}

void Enemy::GetEscape(Cannon *cannon)
{
    cannonAttackList.removeOne(cannon);
}

void Enemy::timerEvent(QTimerEvent * ev)
{
    if(game->isGameLose||game->isGameWin) return;
    if(!state) return;
    if(ev->timerId()==eventIdFlame){
        indexFlame++;
        if(indexFlame>=pixFlame.size()) indexFlame=0;
    }
    if(frozenFlag) return;//冻住了就不做动画了
    if(ev->timerId()==eventId){
        index++;
        if(index>=4) index=0;
    }
    if(ev->timerId()==eventIdEffect){
        indexEffect++;
        if(indexEffect>=pixEffect.size()) indexEffect=0;
    }
}

void Enemy::LockTargetWithinAttackRange()
{
    if(game->isGameLose||game->isGameWin) return;
    if(!state) return;
    if(!lockedPagoda){
        foreach(Pagoda* pagoda,game->pagodaList){
            if(LineDistance(curPoint,pagoda->getPos(),FixedLen*2)){
                if(CheckDirectionBetweenPagodaEnemy(pagoda,this)){
                    lockedPagoda=pagoda;
                    lockedPagoda->GetLocked(this);
                    break;
                }
            }
        }
    }
    if(lockedPagoda){//锁定的塔可能不在了，要么手动撤销，要么被敌人打死了
        if(game->pagodaList.indexOf(lockedPagoda)==-1){
            lockedPagoda=NULL;
        }
        else{//直接反击
            if(!attackTimer->isActive()) {
                AttackPagoda();
                attackTimer->start(attackRate);
            }
        }
    }
}

void Enemy::Move()
{
    if(game->isGameLose||game->isGameWin) return;
    if(!state) return;
    if(frozenFlag) return;  //如果被冰冻了，就不走了
    if(curHp<=0){           //没血直接G
       game->DeleteEnemy(this);
       return;
    }
    //先找防御塔
    LockTargetWithinAttackRange();
    if(lockedPagoda){//遇到敌人就停止了，不走了
        if(LineDistance(curPoint,lockedPagoda->getPos(),speed)){//16与下面curpoint递增的速度相一致
            return;
        }
    }
    //巡线开动
    if(CircleDistance(curPoint,5,destRouPoint->Pos(),5))//前一个5是enemy的半径r，后一个5固定
    {
        //本质上就是两个圆相切的问题，为了使敌人运动的路径上一定能检测到路标拐点，不能使敌人的速度和其圆心半径差距过大
        //比如敌人速度是10，目标拐点对应同心圆的半径为1，敌人自身圆心的半径也为1，本来正常运动过程中应该有两圆重合，但是敌人移动速度太快，直接飞过，测不到二者相交
        if(destRouPoint->NextPoint()){
            curPoint=destRouPoint->Pos();
            destRouPoint=destRouPoint->NextPoint();
            UpdateDirection(curPoint,destRouPoint->Pos());
        }
        else{
            game->curBlood-=curHp;//扣除游戏的血量
            game->DeleteEnemy(this,true);//删除该怪物
            if(game->curBlood<=0){
                game->curBlood=0;
                game->isGameLose=true;
                game->EndGame();
            }
            return;
        }
    }

    QPoint tarPoint=destRouPoint->Pos();
    QVector2D normal(tarPoint-curPoint);
    normal.normalize();
    QPoint cellPoint=normal.toPoint();
    curPoint=curPoint+cellPoint*speed;//最右侧的16指代enemy的速度v，之后用v代替
    /*为使运动过程中两圆相切上，要求5<r<=v<2*r+10*/
}

void Enemy::Draw(QPainter * painter)
{
    if(!state) return;
    //绘制
    painter->save();
    QPoint offsetPoint(-pixmap[direction][index].width()*0.5,-pixmap[direction][index].height()*0.5);
    QPoint offsetPointEffect(-pixEffect[indexEffect].width()*0.5,-pixEffect[indexEffect].height()*0.5);
    QPoint offsetPointFlame(-pixFlame[indexFlame].width()*0.5,-pixFlame[indexFlame].height()*0.5);
    QPoint offsetPointProtected(-pixProtect.width()*0.5,-pixProtect.height()*0.5);
    //加入重复图像的补丁
    QPoint overLapPoint(0,0);
    int overNum=(overlapNum>0)?overlapNum-1:0;
    switch(direction)
    {
    case 0:
        overLapPoint=QPoint(0,overNum*incre);break;
    case 1:
        overLapPoint=QPoint(0,-overNum*incre);break;
    case 2:
        overLapPoint=QPoint(overNum*incre,0);break;
    case 3:
        overLapPoint=QPoint(-overNum*incre,0);break;
    default:;
    }
    offsetPoint+=overLapPoint;
    offsetPointEffect+=overLapPoint;
    offsetPointFlame+=overLapPoint;
    offsetPointProtected+=overLapPoint;
    painter->translate(curPoint);//挪到单元格中心
    //画敌人
    painter->drawPixmap(offsetPoint,pixmap[direction][index]);
    //画防护罩
    if(protectedFlag) painter->drawPixmap(offsetPointProtected,pixProtect);
    if(lockedPagoda&&!frozenFlag)//没被冻住
    {   //攻击特效
        painter->drawPixmap(offsetPointEffect,pixEffect[indexEffect]);
    }
    if(!pagodaAttackList.isEmpty()||!cannonAttackList.isEmpty()||frozenFlag||toxicFlag||flameFlag)
    {   //血条特效
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawRect(QRect(QPoint(-healthWith/2,-FixedLen)+overLapPoint,QSize(healthWith,5)));
        painter->setBrush(Qt::green);
        painter->drawRect(QRect(QPoint(-healthWith/2,-FixedLen)+overLapPoint,QSize((double)curHp/maxHp*healthWith,5)));
    }
    if(flameFlag){//旋风特效
        painter->drawPixmap(offsetPointFlame,pixFlame[indexFlame]);
    }
    painter->restore();
}

void Enemy::InitSpeedPixmap()
{
    //加载神速行走
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            QString fileName=QString(":/image/crazyWerewolf%1%2.png").arg(i).arg(j);
            QPixmap map(fileName);
            map=map.scaled(QSize(map.width()*1.2,map.height()*1.2),Qt::KeepAspectRatio);
            pixmap[i][j]=map;
        }
    }
}

void Enemy::InitPixmap()
{
    //加载行走
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            QString fileName=QString(":/image/werewolf%1%2.png").arg(i).arg(j);
            QPixmap map(fileName);
            map=map.scaled(QSize(map.width()*1.2,map.height()*1.2),Qt::KeepAspectRatio);
            pixmap[i][j]=map;
        }
    }
    //加载抓痕
    for(int i=0;i<2;i++)
    {
        QString fileName=QString(":/image/red_claw%1.png").arg(i);
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*0.35,map.height()*0.35),Qt::KeepAspectRatio);
        pixEffect.push_back(map);
    }
    //加载flame旋风
    for(int i=0;i<3;i++)
    {
        QString fileName=QString(":/image/purpleWind%1.png").arg(i);
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*0.5,map.height()*0.5),Qt::KeepAspectRatio);
        pixFlame.push_back(map);
    }
    pixProtect=QPixmap(":/image/particleCircle.png");
    pixProtect=pixProtect.scaled(QSize(pixProtect.width()*0.55,pixProtect.height()*0.55),Qt::KeepAspectRatio);
}

void Enemy::UpdateDirection(QPoint cur,QPoint next)
{
    //0表向上，1表向下，2表向左，3表示向右
    if(cur.x()==next.x())
    {
        if(cur.y()>next.y()) direction=0;
        else if(cur.y()<next.y()) direction=1;
    }
    else if(cur.y()==next.y())
    {
        if(cur.x()>next.x()) direction=2;
        else if(cur.x()<next.x()) direction=3;
    }
}

