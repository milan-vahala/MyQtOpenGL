
QT += opengl

HEADERS += MyWidget.h \
    objparser.h \
    point3D.h \
    triangle.h \
    md2model.h
SOURCES += main.cpp \
    MyWidget.cpp \
    objparser.cpp \
    triangle.cpp \
    md2model.cpp

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    TODO.txt
