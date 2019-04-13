#ifndef STATE_H
#define STATE_H

#include <QVector2D>

class Position {

public:
    Position(float x, float y, float z, float angle);
    QVector2D getPosition2D() const { return position2D; }
    float getZ() const { return z; }
    float getAngle() const { return angle; }

private:
    QVector2D position2D;
    float z;
    float angle;
};

#endif // STATE_H
