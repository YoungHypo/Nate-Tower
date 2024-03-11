#include "gamescene.h"
#include "Common.h"
#include <vld.h>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <algorithm>

void GameScene::mousePressEvent(QMouseEvent * e)
{
    QString str = QString( "mouse presseed point  x = %1   y = %2" ).arg(e->pos().x()).arg(e->pos().y());
    qDebug()<<str;
    //建塔台
    QPoint presspos=e->pos();
    int index=GetPointIndex(presspos);
    qDebug()<<"pressed index:"<<index;
    if(isGameLose||isGameWin) return;   //游戏结束不建
    if(index<0) return;                 //敌人路径不能再建
    if(gameMoney<=0) return;            //没钱不建

    switch(type)
    {
    case 1:mouseTowerEvent(index);break;
    case 2:mouseCannonEvent(index);break;
    case 3:mousePagodaEvent(index);break;
    case 4:mouseFierceCannonEvent(index);break;
    case 5:mouseFiercePagodaEvent(index);break;
    case 6:mouseCannonInheritEvent<FlameCannon>(index,8,3,":/image/MG3.png");break;
    case 7:mousePagodaInheritEvent<IcePagoda>(index,5,3,":/image/icePagoda.png");break;
    case 8:mouseCannonInheritEvent<RangeCannon>(index,5,4,":/image/Missile_Launcher3.png");break;
    case 9:mousePagodaInheritEvent<GoldPagoda>(index,6,4,":/image/goldPagoda.png");break;
    default:;
    }
}

void GameScene::mouseTowerEvent(int index)
{
    if(pointFlag[index].isRoute[0]||pointFlag[index].isRoute[1]){
        AudioPlay(errorPlayer);
        return;
    }
    //开始建塔
    if(!pointFlag[index].isBase)
    {
        pointFlag[index].isBase=1;
        QPoint towerpos=GetPointRoute(index);
        Tower* tower=new Tower(towerpos,":/image/Tower.png",this);
        towerList.push_back(tower);
        AudioPlay(setPlayer);
        //资源金钱回收
        gameMoney-=tower->Coin();
        return;
    }
    //拆塔
    if(pointFlag[index].isBase&&!pointFlag[index].isTower)
    {
        pointFlag[index].isBase=0;
        Tower* tower=NULL;
        foreach (Tower *tmp, towerList) {
           QPoint pointmp=tmp->getPos();
           int intmp=GetPointIndex(pointmp);
           if(intmp==index){
               tower=tmp;break;
           }
        }
        if(!tower){
            //should never reach here
            qDebug()<<"no tower in Towerlist";
            return;
        }
        //资源金钱回收
        gameMoney+=tower->Coin()*0.8;
        towerList.removeOne(tower);
        AudioPlay(removalPlayer);
        delete tower;
    }
}

void GameScene::mouseCannonEvent(int index)
{
    //敌人路径不能建远程塔
    if(pointFlag[index].isRoute[0]||pointFlag[index].isRoute[1]){
        AudioPlay(errorPlayer);
        return;
    }
    //没有塔台不能搭炮
    if(!pointFlag[index].isBase){
        AudioPlay(errorPlayer);
        return;
    }
    if(!pointFlag[index].isTower)
    {
        pointFlag[index].isTower=1;
        QPoint cannonpos=GetPointRoute(index);
        Cannon* cannon=new Cannon(cannonpos,":/image/cannon.png",this);
        cannonList.push_back(cannon);
        AudioPlay(setPlayer);
        //资源金钱回收
        gameMoney-=cannon->Coin();
        return;
    }
    if(pointFlag[index].isTower)
    {
        Cannon* cannon=NULL;
        foreach (Cannon* cantmp, cannonList) {
            QPoint pointmp=cantmp->getPos();
            int intmp=GetPointIndex(pointmp);
            if(intmp==index){
                cannon=cantmp;break;
            }
        }
        if(!cannon){
            //should never reach here
            qDebug()<<"no cannon in cannonlist";
            return;
        }
        if(cannon->Type()!=1) {
            AudioPlay(errorPlayer);
            return;
        }
        pointFlag[index].isTower=0;
        gameMoney+=cannon->Coin()*0.8;
        AudioPlay(removalPlayer);
        DeleteCannon(cannon,true);
    }
}

void GameScene::mousePagodaEvent(int index)
{
    //起点终点不种pagoda
    if(!routePoint[0].isEmpty()){
        if(index==routePoint[0].first()||index==routePoint[0].last()){
            AudioPlay(errorPlayer);
            return;
        }
    }
    if(!routePoint[1].isEmpty()){
        if(index==routePoint[1].first()||index==routePoint[1].last()){
            AudioPlay(errorPlayer);
            return;
        }
    }
    //非敌人路径不种
    if(!pointFlag[index].isRoute[0]&&!pointFlag[index].isRoute[1]){
        AudioPlay(errorPlayer);
        return;
    }
    //建塔
    if(!pointFlag[index].isPagoda)
    {
        pointFlag[index].isPagoda=1;
        QPoint pagodapos=GetPointRoute(index);
        Pagoda* pagoda=new Pagoda(pagodapos,":/image/pagoda.png",this);
        pagodaList.push_back(pagoda);
        AudioPlay(setPlayer);
        gameMoney-=pagoda->Coin();
        return;
    }
    //拆塔
    if(pointFlag[index].isPagoda)
    {
        Pagoda* pagoda=NULL;
        foreach (Pagoda* pagotmp, pagodaList) {
            QPoint pointmp=pagotmp->getPos();
            int intmp=GetPointIndex(pointmp);
            if(intmp==index){
                pagoda=pagotmp;break;
            }
        }
        if(!pagoda){
            //should never reach here
            qDebug()<<"no pagoda in pagodalist";
            return;
        }
        if(pagoda->Type()!=1){
            AudioPlay(errorPlayer);
            return;
        }
        pointFlag[index].isPagoda=0;
        gameMoney+=pagoda->Coin()*0.8;
        AudioPlay(removalPlayer);
        DeletePagoda(pagoda,true);
    }
}

void GameScene::mouseFiercePagodaEvent(int index)
{
    //该点无pagoda不种
    if(!pointFlag[index].isPagoda){
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
        if(fiercePagodaNum<=0){
            AudioPlay(errorPlayer);
            return;
        }
        pagoda->UpdateFierce();
        fiercePagodaNum--;
        //升级特效
        Boom* boom=new Boom(pagoda->getPos(),4,this);
        boomList.push_back(boom);
        gameMoney-=pagoda->Coin();
        AudioPlay(upgradePagodaPlayer);
    }
    else if(pagoda->Type()==2){//拆塔
        pointFlag[index].isPagoda=0;
        gameMoney+=pagoda->Coin()*0.8;
        AudioPlay(removalPlayer);
        DeletePagoda(pagoda,true);
    }
}

void GameScene::mouseFierceCannonEvent(int index)
{
    //该点无Tower不种
    if(!pointFlag[index].isTower){
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
        if(fierceCannonNum<=0){
            AudioPlay(errorPlayer);
            return;
        }
        cannon->UpdateFierce();
        fierceCannonNum--;
        //升级特效
        Boom* boom=new Boom(cannon->getPos(),4,this);
        boomList.push_back(boom);
        gameMoney-=cannon->Coin();
        AudioPlay(upgradeCannonPlayer);
    }
    else if(cannon->Type()==2){
        pointFlag[index].isTower=0;
        gameMoney+=cannon->Coin()*0.8;
        AudioPlay(removalPlayer);
        DeleteCannon(cannon,true);
    }

}

