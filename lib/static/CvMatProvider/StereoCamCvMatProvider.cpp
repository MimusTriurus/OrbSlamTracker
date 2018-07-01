#include "StereoCamCvMatProvider.h"

StereoCamCvMatProvider::StereoCamCvMatProvider( QObject *parent ) :
    _leftCamIndx    { 1 },
    _rightCamIndx   { 0 },
    _frameWidth     { 640 },
    _frameHeight    { 480 },
    QObject( parent )
{

}

void StereoCamCvMatProvider::init( const QString &configPath ) {
    cv::FileStorage fs( configPath.toStdString( ), cv::FileStorage::READ );
    fs[ "leftCamIndex" ] >> _leftCamIndx;
    fs[ "rightCamIndex" ] >> _rightCamIndx;
    fs[ "frameWidth" ] >> _frameWidth;
    fs[ "frameHeight" ] >> _frameHeight;
    cv::String calibFilePath;
    fs[ "calibrationFilePath" ] >> calibFilePath;
    fs.release( );
    //qDebug( ) << _leftCamIndx << _rightCamIndx << _frameWidth << _frameHeight << calibFilePath.c_str( );
    _calibrator.init( cv::Size( _frameWidth, _frameHeight ), calibFilePath.c_str( ) );
}

void StereoCamCvMatProvider::start( ) {
    _left.open( _leftCamIndx );
    _left.set( cv::CAP_PROP_FRAME_WIDTH, _frameWidth );
    _left.set( cv::CAP_PROP_FRAME_HEIGHT, _frameHeight );
    _right.set( cv::CAP_PROP_FRAME_WIDTH, _frameWidth );
    _right.set( cv::CAP_PROP_FRAME_HEIGHT, _frameHeight );

    if ( !_left.isOpened( ) )
        qDebug( ) << "error on open left cam:" << _leftCamIndx;
    else {
        qDebug( ) << "left cam is opend:" << _leftCamIndx
                  << _left.get( cv::CAP_PROP_FRAME_WIDTH )
                  << _left.get( cv::CAP_PROP_FRAME_HEIGHT );

    }
    _right.open( _rightCamIndx );
    if ( !_right.isOpened( ) ) {
        qDebug( ) << "error on open right cam";
    }
    else {
        qDebug( ) << "right cam is opend:" << _rightCamIndx
                  << _right.get( cv::CAP_PROP_FRAME_WIDTH )
                  << _right.get( cv::CAP_PROP_FRAME_HEIGHT );
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
