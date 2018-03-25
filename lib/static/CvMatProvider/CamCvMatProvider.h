#ifndef CAMCVMATPROVIDER_H
#define CAMCVMATPROVIDER_H

#include <QObject>
#include <opencv2/opencv.hpp>

#include "ICvMatProvider.h"

class CamCvMatProvider : public QObject, public ICvMatProvider {
    Q_OBJECT
public:
    explicit CamCvMatProvider( QObject *parent = nullptr );

    void    init( const QString &configPath )   override;
    void    start( )                            override;
    void    stop( )                             override;
    cv::Mat cvMat( )                            override;
private:
    cv::VideoCapture _cap;
};

#endif // CAMCVMATPROVIDER_H
