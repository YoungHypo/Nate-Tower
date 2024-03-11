#ifndef COMMON_H
#define COMMON_H

#include <QPoint>
#include <QSize>
#include <QMediaPlayer>
#include <QAudioOutput>

#define FixedLen 45
#define Second 1000
#define incre 5
#define UnitRange 127
#define UintDurat 500

class Enemy;
class Pagoda;

struct PointFlags
{
    bool isRoute[2]{0,0};
    bool isBase = 0;
    bool isTower = 0;
    bool isPagoda = 0;
};

const QSize FixedSize(90,90);

/*两圆距离*/
bool CircleDistance(QPoint point1, int radius1, QPoint point2, int radius2);

/*两点距离*/
bool LineDistance(QPoint point1, QPoint point2, int dis);

/*近战塔与敌人的距离*/
bool CheckDirectionBetweenPagodaEnemy(Pagoda* pagoda,Enemy* enemy);

/*通过下标获取坐标*/
QPoint GetPointRoute(int index);

/*通过下标获取图片中心坐标*/
QPoint GetPointPix(int index,int flag=1);

/*获取旋转后的坐标点*/
QPoint GetPointRotate(QPoint rotatePos,qreal angle);

/*两坐标点间距*/
double PointDistance(QPoint point1, QPoint point2);

/*检查txt文件名的合法性*/
bool CheckFileNameLegitimacy(std::string fileStr);

/*检查targetpos是否在pos1和pos2之间*/
bool CheckPosBetweenTwoPoints(QPoint pos1,QPoint pos2,QPoint targetPos);

/*通过坐标获取下标*/
int GetPointIndex(QPoint& po);

#endif // COMMON_H
