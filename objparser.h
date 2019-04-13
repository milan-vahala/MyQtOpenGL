#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <QString>
#include <QTextStream>
#include <QVector>
#include <QVector3D>
#include <QVector2D>

#include "triangle.h"
#include "point3D.h"

class ObjParser
{
public:
    ObjParser(const QString& fileName);
    QVector<Triangle>  getTriangles();

private:
    QVector<Triangle> triangles;
    QVector<QVector3D> vertices;
    QVector<QVector2D> textureCoordinates;
    QVector<Point3D> faces;

    void fetchVertices(QTextStream& t);
    void fetchTextureCoordinates(QTextStream& t);
    void fetchFaces(QTextStream& t);

};


#endif // OBJPARSER_H
