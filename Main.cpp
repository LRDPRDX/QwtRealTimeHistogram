#include <QApplication>

#include "RealTimeHistogram.h"


int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    RealTimeHistogram window;

    window.setWindowTitle( "Real Time Histogram" );
    window.show();

    return app.exec();
}
