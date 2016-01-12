//
// Created by geoffrey on 30/12/15.
//

#ifndef RANSAC_RANSAC_H
#define RANSAC_RANSAC_H

#include <vector>
#include <random>
#include <opencv2/core.hpp>
#include "linearmodel.h"
#include <iostream>

//TODO: add OpenCV lib use to process images and points (for the modelling)

class RANSAC {
private:
    std::vector<Point2f> data; //vector of data
    int size; // data size
    double probability; //probability for drawing an outlier free subset
    int minS; //minimal number of data element to correctly estimate the model
    double threshold; //Threshold which defines if a data element, di, agrees with model M.
    std::vector<Point2f> inliers;
    std::vector<Point2f> outliers;
    //LinearModel model;

public:
    RANSAC(std::vector<Point2f> data, int dataSize, double probability, int minS, double threshold) :
            data(data), probability(probability), minS(minS), size(dataSize)/*, model(estimateModel())*/ {
        this->data = data;
    }

    std::vector<Point2f> getInliers() {
        return inliers;
    }

    std::vector<Point2f> getOutliers() {
        return outliers;
    }


    // main function of the algorithm
    // TODO: test this
    LinearModel estimateLinearModel() {
        std::vector<Point2f> maximalConsensusSet;
        int maximalConsensusSize = 0;
        std::cout << "datesize is " << size << std::endl;
        double N = binomial(size, minS);
        // iterate N times :
        std::cout << "N is" << N << std::endl;
        for (int i = 0; i < N; i++) {
            int consensusSize = 0;
            std::vector<Point2f> consensusSet;
            std::vector<Point2f> curOutliers;
            std::vector<Point2f> sample;
            getRandomSample(data, size, sample, minS);
//          get the model from the random sample
            LinearModel curModel(sample, minS);
            std::cout << curModel.getDistanceToOrigin() << std::endl;
            for (int j = 0; j < size; j++) {
                std::cout << data[j].x << " " << data[j].y << std::endl;
                std::cout << curModel.agree(data[j], threshold) << std::endl;
                if (curModel.agree(data[j], threshold)) {
                    consensusSet.push_back(data[j]);
                    consensusSize++;
                } else {
                    curOutliers.push_back(data[j]);
                }
            }
            if (consensusSize > maximalConsensusSize) {
                maximalConsensusSet = consensusSet;
                maximalConsensusSize = consensusSize;
                outliers = curOutliers;
//              update N
                N = log(1 - probability) / log(1 - pow(consensusSize / (double) size, minS));
            }
        }
        inliers = maximalConsensusSet;

        std::cout << "hello" << std::endl;
        LinearModel result(maximalConsensusSet, maximalConsensusSize);
        return result;

    }

/*
    //return model
    LinearModel getModel(){
        return model;
    }
*/

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

    // sampling with remplacement.
    //TODO: sample with removal and not replacement
    static std::vector<Point2f> &getRandomSample(std::vector<Point2f> data, int dataSize, std::vector<Point2f> &sample,
                                                 int sampleSize) {
        std::vector<Point2f> result;
        if (data.empty() || sampleSize < 1) return sample;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(0, dataSize - 1);
        for (int i = 0; i < sampleSize; i++) {
            result.push_back(data[distribution(gen)]);
        }
        sample = result;
        return sample;
    }
};


#endif //RANSAC_RANSAC_H
