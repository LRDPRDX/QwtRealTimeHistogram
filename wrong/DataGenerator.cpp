#include <algorithm>
#include <random>

#include <QMutexLocker>

#include "DataGenerator.h"


DataGeneratorThread::DataGeneratorThread( QObject* parent ) :
    QThread( parent )
{
    qRegisterMetaType<QVector<QwtIntervalSample>>();
}

DataGeneratorThread::~DataGeneratorThread()
{
    fMutex.lock();
        fAbort = true;
        fCondition.wakeOne();
    fMutex.unlock();

    wait();// Wait for return from run() before QThread::~QThread
}

void DataGeneratorThread::OnStart( size_t size )
{
    QMutexLocker locker( &fMutex );

    fAbort = false;
    fPause = false;

    fSize = size;

    if( !isRunning() )
    {
        start( LowPriority );
    }
    else
    {
        fRestart = true;
        fCondition.wakeOne();
    }
}

void DataGeneratorThread::OnStop()
{
    QMutexLocker locker( &fMutex );
    fAbort = true;
    fCondition.wakeOne();
}

void DataGeneratorThread::OnPause()
{
    QMutexLocker locker( &fMutex );
    fPause = true;
    fCondition.wakeOne();
}

void DataGeneratorThread::OnResume()
{
    QMutexLocker locker( &fMutex );
    fPause = false;
    fCondition.wakeOne();
}

void DataGeneratorThread::Dump()
{
    QMutexLocker locker( &fMutex );
    fDumpRequest = true;
}

void DataGeneratorThread::run()
{
    forever
    {
        fMutex.lock();
            const size_t size = fSize;
        fMutex.unlock();

        QVector<QwtIntervalSample> data( size );
        unsigned n = 0;

        for( size_t i = 0; i < size; ++i )
        {
            data[i] = QwtIntervalSample( 0.0, i, i + 1 );
        }

        std::random_device rd;
        std::mt19937 gen( rd() );

        forever
        {
            if( fAbort )
            {
                return;
            }
            if( fDumpRequest )
            {
                emit DataGenerated( data, n );
                fMutex.lock();
                    fDumpRequest = false;
                fMutex.unlock();
            }
            if( fPause )
            {
                fMutex.lock();
                    fCondition.wait( &fMutex );
                fMutex.unlock();
            }
            if( fRestart )
            {
                break;
            }
            std::normal_distribution<> g( size / 2., size / 20. );
            size_t rand = std::round( g( gen ) );
            if( rand < size )
            {
                data[ rand ].value += 1;
                n++;
            }
        }
        fMutex.lock();
            fRestart = false;
        fMutex.unlock();
    }
}
