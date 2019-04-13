#include "figure.h"

Figure::Figure() : floor(QVector<Triangle>())
{
    modelState.position = QVector2D(0,4);
    modelState.z = 0;
    modelState.myHeight = 0.8f;
    modelState.stepSize = 2.4f;
    modelState.angle = 180;

    maxStepZ = 8.8f * 0.8f; // The same as maxStepZ in MyWidget
}

Figure::~Figure()
{
    delete model;
}

void Figure::setFloor(QVector<Triangle> aFloor) {
   floor = aFloor;
}

void Figure::init() {
    model = MD2Model::load(":/models/girl.md2");
    if (model) {
        model->setAnimation("run");
    }
}

namespace {

QVector2D getView(float stepSize, float angle) {
    double stepSizeD = static_cast<double>(stepSize);
    double angleD = static_cast<double>(angle);

    return QVector2D(stepSizeD*QTransform().rotate(-angleD).map(QPointF(0,1)));
}

}

void Figure::updateMotion() {
    float progress = 0.05f;
    if (model) {
        model->advance(progress);
    }
    QVector2D view = getView(modelState.stepSize, modelState.angle);
    if (!makeStep(modelState.position + progress*view, modelState)) {
        modelState.angle += 60;
    }
}

void Figure::draw() {
    if (model) {
        glPushMatrix();
        glTranslatef(modelState.position.x(), modelState.position.y(), modelState.z+modelState.myHeight);
        //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        //glRotatef(modelState.angle-90, 1.0f, 0.0f, 0.0f);
        glRotatef(90-modelState.angle, 0.0, 0.0, 1.0);
        float scale=0.035f;
        glScalef(scale, scale, scale);
        model->draw();
        glPopMatrix();
    }
}

//TODO: almost the same as makeStep(const QPointF &newPosition), merge somehow
bool Figure::makeStep(const QVector2D &newPosition, figureState& newState)
{
    //different code here
    float z=newState.z;

    //end of different code
    QVector<float> possibleZ;
    float newZ;
    for (QVector<Triangle>::iterator it=floor.begin(); it!=floor.end(); it++){
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
        //position=newPosition;
        newState.position=newPosition;
        //TODO: Do update realy here?
        //update();
        //if ((z-Z)>maxStepZ){
            //falling for anything other then me not implemented
            //startGravity(QVector3D(0,0,0)); //start falling
        //} else {
            //z=Z;
            newState.z=Z;
        //}
        //update();
        return true;
    }
    return false;
}

