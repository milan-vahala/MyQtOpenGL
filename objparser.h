#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <QString>
#include <QTextStream>
#include <QVector>
#include <QVector3D>

//#include "rect.h"
#include "triangle.h"
#include "point3D.h"

class ObjParser
{
public:
    ObjParser();
    void parse(QString fileName);
    //QVector<Rect> getRects();     //deprecated
    QVector<Triangle>  getTriangles();

private:
    //QVector<Rect> rects;
    QVector<Triangle> triangles;
    QVector<QVector3D> vertices;
    QVector<QPointF> textureCoordinates;
    QVector<Point3D> faces;

    void fetchVertices(QTextStream& t);
    void fetchTextureCoordinates(QTextStream& t);
    void fetchFaces(QTextStream& t);
    //void extractRects();  //deprecated


};


#endif // OBJPARSER_H
