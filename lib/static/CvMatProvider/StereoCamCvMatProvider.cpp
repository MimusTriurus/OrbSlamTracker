#include "StereoCamCvMatProvider.h"

StereoCamCvMatProvider::StereoCamCvMatProvider( QObject *parent ) : QObject( parent ) {

}

void StereoCamCvMatProvider::init( const QString &configPath ) {
    Q_UNUSED( configPath )
}

void StereoCamCvMatProvider::start( ) {
    _left.open( 1 );
    if ( !_left.isOpened( ) )
        qDebug( ) << "error on open left cam";
    else {
        cv::Size size{ ( int )_left.get( cv::CAP_PROP_FRAME_WIDTH ),
                       ( int )_left.get( cv::CAP_PROP_FRAME_HEIGHT ) };
        _calibrator.init( size, "E:/Projects/Qt/Builds/OrbSlam2/bin/" );
        qDebug( ) << "left cam is opend";

    }
    _right.open( 0 );
    if ( !_right.isOpened( ) )
        qDebug( ) << "error on open right cam";
    else {
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
    if ( _left.isOpened( ) && _right.isOpened( ) ) {
        if ( _left.grab( ) && _right.grab( ) ) {
            _right.retrieve( right );
            _left.retrieve( left );
            _calibrator.calibrate( left, right );
        }
        else
            qDebug( ) << "not grab";
    }
    else
        qDebug( ) << "not opened";
}
