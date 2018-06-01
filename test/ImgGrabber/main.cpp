//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <iostream>

#include <QtCore>

using namespace std;
using namespace cv;

int x = 0;

int main( int argc, char *argv[ ] ) {
    const String keys =
    "{hBoard|6|Height of the board}"
    "{wBoard|9|Width of the board}"
    "{hImg|480|Height of the board}"
    "{wImg|640|Width of the board}"
    "{imgFolder|./calibrationImgs|Directory of images}"
    "{preL|image_left_|Left image name prefix. Ex: image_left_}"
    "{preR|image_right_|Right image name postfix. Ex: image_right_}"
    "{imgExt|jpg|Image extension. Ext: jpg,png etc}"
    "{cam1Ind|0|Camera 1 Index}"
    "{cam2Ind|1|Camera 2 Index}";

    CommandLineParser parser( argc, argv, keys );

    String imgFolder    = parser.get<String>( "imgFolder" );
    String imgExt       = parser.get<String>( "imgExt" );
    String preL         = parser.get<String>( "preL" );
    String preR         = parser.get<String>( "preR" );
    int imgWidth        = parser.get<int>   ( "wImg" );
    int imgHeight       = parser.get<int>   ( "hImg" );
    int cam1Ind         = parser.get<int>   ( "cam1Ind" );
    int cam2Ind         = parser.get<int>   ( "cam2Ind" );

    Size boardSize( parser.get<int>( "wBoard" ), parser.get<int>( "hBoard" ) );

    cout << "output imgs dir: " << imgFolder << endl;

    VideoCapture camera1( cam1Ind );
    VideoCapture camera2( cam2Ind );

    if ( !camera1.isOpened( ) || !camera2.isOpened( ) ) {
      cout << "error opening cameras: " << cam1Ind << " " << cam2Ind << endl;
      return -1;
    }

    cout << "press any key for save pair img";

    vector<cv::Point2f> cornersLeft, cornersRight;
    Mat img1, img2, lImg4Save, rImg4Save;
    while ( true ) {
        camera1 >> img1;
        camera2 >> img2;
        if ( img1.empty( ) || img2.empty( ) ) {
            waitKey( 30 );
            continue;
        }
        resize( img1, img1, Size( imgWidth, imgHeight ) );
        resize( img2, img2, Size( imgWidth, imgHeight ) );

        bool foundLeft = cv::findChessboardCorners( img1, boardSize , cornersLeft, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE );
        bool foundRight = cv::findChessboardCorners( img2, boardSize, cornersRight, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE );

        img1.copyTo( lImg4Save );
        img2.copyTo( rImg4Save );

        if ( foundLeft ) {
            cv::drawChessboardCorners( img1, boardSize, cornersLeft, foundLeft );
        }

        if ( foundRight ) {
            cv::drawChessboardCorners( img2, boardSize, cornersRight, foundRight );
        }

        imshow( "left" , img1 );
        imshow( "right", img2 );

        if ( waitKey( 30 ) > 0 ) {
            if ( foundLeft && foundRight ) {
                x++;
                std::ostringstream leftString, rightString;
                leftString  << imgFolder << "/" << preL << x << "." << imgExt;
                rightString << imgFolder << "/" << preR << x << "." << imgExt;
                imwrite( leftString.str( ).c_str( ) , lImg4Save );
                imwrite( rightString.str( ).c_str( ), rImg4Save );
                cout << "save " << x << " imgs pair" << endl;
            }
        }
    }
    return 0;
}
