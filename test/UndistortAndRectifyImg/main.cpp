#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

#include <QtCore>
#include <QDebug>

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
    "{calibFile|./extrinsics.yaml|1}"
    "{lImgOutFName|./rectL.jpg|1}"
    "{rImgOutFName|./rectR.jpg|1}";

    CommandLineParser parser( argc, argv, keys );

    String lImgFilename = parser.get<String>( "lImgFilename" );
    String rImgFilename = parser.get<String>( "rImgFilename" );
    String calibFile    = parser.get<String>( "calibFile" );
    String lImgOutFName = parser.get<String>( "lImgOutFName" );
    String rImgOutFName = parser.get<String>( "rImgOutFName" );

    Mat R1, R2, P1, P2, Q;
    Mat K1, K2, R;
    Vec3d T;
    Mat D1, D2;
    Mat img1 = imread( lImgFilename, CV_LOAD_IMAGE_COLOR );
    Mat img2 = imread( rImgFilename, CV_LOAD_IMAGE_COLOR );
    cout << img1.size << " " << img2.size;
    cv::FileStorage fs1( calibFile, cv::FileStorage::READ );
    fs1[ "K1" ] >> K1;
    fs1[ "K2" ] >> K2;
    fs1[ "D1" ] >> D1;
    fs1[ "D2" ] >> D2;
    fs1[ "R" ]  >> R;
    fs1[ "T" ]  >> T;

    fs1[ "R1" ] >> R1;
    fs1[ "R2" ] >> R2;
    fs1[ "P1" ] >> P1;
    fs1[ "P2" ] >> P2;
    fs1[ "Q" ]  >> Q;
    cout << "1";
    cv::Mat lmapx, lmapy, rmapx, rmapy;
    cv::Mat imgU1, imgU2;

    cv::initUndistortRectifyMap( K1, D1, R1, P1, img1.size( ), CV_32F, lmapx, lmapy );
    cv::initUndistortRectifyMap( K2, D2, R2, P2, img2.size( ), CV_32F, rmapx, rmapy );

    cv::remap( img1, imgU1, lmapx, lmapy, cv::INTER_LINEAR );
    cv::remap( img2, imgU2, rmapx, rmapy, cv::INTER_LINEAR );

    imwrite( lImgOutFName, imgU1 );
    imwrite( rImgOutFName, imgU2 );

    cout << "done";

    return a.exec( );
}
