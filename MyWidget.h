#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <qgl.h>

#include <QtGui>
#include <QVector>
#include <QPoint>
#include <QTimer>

#include "triangle.h"
#include "cameraview.h"
#include "figure.h"

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

    static const float STEP_SIZE;
    static const float ROTATE_STEP_SIZE;

    QTimer *gravityTimer;
    QTimer *motionTimer;

    CameraView *cameraView;
    Figure *figure;

    QVector<GLuint> texture;
    QVector<Triangle> floor;
    QVector<int> texIndex;
    QPoint dragStartPosition;

private slots:
    void applyGravity();
    void updateMotion();
};

#endif // MYWIDGET_H
