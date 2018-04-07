#include "UdpCvMatProvider.h"
#include <MatSerialization.h>
#include <QDebug>

UdpCvMatProvider::UdpCvMatProvider( QObject *parent ) :
    QObject( parent ) {
//    connect( &_receiver, SIGNAL( imgDataReceived( QByteArray ) ),
//             this, SLOT( onReceiveData( QByteArray ) ) );
}

void UdpCvMatProvider::init( const QString &configPath ) {
    Q_UNUSED( configPath )
}

void UdpCvMatProvider::start( ) {
    this->_receiver.listen( 10000 );
}

void UdpCvMatProvider::stop( ) {
    this->_receiver.stopListen( );
}

void UdpCvMatProvider::read( cv::Mat &frame ) {
    _mat.copyTo( frame );
}

cv::Mat UdpCvMatProvider::cvMat( ) {
    if ( !_receiver.imgData( ).isEmpty( ) )
        _mat = MatSerialization::deserializeMat( _receiver.imgData( ) );
    return _mat;
}

void UdpCvMatProvider::onReceiveData( QByteArray data ) {
    _mat = MatSerialization::deserializeMat( data );
}
