#include "OrbSlamTracker.h"
#include <QMatrix3x3>
#include <QQuaternion>
#include <QDebug>

#include "ICvMatProvider.h"
#include "UdpCvMatProvider.h"
#include "CamCvMatProvider.h"
#include "StereoCamCvMatProvider.h"
#include "StereoVideoCvMatProvider.h"

OrbSlamTracker::OrbSlamTracker( const QString &assetsPath, int regim , bool showViewer ) :
    _assetsPath { assetsPath },
    _regim      { static_cast<ORB_SLAM2::System::eSensor>( regim ) },
    _showViewer { showViewer },
    _localDevice{ true },
    _inProgress { false }
{
    _position.resize( 3 );
    _position.fill( 0.0f );

    _rotation.resize( 4 );
    _rotation[ 0 ] = 1.0f;
    _rotation[ 1 ] = 0.0f;
    _rotation[ 2 ] = 0.0f;
    _rotation[ 3 ] = 0.0f;
}

void OrbSlamTracker::localDevice( bool value ) {
    QMutexLocker locker ( &_mutex );
    _localDevice = value;
}

void OrbSlamTracker::track( bool value ) {
    _inProgress = value;
    if ( _inProgress && !this->isRunning( ) ) {
        this->start( );
    }
}

QVector<float> &OrbSlamTracker::position( ) {
    QMutexLocker locker ( &_mutex );
    return _position;
}

QVector<float> &OrbSlamTracker::rotation( ) {
    QMutexLocker locker ( &_mutex );
    return _rotation;
}

bool OrbSlamTracker::inProgress( ) const {
    QMutexLocker locker ( &_mutex );
    return _inProgress;
}

void OrbSlamTracker::run( ) {
    QString vocabPath    { _assetsPath + "/ORBvoc.txt" };
    QString settingsPath { _assetsPath + "/webcam.yaml" };
    tframe = 0.0f;
    ICvMatProvider *cvMatProvider{ nullptr };
    switch ( _regim ) {
        case ORB_SLAM2::System::MONOCULAR:
            if ( _localDevice )
                cvMatProvider = new CamCvMatProvider( );
            else
                cvMatProvider = new UdpCvMatProvider( );
        break;
        case ORB_SLAM2::System::STEREO:
            cvMatProvider = new StereoCamCvMatProvider( );
        break;
        case ORB_SLAM2::System::RGBD:
            log( "RGBD regim unsupported yet.Try monocular regim." );
        break;
        default:
            log( "unknown regim.Try monocular regim." );
        break;
    }
    if ( !cvMatProvider )
        return;
    cvMatProvider->init( QString { _assetsPath + "/settings.yaml" } );
    cvMatProvider->start( );
    ORB_SLAM2::System *orbSlam2 = new ORB_SLAM2::System( vocabPath.toStdString( ),
                                                         settingsPath.toStdString( ),
                                                         _regim, _showViewer );
    log( "start run" );
    while ( _inProgress ) {
        QCoreApplication::processEvents( );
        cv::Mat trackResult;
        cv::Mat left;
        cv::Mat right;
        __int64 curNow = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( ) / 1000.0;
        tframe += 0.0003f;
        switch ( _regim ) {
            case ORB_SLAM2::System::MONOCULAR:
                cvMatProvider->read( left );
                trackResult = orbSlam2->TrackMonocular( left, curNow );
            break;
            case ORB_SLAM2::System::STEREO:
                cvMatProvider->read( left, right );
                trackResult = orbSlam2->TrackStereo( left, right, tframe );
            break;
        }
        if ( !trackResult.empty( ) ) {
            // Получаем позицию камеры в пространстве
            cv::Mat translationMat( 3, 1, CV_32F );
            translationMat = trackResult.rowRange( 0, 3 ).col( 3 );
            qCopy( translationMat.begin<float>( ), translationMat.end<float>( ), _position.begin( ) );
            // Получаем поворот камеры
            cv::Mat rotationMat   ( 3, 3, CV_32F );
            rotationMat = trackResult.rowRange( 0, 3 ).colRange( 0, 3 );
            QMatrix3x3 qRotationMatrix;
            for ( int i{ 0 }; i < rotationMat.rows; ++i )
                for ( int j{ 0 }; j < rotationMat.cols; ++j ) {
                    qRotationMatrix( i, j ) = rotationMat.at<float>( i, j );
                }

            QQuaternion rotation{ QQuaternion::fromRotationMatrix( qRotationMatrix ) };

            _rotation[ 0 ] = rotation.scalar( );
            _rotation[ 1 ] = rotation.x( );
            _rotation[ 2 ] = rotation.y( );
            _rotation[ 3 ] = rotation.z( );
        }
//        QThread::msleep( 5 );
    }
    log( "end run" );
    cvMatProvider->stop( );
    orbSlam2->Shutdown( );

    delete cvMatProvider;
    delete orbSlam2;
}

void OrbSlamTracker::log( const QString &mess ) {
    if ( logMessage != nullptr )
        logMessage( mess.toUtf8( ) );
}

//ICvMatProvider *OrbSlamTracker::makeCvMatProvider( ) {
//    ICvMatProvider *cvMatProvider{ nullptr };
//    switch ( _regim ) {
//        case ORB_SLAM2::System::MONOCULAR:
//            if ( _localDevice )
//                cvMatProvider = new CamCvMatProvider( );
//            else
//                cvMatProvider = new UdpCvMatProvider( );
//            break;
//        case ORB_SLAM2::System::STEREO:
//            log( "stereo regim unsupported yet. Try monocular regim." );
//            return nullptr;
//        case ORB_SLAM2::System::RGBD:
//            log( "RGBD regim unsupported yet.Try monocular regim." );
//            return nullptr;
//        default:
//            log( "unknown regim.Try monocular regim." );
//            return nullptr;
//    }
//    if ( cvMatProvider )
//        cvMatProvider->init( _assetsPath );
//    return cvMatProvider;
//}
