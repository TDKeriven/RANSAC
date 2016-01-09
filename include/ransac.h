//
// Created by geoffrey on 30/12/15.
//

#ifndef RANSAC_RANSAC_H
#define RANSAC_RANSAC_H

#include <vector>
#include <random>
#include <opencv2/core.hpp>
#include "linearmodel.h"

//TODO: add OpenCV lib use to process images and points (for the modelling)

class RANSAC {
private:
    Point2f *data; //pointer to the data we want to get a model for
    int size; // data size
    double probability; //probability for drawing an outlier free subset
    int minS; //minimal number of data element to correctly estimate the model
    double threshold; //Threshold which defines if a data element, di, agrees with model M.
    Point2f *inliers;
    LinearModel model;

public:
    RANSAC(Point2f *data, int dataSize, double probability, int minS, double threshold) :
            data(data), probability(probability), minS(minS), size(dataSize), model(estimateModel()) {
        this->data = data;
    }


    // main function of the algorithm
    // TODO: finish implementation of this
    LinearModel estimateModel() {
        Point2f *maximalConsensusSet;
        int maximalConsensusSize = 0;
        double N = binomial(minS, size);
        // iterate N times :
        for (int i = 0; i < N; i++) {
            int consensusSize = 0;
            Point2f consensusSet[minS];
            Point2f *sample;
            getRandomSample(data, size, sample, minS);
//          TODO: implement this
//          get the model from the random sample
            LinearModel curModel(consensusSet, minS);
            for (int j = 0; j < size; j++) {
                if (curModel.agree(data[j], threshold)) {
                    consensusSet[j] = (data[j]);
                    consensusSize++;
                }
            }
            if (consensusSize > maximalConsensusSize) {
                maximalConsensusSet = consensusSet;
                maximalConsensusSize = consensusSize;
//              update N
                N = log(1 - probability) / log(1 - pow(consensusSize / (double) size, minS));
            }
        }
        inliers = maximalConsensusSet;
        LinearModel result(maximalConsensusSet, maximalConsensusSize);
        return result;
    }


    //simple efficent binomal coefficient function
    static double binomial(int k, int n) {
        double res = 0.;
        for (int i = 0; i < k; i++) {
            res *= ((double) n + 1 - i) / (double) i;
        }
        return res;
    }

    // sampling with remplacement.
    //TODO: sample with removal and not replacement
    static Point2f *getRandomSample(Point2f *data, int dataSize, Point2f *sample, int sampleSize) {
        Point2f result[sampleSize];
        if (data == 0 || sampleSize < 1) return sample;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(0, dataSize - 1);
        for (int i = 0; i < sampleSize; i++) {
            result[i] = data[distribution(gen)];
        }
        sample = result;
    }
};


#endif //RANSAC_RANSAC_H
