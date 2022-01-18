#include <QApplication>

#include "Producer.h"
#include "Style.h"

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    QPalette darkPalette;
    /****** ALL GROUPS ******/
    darkPalette.setColor( QPalette::Window,             style::darkGray);
    darkPalette.setColor( QPalette::WindowText,         style::white );
    darkPalette.setColor( QPalette::Base,               style::black);
    darkPalette.setColor( QPalette::AlternateBase,      style::darkGray);
    darkPalette.setColor( QPalette::ToolTipBase,        style::violet );
    darkPalette.setColor( QPalette::ToolTipText,        style::black );
    darkPalette.setColor( QPalette::Text,               style::white );
    darkPalette.setColor( QPalette::Button,             style::darkGray );
    darkPalette.setColor( QPalette::ButtonText,         style::white );
    darkPalette.setColor( QPalette::Link,               style::violet );
    darkPalette.setColor( QPalette::Highlight,          style::violet );
    darkPalette.setColor( QPalette::HighlightedText,    style::black );

    /****** ACTIVE GROUP ******/
    darkPalette.setColor( QPalette::Active,
                          QPalette::Button, QColor( style::gray ).darker() );

    /****** DISABLED GROUP ******/
    darkPalette.setColor( QPalette::Disabled,
                          QPalette::ButtonText, style::gray );

    darkPalette.setColor( QPalette::Disabled,
                          QPalette::WindowText, style::gray );

    darkPalette.setColor( QPalette::Disabled,
                          QPalette::Text, style::gray );

    darkPalette.setColor( QPalette::Disabled,
                          QPalette::Light, style::darkGray);

    qApp->setPalette( darkPalette );

    /****** FONT ******/
    QFont font( "Hermit", 9 );
    qApp->setFont( font );

    Producer window;

    window.setWindowTitle( "Real Time Histogram" );
    window.resize( 300, 300 );
    window.show();

    return app.exec();
}
