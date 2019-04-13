#ifndef POINT3D_H
#define POINT3D_H


class Point3D
{
public:
    Point3D() : iX(0), iY(0), iZ(0) {}
    Point3D(int x, int y, int z) : iX(x), iY(y), iZ(z) {}


    int x() const {return iX;}
    int y() const {return iY;}
    int z() const {return iZ;}
private:
    int iX;
    int iY;
    int iZ;
};

#endif // POINT3D_H
