#ifndef STATE_H
#define STATE_H

#include <QVector2D>

#include "triangle.h"

class Position {

public:
    Position(QVector<Triangle> *aFloor, float aHeight,
             float ax = 0, float ay = 0, float az = 0, float aAngle = 0);

    float getX() const { return position.x(); }
    float getY() const { return position.y(); }
    float getZ() const { return z; }
    float getAngle() const { return angle; }
    float getHeight() const { return myHeight; }

    bool step(float stepSize);
    void turnHorizontaly(float turnAngle);

    void startGravity(const QVector3D& velocity);
    void applyGravity();
    bool gravityOn() const { return applyingGravity; }

    static void normalizeAngle(float &angle);

private:
    QVector<Triangle> *floor;

    QVector2D position;
    float z;
    float angle;

    float myHeight;   //z-coordinato of head, must be greater z-coordinate of eyes
    float maxStepZ;

    int dt;     //time change (in miliseconds)
    float t;    //time
    QVector3D v0;   //starting velocity

    bool applyingGravity;

    bool makeStep(const QVector2D& newPosition);
    bool canMove(const QVector3D& direction);
};

#endif // STATE_H
