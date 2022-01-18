#include <algorithm>
#include <cmath>
#include <cerrno>

#include <QMutexLocker>

#include "Processor.h"


Processor::Processor( QObject* parent ) :
    QObject( parent ),

    fBins( 4096 ),
    fEntries( 0 )
{
    qRegisterMetaType<QVector<QwtIntervalSample>>();
    qRegisterMetaType<std::vector<unsigned>>();
}

Processor::~Processor()
{
}

void Processor::OnStart()
{
    fData.resize( fBins );
    for( size_t i = 0; i < fBins; ++i )
    {
        fData[i] = QwtIntervalSample( 0.0, i, i + 1 );
    }
    fEntries = 0;
    emit DataProcessed( true );
}

void Processor::Process( std::vector<unsigned> data )
{
    auto updateBin = [&]( const int& z ) {
        if( (z >= 0) && ((size_t)z < fBins) ) {
            fData[ z ].value += 1; fEntries++; } };

    for( size_t index = 0; index < data.size() - 1; index += 2 )
    {
        double u1 = (double)( data[index]     % fBins ) / fBins;
        double u2 = (double)( data[index + 1] % fBins) / fBins;
        int z1, z2;

        if( GenerateGauss( u1, u2, z1, z2 ) )
        {
            updateBin( z1 );
            updateBin( z2 );
        }
    }
    emit DataProcessed( true );
    emit PlotDataReady( fData, fEntries );
}

bool Processor::GenerateGauss( double u1, double u2, int& z1, int& z2 )
{
    double sigma = (double)fBins / 20.;
    double mu    = (double)fBins / 2.;

    errno = 0;

    double r = sigma * std::sqrt( -2. * log( u1 ) );
    z1 = std::round( r * cos( 2.0 * M_PI * u2 ) + mu );
    z2 = std::round( r * sin( 2.0 * M_PI * u2 ) + mu );

    return (errno == 0);
}
