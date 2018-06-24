#ifndef WINDOSSTEREOSGBM_H
#define WINDOSSTEREOSGBM_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QLayout>
#include <QPushButton>

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

#include "StereoCamCvMatProvider.h"

using namespace std;

class WindowStereoSGBM : public QWidget {
    Q_OBJECT
public:
    explicit WindowStereoSGBM( QWidget *parent = nullptr );
public slots:
private:
    QGridLayout _layout;

    QLabel      _lblPrefilterCap     { "PrefilterCap" };
    QLabel      _lblBlockSize        { "BlockSize" };
    QLabel      _lblMinDisparity     { "MinDisparity" };
    QLabel      _lblNumDisparities   { "NumDisparities" };
    QLabel      _lblUniquenessRatio  { "UniquenessRatio" };
    QLabel      _lblSpeckleWindowSize{ "SpeckleWindowSize" };
    QLabel      _lblSpeckleRange     { "SpeckleRange" };
    QLabel      _lblDisp12MaxDiff    { "Disp12MaxDiff" };

    QPushButton _btnOpen             { "Open pair frame" };
    QPushButton _btnOpenCam          { "Open stereo camera" };

    QSlider _sldrPrefilterCap       { Qt::Horizontal };
    QSlider _sldrBlockSize          { Qt::Horizontal };
    QSlider _sldrMinDisparity       { Qt::Horizontal };
    QSlider _sldrNumDisparities     { Qt::Horizontal };
    QSlider _sldrUniquenessRatio    { Qt::Horizontal };
    QSlider _sldrSpeckleWindowSize  { Qt::Horizontal };
    QSlider _sldrSpeckleRange       { Qt::Horizontal };
    QSlider _sldrDisp12MaxDiff      { Qt::Horizontal };

    cv::Ptr<cv::StereoSGBM> _sgbm;

    cv::Mat _leftFrame;
    cv::Mat _rightFrame;

    cv::Mat _map;
    cv::Mat _map8;

    QTimer _tmrGetFrames;
    StereoCamCvMatProvider _stereoCam;

    void initInterface( );
    void setBaseValues( );
    void subscribe( );
    void compute( );
private slots:
    void onChangePrefilterCap       ( int i );
    void onChangeBlockSize          ( int i );
    void onChangeMinDisparity       ( int i );
    void onChangeNumDisparities     ( int i );
    void onChangeUniquenessRatio    ( int i );
    void onChangeSpeckleWindowSize  ( int i );
    void onChangeSpeckleRange       ( int i );
    void onChangeDisp12MaxDiff      ( int i );

    void onOpenFrame( );
    void onOpenStereoCam( );
    void update( );
};

#endif // WINDOSSTEREOSGBM_H
