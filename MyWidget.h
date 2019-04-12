#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <qgl.h>

#include <QtGui>
#include <QVector>
#include <QPointF>
#include <QPoint>
#include <QTimer>

#include "triangle.h"
#include "md2model.h"

class MyWidget : public QGLWidget
{
    Q_OBJECT    
public:
    MyWidget(QWidget *parent=nullptr);
    ~MyWidget();

protected:
    void initializeGL();
    void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    void resizeGL( int width, int height );
    void paintGL();
    void loadGLtextures();
    void keyPressEvent( QKeyEvent *event );
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    //TODO: is using struct a good idea?
    struct figureState{
        QPointF position;
        float z;
        float myHeight;
        float angle;
        float stepSize;
    };

    QPointF position;   //x and y coordinates of current position
    float z;
    float viewZ;    //z-coordinate of eyes
    float myHeight;   //z-coordinato of head, must be greater than viewZ
    float maxStepZ;
    float stepSize;
    float angle;
    float verticalAngle;
    float rotateStepSize;
    float VangleStep;
    float HangleStep;
    QVector<GLuint> texture;
    QVector<Triangle> floor;
    QVector<int> texIndex;
    QPoint dragStartPosition;

    QTimer *gravityTimer;
    QTimer *motionTimer;

    MD2Model* model;
    figureState modelState;

    float g;    //gravity constant
    int dt;     //time change (in miliseconds)
    float t;    //time
    QVector3D v0;   //starting velocity

    void makeStep(const QPointF& newPosition);
    bool makeStep(const QPointF& newPosition, figureState& newState);
    void normalizeAngle(float& angle);
    void startGravity(const QVector3D& velocity);
    bool canMove(const QVector3D& direction);

private slots:
    void applyGravity();
    void updateMotion();
};

#endif // MYWIDGET_H
