#ifndef STATE_H
#define STATE_H

#include <QVector2D>

#include "triangle.h"

class Position {

public:
    Position(QVector<Triangle> *aFloor,
             float ax = 0, float ay = 0, float az = 0, float aAngle = 0)
        : floor(aFloor), position(ax, ay), z(az), angle(aAngle),
          myHeight(1.8f), maxStepZ(0.8f * 0.8f), dt(30), applyingGravity(false)
    { }
    float getX() const { return position.x(); }
    float getY() const { return position.y(); }
    float getZ() const { return z; }
    float getAngle() const { return angle; }

    void step(float stepSize);
    void turnHorizontaly(float turnAngle);

    void startGravity(const QVector3D& velocity);
    void applyGravity();
    bool gravityOn() const { return applyingGravity; }

protected:
    void normalizeAngle(float &angle);

    void makeStep(const QVector2D& newPosition);
    bool canMove(const QVector3D& direction);

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
};

#endif // STATE_H
