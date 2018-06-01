#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

#include <QtCore>
#include <QDebug>

using namespace std;
using namespace cv;

vector< vector< Point3f > > objectPoints;
vector< vector< Point2f > > imagePoints;
vector< Point2f > corners;
//vector< vector< Point2f > > left_img_points;

Mat img, gray;
Size im_size;

void setupCalibration( int boardWidth, int boardHeight, int imgsCount,
                       float squareSize, const char* imgsDirectory, const char* imgsFilename,
                       const char* extension )
{
    Size boardSize = Size( boardWidth, boardHeight );
//    int board_n = boardWidth * boardHeight;

    for ( int k = 1; k <= imgsCount; k++ ) {
        char img_file[ 100 ];
        sprintf( img_file, "%s%s%d.%s", imgsDirectory, imgsFilename, k, extension );
        cout << img_file << endl;
        img = imread( img_file, CV_LOAD_IMAGE_COLOR );
        if ( img.empty( ) ) {
            qDebug( ) << "is empty!!!!";
            return;
        }
        cv::cvtColor( img, gray, CV_BGR2GRAY );

        bool found = false;
        found = cv::findChessboardCorners( img, boardSize, corners,
                                          CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );
        if ( found ) {
            cornerSubPix( gray, corners, cv::Size( 5, 5 ), cv::Size( -1, -1 ),
                       TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1 ) );
            drawChessboardCorners( gray, boardSize, corners, found );
        }

        vector< Point3f > obj;
        for ( int i = 0; i < boardHeight; i++ )
          for ( int j = 0; j < boardWidth; j++ )
            obj.push_back( Point3f( ( float )j * squareSize, ( float )i * squareSize, 0 ) );

        if ( found ) {
          cout << k << ". Found corners!" << endl;
          imagePoints.push_back( corners );
          objectPoints.push_back( obj );
        }
    }
}

double computeReprojectionErrors(const vector< vector< Point3f > >& objectPoints,
                                 const vector< vector< Point2f > >& imagePoints,
                                 const vector< Mat >& rvecs, const vector< Mat >& tvecs,
                                 const Mat& cameraMatrix , const Mat& distCoeffs)
{
        vector< Point2f > imagePoints2;
        int i, totalPoints = 0;
        double totalErr = 0, err;
        vector< float > perViewErrors;
        perViewErrors.resize( objectPoints.size( ) );

        for ( i = 0; i < ( int )objectPoints.size( ); ++i ) {
        projectPoints( Mat( objectPoints[ i ]), rvecs[ i ], tvecs[ i ], cameraMatrix,
                      distCoeffs, imagePoints2 );
        err = norm( Mat( imagePoints[ i ] ), Mat( imagePoints2 ), CV_L2 );
        int n = ( int )objectPoints[ i ].size( );
        perViewErrors[ i ] = ( float ) std::sqrt( err*err / n );
        totalErr += err * err;
        totalPoints += n;
    }
    return std::sqrt( totalErr / totalPoints );
}

//--outputFile=lIntrinsic.yaml --pre=image_left_  --imgsCount=35
//--outputFile=rIntrinsic.yaml --pre=image_right_  --imgsCount=35

int main( int argc, char *argv[ ] ) {
    const String keys =
    "{hBoard|6|Height of the board}"
    "{wBoard|9|Width of the board}"
    "{imgFolder|./calibrationImgs/|Directory of images}"
    "{pre|image_left_|Left image name prefix. Ex: image_left_}"
    "{imgsCount|10|count of img}"
    "{squareSize|0.02423|count of img}"
    "{outputFile|intrinsic.yaml|intrinsic camera calibration params}"
    "{imgExt|jpg|Image extension. Ext: jpg,png etc}";

    CommandLineParser parser( argc, argv, keys );

    String imgFolder    = parser.get<String>( "imgFolder" );
    String imgExt       = parser.get<String>( "imgExt" );
    String pre          = parser.get<String>( "pre" );
    int imgsCount       = parser.get<int>   ( "imgsCount" );
    float squareSize    = parser.get<float> ( "squareSize" );
    string outputFile   = parser.get<String>( "outputFile" );

    Size boardSize( parser.get<int>( "wBoard" ), parser.get<int>( "hBoard" ) );
    cout << boardSize << endl;
    setupCalibration( boardSize.width, boardSize.height, imgsCount, squareSize,
                   imgFolder.c_str( ), pre.c_str( ), imgExt.c_str( ) );

    cout << ( "Starting Calibration" ) << endl;

    Mat K;
    Mat D;
    vector< Mat > rvecs, tvecs;
    int flag = 0;
    flag |= CV_CALIB_FIX_K4;
    flag |= CV_CALIB_FIX_K5;
    calibrateCamera( objectPoints, imagePoints, img.size( ), K, D, rvecs, tvecs, flag );

    cout << "Calibration error: " << computeReprojectionErrors( objectPoints, imagePoints, rvecs, tvecs, K, D ) << endl;

    FileStorage fs( outputFile, FileStorage::WRITE );
    fs << "K" << K;
    fs << "D" << D;
    fs << "board_width" << boardSize.width;
    fs << "board_height" << boardSize.height;
    fs << "square_size" << squareSize;

    cout << "Done Calibration" << endl;

    return 0;
}
