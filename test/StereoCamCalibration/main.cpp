#include <QCoreApplication>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <QDebug>

using namespace std;
using namespace cv;

vector< vector< Point3f > > objectPoints;
vector< vector< Point2f > > imagePoints1, imagePoints2;
vector< Point2f > corners1, corners2;
vector< vector< Point2f > > leftImgPoints, rightImgPoints;

Mat img1, img2, gray1, gray2;

void loadImagePoints( int boardWidth, int boardHeight, int imgsCount, float squareSize,
                      const char* leftimgDir, const char* rightimgDir,
                      const char* leftImgFilename, const char* rightImgFilename, const char* ext )
{

    Size boardSize = Size( boardWidth, boardHeight );
    //  int board_n = boardWidth * boardHeight;
    for ( int i = 1; i <= imgsCount; i++ ) {
        //char leftImg[ 100 ], rightImg[ 100 ];
        QString leftImg{ leftimgDir };// + leftImgFilename + "." + ext };
        leftImg.append( leftImgFilename );
        leftImg.append( QString::number( i ) );
        leftImg.append( "." );
        leftImg.append( ext );
        QString rightImg{ rightimgDir };//+ rightImgFilename + "." + ext };
        rightImg.append( rightImgFilename );
        rightImg.append( QString::number( i ) );
        rightImg.append( "." );
        rightImg.append( ext );
        //sprintf( leftImg, "%s%s%d." + ext, leftimgDir, leftImgFilename, i );
        //sprintf( rightImg, "%s%s%d." + ext, rightimgDir, rightImgFilename, i );
        cout << "imread:" << leftImg.toStdString( ) << endl;
        img1 = imread( leftImg.toStdString( ), CV_LOAD_IMAGE_COLOR );
        img2 = imread( rightImg.toStdString( ), CV_LOAD_IMAGE_COLOR );
        cvtColor( img1, gray1, CV_BGR2GRAY );
        cvtColor( img2, gray2, CV_BGR2GRAY );

        bool found1 = false, found2 = false;
        cout << "end imread" << endl;
        found1 = cv::findChessboardCorners( img1, boardSize, corners1,
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        found2 = cv::findChessboardCorners( img2, boardSize, corners2,
            CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        if ( found1 ) {
            cv::cornerSubPix( gray1, corners1, cv::Size( 5, 5 ), cv::Size( -1, -1 ),
            cv::TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1 ) );
            cv::drawChessboardCorners( gray1, boardSize, corners1, found1 );
        }
        if ( found2 ) {
            cv::cornerSubPix( gray2, corners2, cv::Size( 5, 5 ), cv::Size( -1, -1 ),
            cv::TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1 ) );
            cv::drawChessboardCorners( gray2, boardSize, corners2, found2 );
        }
        vector< Point3f > obj;
        for ( int i = 0; i < boardHeight; i++ )
            for ( int j = 0; j < boardWidth; j++ )
                obj.push_back( Point3f( ( float )j * squareSize, ( float )i * squareSize, 0 ) );

        if ( found1 && found2 ) {
            cout << i << ". Found corners!" << endl;
            imagePoints1.push_back( corners1 );
            imagePoints2.push_back( corners2 );
            objectPoints.push_back( obj );
        }
    }
    for ( int i = 0; i < imagePoints1.size( ); i++ ) {
        vector< Point2f > v1, v2;
        for ( int j = 0; j < imagePoints1[ i ].size( ); j++ ) {
            v1.push_back( Point2f( ( double )imagePoints1[ i ][ j ].x, ( double )imagePoints1[ i ][ j ].y ) );
            v2.push_back( Point2f( ( double )imagePoints2[ i ][ j ].x, ( double )imagePoints2[ i ][ j ].y ) );
        }
        leftImgPoints.push_back( v1 );
        rightImgPoints.push_back( v2 );
    }
}

int main( int argc, char *argv[ ] ) {
    QCoreApplication a( argc, argv );

    const String keys =
    "{imgLFolder|./calibrationImgs/|Directory of left cam images}"
    "{imgRFolder|./calibrationImgs/|Directory of right cam images}"
    "{fLCalib|./lIntrinsic.yaml|file left cam calib}"
    "{fRCalib|./rIntrinsic.yaml|file right cam calib}"
    "{preL|image_left_|Left image name prefix}"
    "{preR|image_right_|Right image name prefix}"
    "{imgsCount|10|count of img}"
    "{outputFile|extrinsics.yaml|extrinsics stereo camera calibration params}"
    "{imgExt|jpg|Image extension. Ext: jpg,png etc}";

    CommandLineParser parser( argc, argv, keys );

    String imgLFolder    = parser.get<String>( "imgLFolder" );
    String imgRFolder    = parser.get<String>( "imgRFolder" );
    String imgExt        = parser.get<String>( "imgExt" );
    String preL          = parser.get<String>( "preL" );
    String preR          = parser.get<String>( "preR" );
    int imgsCount        = parser.get<int>   ( "imgsCount" );
    String outputFile    = parser.get<String>( "outputFile" );
    String fLCalib       = parser.get<String>( "fLCalib" );
    String fRCalib       = parser.get<String>( "fRCalib" );

    FileStorage fsl( fLCalib, FileStorage::READ );
    FileStorage fsr( fRCalib, FileStorage::READ );
    cout << "start laod image points" << endl;
    loadImagePoints( fsl[ "board_width" ], fsl[ "board_height" ], imgsCount, fsl[ "square_size" ],
                   imgLFolder.c_str( ), imgRFolder.c_str( ), preL.c_str( ), preR.c_str( ), imgExt.c_str( ) );

    cout << "Starting Calibration" << endl;
    Mat K1, K2, R, F, E;
    Vec3d T;
    Mat D1, D2;
    fsl[ "K" ] >> K1;
    fsr[ "K" ] >> K2;
    fsl[ "D" ] >> D1;
    fsr[ "D" ] >> D2;
    int flag = 0;
    flag |= CV_CALIB_FIX_INTRINSIC;

    cout << "Read intrinsics" << endl;

    double rms = stereoCalibrate( objectPoints, leftImgPoints, rightImgPoints, K1, D1, K2, D2, img1.size( ), R, T, E, F );
    cout << "rms: " << rms << endl;

    cv::FileStorage fs1( outputFile, cv::FileStorage::WRITE );
    fs1 << "K1" << K1;
    fs1 << "K2" << K2;
    fs1 << "D1" << D1;
    fs1 << "D2" << D2;
    fs1 << "R" << R;
    fs1 << "T" << T;
    fs1 << "E" << E;
    fs1 << "F" << F;

    cout << "Done Calibration" << endl;

    cout << "Starting Rectification" <<  endl;

    cv::Mat R1, R2, P1, P2, Q;
    //flag = CV_CALIB_ZERO_DISPARITY;
    flag = 0; double alpha = -1;
    Rect validROI[2];
    stereoRectify( K1, D1, K2, D2, img1.size( ), R, T, R1, R2, P1, P2, Q, flag, alpha, img1.size( ), &validROI[ 0 ], &validROI[ 1 ] );

    cout << "roi1: " << validROI[ 0 ] << " roi2: " << validROI[ 1 ] << endl;

    fs1 << "R1" << R1;
    fs1 << "R2" << R2;
    fs1 << "P1" << P1;
    fs1 << "P2" << P2;
    fs1 << "Q" << Q;
    fs1 << "vRoi0" << validROI[ 0 ];
    fs1 << "vRoi1" << validROI[ 1 ];
    fs1.release( );
    cout << "Done Rectification" << endl;

    return a.exec( );
}
