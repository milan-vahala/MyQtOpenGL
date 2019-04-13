#include "triangle.h"

#include <QVector2D>

Triangle::Triangle()
{
}

Triangle::Triangle(QVector3D aA, QVector3D aB, QVector3D aC)
    : a(aA), b(aB), c(aC), texIndex(0)
{
}

Triangle::Triangle(QVector3D aA, QVector3D aB, QVector3D aC, int aIndex)
    : a(aA), b(aB), c(aC), texIndex(aIndex-1)
{
}

int Triangle::getTextureIndex(){
    return texIndex;
}

bool Triangle::contains(const QVector2D pos, float &atZ)
{
    atZ = 0;
    QVector2D v1 = (b-a).toVector2D();
    QVector2D v2 = (c-a).toVector2D();
    float e=v1.x();
    float f=v2.x();
    float g=v1.y();
    float h=v2.y();
    float x = pos.x() - a.x();
    float y = pos.y() - a.y();

    float denom=e*h-f*g;
    float r1=(h*x-f*y)/denom;

    if (r1>=0 && r1<=1){
        float r2=(-g*x+e*y)/denom;  // can be compuded using r1
        //if(r2>=0 && r2<=1 && r1+r2<1){
        if(r2>=0 && r1+r2<1){
            atZ=a.z() + (b.z()-a.z())*r1 + (c.z()-a.z())*r2;
            return true;
        }
    }
    return false;
}

void Triangle::render(const GLuint& texture)
{   //TODO: change values of glTexCoord3f to those loaded from .obj file
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_TRIANGLES);
    //using big planes, so use texture 3 times per triangle
    glTexCoord2f(0.0f, 0.0f); glVertex3f(a.x(), a.y(), a.z());
    glTexCoord2f(3.0f, 0.0f); glVertex3f(b.x(), b.y(), b.z());
    glTexCoord2f(0.0f, 3.0f); glVertex3f(c.x(), c.y(), c.z());
    glEnd();
}

void Triangle::render()
{
    glBegin(GL_TRIANGLES);
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    glEnd();
}
