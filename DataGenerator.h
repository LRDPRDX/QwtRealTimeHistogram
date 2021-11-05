#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QMetaType>

#include "qwt_samples.h"

Q_DECLARE_METATYPE(QVector<QwtIntervalSample>)

class DataGeneratorThread : public QThread
{
    Q_OBJECT

    public :
        DataGeneratorThread( QObject *parent = nullptr );
        ~DataGeneratorThread() override;

    public slots :
        void OnStart( size_t size );
        void OnStop();
        void OnPause();
        void OnResume();
        void Dump();

    signals :
        void DataGenerated( const QVector<QwtIntervalSample>& data, unsigned nEvents );

    protected :
        void run() override;

    private :
        bool fAbort = false;
        bool fPause = false;
        bool fRestart = false;
        bool fDumpRequest = false;

        QMutex fMutex;
        QWaitCondition fCondition;

        size_t fSize;
};
