#include "gamescene.h"
#include "Common.h"
#include "choosemapscene.h"
#include <vld.h>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <algorithm>
#include <QAudioOutput>

void GameScene::InitPixmap()
{
    pixbg=QPixmap(":/image/background.png");
    pixpath[0]=QPixmap(":/image/groundpath1.png");
    pixpath[0]=pixpath[0].scaled(FixedSize,Qt::KeepAspectRatio);
    pixpath[1]=QPixmap(":/image/groundpath.png");
    pixpath[1]=pixpath[1].scaled(FixedSize,Qt::KeepAspectRatio);
    pixground=QPixmap(":/image/ground.png");
    pixground=pixground.scaled(FixedSize,Qt::KeepAspectRatio);
    pixFlag=QPixmap(":/image/Wormholes.png");
    pixFlag=pixFlag.scaled(FixedSize,Qt::KeepAspectRatio);

    pixBlood=QPixmap(":/image/Blood.png");
    pixBlood=pixBlood.scaled(pixBlood.width()*0.4,pixBlood.height()*0.4,Qt::KeepAspectRatio);
    pixMoney=QPixmap(":/image/money.png");
    pixMoney=pixMoney.scaled(pixMoney.width()*0.38,pixMoney.height()*0.38,Qt::KeepAspectRatio);
    pixenemy=QPixmap(":/image/werewolf30.png");

    pixmain=QPixmap(":/image/mainbg1.png");
    pixWin=QPixmap(":/image/success.png");
    pixWin=pixWin.scaled(pixWin.width()*1.2,pixWin.height()*1.2,Qt::KeepAspectRatio);
    pixLose=QPixmap(":/image/failure.png");
    pixLose=pixLose.scaled(pixLose.width()*0.5,pixLose.height()*0.5,Qt::KeepAspectRatio);

    winLabel=new QLabel;
    winLabel->setParent(this);
    winLabel->setGeometry(0,0,pixWin.width(),pixWin.height());
    winLabel->setPixmap(pixWin);
    winLabel->move((this->width()-pixWin.width())*0.5+60,-pixWin.height());

    loseLabel=new QLabel;
    loseLabel->setParent(this);
    loseLabel->setGeometry(0,0,pixLose.width(),pixLose.height());
    loseLabel->setPixmap(pixLose);
    loseLabel->move((this->width()-pixLose.width())*0.5+50,-pixLose.height());

    //装载pixmap——door
    for(int i=0;i<4;i++)
    {
         QString fileName=QString(":/image/door%1.png").arg(i);
         QPixmap map(fileName);
         map=map.scaled(QSize(map.width()*0.9,map.height()*0.9),Qt::KeepAspectRatio);
         pixdoor[i]=map;//保存在数组里
    }
    //开启门动画计时器
    for(int t=0;t<2;t++)
    {
        if(!routePoint[t].empty()) eventdoorId[t]=startTimer(200);
    }
}

void GameScene::InitMediaPlayer()
{
    buttonPlayer = new QMediaPlayer;
    buttonAudioOutput = new QAudioOutput;
    buttonPlayer->setAudioOutput(buttonAudioOutput);
    buttonPlayer->setSource(QUrl::fromLocalFile("../project2/media/buttonClick.mp3"));

    backPlayer = new QMediaPlayer;
    backAudioOutput = new QAudioOutput;
    backPlayer->setAudioOutput(backAudioOutput);
    backPlayer->setSource(QUrl::fromLocalFile("../project2/media/messageReturn.mp3"));

    upgradePagodaPlayer = new QMediaPlayer;
    upgradePagodaAudioOutput = new QAudioOutput;
    upgradePagodaPlayer->setAudioOutput(upgradePagodaAudioOutput);
    upgradePagodaPlayer->setSource(QUrl::fromLocalFile("../project2/media/upgradePagoda.mp3"));

    upgradeCannonPlayer = new QMediaPlayer;
    upgradeCannonAudioOutput = new QAudioOutput;
    upgradeCannonPlayer->setAudioOutput(upgradeCannonAudioOutput);
    upgradeCannonPlayer->setSource(QUrl::fromLocalFile("../project2/media/upgradeCannon.mp3"));

    removalPlayer = new QMediaPlayer;
    removalAudioOutput = new QAudioOutput;
    removalPlayer->setAudioOutput(removalAudioOutput);
    removalPlayer->setSource(QUrl::fromLocalFile("../project2/media/delete.mp3"));

    errorPlayer = new QMediaPlayer;
    errorAudioOutput = new QAudioOutput;
    errorPlayer->setAudioOutput(errorAudioOutput);
    errorPlayer->setSource(QUrl::fromLocalFile("../project2/media/error.mp3"));

    setPlayer = new QMediaPlayer;
    setAudioOutput = new QAudioOutput;
    setPlayer->setAudioOutput(setAudioOutput);
    setPlayer->setSource(QUrl::fromLocalFile("../project2/media/click.mp3"));

    diamondPlayer = new QMediaPlayer;
    diamondAudioOutput = new QAudioOutput;
    diamondPlayer->setAudioOutput(diamondAudioOutput);
    diamondPlayer->setSource(QUrl::fromLocalFile("../project2/media/pickDiamond.mp3"));

    winPlayer = new QMediaPlayer;
    winAudioOutput = new QAudioOutput;
    winPlayer->setAudioOutput(winAudioOutput);
    winPlayer->setSource(QUrl::fromLocalFile("../project2/media/win.mp3"));

    losePlayer = new QMediaPlayer;
    loseAudioOutput = new QAudioOutput;
    losePlayer->setAudioOutput(loseAudioOutput);
    losePlayer->setSource(QUrl::fromLocalFile("../project2/media/lose.mp3"));

    deadPlayer = new QMediaPlayer;
    deadAudioOutput = new QAudioOutput;
    deadPlayer->setAudioOutput(deadAudioOutput);
    deadPlayer->setSource(QUrl::fromLocalFile("../project2/media/enemyDelete.mp3"));

    flashPlayer = new QMediaPlayer;
    flashAudioOutput = new QAudioOutput;
    flashPlayer->setAudioOutput(flashAudioOutput);
    flashPlayer->setSource(QUrl::fromLocalFile("../project2/media/flash.mp3"));

    frozenPlayer = new QMediaPlayer;
    frozenAudioOutput = new QAudioOutput;
    frozenPlayer->setAudioOutput(frozenAudioOutput);
    frozenPlayer->setSource(QUrl::fromLocalFile("../project2/media/iceFrozen.mp3"));

    lightPlayer = new QMediaPlayer;
    lightAudioOutput = new QAudioOutput;
    lightPlayer->setAudioOutput(lightAudioOutput);
    lightPlayer->setSource(QUrl::fromLocalFile("../project2/media/light.mp3"));

    pagodaDeletePlayer = new QMediaPlayer;
    pagodaDeleteAudioOutput = new QAudioOutput;
    pagodaDeletePlayer->setAudioOutput(pagodaDeleteAudioOutput);
    pagodaDeletePlayer->setSource(QUrl::fromLocalFile("../project2/media/pagodaDelete.mp3"));

    cannonDeletePlayer = new QMediaPlayer;
    cannonDeleteAudioOutput = new QAudioOutput;
    cannonDeletePlayer->setAudioOutput(cannonDeleteAudioOutput);
    cannonDeletePlayer->setSource(QUrl::fromLocalFile("../project2/media/cannonDelete.mp3"));
}

void GameScene::InitButton()
{
    backBtn=new MyButton(":/image/backBtn.png",0.2);
    backBtn->setParent(this);
    backBtn->move(90-backBtn->width()*0.5,this->height()-backBtn->height());

    towerBtn=new MyButton(":/image/Tower.png",0.21);
    towerBtn->setParent(this);
    towerBtn->move(79-towerBtn->width()*0.5,33);

    pagodaBtn=new MyButton(":/image/pagoda.png",0.65);
    pagodaBtn->setParent(this);
    pagodaBtn->move(30-pagodaBtn->width()*0.5,120);

    fiercePagodaBtn=new MyButton(":/image/fiercePagoda.png",0.65);
    fiercePagodaBtn->setParent(this);
    fiercePagodaBtn->move(115-fiercePagodaBtn->width()*0.5,120);

    icePagodaBtn=new MyButton(":/image/icePagoda.png",0.65);
    icePagodaBtn->setParent(this);
    icePagodaBtn->move(30-icePagodaBtn->width()*0.5,210);

    goldPagodaBtn=new MyButton(":/image/goldPagoda.png",0.65);
    goldPagodaBtn->setParent(this);
    goldPagodaBtn->move(115-goldPagodaBtn->width()*0.5,210);

    cannonBtn=new MyButton(":/image/cannon.png",0.25);
    cannonBtn->setParent(this);
    cannonBtn->move(30-cannonBtn->width()*0.5,320);

    fierceCannonBtn=new MyButton(":/image/Cannon3.png",0.25);
    fierceCannonBtn->setParent(this);
    fierceCannonBtn->move(113-fierceCannonBtn->width()*0.5,320);

    flameCannonBtn=new MyButton(":/image/MG3.png",0.30);
    flameCannonBtn->setParent(this);
    flameCannonBtn->move(27-flameCannonBtn->width()*0.5,405);

    rangeCannonBtn=new MyButton(":/image/Missile_Launcher3.png",0.30);
    rangeCannonBtn->setParent(this);
    rangeCannonBtn->move(118-rangeCannonBtn->width()*0.5,420);
}

void GameScene::SignalConnection()
{
    connect(backBtn,&MyButton::clicked,this,[=](){

        backBtn->zoomdown();
        backBtn->zoomup();
        AudioPlay(backPlayer);

        QTimer::singleShot(400,this,[=](){
            emit chooseBack();
        });
    });

    connect(towerBtn,&MyButton::clicked,this,[=](){
        towerBtn->zoomdown();
        towerBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=1;
        qDebug()<<type;
    });

    //cannon信号连接
    connect(cannonBtn,&MyButton::clicked,this,[=](){
        cannonBtn->zoomdown();
        cannonBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=2;
        qDebug()<<type;
    });
    connect(fierceCannonBtn,&MyButton::clicked,this,[=](){

        fierceCannonBtn->zoomdown();
        fierceCannonBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=4;
        qDebug()<<type;
    });
    connect(flameCannonBtn,&MyButton::clicked,this,[=](){

        flameCannonBtn->zoomdown();
        flameCannonBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=6;
        qDebug()<<type;
    });
    connect(rangeCannonBtn,&MyButton::clicked,this,[=](){

        rangeCannonBtn->zoomdown();
        rangeCannonBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=8;
        qDebug()<<type;
    });

    //pagoda信号连接
    connect(pagodaBtn,&MyButton::clicked,this,[=](){
        pagodaBtn->zoomdown();
        pagodaBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=3;
        qDebug()<<type;
    });
    connect(fiercePagodaBtn,&MyButton::clicked,this,[=](){

        fiercePagodaBtn->zoomdown();
        fiercePagodaBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=5;
        qDebug()<<type;
    });
    connect(icePagodaBtn,&MyButton::clicked,this,[=](){

        icePagodaBtn->zoomdown();
        icePagodaBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=7;
        qDebug()<<type;
    });
    connect(goldPagodaBtn,&MyButton::clicked,this,[=](){

        goldPagodaBtn->zoomdown();
        goldPagodaBtn->zoomup();
        AudioPlay(buttonPlayer);

        type=9;
        qDebug()<<type;
    });

    connect(gameMapScene,&chooseMapScene::enterGame,this,[=](){
        LoadEnemy();
        UpdateScene();
    });
}
