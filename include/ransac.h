//
// Created by geoffrey on 30/12/15.
//

#ifndef RANSAC_RANSAC_H
#define RANSAC_RANSAC_H

#include <vector>
#include <random>
#include <opencv2/core.hpp>
#include "linearmodel.h"
#include "homography.h"
#include <iostream>
#include "utils.h"

//TODO: add OpenCV lib use to process images and points (for the modelling)


template<class Model>
class Ransac {
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
    Ransac(std::vector<Point2f> data, int dataSize, double probability, int minS, double threshold, int nbit) :
            data(data), probability(probability), minS(Model::nSample), size(dataSize), nbit(nbit) {
        this->data = data;
    }

    std::vector<Point2f> getInliers() {
        return inliers;
    }

    std::vector<Point2f> getOutliers() {
        return outliers;
    }


    // main function of the algorithm
    Model estimateModel() {
        std::vector<Point2f> maximalConsensusSet;
        int maximalConsensusSize = 0;
        std::cout << "datesize is " << size << std::endl;

        for (int i = 0; i < nbit; i++) {
            int consensusSize = 0;
            std::vector<Point2f> consensusSet;
            std::vector<Point2f> curOutliers;
            std::vector<Point2f> sample;
//            Choose a random sample of the right size, depending on the model
            getRandomSample(data, size, sample, minS);
//          get the model from the random sample
            Model curModel(sample);
            for (int j = 0; j < size; j++) {
                if (curModel.agree(data[j], threshold)) {
                    consensusSet.push_back(data[j]);
                    consensusSize++;
                } else {
                    curOutliers.push_back(data[j]);
                }
            }
            for (int i = 0; i < consensusSet.size(); i++)
            std::cout << consensusSet[consensusSize - 1];
            if (consensusSize > maximalConsensusSize) {
                maximalConsensusSet = consensusSet;
                maximalConsensusSize = consensusSize;
                outliers = curOutliers;
            }

        }
        inliers = maximalConsensusSet;

        Model result(maximalConsensusSet, maximalConsensusSize);
        return result;
    }


    /*
    static double factorial(double nValue) {
        if (nValue == 0) return 1;
        return nValue * factorial(nValue - 1);
    }

    static double binomial(double n, double k) {
        double result;
        if (k == 1)return n;
        result = (factorial(n)) / (factorial(k) * factorial((n - k)));
        return result;
    }
*/

};


#endif //RANSAC_RANSAC_H
