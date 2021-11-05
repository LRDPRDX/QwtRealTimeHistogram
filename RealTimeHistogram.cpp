#include <QPushButton>
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

#include "RealTimeHistogram.h"


RealTimeHistogram::RealTimeHistogram( QWidget* parent ) :
    QWidget( parent )
{
    CreateWidgets();
    CreateTimer();

    connect( &fThread, &DataGeneratorThread::DataGenerated, this, &RealTimeHistogram::UpdateData );
}

void RealTimeHistogram::CreateWidgets()
{
    QVBoxLayout* vLayout = new QVBoxLayout();

    // Buttons
    fStartButton = new QPushButton( "START" );
        fStartButton->setStyleSheet( "background-color: #a8ffe9" );
        connect( fStartButton, &QPushButton::clicked, this, &RealTimeHistogram::InitHistogram );
        connect( fStartButton, &QPushButton::clicked, this, &RealTimeHistogram::StartTimer );
    fPauseButton = new QPushButton( "PAUSE" );
        fPauseButton->setStyleSheet( "background-color: #b6b6b6" );
        connect( fPauseButton, &QPushButton::clicked, this, &RealTimeHistogram::Pause );
    fStopButton = new QPushButton( "STOP" );
        fStopButton->setStyleSheet( "background-color: #6d6d6d; color: #ffffff" );
        connect( fStopButton, &QPushButton::clicked, &fThread, &DataGeneratorThread::OnStop );
        connect( fStopButton, &QPushButton::clicked, this, &RealTimeHistogram::StopTimer );

    QFrame *buttonFrame = new QFrame();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget( fStartButton );
    buttonLayout->addWidget( fPauseButton );
    buttonLayout->addWidget( fStopButton );

    buttonFrame->setLayout( buttonLayout );

    vLayout->addWidget( buttonFrame );

    // Histogram
    fPlot = new QwtPlot();
        fPlot->setAxisTitle( QwtPlot::yLeft, "Number of events" );
        fPlot->setAxisTitle( QwtPlot::xBottom, "Value" );
        fPlot->setCanvasBackground( QColor( "#b6b6b6" ) );
        fPlot->plotLayout()->setAlignCanvasToScales( true );
        fPlot->insertLegend( new QwtLegend(), QwtPlot::RightLegend );

    QwtPlotGrid *grid = new QwtPlotGrid;
        grid->enableX( true );
        grid->enableY( true );
        grid->attach( fPlot );
        grid->setMajorPen( QPen( QColor( "#ffffff" ), 0, Qt::DotLine ) );

    fHisto = new QwtPlotHistogram( "Data" );
        fHisto->setPen( QPen( QColor( "#a8ffe9" ) ) );
        fHisto->setBrush( QBrush( QColor( "#a8ffe9" ) ) );
    fHisto->attach( fPlot );

    fStatistics = new QwtPlotMarker();
    fStatistics->attach( fPlot );

    fPlot->show();
    fPlot->replot();

    UpdateStat( 0 );

    vLayout->addWidget( fPlot );

    setLayout( vLayout );
}

void RealTimeHistogram::CreateTimer()
{
    fTimer = new QTimer( this );
        fTimer->setInterval( 300 );
    connect( fTimer, &QTimer::timeout, &fThread, &DataGeneratorThread::Dump );
}

void RealTimeHistogram::InitHistogram()
{
    QVector<QwtIntervalSample> histData;

    for( size_t i = 0; i < N_BINS; ++i )
    {
        histData.push_back( QwtIntervalSample( 0.0, i, i + 1 ) );
    }

    fHisto->setSamples( histData );
    fThread.OnStart( N_BINS );
}

void RealTimeHistogram::StartTimer()
{
    if( not fTimer->isActive() )
    {
        fTimer->start();
    }
}

void RealTimeHistogram::StopTimer()
{
    if( fTimer->isActive() )
    {
        fTimer->stop();
    }
}

void RealTimeHistogram::Pause()
{
    QPushButton* button = qobject_cast<QPushButton*>( this->sender() );
    if( button->text() == "PAUSE" )
    {
        fThread.OnPause();
        button->setText( "RESUME" );
    }
    else if( button->text() == "RESUME" )
    {
        fThread.OnResume();
        button->setText( "PAUSE" );
    }
}

void RealTimeHistogram::UpdateData( const QVector<QwtIntervalSample>& data, unsigned nEvents )
{
    fHisto->setSamples( data );
    fPlot->replot();
    UpdateStat( nEvents );
}

void RealTimeHistogram::UpdateStat( unsigned nEvents )
{
    QwtScaleMap xMap = fPlot->canvasMap( QwtPlot::xBottom );
        double x = (xMap.s1() + xMap.s2()) * 0.5;
    QwtScaleMap yMap = fPlot->canvasMap( QwtPlot::yLeft );
        double y = (yMap.s1() + yMap.s2()) * 0.3;
    fStatistics->setValue( x, y );
    fStatistics->setLabel( QwtText( QString( "Number of events = %1" ).arg( nEvents ) ) );
}
