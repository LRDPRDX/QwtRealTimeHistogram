#pragma once

#include <utility>
#include <cmath>

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QMetaType>

#include "qwt_samples.h"

Q_DECLARE_METATYPE(QVector<QwtIntervalSample>)
Q_DECLARE_METATYPE(std::vector<unsigned>)

class Processor : public QObject
{
    Q_OBJECT

    public :
        Processor( QObject *parent = nullptr );
        ~Processor() override;

    public slots :
        void OnStart();
        void Process( std::vector<unsigned> data );

    signals :
        void PlotDataReady( const QVector<QwtIntervalSample>& data, unsigned nEvents );
        void DataProcessed( bool status );

    public :
        bool GenerateGauss( double, double, int&, int& );

    private :
        size_t                      fBins;

        QVector<QwtIntervalSample>  fData;
        size_t                      fEntries;
};
