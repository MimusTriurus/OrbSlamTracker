#include "ICvMatProvider.h"

ICvMatProvider::~ICvMatProvider( ) {

}

cv::Mat ICvMatProvider::cvMat( ) {
    return _mat;
}
