#include "Common.h"
#include "enemy.h"
#include "pagoda.h"
#include <QtMath>
#include <QDebug>


bool CircleDistance(QPoint point1, int radius1, QPoint point2, int radius2)
{
    const int distance = int(PointDistance(point1,point2));
    if (distance < radius1 + radius2)
        return true;
    else return false;
}

bool LineDistance(QPoint point1, QPoint point2, int dis)
{
    const int xdif=abs(point1.x() - point2.x());
    const int ydif=abs(point1.y() - point2.y());
    if(xdif<dis && ydif==0)
        return true;
    if(ydif<dis && xdif==0)
        return true;
    return false;
}

bool CheckDirectionBetweenPagodaEnemy(Pagoda* pagoda,Enemy* enemy)
{
    int dirc=enemy->Direction();
    QPoint pagodaPos=pagoda->getPos();
    QPoint enemyPos=enemy->getPos();
    //qDebug()<<"pagoda,enemy pos"<<pagodaPos<<" "<<enemyPos;
    switch(dirc)
    {
    case 0:{
        if(pagodaPos.x()==enemyPos.x()&&pagodaPos.y()<=enemyPos.y()) return true;
        else return false;
        break;
    }
    case 1:{
        if(pagodaPos.x()==enemyPos.x()&&pagodaPos.y()>=enemyPos.y()) return true;
        else return false;
        break;
    }
    case 2:{
        if(pagodaPos.y()==enemyPos.y()&&pagodaPos.x()<=enemyPos.x()) return true;
        else return false;
        break;
    }
    case 3:{
        if(pagodaPos.y()==enemyPos.y()&&pagodaPos.x()>=enemyPos.x()) return true;
        else return false;
        break;
    }
    default:return false;
    }


}

double PointDistance(QPoint point1, QPoint point2)
{
    const int xdif=abs(point1.x() - point2.x());
    const int ydif=abs(point1.y() - point2.y());
    return qSqrt(xdif * xdif + ydif * ydif);
}

QPoint GetPointRoute(int index)
{
    return QPoint((index%12)*90+180+FixedLen,(index/12)*90+FixedLen);
}

QPoint GetPointPix(int index,int flag)
{
    int tmp=0;
    if(flag) tmp=2;
    return QPoint((index%12)*90+180,(index/12)*90+tmp);
}

QPoint GetPointRotate(QPoint rotatePos,qreal angle)
{
    int x1=(int)(qCos(qDegreesToRadians(angle))*rotatePos.x()-qSin(qDegreesToRadians(angle))*rotatePos.y());
    int y1=(int)(qCos(qDegreesToRadians(angle))*rotatePos.y()+qSin(qDegreesToRadians(angle))*rotatePos.x());
    return QPoint(x1,y1);
}

int GetPointIndex(QPoint& po)
{
    if(po.x()-180<0) return -1;
    int row=(po.x()-180)/90;
    int col=po.y()/90;
    return col*12+row;
}

bool CheckFileNameLegitimacy(std::string fileStr){

    for(int i=0;i<(int)fileStr.size();i++){
        if(!std::isdigit(fileStr[i])&&!std::isalpha(fileStr[i])&&fileStr[i]!='_'){//字符既不是数字也不是字母也不是'_'
            return false;
        }
    }
    return true;
}

bool CheckPosBetweenTwoPoints(QPoint pos1,QPoint pos2,QPoint targetPos)
{
    if(pos1.x()==pos2.x()&&pos2.x()==targetPos.x()){
        if(pos1.y()<pos2.y()){
            if(pos1.y()<targetPos.y()&&targetPos.y()<=pos2.y()) return true;
            else return false;
        }
        else if(pos1.y()>pos2.y()){
            if(pos1.y()>targetPos.y()&&targetPos.y()>=pos2.y()) return true;
            else return false;
        }
    }
    else if(pos1.y()==pos2.y()&&pos2.y()==targetPos.y()){
        if(pos1.x()<pos2.x()){
            if(pos1.x()<targetPos.x()&&targetPos.x()<=pos2.x()) return true;
            else return false;
        }
        else if(pos1.x()>pos2.x()){
            if(pos1.x()>targetPos.x()&&targetPos.x()>=pos2.x()) return true;
            else return false;
        }
    }
    return false;
}

