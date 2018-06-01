#ifndef ORBSLAMTRACKER_H
#define ORBSLAMTRACKER_H

#include "orbslamtracker_global.h"
#include <QThread>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <QVector>
#include "System.h"

class ORBSLAMTRACKERSHARED_EXPORT OrbSlamTracker : public QThread {
    Q_OBJECT
public:
    OrbSlamTracker( const QString &assetsPath, int regim, bool showViewer = false );
    /**
     * @brief использование в качестве поставщика данных
     * локальную камеру(true) или udp видеопоток(false)
     * @param value true - локальная камера; false - udp видеопоток
     */
    void localDevice( bool value );
    /**
     * @brief track
     * @param value
     */
    void track( bool value );

    QVector<float> &position( );
    QVector<float> &rotation( );

    bool inProgress( ) const;

    void ( *logMessage )( const char *value );
protected:
    void run( ) override;
private:
    bool                        _inProgress;
    bool                        _showViewer;
    bool                        _localDevice;

    QVector<float>              _rotation;
    QVector<float>              _position;

    double tframe;
    mutable QMutex              _mutex;
    QString                     _assetsPath;
    ORB_SLAM2::System::eSensor  _regim;

    void log( const QString &mess );
};

#endif // ORBSLAMTRACKER_H
