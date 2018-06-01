#ifndef STEREOCAMCALIBRATOR_H
#define STEREOCAMCALIBRATOR_H

#include <QObject>
#include "opencv2/opencv.hpp"

class StereoCamCalibrator : public QObject {
    Q_OBJECT
public:
    explicit StereoCamCalibrator( cv::Size frameSize = cv::Size{ 640, 480 },
                                  const QString &dirPath = "",
                                  QObject *parent = nullptr );

    void calibrate( cv::Mat &left, cv::Mat &right );
    /**
     * @brief
     * @param i = 0 - left, i = 1 -right
     */
    void setValidRoiIndex( int i );
private:
    const QString INTRINSICS_FILE;
    const QString EXTRINSICS_FILE;

    cv::Mat     _rmap[ 2 ][ 2 ];
    cv::Rect    _validROI[ 2 ];
    int         _validRoiIndex;
    int         _w, _h;
    double      _sf;
};

#endif // STEREOCAMCALIBRATOR_H
