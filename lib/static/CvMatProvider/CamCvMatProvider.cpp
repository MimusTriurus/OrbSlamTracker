#include "CamCvMatProvider.h"
#include <QDebug>

CamCvMatProvider::CamCvMatProvider( QObject *parent ) :
    QObject( parent ) {

}

void CamCvMatProvider::init( const QString &configPath ) {
    Q_UNUSED( configPath )
}

void CamCvMatProvider::start( ) {
    _capture.open( 0 );
    if ( !_capture.isOpened( ) )
        qDebug( ) << "error on open cam";
    else
        qDebug( ) << "cam is opend";
}

void CamCvMatProvider::stop( ) {
    if ( _capture.isOpened( ) ) {
        _capture.release( );
    }
}

void CamCvMatProvider::read( cv::Mat &frame ) {
    if ( _capture.isOpened( ) ) {
        _capture.read( frame );
    }
}

cv::Mat CamCvMatProvider::cvMat( ) {
    if ( _capture.isOpened( ) )
        _capture.read( _mat );
    if ( _mat.empty( ) ) {
        qDebug( ) << "error frame is empty";
    }
    return _mat;
}
