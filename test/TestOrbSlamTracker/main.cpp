#include <QCoreApplication>
#include "opencv2/opencv.hpp"
#include <QDebug>
#include <OrbSlamTracker.h>
#include <OrbSlamTrackerWrapper.h>
using namespace cv;

int main( int argc, char *argv[ ] ) {
    QCoreApplication a( argc, argv );

    VideoCapture cap( 0 ); // open the default camera
    if( !cap.isOpened( ) )  // check if we succeeded
        return -1;

//    OrbSlamTracker _tracker( qApp->applicationDirPath( ), 0, true );
//    _tracker.track( true );
    OrbSlamTrackerWrapper::init( qApp->applicationDirPath( ).toUtf8( ), 0, true );
    OrbSlamTrackerWrapper::track( true );

    //namedWindow("edges",1);
    for( ;; ) {
        Mat frame;
        cap >> frame; // get a new frame from camera
//        _tracker.update( frame );
//        OrbSlamTrackerWrapper::update( frame );
//        qDebug( ) << "*********";
//        qDebug( ) << "x:" << OrbSlamTrackerWrapper::posX( );
//        qDebug( ) << "y:" << OrbSlamTrackerWrapper::posY( );
//        qDebug( ) << "z:" << OrbSlamTrackerWrapper::posZ( );
//        qDebug( ) << "*********";
        //imshow( "edges", frame );
        if( waitKey( 30 ) >= 0 ) break;
    }

    return a.exec( );
}
