#ifndef ROUTEPOINT_H
#define ROUTEPOINT_H

#include <QPoint>

class RoutePoint
{
public:
    RoutePoint(QPoint Pos);
    void AddNextPoint(RoutePoint * Nt){next=Nt;}
    RoutePoint * NextPoint(){return next;}
    QPoint Pos(){return pos;}

private:
    QPoint pos;
    RoutePoint * next;
};

#endif // ROUTEPOINT_H
