
QT += opengl

HEADERS += MyWidget.h \
    objparser.h \
    point3D.h \
    triangle.h \
    md2model.h \
    position.h \
    figure.h \
    cameraview.h
SOURCES += main.cpp \
    MyWidget.cpp \
    objparser.cpp \
    triangle.cpp \
    md2model.cpp \
    figure.cpp \
    cameraview.cpp

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    TODO.txt
