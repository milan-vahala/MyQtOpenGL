#include "cameraview.h"

CameraView::CameraView(QVector<Triangle>* aFloor) :
    position(aFloor, 1.8f), verticalAngle(0), viewZ(1.5)
{    
}

bool CameraView::step(float stepSize) {
    return position.step(stepSize);
}

void CameraView::turnHorizontaly(float turnAngle) {
    position.turnHorizontaly(turnAngle);
}



float CameraView::getX() const {
    return position.getX();
}

float CameraView::getY() const {
    return position.getY();
}

float CameraView::getZ() const {
    return position.getZ() + viewZ;
}

float CameraView::getAngle() const {
    return position.getAngle();
}


bool CameraView::gravityOn() const {
    return position.gravityOn();
}

void CameraView::startGravity(const QVector3D &velocity) {
    position.startGravity(velocity);
}

void CameraView::applyGravity() {
    position.applyGravity();
}



float CameraView::getVerticalAngle() const {
    return verticalAngle;
}

void CameraView::turnVerticaly(float turnAngle) {
    verticalAngle += turnAngle;

    Position::normalizeAngle(verticalAngle);
}
