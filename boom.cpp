#include "boom.h"
#include "gamescene.h"
#include <QPoint>
#include <QTimerEvent>

Boom::Boom(QPoint point,int t,GameScene * g,double pBulletSize,QObject *parent)
    : QObject{parent},
      game(g),
      pos(point),
      bulletBoomSize(pBulletSize),
      index(0),
      type(t)
{
    InitPixmap();
}

void Boom::Draw(QPainter * painter)
{
    painter->save();
    QPoint offsetPoint(-pix[index].width()*0.5,-pix[index].height()*0.5);
    painter->translate(pos);
    painter->drawPixmap(offsetPoint,pix[index]);
    painter->restore();
}

void Boom::GeneratePixArray(QString path,int num,double pixSize,int rate)
{
    for(int i=0;i<num;i++)
    {
        QString fileName;
        if(num>10) fileName=QString(path).arg(i,2,10,QLatin1Char('0'));
        else fileName=QString(path).arg(i);
        QPixmap map(fileName);
        map=map.scaled(QSize(map.width()*pixSize,map.height()*pixSize),Qt::KeepAspectRatio);
        pix.push_back(map);
    }
    eventId=startTimer(rate);
}

void Boom::InitPixmap()
{
    switch(type)
    {
    case 1:{//塔击倒爆炸
        GeneratePixArray(":/image/pagodaBoom%1.png",16,1.2);
        break;
    }
    case 2:{//狼人死了爆炸
        GeneratePixArray(":/image/werewolfBoom%1.png",6,0.45);
        break;
    }
    case 3:{//子弹击中目标爆炸
        GeneratePixArray(":/image/BulletBoom%1.png",7,bulletBoomSize,120);
        break;
    }
    case 4:{//升级光效,黄光
        GeneratePixArray(":/image/yellowLight%1.png",20,0.8,80);
        break;
    }
    case 5:{//升级特效，蓝光
        GeneratePixArray(":/image/buleLight%1.png",20,0.8,80);
        break;
    }
    case 6:{//升级特效，金光
        GeneratePixArray(":/image/goldLight%1.png",20,0.8,80);
        break;
    }
    case 7:{//冰冻特效
        GeneratePixArray(":/image/iceFroze%1.png",26,0.28,100);
        break;
    }
    case 8:{//升级特效，红光
        GeneratePixArray(":/image/redLight%1.png",20,0.8,80);
        break;
    }
    case 9:{//敌人瞬移特效
        GeneratePixArray(":/image/wolfTransfer%1.png",8,0.48,110);
        break;
    }
    case 10:{//boss闪电
        GeneratePixArray(":/image/flashLight%1.png",4,0.40,150);
        break;
    }
    case 11:{//炮台闪电
        GeneratePixArray(":/image/thunder%1.png",9,0.38,120);
        break;
    }
    case 12:{
        GeneratePixArray(":/image/reddiamond%1.png",9,0.6,400);
        break;
    }
    case 13:{
        GeneratePixArray(":/image/bluediamond%1.png",9,0.6,400);
        break;
    }
    case 14:{
        GeneratePixArray(":/image/greendiamond%1.png",9,0.6,400);
        break;
    }
    case 15:{
        GeneratePixArray(":/image/yellowdiamond%1.png",9,0.6,400);
        break;
    }
    case 16:{
        GeneratePixArray(":/image/BGdiamond%1.png",9,0.6,400);
        break;
    }
    default:;
    }
}

void Boom::timerEvent(QTimerEvent * ev)
{
    if(ev->timerId()==eventId)
    {
        index++;
        if(index>=pix.size())
        {
            index=pix.size()-1;
            game->DeleteBoom(this);
        }
    }
}

