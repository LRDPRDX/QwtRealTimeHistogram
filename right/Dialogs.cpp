#include "Dialogs.h"
#include "Style.h"

#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

ErrorMessageBox::ErrorMessageBox( const QString& message, QWidget *parent ) :
    QDialog( parent )
{
        setModal( true );
        setAttribute( Qt::WA_DeleteOnClose, true );
        setWindowTitle( "Error" );

        QLabel* text = new QLabel( message );
        fButton = new QPushButton( "OK" );
            connect( fButton, &QPushButton::clicked, this, &QDialog::close );
        ColorButton( fButton, style::violet );

        QVBoxLayout* vLayout = new QVBoxLayout();

        vLayout->addWidget( text );
        vLayout->addWidget( fButton );

        setLayout( vLayout );
}
