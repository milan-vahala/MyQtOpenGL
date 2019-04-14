#include "cameraview.h"

#include <QTimer>

CameraView::CameraView(QVector<Triangle>* aFloor, QTimer *timer) :
    z(0), viewZ(1.5), myHeight(1.8f), angle(0),
    verticalAngle(0), dt(30),
    floor(aFloor), gravityTimer(timer)
{
    position = QVector2D(0, 0);
    maxStepZ = 0.8f * 0.8f;
}

namespace {

QVector2D getView(float stepSize, float angle) {
    double stepSizeD = static_cast<double>(stepSize);
    double angleD = static_cast<double>(angle);

    return QVector2D(stepSizeD*QTransform().rotate(-angleD).map(QPointF(0,1)));
}

//TODO: test if this method is working correctly
void normalizeAngle(float &angle) {
    if (angle > 360) {
        angle -= (static_cast<int>(angle) / 360) * 360;
    }
    else if (angle < 0) {
        angle += (-static_cast<int>(angle) / 360) * 360;
    }
}

}

float CameraView::getAngle() {
    return angle;
}

float CameraView::getVerticalAngle() {
    return verticalAngle;
}

float CameraView::getX() {
    return position.x();
}

float CameraView::getY() {
    return position.y();
}

float CameraView::getZ() {
    return z + viewZ;
}

void CameraView::turnHorizontaly(float turnAngle) {
    angle += turnAngle;
    normalizeAngle(angle);
}

void CameraView::turnVerticaly(float turnAngle) {
    verticalAngle += turnAngle;
    normalizeAngle(verticalAngle);
}

void CameraView::step(float stepSize) {
    QVector2D view = getView(stepSize, angle);

    makeStep(position + view);
}

void CameraView::makeStep(const QVector2D &newPosition)
{
    QVector<float> possibleZ;
    float newZ;
    for (QVector<Triangle>::iterator it=floor->begin(); it!=floor->end(); it++){
        if (it->contains(newPosition, newZ)){
            if ((newZ-z)<maxStepZ){
                possibleZ.append(newZ);
            }
        }
    }
    const float minZ=-1000;
    float Z=minZ;    //TODO: make this better
    //searching highest Z which is under my possition
    for (int i=0; i<possibleZ.size(); ++i){
        float tmpZ(possibleZ.at(i));
        if (tmpZ>Z){
            Z=tmpZ;
        }
    }
    if (Z>minZ){ //or possibleZ.size()==0
        position=newPosition;
        //update();
        if ((z-Z)>maxStepZ){
            startGravity(QVector3D(0,0,0)); //start falling
        } else {
            z=Z;
        }
    }
}

bool CameraView::canMove(const QVector3D &direction) {
    QVector2D newPosition=position + direction.toVector2D();
    //TODO: copy of code from makeStep(...) method, merge
    QVector<float> possibleZ;
    float newZ;
    for (QVector<Triangle>::iterator it=floor->begin(); it!=floor->end(); it++){
        if (it->contains(newPosition, newZ)){
            if ((newZ-z)<maxStepZ){
                possibleZ.append(newZ);
            }
            //testing if I crash my head into ceiling when jumping up
            if (gravityTimer->isActive()){ // gravity on, I'm jumping or falling
                if ( (newZ-(z+myHeight))*(newZ-(z+myHeight+direction.z()))<0 ){
                    v0=QVector3D(0,0,0);
                }
            }
        }
    }
    // end of copy from makeStep(...) method
    int size=possibleZ.size();
    if (size==0){
        return false;
    }
    for (int i=0; i<size; ++i){
        float tmpZ(possibleZ.at(i));
        //test if crossing some plane when moving in vertical direction
        if ( (tmpZ-z)*(tmpZ-(z+direction.z()))<0 ){
            z=tmpZ;
            return false;
        }
    }
    return true;
}

void CameraView::startGravity(const QVector3D& velocity) {
    v0 = velocity;
    t = 0;
    gravityTimer->start(dt);
}

void CameraView::applyGravity() {
    static const float g = 9.81f;   // Gravitational acceleration

    t += dt * 0.001f;
    QVector3D moveDirection = (v0 + g * t * QVector3D(0, 0, -1)) * t;
    if (canMove(moveDirection)) {
        position += moveDirection.toVector2D();
        z = z + moveDirection.z();
    } else {
        gravityTimer->stop();
    }
}

