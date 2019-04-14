#include "figure.h"

Figure::Figure(QVector<Triangle> *aFloor)
    : position(aFloor, 0.8f, 0, 6, 50, 180), stepSize(2.4f)
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
    if (position.gravityOn()) {
         position.applyGravity();
    } else {
        float progress = 0.05f;
        if (model) {
            model->advance(progress);
        }

        if (!position.step(progress * stepSize)) {
            position.turnHorizontaly(60);
        }
    }
}

void Figure::draw() {
    if (model) {
        glPushMatrix();
        glTranslatef(position.getX(), position.getY(), position.getZ() + position.getHeight());
        glRotatef(90 - position.getAngle(), 0.0, 0.0, 1.0);
        float scale = 0.035f;
        glScalef(scale, scale, scale);
        model->draw();
        glPopMatrix();
    }
}
