#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "opencv2/opencv.hpp"
using namespace cv;

#include "StereoCamCalibrator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow( );
private:
    Ui::MainWindow *ui;
    void getCameraParams( );
    Mat rmap[ 2 ][ 2 ];
    Rect validROI[ 2 ];
    Size imageSize{ 640, 480 };
    VideoCapture _cap1;
    VideoCapture _cap2;
    QTimer       _tmrUpdate;

    Mat canvas;
    int w, h;
    double sf;

    StereoCamCalibrator _calibrator;
private slots:
    void getFrames( );
};

#endif // MAINWINDOW_H
