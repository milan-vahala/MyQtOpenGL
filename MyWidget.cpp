#include "MyWidget.h"
#include "objparser.h"

#include <QApplication>

const float MyWidget::STEP_SIZE = 0.8f;
const float MyWidget::ROTATE_STEP_SIZE = 5;

MyWidget::MyWidget(QWidget *parent) : QGLWidget(parent)
{
    floor = ObjParser(":/maps/cosik.obj").getTriangles();

    figure = new Figure(&floor);

    cameraView = new CameraView(&floor);

    motionTimer = new QTimer(this);
    connect(motionTimer, SIGNAL(timeout()), this, SLOT(updateMotion()));
    motionTimer->start(30); //same timeout as when aplying gravity
}

MyWidget::~MyWidget() {
    delete figure;
    delete cameraView;
    delete motionTimer;
}

void MyWidget::keyPressEvent ( QKeyEvent * event )
{
    switch (event->key()) {
    case Qt::Key_Up :
        if (!cameraView->gravityOn()) {
            cameraView->step(STEP_SIZE);
        }
        break;
    case Qt::Key_Down :
        if (!cameraView->gravityOn()) {
            cameraView->step(-STEP_SIZE);
        }
        break;
    case Qt::Key_Left :
        cameraView->turnHorizontaly(-ROTATE_STEP_SIZE);
        update();
        break;
    case Qt::Key_Right :
        cameraView->turnHorizontaly(ROTATE_STEP_SIZE);
        update();
        break;
    case Qt::Key_Space :
        if (!cameraView->gravityOn()) {
            cameraView->startGravity(QVector3D(0,0,5));
        }
        break;
    }
}

void MyWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        dragStartPosition=event->pos();
    }
}

void MyWidget::mouseMoveEvent(QMouseEvent *event){
    if (!(event->buttons() & Qt::LeftButton))  //TODO: refactor this
         return;
     if ((event->pos() - dragStartPosition).manhattanLength()
          < QApplication::startDragDistance())
         return;

     QPoint dragVector=event->pos() - dragStartPosition;
     cameraView->turnVerticaly(45.0f * dragVector.y() / height());
     cameraView->turnHorizontaly(-45.0f*dragVector.x()/height());
     update();
     dragStartPosition=event->pos();
}

void MyWidget::updateMotion() {
    cameraView->applyGravity();
    figure->updateMotion();

    updateGL();
}


//-------------------------------- OpenGL related stuff ----------------------------

void MyWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glRotatef(-90 - cameraView->getVerticalAngle(), 1, 0, 0);   //z-axis is UP, y-axis is FRONT
    glRotatef(cameraView->getAngle(), 0, 0, 1); //rotate camera to left and right
    glTranslatef(-cameraView->getX(), -cameraView->getY(), -cameraView->getZ());

    GLfloat ambientColor[] = {1.3f, 1.3f, 1.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add positioned light

    GLfloat lightColor0[] = {3.7f, 3.7f, 3.7f, 1.0f};
    GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Last param is 1 for position
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    //Add direct light
    /*
    GLfloat lightColor1[] = {2.2f, 2.2f, 2.2f, 1.0f};
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};    //Last param is 0 for direct
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    */
    for (QVector<Triangle>::iterator it=floor.begin(); it!=floor.end(); it++){
        it->render(texture[it->getTextureIndex()]);
    }

    //Draw the guy
    figure->draw();
}

void MyWidget::perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void MyWidget::resizeGL( int width, int height )
{
    //TODO: What the hell is this?
    height = height?height:1;

    glViewport(0, 0, static_cast<GLint>(width), static_cast<GLint>(height) );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspectiveGL(45.0, static_cast<GLdouble>(width) / static_cast<GLdouble>(height), 0.1, 150.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyWidget::initializeGL()
{
    loadGLtextures();

    figure->initModel();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);  //enable smooth shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //background color is "sky blue"
    //glClearColor(0.7f, 0.9f, 1.0f, 0.3f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glShadeModel(GL_NORMALIZE);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void MyWidget::loadGLtextures()
{
    QVector<QString> fileName;
    fileName << ":/textures/Marble2.jpg"
            << ":/textures/Marble.jpg"
            << ":/textures/Crate.bmp";

    int texCount=fileName.size();
    texture.resize(texCount);
    for (int i=0; i<texCount; i++){
        QImage tmp=QImage(fileName[i]);
        if (tmp.isNull()){
            tmp=QImage(16,16,QImage::Format_RGB32);
            tmp.fill(Qt::green);
        }
        QImage t=QGLWidget::convertToGLFormat( tmp );
        glGenTextures( 1, &texture[i] );
        glBindTexture( GL_TEXTURE_2D, texture[i] );
        glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    }
}
