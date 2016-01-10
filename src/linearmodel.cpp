//
// Created by geoffrey on 31/12/15.
//

#include "opencv2/core.hpp"
#include "linearmodel.h"
#include <iostream>

// Ordinary least squares regression estimate on a Point2f array of size nSample
LinearModel::LinearModel(std::vector<Point2f> dataSample, int nSample) {
    float nSamplef = (float) nSample;
    float var = 0.f;
    float meanX = 0.f;
    float meanY = 0.f;
    float covar = 0.f;
    float prodX = 0.f;
    float prodY = 0.f;


    for (int i = 0; i < nSamplef; i++) {
        var += pow((dataSample[i]).x, 2);
        meanX += (dataSample[i]).x;
        meanY += (dataSample[i]).y;
        prodX *= (dataSample[i]).x;
        prodY *= (dataSample[i]).y;
        covar += (dataSample[i]).x * (dataSample[i]).y;
    }
    std::cout << std::endl << "Var: " << var << std::endl;
    std::cout << "meanX: " << meanX << std::endl;
    std::cout << "meanY: " << meanY << std::endl;
    std::cout << "covar: " << covar << std::endl;

    var = var - pow(meanX, 2) / nSamplef;
    covar = covar - meanX * meanY / nSamplef; //Correction pily


    float a = 0.f;
    float b = 1.f;
    float c = 0.f;

    a = covar / var; //Correction pily changement de signe
    c = -a * meanX/nSamplef + meanY/nSamplef;

    Point2f dir(a, b);
    direction = dir;
    distanceToOrigin = c;

}


double LinearModel::distance(const Point2f &point) const {
    return abs(direction.ddot(point) + distanceToOrigin) / norm(point);
}

bool LinearModel::agree(const Point2f &point, double threshold) const {
    return distance(point) < threshold;
}

Point2f LinearModel::getDirection() const {
    return direction;
}

double LinearModel::getDistanceToOrigin() const {
    return distanceToOrigin;
}

std::ostream &operator<<(std::ostream &ostream, const LinearModel &model) {
    ostream << "Direction: " << model.getDirection();
    ostream << ", Distance to origin: " << model.getDistanceToOrigin();
    return ostream;
}

LinearModel::LinearModel(Point2f direction, double distanceToOrigin) : direction(direction),
                                                                       distanceToOrigin(distanceToOrigin) { }
