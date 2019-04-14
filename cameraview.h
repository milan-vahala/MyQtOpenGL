#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QVector2D>

#include "position.h"

class CameraView
{
public:
    CameraView(QVector<Triangle>* aFloor);

    float getVerticalAngle() const;
    void turnVerticaly(float turnAngle);

    bool step(float stepSize);
    void turnHorizontaly(float turnAngle);

    float getX() const;
    float getY() const;
    float getZ() const;
    float getAngle() const;

    bool gravityOn() const;
    void startGravity(const QVector3D& velocity);
    void applyGravity();

private:
    Position position;
    float verticalAngle;

    float viewZ;    //z-coordinate of eyes

};

#endif // CAMERAVIEW_H
