#pragma once

#include <QWidget>
#include <QMutex>

#include "Processor.h"

class QTimer;
class QPushButton;
class QSpinBox;
class QLineEdit;

class QwtPlot;
class QwtPlotHistogram;
class QwtPlotMarker;

class Producer : public QWidget
{
    Q_OBJECT

    private :
        static constexpr size_t N_BINS = 4096;

        QTimer      *fTimer;
        QPushButton *fStartButton, *fStopButton;
        QSpinBox    *fBlockSpin;
        QLineEdit   *fFileEdit;

        QwtPlot             *fPlot;
        QwtPlotMarker       *fStatistics;
        QwtPlotHistogram    *fHisto;

        QThread fThread;
        Processor* fProcessor;
        bool fDataProcessed;
        QMutex fMutex;

    private :
        void CreateWidgets();
        void CreateTimer();

    public slots :
        void UpdatePlot( const QVector<QwtIntervalSample>& data, unsigned nEvents );
        void UpdateStat( unsigned nEvents );
        void StartTimer();
        void StopTimer();
        void ReadData();
        void OnDataProcessed( bool status );
        void OnError();

    signals :
        void DataReady( std::vector<unsigned> data );
        void Error();

    public :
        Producer( QWidget* parent = nullptr );
        ~Producer() override;
};
