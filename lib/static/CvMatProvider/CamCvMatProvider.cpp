#include "CamCvMatProvider.h"
#include <QDebug>

CamCvMatProvider::CamCvMatProvider( QObject *parent ) :
    QObject( parent ) {

}

void CamCvMatProvider::init( const QString &configPath ) {
    Q_UNUSED( configPath )
}

void CamCvMatProvider::start( ) {
    _cap.open( 0 );
    if ( !_cap.isOpened( ) )
        qDebug( ) << "error on open cam";
    else
        qDebug( ) << "cam is opend";
}

void CamCvMatProvider::stop( ) {
    if ( _cap.isOpened( ) ) {
        _cap.release( );
    }
}

cv::Mat CamCvMatProvider::cvMat( ) {
    if ( _cap.isOpened( ) )
        _cap.read( _mat );
    if ( _mat.empty( ) ) {
        qDebug( ) << "error frame is empty";
    }
    return _mat;
}
