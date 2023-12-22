#include <fstream>

#include <QPushButton>
#include <QMutex>
#include <QMutexLocker>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QTimer>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <qwt_plot.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_plot_marker.h>
#include <qwt_scale_map.h>
#include <qwt_text.h>

#include "Producer.h"
#include "Dialogs.h"
#include "Style.h"

#include "qnamespace.h"


Producer::Producer( QWidget* parent ) :
    QWidget( parent ),
    fDataProcessed( false )
{
    fProcessor = new Processor();
    fProcessor->moveToThread( &fThread );

    connect( &fThread, &QThread::finished, fProcessor, &QObject::deleteLater );
    connect( fProcessor, &Processor::DataProcessed, this, &Producer::OnDataProcessed );
    connect( fProcessor, &Processor::PlotDataReady, this, &Producer::UpdatePlot );
    connect( this, &Producer::DataReady, fProcessor, &Processor::Process );

    CreateWidgets();
    CreateTimer();

    connect( this, &Producer::Error, this, &Producer::OnError );

    fThread.start();
}

Producer::~Producer()
{
    fThread.quit();
    fThread.wait();
}

void Producer::CreateWidgets()
{
    QVBoxLayout* vLayout = new QVBoxLayout();

    // Settings
    QLabel* fileLabel = new QLabel( "Read from: ");
        fileLabel->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    fFileEdit = new QLineEdit();
        fFileEdit->setText( "/dev/urandom" );
        fFileEdit->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
    QLabel* blockLabel = new QLabel( "Transfer size: " );
        blockLabel->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    fBlockSpin = new QSpinBox();
        fBlockSpin->setRange( 1, 10000 );
        fBlockSpin->setValue( 1000 );

    QGroupBox *settingFrame = new QGroupBox( "Settings");
    QHBoxLayout *settingLayout = new QHBoxLayout();

    settingLayout->addWidget( fileLabel, Qt::AlignRight );
    settingLayout->addWidget( fFileEdit );
    settingLayout->addWidget( blockLabel, Qt::AlignRight );
    settingLayout->addWidget( fBlockSpin );

    settingFrame->setLayout( settingLayout );

    // Buttons
    fStartButton = new QPushButton( "START" );
        ColorButton( fStartButton, style::yellow );
        connect( fStartButton, &QPushButton::clicked, fFileEdit, [this](){fFileEdit->setEnabled( false );} );
        connect( fStartButton, &QPushButton::clicked, fProcessor, &Processor::OnStart );
        connect( fStartButton, &QPushButton::clicked, this, &Producer::StartTimer );
    fStopButton = new QPushButton( "STOP" );
        ColorButton( fStopButton, style::pink );
        connect( fStopButton, &QPushButton::clicked, this, &Producer::StopTimer );
        connect( fStopButton, &QPushButton::clicked, fFileEdit, [this](){fFileEdit->setEnabled( true );} );


    QFrame *buttonFrame = new QFrame();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget( fStartButton );
    buttonLayout->addWidget( fStopButton );

    buttonFrame->setLayout( buttonLayout );

    vLayout->addWidget( settingFrame );
    vLayout->addWidget( buttonFrame );

    // Histogram
    fPlot = new QwtPlot();
        fPlot->setAxisTitle( QwtPlot::yLeft, "Number of events" );
        fPlot->setAxisTitle( QwtPlot::xBottom, "Value" );
        fPlot->plotLayout()->setAlignCanvasToScales( true );
        fPlot->insertLegend( new QwtLegend(), QwtPlot::RightLegend );

    QwtPlotGrid *grid = new QwtPlotGrid;
        grid->enableX( true );
        grid->enableY( true );
        grid->attach( fPlot );
        grid->setMajorPen( QPen( QColor( style::pink ), 0, Qt::DotLine ) );

    fHisto = new QwtPlotHistogram( "Data" );
        fHisto->setPen( QPen( QColor( style::red ) ) );
        fHisto->setBrush( QBrush( QColor( style::red ) ) );
    fHisto->attach( fPlot );

    fStatistics = new QwtPlotMarker();
    fStatistics->attach( fPlot );

    fPlot->show();
    fPlot->replot();

    UpdateStat( 0 );

    vLayout->addWidget( fPlot );

    setLayout( vLayout );
}

void Producer::CreateTimer()
{
    fTimer = new QTimer( this );
        fTimer->setInterval( 1000 );
    connect( this, &Producer::Error, fTimer, &QTimer::stop );
    connect( fTimer, &QTimer::timeout, this, &Producer::ReadData );
}

void Producer::StartTimer()
{
    if( not fTimer->isActive() )
    {
        fTimer->start();
    }
}

void Producer::StopTimer()
{
    if( fTimer->isActive() )
    {
        fTimer->stop();
    }
}

void Producer::UpdatePlot( const QVector<QwtIntervalSample>& data, unsigned nEvents )
{
    fHisto->setSamples( data );
    fPlot->replot();
    UpdateStat( nEvents );
}

void Producer::UpdateStat( unsigned nEvents )
{
    QwtScaleMap xMap = fPlot->canvasMap( QwtPlot::xBottom );
        double x = (xMap.s1() + xMap.s2()) * 0.7;
    QwtScaleMap yMap = fPlot->canvasMap( QwtPlot::yLeft );
        double y = (yMap.s1() + yMap.s2()) * 0.7;
    fStatistics->setValue( x, y );
    QwtText text( QString( "Number of events = %1" ).arg( nEvents ) );
        text.setColor( QColor( style::white ) );
    fStatistics->setLabel( text );
}

void Producer::ReadData()
{
    if( !fDataProcessed )
    {
        qInfo() << "Waiting for the Processor to finish...";
        return;
    }

    std::ifstream random( (fFileEdit->text()).toStdString() );
    const int size = fBlockSpin->value();
    unsigned dataArray[size];
    if( random.is_open() )
    {
        if( random.read( reinterpret_cast<char*>( dataArray ), sizeof( dataArray ) ) )
        {
            std::vector<unsigned> dataVector( dataArray, dataArray + size );

            fMutex.lock();
                fDataProcessed = false;
            fMutex.unlock();

            emit DataReady( dataVector );
        }
    }
    else
    {
        emit Error();
    }
}

void Producer::OnDataProcessed( bool status )
{
    QMutexLocker locker( &fMutex );
    fDataProcessed = status;
}

void Producer::OnError()
{
    ErrorMessageBox *eDialog = new ErrorMessageBox( QString( "Error reading the file: %1" ).arg( fFileEdit->text() ) , this );
    eDialog->show();
}
