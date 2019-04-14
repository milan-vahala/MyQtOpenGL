#ifndef FIGURE_H
#define FIGURE_H

#include "position.h"
#include "md2model.h"

class Figure
{
public:
    Figure(QVector<Triangle>* aFloor);
    ~Figure();

    void initModel();

    void updateMotion();
    void draw();

private:
    Position position;

    float stepSize;

    MD2Model* model;

};

#endif // FIGURE_H
