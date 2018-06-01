#include "StereoCamCvMatProvider.h"

StereoCamCvMatProvider::StereoCamCvMatProvider( QObject *parent ) : QObject( parent ) {

}

void StereoCamCvMatProvider::init( const QString &configPath ) {
    Q_UNUSED( configPath )
}

void StereoCamCvMatProvider::start( ) {
    _left.open( 0 );
    if ( !_left.isOpened( ) )
        qDebug( ) << "error on open left cam";
    else {
//        _left.set( cv::CAP_PROP_FRAME_HEIGHT, 720 );
//        _left.set( cv::CAP_PROP_FRAME_WIDTH, 1280 );
        qDebug( ) << "left cam is opend";

    }
    _right.open( 1 );
    if ( !_right.isOpened( ) )
        qDebug( ) << "error on open right cam";
    else {
//        _right.set( cv::CAP_PROP_FRAME_HEIGHT, 720 );
//        _right.set( cv::CAP_PROP_FRAME_WIDTH, 1280 );
        qDebug( ) << "right cam is opend";
    }
}

void StereoCamCvMatProvider::stop( ) {
    if ( _left.isOpened( ) ) {
        _left.release( );
    }
    if ( _right.isOpened( ) ) {
        _right.release( );
    }
}

void StereoCamCvMatProvider::read( cv::Mat &left, cv::Mat &right ) {
    if ( _left.isOpened( ) ) {
        _left.read( left );
    }
    if ( _right.isOpened( ) ) {
        _right.read( right );
    }

    //_calibrator.calibrate( left, right );
}
