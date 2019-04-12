#ifndef POINT3D_H
#define POINT3D_H

#include <QPoint>

class Point3D : public QPoint
{
public:
    Point3D() : QPoint(0,0), iZ(0) {}
    Point3D(int x, int y, int z) : QPoint(x,y), iZ(z) {}
    Point3D(const Point3D& point) : QPoint(point)
    {
        iZ = point.iZ;
    }

    int z()
    {return iZ;}
private:
    int iZ;
};

#endif // POINT3D_H
