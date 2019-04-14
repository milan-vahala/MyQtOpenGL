#include "cameraview.h"

#include <QTimer>

CameraView::CameraView(QVector<Triangle>* aFloor, QTimer *timer) :
    Position(aFloor, timer), verticalAngle(0), viewZ(1.5)
{    
}

float CameraView::getZ() const {
    return Position::getZ() + viewZ;
}

float CameraView::getVerticalAngle() const {
    return verticalAngle;
}

void CameraView::turnVerticaly(float turnAngle) {
    verticalAngle += turnAngle;
    normalizeAngle(verticalAngle);
}





