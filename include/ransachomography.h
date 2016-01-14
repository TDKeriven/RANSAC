//
// Created by pily on 14/01/16.
//

#ifndef RANSAC_RANSACHOMOGRAPHY_H
#define RANSAC_RANSACHOMOGRAPHY_H

#include <vector>
#include <random>
#include <opencv2/core.hpp>
#include "homography.h"
#include <iostream>


class ransachomography {
private:
    std::vector<Point2f> data; //vector of data
    int size; // data size
    double probability; //probability for drawing an outlier free subset
    int minS; //minimal number of data element to correctly estimate the model
    double threshold; //Threshold which defines if a data element, di, agrees with model M.
    int nbit; //nombre d'itérations nécessaire pour aboutir a un bon modèle (empirique)
    std::vector<Point2f> inliers;
    std::vector<Point2f> outliers;

public:
    RANSACH(std::vector<Point2f> data, int dataSize, double probability, int minS, double threshold, double nbit) :
            data(data), probability(probability), minS(minS), size(dataSize),nbit(nbit) {
        this->data = data;
    }
};
#endif //RANSAC_RANSACHOMOGRAPHY_H
