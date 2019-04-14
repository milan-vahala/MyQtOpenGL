#include "figure.h"

Figure::Figure(QVector<Triangle> *aFloor)
    : Position(aFloor, 0.8f, 0, 4, 0, 100), stepSize(2.4f)
{
}

Figure::~Figure()
{
    if (model) {
        delete model;
    }
}

void Figure::initModel() {
    model = MD2Model::load(":/models/girl.md2");
    if (model) {
        model->setAnimation("run");
    }
}

void Figure::updateMotion() {
    float progress = 0.05f;
    if (model) {
        model->advance(progress);
    }

    if (!step(progress * stepSize)) {
        turnHorizontaly(60);
    }
}

void Figure::draw() {
    if (model) {
        glPushMatrix();
        glTranslatef(getX(), getY(), getZ() + getHeight());
        glRotatef(90 - getAngle(), 0.0, 0.0, 1.0);
        float scale = 0.035f;
        glScalef(scale, scale, scale);
        model->draw();
        glPopMatrix();
    }
}
