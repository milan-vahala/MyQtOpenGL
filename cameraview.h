#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QVector2D>

#include "position.h"

class CameraView : public Position
{
public:
    CameraView(QVector<Triangle>* aFloor, QTimer *timer);

    float getVerticalAngle() const;
    void turnVerticaly(float turnAngle);

    float getZ() const;


private:
    float verticalAngle;

    float viewZ;    //z-coordinate of eyes

};

#endif // CAMERAVIEW_H
