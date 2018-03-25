#ifndef ICVMATPROVIDER_H
#define ICVMATPROVIDER_H

#include <QObject>
#include "opencv2/opencv.hpp"
#include "VideoReceiver.h"

class ICvMatProvider {
public:
    virtual ~ICvMatProvider( );
    virtual void init( const QString &configPath )  = 0;
    virtual void start( )                           = 0;
    virtual void stop( )                            = 0;
    virtual cv::Mat cvMat( );
protected:
    cv::Mat _mat;
};

#endif // ICVMATPROVIDER_H
