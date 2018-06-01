#include "StereoCamCalibrator.h"

StereoCamCalibrator::StereoCamCalibrator( cv::Size frameSize, const QString &dirPath, QObject *parent ) :
    INTRINSICS_FILE{ "intrinsics.yml" },
    EXTRINSICS_FILE{ "extrinsics.yml" },
    _validRoiIndex{ 1 },
    QObject( parent )
{
    cv::Mat cameraMatrix[ 2 ], distCoeffs[ 2 ];
    cameraMatrix[ 0 ] = cv::Mat::eye( 3, 3, CV_64F );
    cameraMatrix[ 1 ] = cv::Mat::eye( 3, 3, CV_64F );

    cv::Mat R, T;

    cv::Mat R1, R2, P1, P2, Q;

    cv::FileStorage fsInt( QString( dirPath + INTRINSICS_FILE ).toStdString( ), cv::FileStorage::READ );
    if ( fsInt.isOpened( ) ) {
        fsInt[ "M1" ] >> cameraMatrix  [ 0 ];
        fsInt[ "D1" ] >> distCoeffs    [ 0 ];
        fsInt[ "M2" ] >> cameraMatrix  [ 1 ];
        fsInt[ "D2" ] >> distCoeffs    [ 1 ];
        fsInt.release( );
    }
    else
        std::cout<<"Error: Could not open intrinsics file.";

//    std::cout << "M1 " << cameraMatrix[ 0 ] << std::endl;
//    std::cout << "M2 " << cameraMatrix[ 1 ] << std::endl;
//    std::cout << "D1 " << distCoeffs[ 0 ]   << std::endl;
//    std::cout << "D2 " << distCoeffs[ 1 ]   << std::endl;

    cv::FileStorage fsExt( "extrinsics.yml", cv::FileStorage::READ );
    if ( fsExt.isOpened( ) ) {
        fsExt[ "R" ]   >> R;
        fsExt[ "T" ]   >> T;
        fsExt[ "R1" ]  >> R1;
        fsExt[ "R2" ]  >> R2;
        fsExt[ "P1" ]  >> P1;
        fsExt[ "P2" ]  >> P2;
        fsExt[ "Q" ]   >> Q;
        fsExt.release( );
    }
    else
        std::cout << "Error: Could not open extrinsics file.";

//    std::cout << "R "  << R  << std::endl;
//    std::cout << "T "  << T  << std::endl;
//    std::cout << "R1 " << R1 << std::endl;
//    std::cout << "R2 " << R2 << std::endl;
//    std::cout << "P1 " << P1 << std::endl;
//    std::cout << "P2 " << P2 << std::endl;
//    std::cout << "Q "  << Q  << std::endl;

    cv::stereoRectify( cameraMatrix[ 0 ], distCoeffs[ 0 ], cameraMatrix[ 1 ], distCoeffs[ 1 ], frameSize, R, T, R1, R2, P1, P2, Q, cv::CALIB_ZERO_DISPARITY, 1, frameSize, &_validROI[ 0 ], &_validROI[ 1 ] );
    cv::initUndistortRectifyMap( cameraMatrix[ 0 ], distCoeffs[ 0 ], R1, P1, frameSize, CV_16SC2, _rmap[ 0 ][ 0 ], _rmap[ 0 ][ 1 ] );
    cv::initUndistortRectifyMap( cameraMatrix[ 1 ], distCoeffs[ 1 ], R2, P2, frameSize, CV_16SC2, _rmap[ 1 ][ 0 ], _rmap[ 1 ][ 1 ] );

    _sf = 600. / MAX( frameSize.width, frameSize.height );
    _w = cvRound( frameSize.width * _sf );
    _h = cvRound( frameSize.height * _sf );
}

void StereoCamCalibrator::calibrate( cv::Mat &left, cv::Mat &right ) {
    if ( left.empty( ) || right.empty( ) ) {
        std::cout << "some frame is empty" << std::endl;
        return;
    }
    cv::remap( left , left  , _rmap[ 0 ][ 0 ], _rmap[ 0 ][ 1 ], cv::INTER_LINEAR );
    cv::remap( right, right , _rmap[ 1 ][ 0 ], _rmap[ 1 ][ 1 ], cv::INTER_LINEAR );

    cv::Rect roi( cvRound( _validROI[ _validRoiIndex ].x * _sf ), cvRound( _validROI[ _validRoiIndex ].y * _sf ),
              cvRound( _validROI[ _validRoiIndex ].width * _sf ), cvRound( _validROI[ _validRoiIndex ].height * _sf ) );

    left  = left ( roi );
    right = right( roi );
}

void StereoCamCalibrator::setValidRoiIndex( int i ) {
    _validRoiIndex = i;
}
