#include "gamescene.h"
#include "Common.h"
#include <vld.h>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <algorithm>

void GameScene::DrawTimes(QPainter& painter,const QRect rect,int num,bool flag)
{
    if(flag) painter.setPen(QPen(Qt::yellow));
    else painter.setPen(QPen(Qt::black));
    QFont font("Times New Roman",14,QFont::Bold,true);
    painter.setFont(font);
    painter.drawText(rect,QString("×%1").arg(num));
}

void GameScene::DrawBlood(QPainter& painter,const QRect rect,int num)
{
    painter.setPen(QPen(Qt::red));
    QFont font("Times New Roman",14,QFont::Bold,true);
    painter.setFont(font);
    painter.drawText(rect,QString("%1").arg(num)+"%");
}

void GameScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pixmain);
    painter.drawPixmap(0,0,this->width(),this->height(),pixbg);
    //pagoda
    DrawTimes(painter,QRect(133,150,50,30),fiercePagodaNum);
    DrawTimes(painter,QRect(48,240,50,30),icePagodaNum);
    DrawTimes(painter,QRect(133,240,50,30),goldPagodaNum);

    //cannon
    DrawTimes(painter,QRect(140,355,50,30),fierceCannonNum);
    DrawTimes(painter,QRect(52,445,50,30),flameCannonNum);
    DrawTimes(painter,QRect(148,445,50,30),rangeCannonNum);

    //bloodVolume
    DrawBlood(painter,QRect(83,550,50,30),(curBlood*100)/maxBlood);
    //money
    DrawTimes(painter,QRect(83,610,150,30),gameMoney,true);
    //enemy
    DrawTimes(painter,QRect(83,686,50,30),enemyNum);


    //画地图
    for (int i = 0; i < 120; i++)
    {
        if (pointFlag[i].isRoute[0]||pointFlag[i].isRoute[1])
        {
            for(int t=0;t<2;t++){
                if (pointFlag[i].isRoute[t])
                    painter.drawPixmap(GetPointPix(i),pixpath[t]);
            }
        }
        else painter.drawPixmap(GetPointPix(i),pixground);
    }

    //画起点和终点
    for(int t=0;t<2;t++)
    {
        if(!routePointList[t].isEmpty())
        {
            QPoint epoint=routePointList[t].last()->Pos()-QPoint(FixedLen+8,FixedLen-2);
            painter.drawPixmap(epoint,pixdoor[doorIndex[t]]);
            QPoint spoint=routePointList[t].first()->Pos()-QPoint(FixedLen,FixedLen);
            painter.drawPixmap(spoint,pixFlag);
        }
    }

    painter.drawPixmap(40,540,pixBlood);
    painter.drawPixmap(36,597,pixMoney);
    painter.drawPixmap(36,658,pixenemy);

    foreach (Tower *tower, towerList)
        tower->Draw(&painter);

    foreach (Cannon* cannon, cannonList)
        cannon->Draw(&painter);

    foreach (Pagoda* pagoda, pagodaList)
        pagoda->Draw(&painter);

    CheckOverlap();

    foreach(Enemy *enemy,enemyList)
        enemy->Draw(&painter);

    foreach(Bullet *bullet,bulletList)
        bullet->Draw(&painter);

    foreach(Boom* boom,boomList)
        boom->Draw(&painter);

    update();
}
