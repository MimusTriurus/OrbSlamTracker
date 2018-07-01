#ifndef STEREOCAMCVMATPROVIDER_H
#define STEREOCAMCVMATPROVIDER_H

#include <QObject>
#include <opencv2/opencv.hpp>

#include "ICvMatProvider.h"
#include "StereoCamCalibrator.h"

class StereoCamCvMatProvider : public QObject, public ICvMatProvider {
    Q_OBJECT
public:
    explicit StereoCamCvMatProvider( QObject *parent = nullptr );

    void init( const QString &configPath )     override;
    void start( )                              override;
    void stop( )                               override;
    void read( cv::Mat &left, cv::Mat &right ) override;
private:
    cv::VideoCapture _left;
    cv::VideoCapture _right;
    int _leftCamIndx;
    int _rightCamIndx;
    int _frameWidth;
    int _frameHeight;
    StereoCamCalibrator _calibrator;
};

#endif // STEREOCAMCVMATPROVIDER_H
