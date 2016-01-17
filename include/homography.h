//
// Created by geoffrey on 13/01/16.
//

#ifndef RANSAC_HOMOGRAPHY_H
#define RANSAC_HOMOGRAPHY_H

#include <opencv2/core.hpp>
#include <vector>

using namespace std;
using namespace cv;


// this will replace LinearModel in the Ransachomography
class Homography {
private:
    Mat H;
    static const int nSample = 4;

public:
    //construct the Homography from a vector of Pairs of matching points (from image1 and 2)
    //using opencv's findHomography with leastSquares method
    Homography(const vector<pair<Point2f, Point2f> > matchPoints);

    virtual double distance(const Point2f &point1,const Point2f &point2) const;

    virtual bool agree(const Point2f &point1,const Point2f &point2, double threshold) const;

};

std::ostream &operator<<(std::ostream &ostream, const Homography &model);


#endif //RANSAC_HOMOGRAPHY_H
