#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "routepoint.h"
#include "enemy.h"
#include "tower.h"
#include "cannon.h"
#include "pagoda.h"
#include "boom.h"
#include "mybutton.h"
#include "bullet.h"
#include "Common.h"


class chooseMapScene;

namespace Ui {
class GameScene;
}

class GameScene : public QWidget
{
    Q_OBJECT

public:
    explicit GameScene(QVector<int>& rpt0,QVector<int>& rpt1,chooseMapScene* cML,QWidget *parent = nullptr);

    void InitPixmap();                  //初始化图像
    void InitButton();                  //初始化按钮
    void InitMediaPlayer();             //初始化音效
    void SignalConnection();            //初始化槽函数
    void LoadMap();                     //加载地图
    void LoadEnemy();                   //加载敌人
    void LoadBullet(Bullet *bullet);    //加载子弹
    void UpdateScene();                 //更新场景
    void CheckDoorState();              //检查终点门的状态，即是否要打开以迎接敌人
    void DropAffixes(int t,QPoint pos); //掉落词缀
    void AudioPlay(QMediaPlayer* player);//播放背景音乐

    //鼠标点击事件
    void mousePressEvent(QMouseEvent * e);
    void mouseTowerEvent(int index);                //鼠标点击建基座
    void mouseCannonEvent(int index);               //鼠标点击建炮
    void mousePagodaEvent(int index);               //鼠标点击建塔
    void mouseFiercePagodaEvent(int index);         //鼠标点击升级近战塔为狂暴
    void mouseFierceCannonEvent(int index);         //鼠标点击升级炮为狂暴
    template<class TPagoda>                         //鼠标点击升级为特定功能塔
    void mousePagodaInheritEvent(int index,int boomType,int pagodaType,QString pName);
    template<class TCannon>                         //鼠标点击升级为特定功能炮
    void mouseCannonInheritEvent(int index,int boomType,int cannonType,QString pName);

    //时间事件
    void timerEvent(QTimerEvent *);
    void StartGame();
    void EndGame();
    void CheckOverlap();                                //检查敌人图像重叠情况
    void DeleteEnemy(Enemy *enemy,bool flag=false);     //删除敌人
    void DeletePagoda(Pagoda* pagoda,bool flag=false);  //删除塔
    void DeleteCannon(Cannon* cannon,bool flag=false);  //删除炮
    void DeleteBullet(Bullet *bullet,bool flag=false);  //删除子弹
    void DeleteBoom(Boom * boom);                       //删除特效

    //绘图事件
    void paintEvent(QPaintEvent *);
    void DrawTimes(QPainter& painter,const QRect rect,int num=0,bool flag=false);   //画数字
    void DrawBlood(QPainter& painter,const QRect rect,int num=0);                   //画总血量

    ~GameScene();

    QTimer * timer;                     //刷新界面计时器
    chooseMapScene* gameMapScene;       //选关对象指针
    PointFlags pointFlag[120];          //地图信息
    QLabel* winLabel;
    QLabel* loseLabel;

    //容器
    QList<RoutePoint *> routePointList[2];  //航点列表，两条路径
    QList<Enemy *>      enemyList;          //敌人列表
    QList<Tower *>      towerList;          //基座列表
    QList<Cannon *>     cannonList;         //炮列表
    QList<Pagoda *>     pagodaList;         //塔列表
    QList<Boom *>       boomList;           //特效列表
    QList<Bullet *>     bulletList;         //子弹列表
    QVector<int>        spawntime[2];       //敌人生成时间列表，两条路径

    int type;   //当前鼠标事件的类型

    //游戏资源数目
    int fiercePagodaNum;    //狂暴塔数目
    int icePagodaNum;       //冰塔数目
    int goldPagodaNum;      //黄金毒塔数目
    int fierceCannonNum;    //狂暴炮数目
    int flameCannonNum;     //持续炮数目
    int rangeCannonNum;     //范围炮数目
    int enemyNum;           //敌人数目
    int maxBlood;           //最大血量
    int curBlood;           //当前血量
    int gameMoney;          //敌人总数目

    //游戏状态
    bool isGameWin;         //游戏胜利标志
    bool isGameLose;        //游戏失败标志

    //单位音效
    QMediaPlayer * deadPlayer;
    QAudioOutput * deadAudioOutput;
    QMediaPlayer * flashPlayer;
    QAudioOutput * flashAudioOutput;
    QMediaPlayer * frozenPlayer;
    QAudioOutput * frozenAudioOutput;
    QMediaPlayer * lightPlayer;
    QAudioOutput * lightAudioOutput;
    QMediaPlayer * pagodaDeletePlayer;
    QAudioOutput * pagodaDeleteAudioOutput;
    QMediaPlayer * cannonDeletePlayer;
    QAudioOutput * cannonDeleteAudioOutput;

private:
    Ui::GameScene *ui;
    QVector<int> routePoint[2];

    //按钮
    //basic button
    MyButton * backBtn;
    MyButton * towerBtn;
    MyButton * cannonBtn;
    MyButton * pagodaBtn;
    //extend pagoda button
    MyButton * icePagodaBtn;
    MyButton * fiercePagodaBtn;
    MyButton * goldPagodaBtn;
    //extend cannon button
    MyButton * fierceCannonBtn;
    MyButton * flameCannonBtn;
    MyButton * rangeCannonBtn;

    //图像
    QPixmap pixdoor[4];
    QPixmap pixbg;
    QPixmap pixground;
    QPixmap pixpath[2];
    QPixmap pixFlag;
    QPixmap pixBlood;
    QPixmap pixMoney;
    QPixmap pixmain;
    QPixmap pixenemy;
    QPixmap pixWin;
    QPixmap pixLose;

    //门图像遍历
    int doorIndex[2];       //门图像下标
    int eventdoorId[2];     //门图像时间ID
    bool doorOpenFlag[2];   //门图像状态

    //整体音效
    QMediaPlayer * buttonPlayer;
    QAudioOutput * buttonAudioOutput;
    QMediaPlayer * backPlayer;
    QAudioOutput * backAudioOutput;
    QMediaPlayer * upgradePagodaPlayer;
    QAudioOutput * upgradePagodaAudioOutput;
    QMediaPlayer * upgradeCannonPlayer;
    QAudioOutput * upgradeCannonAudioOutput;
    QMediaPlayer * removalPlayer;
    QAudioOutput * removalAudioOutput;
    QMediaPlayer * errorPlayer;
    QAudioOutput * errorAudioOutput;
    QMediaPlayer * setPlayer;
    QAudioOutput * setAudioOutput;
    QMediaPlayer * diamondPlayer;
    QAudioOutput * diamondAudioOutput;
    QMediaPlayer * winPlayer;
    QAudioOutput * winAudioOutput;
    QMediaPlayer * losePlayer;
    QAudioOutput * loseAudioOutput;

signals:
    void chooseBack();
};

template<class TPagoda>
void GameScene::mousePagodaInheritEvent(int index,int boomType,int pagodaType,QString pName)
{
    if(!pointFlag[index].isPagoda){//该点无pagoda不种
        AudioPlay(errorPlayer);
        return;
    }
    //查找pagoda
    Pagoda* pagoda=NULL;
    QPoint pagodapos=GetPointRoute(index);
    foreach (Pagoda* pagotmp, pagodaList) {
        QPoint pointmp=pagotmp->getPos();
        if(pointmp==pagodapos){
            pagoda=pagotmp;
            break;
        }
    }
    if(!pagoda){
        AudioPlay(errorPlayer);
        return;
    }
    //升级
    if(pagoda->Type()==1){
        if(pagodaType==3){
            if(icePagodaNum<=0){
                AudioPlay(errorPlayer);
                return;
            }
            icePagodaNum--;
        }
        else if(pagodaType==4){
            if(goldPagodaNum<=0){
                AudioPlay(errorPlayer);
                return;
            }
            goldPagodaNum--;
        }
        //删除原有普通塔
        QPoint pagodaPos=pagoda->getPos();
        DeletePagoda(pagoda,true);
        //升级特效
        Boom* boom=new Boom(pagoda->getPos(),boomType,this);
        boomList.push_back(boom);
        //加入拓展塔
        pointFlag[index].isPagoda=true;
        TPagoda* tPagoda=new TPagoda(pagodaPos,pName,this);
        pagodaList.push_back(tPagoda);
        gameMoney-=tPagoda->Coin();
        AudioPlay(upgradePagodaPlayer);

    }
    else if(pagoda->Type()==pagodaType){
        pointFlag[index].isPagoda=0;
        gameMoney+=pagoda->Coin()*0.8;
        AudioPlay(removalPlayer);
        DeletePagoda(pagoda,true);
    }
}

template<class TCannon>
void GameScene::mouseCannonInheritEvent(int index,int boomType,int cannonType,QString pName)
{
    if(!pointFlag[index].isTower){//该点无Tower不种
        AudioPlay(errorPlayer);
        return;
    }
    //查找cannon
    Cannon* cannon=NULL;
    QPoint cannonpos=GetPointRoute(index);
    foreach (Cannon* cantmp, cannonList) {
        QPoint pointmp=cantmp->getPos();
        if(pointmp==cannonpos){
            cannon=cantmp;
            break;
        }
    }
    if(!cannon){
        AudioPlay(errorPlayer);
        return;
    }
    //升级
    if(cannon->Type()==1){
        if(cannonType==3){
            if(flameCannonNum<=0){
                AudioPlay(errorPlayer);
                return;
            }
            flameCannonNum--;
        }
        else if(cannonType==4){
            if(rangeCannonNum<=0){
                AudioPlay(errorPlayer);
                return;
            }
            rangeCannonNum--;
        }
        //删除原有普通塔
        QPoint cannonPos=cannon->getPos();
        DeleteCannon(cannon,true);
        //升级特效
        Boom* boom=new Boom(cannon->getPos(),boomType,this);
        boomList.push_back(boom);
        //加入拓展塔
        pointFlag[index].isTower=true;
        TCannon * tCannon=new TCannon(cannonPos,pName,this);
        cannonList.push_back(tCannon);
        gameMoney-=tCannon->Coin();
        AudioPlay(upgradeCannonPlayer);
    }
    else if(cannon->Type()==cannonType){
        pointFlag[index].isTower=0;
        gameMoney+=cannon->Coin()*0.8;
        AudioPlay(removalPlayer);
        DeleteCannon(cannon,true);
    }
}

#endif // GAMESCENE_H

