#pragma once

#include <QWidget>
#include "DataGenerator.h"

class QTimer;
class QPushButton;

class QwtPlot;
class QwtPlotHistogram;
class QwtPlotMarker;


class RealTimeHistogram : public QWidget
{
    Q_OBJECT

    private :
        static constexpr size_t N_BINS = 4096;

        QTimer      *fTimer;
        QPushButton *fStartButton, *fStopButton, *fPauseButton;

        QwtPlot             *fPlot;
        QwtPlotMarker       *fStatistics;
        QwtPlotHistogram    *fHisto;

        DataGeneratorThread fThread;

    private :
        void CreateWidgets();
        void CreateTimer();

    public slots :
        void InitHistogram();
        void UpdateData( const QVector<QwtIntervalSample>& data, unsigned nEvents );
        void UpdateStat( unsigned nEvents );
        void StartTimer();
        void StopTimer();
        void Pause();

    public :
        RealTimeHistogram( QWidget* parent = nullptr );
        ~RealTimeHistogram() override = default;
};
