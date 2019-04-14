#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QVector2D>

#include "triangle.h"

class CameraView
{
public:
    CameraView(QVector<Triangle>* aFloor, QTimer *timer);

    float getAngle();
    float getVerticalAngle();
    float getX();
    float getY();
    float getZ();

    void turnHorizontaly(float turnAngle);
    void turnVerticaly(float turnAngle);

    void startGravity(const QVector3D& velocity);
    void applyGravity();

    void stepForward();
    void stepBack();
    void rotateLeft();
    void rotateRight();

private:
    QVector2D position;   //x and y coordinates of current position
    float z;
    float viewZ;    //z-coordinate of eyes
    float myHeight;   //z-coordinato of head, must be greater than viewZ
    float maxStepZ;
    float stepSize;
    float angle;
    float verticalAngle;
    float rotateStepSize;
    float VangleStep;
    float HangleStep;

    float g;    //gravity constant
    int dt;     //time change (in miliseconds)
    float t;    //time
    QVector3D v0;   //starting velocity

    QVector<Triangle> *floor;

    QTimer *gravityTimer;

    void makeStep(const QVector2D& newPosition);
    bool canMove(const QVector3D& direction);
};

#endif // CAMERAVIEW_H
