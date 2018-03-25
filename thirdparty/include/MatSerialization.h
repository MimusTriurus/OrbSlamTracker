#ifndef MATSERIALIZATION_H
#define MATSERIALIZATION_H

#include <QtCore>
#include "opencv2/opencv.hpp"

/**
 * @brief Сериализация и десереализаци cv::Mat
 */
class MatSerialization {
public:
    MatSerialization( ) = delete;

    static void serializeMat( const cv::Mat &mat, std::vector<uchar> &output, int quality = 70 );
    static cv::Mat deserializeMat( const QByteArray &matData, cv::ImreadModes mode = cv::IMREAD_GRAYSCALE );
};

#endif // MATSERIALIZATION_H
