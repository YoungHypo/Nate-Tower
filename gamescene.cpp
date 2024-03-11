#include "gamescene.h"
#include "ui_gamescene.h"
#include "Common.h"
#include "choosemapscene.h"
#include <vld.h>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <algorithm>

GameScene::GameScene(QVector<int>& rpt0,QVector<int>& rpt1,chooseMapScene* cML,QWidget *parent) :
    QWidget(parent),
    timer(NULL),
    gameMapScene(cML),
    type(0),
    fiercePagodaNum(5),
    icePagodaNum(5),
    goldPagodaNum(5),
    fierceCannonNum(5),
    flameCannonNum(5),
    rangeCannonNum(5),
    enemyNum(21),
    maxBlood(1000),
    curBlood(maxBlood),
    gameMoney(1500),
    isGameWin(false),
    isGameLose(false),
    ui(new Ui::GameScene)
{
    ui->setupUi(this);
    this->setFixedSize(1260,900);
    this->setWindowTitle("Tower Defence");
    this->setWindowIcon(QIcon(":/image/yellowdiamond4.png"));

    doorIndex[0]=doorIndex[1]=0;
    eventdoorId[0]=eventdoorId[1]=-1;
    doorOpenFlag[0]=doorOpenFlag[1]=false;

    routePoint[0]=rpt0;
    routePoint[1]=rpt1;

    /*初始化工作*/
    InitPixmap();
    InitButton();
    InitMediaPlayer();

    SignalConnection();

    //加载地图
    LoadMap();
}

void GameScene::StartGame()
{
    qDebug()<<"start game";
    timer->start(300);
}

void GameScene::EndGame()
{
    qDebug()<<"end game";
    timer->stop();
    boomList.clear();
    bulletList.clear();

    QPropertyAnimation *animation;
    if(isGameWin){
        animation=new QPropertyAnimation(winLabel,"geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(QPoint(winLabel->x(),winLabel->y()),QPoint(winLabel->x()+winLabel->width(),winLabel->height())));
        animation->setEndValue(QRect(QPoint(winLabel->x(),winLabel->y()+400),QPoint(winLabel->x()+winLabel->width(),winLabel->height()+400)));

        animation->setEasingCurve(QEasingCurve::OutBounce);
        animation->start();
        AudioPlay(winPlayer);
    }
    else if(isGameLose){
        animation=new QPropertyAnimation(loseLabel,"geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(QPoint(loseLabel->x(),loseLabel->y()),QPoint(loseLabel->x()+loseLabel->width(),loseLabel->height())));
        animation->setEndValue(QRect(QPoint(loseLabel->x(),loseLabel->y()+400),QPoint(loseLabel->x()+loseLabel->width(),loseLabel->height()+400)));
        animation->setEasingCurve(QEasingCurve::OutBounce);
        animation->start();
        AudioPlay(losePlayer);
    }
}

void GameScene::UpdateScene()
{
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        foreach(Enemy *enemy,enemyList)
            enemy->Move();

        CheckDoorState();

        foreach (Pagoda* pagoda, pagodaList)
            pagoda->LockTargetWithinAttackRange();

        foreach (Cannon* cannon, cannonList)
            cannon->LockEnemyWithinAttackRange();

        foreach(Bullet *bullet,bulletList)
            bullet->SetAnimationEndValue();

        update();
    });
}

void GameScene::LoadMap()
{
    for(int t=0;t<2;t++)
    {
        if(routePoint[t].empty()) continue;
        //更新point标志
        int i = 0, j = 0;
        while (i < (int)routePoint[t].size())
        {
            j = i + 1;
            if (j >= (int)routePoint[t].size())break;
            int p = std::min(routePoint[t][i], routePoint[t][j]);
            int q = std::max(routePoint[t][i], routePoint[t][j]);
            if (routePoint[t][i] / 12 == routePoint[t][j] / 12)//同一行
                for (int k = p; k <= q; k ++)
                    pointFlag[k].isRoute[t] = true;
            else if (routePoint[t][i] % 12 == routePoint[t][j] % 12)//同一列
                for (int k = p; k <= q; k += 12)
                    pointFlag[k].isRoute[t] = true;
            i = j;
        }
        //装载routepointList
        RoutePoint * former=new RoutePoint(GetPointRoute(routePoint[t][0]));
        routePointList[t].push_back(former);
        for(int i=1;i<(int)routePoint[t].size();i++){
            RoutePoint * rpt=new RoutePoint(GetPointRoute(routePoint[t][i]));
            routePointList[t].push_back(rpt);
            former->AddNextPoint(rpt);
            former=rpt;
        }
        qDebug()<<"success load route";
    }
}

void GameScene::LoadEnemy()
{   
    for(int wave=0;wave<2;wave++){
        //初始化
        spawntime[0].clear();spawntime[1].clear();
        int delta=0,startTime=0;
        if(wave==0) startTime=10*Second;
        else if(wave==1) startTime=5*Second*enemyNum+25*Second;
        //加载两条路线敌人
        for(int t=0;t<2;t++)
        {
            if(routePoint[t].empty()) continue;
            if(t==1) delta=0.5*Second;
            RoutePoint * spoint=routePointList[t].front();
            //生成加载时间数组
            for(int i=0;i<enemyNum;i++) spawntime[t].push_back(5*i*Second+delta+startTime);
            for(int i=0;i<spawntime[t].size();i++)
            {   //加载不同类型的敌人
                if(i<13){
                    bool tFlag=(i>=5&&i<9)?true:false;
                    bool pFlag=(i>=9&&i<13)?true:false;
                    Enemy * enemy = new Enemy(spoint,this,tFlag,pFlag);
                    QTimer::singleShot(spawntime[t][i], this,[=](){
                        if(isGameLose||isGameWin) return;
                        enemy->DoActivate();
                        enemyList.push_back(enemy);
                    });
                }
                if(i>=13&&i<17){
                    FlashEnemy* flashEnemy=new FlashEnemy(spoint,this);
                    QTimer::singleShot(spawntime[t][i], this,[=](){
                        if(isGameLose||isGameWin) return;
                        flashEnemy->DoActivate();
                        enemyList.push_back(flashEnemy);
                    });
                }
                if(i>=17&&i<21){
                    BossEnemy* bossEnemy=new BossEnemy(spoint,this);
                    QTimer::singleShot(spawntime[t][i], this,[=](){
                        if(isGameLose||isGameWin) return;
                        bossEnemy->DoActivate();
                        enemyList.push_back(bossEnemy);
                    });
                }
            }
        }
    }
    if(!routePoint[0].empty()&&!routePoint[1].empty()) enemyNum*=4;
    else enemyNum*=2;
}

void GameScene::LoadBullet(Bullet *bullet)
{
    bulletList.push_back(bullet);
}

void GameScene::timerEvent(QTimerEvent * ev)
{
    if(isGameLose||isGameWin) return;
    //处理敌人终点的开关门事件
    for(int t=0;t<2;t++)
    {
        if(routePoint[t].empty()) continue;
        if(ev->timerId()==eventdoorId[t])
        {
            if(doorOpenFlag[t])//门要开了，放开门动画
            {
                doorIndex[t]++;
                if(doorIndex[t]>=4){
                    doorIndex[t]=3;
                }
            }
            else if(!doorOpenFlag[t])//门要关了，放关门动画
            {
                doorIndex[t]--;
                if(doorIndex[t]<=-1){
                    doorIndex[t]=0;
                }
            }
        }

    }
}

void GameScene::CheckOverlap()
{
    //重新更新overlap
    for(QList<Enemy *>::iterator i=enemyList.begin();i!=enemyList.end();i++)
        (*i)->overlapNum=0;
    //时时更新
    for(QList<Enemy *>::iterator i=enemyList.begin();i!=enemyList.end();i++){//已经和别的重合的就不比了
        if((*i)->overlapNum>1) continue;
        int overNum=1;
        for(QList<Enemy *>::iterator j=i+1;j!=enemyList.end();j++){
            if((*i)->getPos()==(*j)->getPos()){
                if(!(*i)->overlapNum) (*i)->overlapNum=1;
                (*j)->overlapNum=++overNum;
            }
        }
    }
}

void GameScene::CheckDoorState()
{
    for(int t=0;t<2;t++){
        if(!routePointList[t].isEmpty()){
            QPoint epoint=routePointList[t].last()->Pos();
            bool testDoorFlag=false;
            foreach(Enemy *enemy,enemyList){
                if(LineDistance(enemy->getPos(),epoint,89)){
                    testDoorFlag=true;
                    break;
                }
            }
            if(!doorOpenFlag[t]&&testDoorFlag){//之前没敌人来，敌人第一次来，整个开门动画
                qDebug()<<"come in,pre animation";
                doorOpenFlag[t]=true;
            }
            else if(doorOpenFlag[t]&&!testDoorFlag){//门之前开了，但是敌人不再来了，整关门特效
                qDebug()<<"come out,pre animation";
                doorOpenFlag[t]=false;
            }
        }
    }
}

void GameScene::DeleteEnemy(Enemy *enemy,bool flag)
{
    //flag为true表示狼人是到达终点消失的，false表示让炮打死了
    if(!flag)
    {
        Boom* boom=new Boom(enemy->getPos(),2,this);
        boomList.push_back(boom);
        DropAffixes(enemy->Type(),enemy->getPos());
    }
    //要是狼人还锁定着塔，尽快从塔的被攻击名单中出去
    if(enemy->LockedPagoda()&&pagodaList.indexOf(enemy->LockedPagoda())!=-1)
    {
        enemy->LockedPagoda()->enemyAttackList.removeOne(enemy);
    }
    enemyList.removeOne(enemy);
    delete enemy;
    enemyNum--;
    if(enemyNum<=0){
        enemyNum=0;
        isGameWin=true;
        EndGame();
    }
}

void GameScene::DeletePagoda(Pagoda* pagoda,bool flag)
{
    if(!flag)
    {
        Boom* boom=new Boom(pagoda->getPos(),1,this);
        boomList.push_back(boom);
    }
    //塔还锁定着狼人
    if(pagoda->LockedEnemy()&&enemyList.indexOf(pagoda->LockedEnemy())!=-1)
    {
        pagoda->LockedEnemy()->pagodaAttackList.removeOne(pagoda);
    }
    QPoint tmpPos=pagoda->getPos();
    pointFlag[GetPointIndex(tmpPos)].isPagoda=0;//注意标记
    pagodaList.removeOne(pagoda);
    delete pagoda;
}

void GameScene::DeleteCannon(Cannon* cannon,bool flag)
{
    if(!flag){
        Boom* boom=new Boom(cannon->getPos(),1,this);
        boomList.push_back(boom);
    }
    if(cannon->LockedEnemy()&&enemyList.indexOf(cannon->LockedEnemy())!=-1)
    {
        cannon->LockedEnemy()->cannonAttackList.removeOne(cannon);
    }
    QPoint tmpPos=cannon->getPos();
    pointFlag[GetPointIndex(tmpPos)].isTower=0;//注意标记
    cannonList.removeOne(cannon);
    delete cannon;
}

void GameScene::DeleteBullet(Bullet *bullet, bool flag)
{
    //注意加特效
    if(!flag&&enemyList.indexOf(bullet->TargetEnemy())!=-1)
    {
        Boom* boom=new Boom(bullet->ReadCurPos(),3,this,bullet->PixSize());
        boomList.push_back(boom);
    }
    bulletList.removeOne(bullet);
    delete bullet;
}

void GameScene::DeleteBoom(Boom * boom)
{
    boomList.removeOne(boom);
    delete boom;
}

void GameScene::DropAffixes(int t,QPoint pos)
{
    switch(t){
    case 1:break;
    case 2:{//神速怪掉落狂暴属性
        Boom* boom=new Boom(pos+QPoint(0,20),12,this);
        boomList.push_back(boom);
        AudioPlay(diamondPlayer);
        QTimer::singleShot(3500, this,[=](){
            if(enemyNum%2){
                fierceCannonNum++;
            }
            else{
                fiercePagodaNum++;
            }
        });
        break;}
    case 3:{
        if(enemyNum%2){
            Boom* boom=new Boom(pos+QPoint(0,20),14,this);
            boomList.push_back(boom);
            AudioPlay(diamondPlayer);
            QTimer::singleShot(3500, this,[=](){
                flameCannonNum++;
            });
        }
        break;
    }
    case 4:{//防护怪掉落冰冻属性
        if(enemyNum%2){//隔一个掉一个
            Boom* boom=new Boom(pos+QPoint(0,20),13,this);
            boomList.push_back(boom);
            AudioPlay(diamondPlayer);
            QTimer::singleShot(3500, this,[=](){
                icePagodaNum++;
            });
        }
        break;
    }
    case 5:{
        if(enemyNum%2){//隔一个掉一个
            Boom* boom=new Boom(pos+QPoint(0,20),15,this);
            boomList.push_back(boom);
            AudioPlay(diamondPlayer);
            QTimer::singleShot(3500, this,[=](){
                goldPagodaNum++;
            });
        }
        else{
            Boom* boom=new Boom(pos+QPoint(0,20),16,this);
            boomList.push_back(boom);
            AudioPlay(diamondPlayer);
            QTimer::singleShot(3500, this,[=](){
                rangeCannonNum++;
            });
        }
        break;
    }
    default:;
    }
}

void GameScene::AudioPlay(QMediaPlayer* player){
    if(player->playbackState()==QMediaPlayer::PlayingState)
        player->stop();
    player->play();
}

GameScene::~GameScene()
{
    foreach(Enemy *enemy,enemyList)
        delete enemy;
    foreach (Pagoda* pagoda, pagodaList)
        delete pagoda;
    foreach (Cannon* cannon, cannonList)
        delete cannon;
    foreach (Tower *tower, towerList)
        delete tower;
    foreach (Bullet *bullet, bulletList)
        delete bullet;
    foreach (Boom *boom, boomList)
        delete boom;
    for(int t=0;t<2;t++)
        foreach(RoutePoint* rtplist,routePointList[t])
            delete rtplist;

    delete backBtn;
    delete towerBtn;
    delete cannonBtn;
    delete pagodaBtn;
    delete icePagodaBtn;
    delete fiercePagodaBtn;
    delete goldPagodaBtn;
    delete fierceCannonBtn;
    delete flameCannonBtn;
    delete rangeCannonBtn;

    delete deadPlayer;
    delete deadAudioOutput;
    delete flashPlayer;
    delete flashAudioOutput;
    delete frozenPlayer;
    delete frozenAudioOutput;
    delete lightPlayer;
    delete lightAudioOutput;
    delete pagodaDeletePlayer;
    delete pagodaDeleteAudioOutput;
    delete cannonDeletePlayer;
    delete cannonDeleteAudioOutput;
    delete buttonPlayer;
    delete buttonAudioOutput;
    delete backPlayer;
    delete backAudioOutput;
    delete upgradePagodaPlayer;
    delete upgradePagodaAudioOutput;
    delete upgradeCannonPlayer;
    delete upgradeCannonAudioOutput;
    delete removalPlayer;
    delete removalAudioOutput;
    delete errorPlayer;
    delete errorAudioOutput;
    delete setPlayer;
    delete setAudioOutput;
    delete diamondPlayer;
    delete diamondAudioOutput;
    delete winPlayer;
    delete winAudioOutput;
    delete losePlayer;
    delete loseAudioOutput;

    delete timer;
    delete ui;
}
