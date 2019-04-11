#include "MyWidget.h"
#include "objparser.h"

#include <QTransform>
#include <cmath>

MyWidget::MyWidget(QWidget *parent) :
        QGLWidget(parent), z(0), viewZ(1.5), myHeight(1.8), stepSize(0.8), angle(0),
        verticalAngle(0), rotateStepSize(5), g(9.81), dt(30)
{
    position = QPointF(0,0);
    maxStepZ=stepSize*0.8;

    ObjParser parser;
    QString fileName("maps/cosik.obj");
    parser.parse(fileName);
    floor=parser.getTriangles();

    gravityTimer = new QTimer(this);
    connect(gravityTimer, SIGNAL(timeout()), this, SLOT(applyGravity()));

    motionTimer = new QTimer(this);
    connect(motionTimer, SIGNAL(timeout()), this, SLOT(updateMotion()));
    motionTimer->start(dt); //same timeout as when aplying gravity

    modelState.position = QPointF(0,4);
    modelState.z = 0;
    modelState.myHeight = 0.8;
    modelState.stepSize = 2.4;
    modelState.angle = 180;
}

MyWidget::~MyWidget(){
    delete gravityTimer;
    delete model;
}

void MyWidget::keyPressEvent ( QKeyEvent * event )
{
    QPointF view=stepSize*QTransform().rotate(-angle).map(QPointF(0,1));
    switch (event->key()){
    case Qt::Key_Up :
        if (!gravityTimer->isActive()){
            makeStep(position + view);
        }
        break;
    case Qt::Key_Down :
        if (!gravityTimer->isActive()){
            makeStep(position - view);
        }
        break;
    case Qt::Key_Left :
        angle-=rotateStepSize;
        normalizeAngle(angle);
        update();
        break;
    case Qt::Key_Right :
        angle+=rotateStepSize;
        normalizeAngle(angle);
        update();
        break;
    case Qt::Key_Space :
        startGravity(QVector3D(0,0,5));
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
     verticalAngle+=45.0f*dragVector.y()/height();
     normalizeAngle(verticalAngle);
     angle-=45.0f*dragVector.x()/height();
     normalizeAngle(angle);
     update();
     dragStartPosition=event->pos();
}

//TODO: test if this method is working correctly
void MyWidget::normalizeAngle(float &angle){
    if (angle>360){
        angle-=(int)angle%360*360;
    }
    else if (angle<0){
        angle+=(-(int)angle)%360*360;
    }
}

void MyWidget::makeStep(const QPointF &newPosition)
{
    QVector<float> possibleZ;
    float newZ;
    for (QVector<Triangle>::iterator it=floor.begin(); it!=floor.end(); it++){
        if (it->contains(newPosition, newZ)){
            if ((newZ-z)<maxStepZ){
                possibleZ.append(newZ);
            }
        }
    }
    const float minZ=-1000;
    float Z=minZ;    //TODO: make this better
    //searching highest Z which is under my possition
    for (int i=0; i<possibleZ.size(); ++i){
        float tmpZ(possibleZ.at(i));
        if (tmpZ>Z){
            Z=tmpZ;
        }
    }
    if (Z>minZ){ //or possibleZ.size()==0
        position=newPosition;
        update();
        if ((z-Z)>maxStepZ){
            startGravity(QVector3D(0,0,0)); //start falling
        } else {
            z=Z;
        }
    }
}

//TODO: almost the same as makeStep(const QPointF &newPosition), merge somehow
bool MyWidget::makeStep(const QPointF &newPosition, figureState& newState)
{
    //different code here
    float z=newState.z;

    //end of different code
    QVector<float> possibleZ;
    float newZ;
    for (QVector<Triangle>::iterator it=floor.begin(); it!=floor.end(); it++){
        if (it->contains(newPosition, newZ)){
            if ((newZ-z)<maxStepZ){
                possibleZ.append(newZ);
            }
        }
    }
    const float minZ=-1000;
    float Z=minZ;    //TODO: make this better
    //searching highest Z which is under my possition
    for (int i=0; i<possibleZ.size(); ++i){
        float tmpZ(possibleZ.at(i));
        if (tmpZ>Z){
            Z=tmpZ;
        }
    }
    if (Z>minZ){ //or possibleZ.size()==0
        //position=newPosition;
        newState.position=newPosition;
        //TODO: Do update realy here?
        //update();
        //if ((z-Z)>maxStepZ){
            //falling for anything other then me not implemented
            //startGravity(QVector3D(0,0,0)); //start falling
        //} else {
            //z=Z;
            newState.z=Z;
        //}
        update();
        return true;
    }
    return false;
}

bool MyWidget::canMove(const QVector3D &direction){
    QPointF newPosition=position+direction.toPointF();
    //TODO: copy of code from makeStep(...) method, merge
    QVector<float> possibleZ;
    float newZ;
    for (QVector<Triangle>::iterator it=floor.begin(); it!=floor.end(); it++){
        if (it->contains(newPosition, newZ)){
            if ((newZ-z)<maxStepZ){
                possibleZ.append(newZ);
            }
            //testing if I crash my head into ceiling when jumping up
            if (gravityTimer->isActive()){ // gravity on, I'm jumping or falling
                if ( (newZ-(z+myHeight))*(newZ-(z+myHeight+direction.z()))<0 ){
                    v0=QVector3D(0,0,0);
                }
            }
        }
    }
    // end of copy from makeStep(...) method
    int size=possibleZ.size();
    if (size==0){
        return false;
    }
    for (int i=0; i<size; ++i){
        float tmpZ(possibleZ.at(i));
        //test if crossing some plane when moving in vertical direction
        if ( (tmpZ-z)*(tmpZ-(z+direction.z()))<0 ){
            z=tmpZ;
            update();
            return false;
        }
    }
    return true;
}

void MyWidget::startGravity(const QVector3D& velocity){
    v0=velocity;
    t=0;
    gravityTimer->start(dt);
}

void MyWidget::applyGravity(){
    t+=dt*0.001;
    QVector3D moveDirection=(v0 + g*t*QVector3D(0,0,-1))*t;
    if (canMove(moveDirection)){
        position+=moveDirection.toPointF();
        z=z+moveDirection.z();
        update();
    } else {
        gravityTimer->stop();
    }
}

void MyWidget::updateMotion(){
    float progress=0.05f;
    if (model) {
        model->advance(progress);
    }
    QPointF view=modelState.stepSize*QTransform().rotate(-modelState.angle).map(QPointF(0,1));
    if(!makeStep(modelState.position+progress*view, modelState)){
        modelState.angle+=60;
    }
    updateGL();
}


//-------------------------------- OpenGL related stuff ----------------------------

void MyWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glRotatef(-90-verticalAngle,1,0,0);   //z-axis is UP, y-axis is FRONT
    glRotatef(angle,0,0,1); //rotate camera to left and right
    glTranslatef(-position.x(),-position.y(),-(z+viewZ));

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
    if (model) {
        glPushMatrix();        
        glTranslatef(modelState.position.x(), modelState.position.y(), modelState.z+modelState.myHeight);
        //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        //glRotatef(modelState.angle-90, 1.0f, 0.0f, 0.0f);
        glRotatef(90-modelState.angle, 0.0f, 0.0f, 1.0f);
        float scale=0.035;
        glScalef(scale, scale, scale);
        model->draw();
        glPopMatrix();
    }
}

void MyWidget::resizeGL( int width, int height )
{
    //TODO: What the hell is this?
    height = height?height:1;

    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,150.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyWidget::initializeGL()
{
    loadGLtextures();

    model = MD2Model::load("models/girl.md2");
    if (model) {
        model->setAnimation("run");
    }

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);  //enable smooth shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //background color is "sky blue"
    //glClearColor(0.7f, 0.9f, 1.0f, 0.3f);
    glClearDepth(1.0f);
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
