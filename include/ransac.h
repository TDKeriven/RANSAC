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
    //LinearModel model;

public:
    RANSAC(std::vector<Point2f> data, int dataSize, double probability, int minS, double threshold) :
            data(data), probability(probability), minS(minS), size(dataSize)/*, model(estimateModel())*/ {
        this->data = data;
    }


    // main function of the algorithm
    // TODO: test this
    LinearModel estimateModel() {
        std::vector<Point2f> maximalConsensusSet;
        int maximalConsensusSize = 0;
        std::cout<<"datesize is "  << size <<std::endl;
        double N = binomial(size, minS);
        // iterate N times :
        std::cout<<"N is" << N << std::endl;
        std::cout<<N << std::endl;
        for (int i = 0; i < N; i++) {
            int consensusSize = 0;
            std::vector<Point2f> consensusSet;
            std::vector<Point2f> sample;
            getRandomSample(data, size, sample, minS);
//          get the model from the random sample
            LinearModel curModel(sample, minS);
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
/*
    //return model
    LinearModel getModel(){
        return model;
    }
*/

    double Factorial(double nValue)
    {
        double result = nValue;
        double result_next;
        double pc = nValue;
        do
        {
            result_next = result*(pc-1);
            result = result_next;
            pc--;
        }while(pc>2);
        nValue = result;
        return nValue;
    }

    double binomial(double nValue, double nValue2)
    {
        double result;
        if(nValue2 == 1)return nValue;
        result = (Factorial(nValue))/(Factorial(nValue2)*Factorial((nValue - nValue2)));
        nValue2 = result;
        return nValue2;
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
