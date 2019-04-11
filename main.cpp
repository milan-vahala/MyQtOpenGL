#include "MyWidget.h"
#include <QApplication>

int main( int argc, char **argv )
{
        QApplication app( argc, argv );
        MyWidget widget;// = new MyWidget();
        widget.show();
        return app.exec();
}
