#ifndef FIGURE_H
#define FIGURE_H

#include <QVector2D>

#include "md2model.h"
#include "triangle.h"

class Figure
{
public:
    Figure(QVector<Triangle>* aFloor);
    ~Figure();

    void initModel();

    void updateMotion();
    void draw();

private:

    //TODO: is using struct a good idea?
    struct figureState {
        QVector2D position;
        float z;
        float myHeight;
        float angle;
        float stepSize;
    };

    QVector<Triangle>* floor;

    float maxStepZ;

    MD2Model* model;
    figureState modelState;

    bool makeStep(const QVector2D& newPosition, figureState& newState);
};

#endif // FIGURE_H
