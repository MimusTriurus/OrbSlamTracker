#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

#include <QtCore>
#include <QDebug>

#include "stereocamcalibrator.h"

using namespace std;
using namespace cv;

// --lImgFilename=E:/Projects/Qt/Builds/OrbSlam2/bin/calibrationImgs/image_left_1.jpg
// --rImgFilename=E:/Projects/Qt/Builds/OrbSlam2/bin/calibrationImgs/image_right_1.jpg
// --calibFile=E:/Projects/Qt/Builds/OrbSlam2/bin/extrinsics.yaml
// --lImgOutFName=E:/Projects/Qt/Builds/OrbSlam2/bin/rectL.jpg
// --rImgOutFName|E:/Projects/Qt/Builds/OrbSlam2/bin/rectR.jpg

int main( int argc, char *argv[ ] ) {
    QCoreApplication a( argc, argv );

    const String keys =
    "{lImgFilename|./calibrationImgs/image_left_1.jpg|1}"
    "{rImgFilename|./calibrationImgs/image_right_1.jpg|1}"
    "{calibFile|extrinsics.yaml|1}"
    "{calibFileDir|E:/Projects/Qt/Builds/OrbSlam2/bin/|1}"
    "{lImgOutFName|./rectL.jpg|1}"
    "{rImgOutFName|./rectR.jpg|1}";

    CommandLineParser parser( argc, argv, keys );

    String lImgFilename = parser.get<String>( "lImgFilename" );
    String rImgFilename = parser.get<String>( "rImgFilename" );
    String calibFile    = parser.get<String>( "calibFile" );
    String calibFileDir = parser.get<String>( "calibFileDir" );
    String lImgOutFName = parser.get<String>( "lImgOutFName" );
    String rImgOutFName = parser.get<String>( "rImgOutFName" );

    Mat img1 = imread( lImgFilename, 1 );
    Mat img2 = imread( rImgFilename, 1 );

    StereoCamCalibrator calib;
    calib.init( img1.size( ), calibFileDir.c_str( ) );
    calib.calibrate( img1, img2 );

    cv::Mat stereoMatEdited =  cv::Mat( img1.size( ).height, img1.size( ).width + img2.size( ).width, img1.type( ) );

    cv::Rect lRect = cv::Rect( 0, 0, img1.size( ).width, img1.size( ).height );
    cv::Rect rRect = cv::Rect( img1.size( ).width, 0, img2.size( ).width, img2.size( ).height );

    cv::Mat lF = stereoMatEdited( lRect );
    cv::Mat rF = stereoMatEdited( rRect );

    img1.copyTo( lF );
    img2.copyTo( rF );

    for( int j = 0; j < stereoMatEdited.rows; j += 16 )
        line( stereoMatEdited, Point( 0, j ), Point( stereoMatEdited.cols, j ), Scalar( 255, 0, 0 ), 1, 8 );

    cv::imshow( "stereoEdited", stereoMatEdited );

    // save rectifired images
    imwrite( lImgOutFName, img1 );
    imwrite( rImgOutFName, img2 );

    return a.exec( );
}
