#include "ICvMatProvider.h"

ICvMatProvider::~ICvMatProvider( ) {

}

cv::Mat ICvMatProvider::cvMat( ) {
    return _mat;
}

void ICvMatProvider::read( cv::Mat &frame ) {
    Q_UNUSED( frame )
}

void ICvMatProvider::read( cv::Mat &left, cv::Mat &right ) {
    Q_UNUSED( left )
    Q_UNUSED( right )
}
