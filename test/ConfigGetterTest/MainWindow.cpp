#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include "ConfigGetter.h"

#include <iostream>
#include <stdlib.h>

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    qDebug( ) << "1 == " << ConfigGetter::getParam<int>( "1", 0 );
    qDebug( ) << "2 == " << ConfigGetter::getParam<int>( "2", 0 );

    connect( &_tmrUpdate, SIGNAL( timeout( ) ), SLOT( getFrames( ) ) );
    _tmrUpdate.setInterval( 0.33 );

    //getCameraParams( );
    sf = 600. / MAX( 640, 480 );
    w = cvRound( 640 * sf );
    h = cvRound( 480 * sf );
    canvas.create( h, w * 2, CV_8UC3 );

    _cap1.open( 0 );
    _cap2.open( 1 );
    std::cout << "left opend " << _cap1.isOpened( ) << std::endl;
    std::cout << "right opend " << _cap2.isOpened( ) << std::endl;
    _tmrUpdate.start( );
}

MainWindow::~MainWindow( ) {
    delete ui;
}

void MainWindow::getCameraParams( ) {
    Mat cameraMatrix[ 2 ], distCoeffs[ 2 ];
    cameraMatrix[ 0 ] = Mat::eye( 3, 3, CV_64F );
    cameraMatrix[ 1 ] = Mat::eye( 3, 3, CV_64F );

    Mat R, T;

    Mat R1, R2, P1, P2, Q;

    FileStorage fs( "intrinsics.yml", FileStorage::READ );
    if ( fs.isOpened( ) ) {
        fs[ "M1" ] >> cameraMatrix  [ 0 ];
        fs[ "D1" ] >> distCoeffs    [ 0 ];
        fs[ "M2" ] >> cameraMatrix  [ 1 ];
        fs[ "D2" ] >> distCoeffs    [ 1 ];
        fs.release();
    }
    else
        std::cout<<"Error: Could not open intrinsics file.";

    std::cout << "M1 " << cameraMatrix[ 0 ] << std::endl;
    std::cout << "M2 " << cameraMatrix[ 1 ] << std::endl;
    std::cout << "D1 " << distCoeffs[ 0 ] << std::endl;
    std::cout << "D2 " << distCoeffs[ 1 ] << std::endl;

    FileStorage fsExt( "extrinsics.yml", FileStorage::READ );
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
        std::cout<<"Error: Could not open extrinsics file.";

    std::cout << "R " << R << std::endl;
    std::cout << "T " << T << std::endl;
    std::cout << "R1 " << R1 << std::endl;
    std::cout << "R2 " << R2 << std::endl;
    std::cout << "P1 " << P1 << std::endl;
    std::cout << "P2 " << P2 << std::endl;
    std::cout << "Q " << Q << std::endl;


    stereoRectify( cameraMatrix[ 0 ], distCoeffs[ 0 ], cameraMatrix[ 1 ], distCoeffs[ 1 ], imageSize, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 1, imageSize, &validROI[ 0 ], &validROI[ 1 ] );
    initUndistortRectifyMap( cameraMatrix[ 0 ], distCoeffs[ 0 ], R1, P1, imageSize, CV_16SC2, rmap[ 0 ][ 0 ], rmap[ 0 ][ 1 ] );
    initUndistortRectifyMap( cameraMatrix[ 1 ], distCoeffs[ 1 ], R2, P2, imageSize, CV_16SC2, rmap[ 1 ][ 0 ], rmap[ 1 ][ 1 ] );

    sf = 600. / MAX( imageSize.width, imageSize.height );
    w = cvRound( imageSize.width * sf );
    h = cvRound( imageSize.height * sf );
    canvas.create( h, w * 2, CV_8UC3 );
}

void MainWindow::getFrames( ) {
    Mat _left;
    Mat _right;

    _cap1.read( _left );
    _cap2.read( _right );

    _calibrator.calibrate( _left, _right );

    Mat canvasPartLeft = canvas( Rect( 0, 0, w, h ) );
    cv::resize( _left, canvasPartLeft, canvasPartLeft.size( ), 0, 0, INTER_AREA );
    Mat canvasPartRight = canvas( Rect( w, 0, w, h ) );
    cv::resize( _right, canvasPartRight, canvasPartRight.size( ), 0, 0, INTER_AREA );

    for( int j = 0; j < canvas.rows; j += 16 )
        line( canvas, Point( 0, j ), Point( canvas.cols, j ), Scalar( 0, 255, 0 ), 1, 8 );

    imshow( "new left", _left );
    imshow( "new right", _right );

    std::cout << "w:" << _left.size( ).width << " h:" << _right.size( ).height << std::endl;

    imshow( "rectified", canvas );
}
