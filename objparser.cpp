#include <QFile>
#include <QMessageBox>
#include <QPointF>

#include "objparser.h"

ObjParser::ObjParser()
{
}

void ObjParser::parse(QString fileName)
{
    QFile file(fileName);
    if ( file.open(QIODevice::ReadOnly) ) {    // file opened successfully
        QTextStream t( &file );
        fetchVertices(t);
        fetchTextureCoordinates(t);
        fetchFaces(t);
        //extractRects();
    } else {
        QMessageBox::warning(nullptr, "Error", "Can't open file");
    }
}

QVector<Triangle> ObjParser::getTriangles(){
    return triangles;
}

namespace  {

float toFloat(const QString& str) {
    return QString(str).replace(',', '.').toFloat();
}

}

void ObjParser::fetchVertices(QTextStream &t){
    QString floatRx("(-?\\d+([\\.,]\\d+)?([eE]-?\\d+)?)");
    QRegExp vertexRx("^v\\s+"+floatRx+"\\s+"+floatRx+"\\s+"+floatRx);
    QString line;
    int pos=-1;

    do {    //skip all non vertex lines
        line=t.readLine();
        pos=vertexRx.indexIn(line);
    } while (pos==-1);

    do {    //matching pattern found
        float x = toFloat(vertexRx.cap(1));
        float y = toFloat(vertexRx.cap(4));
        float z =  toFloat(vertexRx.cap(7));
        vertices << QVector3D(x,z,y);   //change because my different view

        line=t.readLine();
        pos=vertexRx.indexIn(line);
    } while (pos>-1);
}

void ObjParser::fetchTextureCoordinates(QTextStream &t){
    QString floatRx("(-?\\d+([\\.,]\\d+)?([eE]-?\\d+)?)");
    QRegExp textureCoordinatesRx("^vt\\s+"+floatRx+"\\s+"+floatRx);
    QString line;
    int pos=-1;

    do {    //skip all non texture coordinates (vt) lines
        line=t.readLine();
        pos=textureCoordinatesRx.indexIn(line);
    } while (pos==-1);

    do {    //matching pattern found
        float x = toFloat(textureCoordinatesRx.cap(1));
        float y = toFloat(textureCoordinatesRx.cap(4));
        textureCoordinates << QPointF(x,y);

        line=t.readLine();
        pos=textureCoordinatesRx.indexIn(line);
    } while (pos>-1);
}

void ObjParser::fetchFaces(QTextStream &t){
    QRegExp facesRx("^f\\s+(\\d+)/\\d+\\s+(\\d+)/\\d+\\s+(\\d+)/\\d+");
    QString line;
    int pos=-1;

    do {    //skip all non faces lines
        line=t.readLine();
        pos=facesRx.indexIn(line);
    } while (pos==-1);

    do {    //matching pattern found
        int x = facesRx.cap(1).toInt();
        int y = facesRx.cap(2).toInt();
        int z = facesRx.cap(3).toInt();
        faces << Point3D(x,y,z) ;
        triangles << Triangle(vertices[x-1],vertices[y-1],vertices[z-1]);

        line=t.readLine();
        pos=facesRx.indexIn(line);
    } while (pos>-1);
}

/*      deprecated
void ObjParser::extractRects(){
    for (int i=1; i<faces.size(); ++i){
        int a=faces[i-1].x();
        int b=faces[i-1].y();
        int d=faces[i-1].z();
        int dd=faces[i].x();
        int c=faces[i].y();
        int aa=faces[i].z();
        //TODO: make check if  d+(a-b) aprox== c
        if (a==aa && d==dd ){
            rects << Rect(vertices[a-1],vertices[b-1],vertices[c-1]);
        }
    }
}
*/
