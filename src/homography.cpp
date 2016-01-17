//
// Created by geoffrey on 13/01/16.
//

#include "homography.h"
#include <opencv2/core.hpp>


//Use 4 points tu evaluate the Homography
Homography::Homography(const vector<pair<Point2f, Point2f>> matchPoints) {
    H=0;
}
