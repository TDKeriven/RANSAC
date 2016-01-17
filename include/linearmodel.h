//
// Created by geoffrey on 31/12/15.
//

#ifndef RANSAC_LINEAR_MODEL_H
#define RANSAC_LINEAR_MODEL_H

#include <opencv2/core.hpp>
#include <iostream>
#include <utility>

using namespace cv;
using namespace std;

//We set ourselves in 2 dimensions
class LinearModel {
private:
    Point2f normale;
    double distanceToOrigin;


public:
    const static int nSample = 2;

    //return a LinearModel from 2 points
    LinearModel(std::vector<Point2f> dataSample);

    //return a linearRegression using the least squares cost function
    LinearModel(std::vector<Point2f> dataSample, int size);

    LinearModel(Point2f normale, double distanceToOrigin);

    virtual double distance(const Point2f &point) const;

    virtual bool agree(const Point2f &point, double threshold) const;

    Point2f getNormale() const;

    double getDistanceToOrigin() const;

};

std::ostream &operator<<(std::ostream &ostream, const LinearModel &model);

#endif //RANSAC_LINEAR_MODEL_H
