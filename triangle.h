#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector3D>
#include <QPointF>
#include <qgl.h>

class Triangle
{
public:
    Triangle();    

    Triangle(QVector3D aA, QVector3D aB, QVector3D aC);
    Triangle(QVector3D aA, QVector3D aB, QVector3D aC, int aIndex);
    void render();
    void render(const GLuint& aTexture);
    bool contains(const QPointF pos, float& z);
    int getTextureIndex();

private:
    QVector3D a;
    QVector3D b;
    QVector3D c;
    int texIndex;
};

#endif // TRIANGLE_H
