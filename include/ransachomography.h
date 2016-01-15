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
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

class RANSACHOMOGRAPHY {
private:
    vector<Point2f> data; //vector of data
    int size; // data size
    double probability; //probability for drawing an outlier free subset
    int minS; //minimal number of data element to correctly estimate the model
    double threshold; //Threshold which defines if a data element, di, agrees with model M.
    int nbit; //nombre d'itérations nécessaire pour aboutir a un bon modèle (empirique)
    vector<Point2f> inliers;
    vector<Point2f> outliers;


public:
    RANSACHOMOGRAPHY(vector<Point2f> data, int dataSize, double probability, int minS, double threshold, double nbit) :
            data(data), probability(probability), minS(minS), size(dataSize),nbit(nbit) {
    }

};
#endif //RANSAC_RANSACHOMOGRAPHY_H
