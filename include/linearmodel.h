//
// Created by geoffrey on 31/12/15.
//

#ifndef RANSAC_LINEAR_MODEL_H
#define RANSAC_LINEAR_MODEL_H

#include <opencv2/core.hpp>

using namespace cv;

//We set ourselves in 2 dimensions
class LinearModel {
private:
    Point2f normale;
    double distanceToOrigin;

public:
    LinearModel(std::vector<Point2f> dataSample, int nSample);

    LinearModel(Point2f normale, double distanceToOrigin);

    virtual double distance(const Point2f &point) const;

    virtual bool agree(const Point2f &point, double threshold) const;

    Point2f getNormale() const;

    double getDistanceToOrigin() const;

};

std::ostream &operator<<(std::ostream &ostream, const LinearModel &model);

#endif //RANSAC_LINEAR_MODEL_H
