//
// Created by geoffrey on 30/12/15.
//

#ifndef RANSAC_RANSAC_H
#define RANSAC_RANSAC_H

#include <vector>
#include <random>
#include <opencv2/core.hpp>
#include "model.h"

//TODO: add OpenCV lib use to process images and points (for the modelling)

template<class dataType>
class RANSAC {
private:
    std::vector<dataType> *data; //pointer to the data we want to get a model for
    int size; // data size
    double probability; //probability for drawing an outlier free subset
    int minS; //minimal number of data element to correctly estimate the model
    double threshold; //Threshold which defines if a data element, di, agrees with model M.
    std::vector<dataType> *inliers;
    Model model;

public:
    RANSAC<dataType>(std::vector<dataType> *data, double probability, int minS, double threshold) :
            data(data), probability(probability), minS(minS) {
        this->data = data;
        size = data->size();
        estimateModel(); //we update the object's model using this
    }

//    TODO: implement this : look at the distance between dataPiece
//    TODO: and the model, and validate if its under the threshold
    static bool agree(const Model model, const dataType dataPiece, double threshold) {

    }

    // main function of the algorithm
    // TODO: finish implementation of this
    void estimateModel() {
        std::vector<dataType> maximalConsensusSet;
        double N = binomial(minS, size);
        // iterate N times :
        for (int i = 0; i < N; i++) {
            std::vector<dataType> consensusSet;
            std::vector<dataType> sample = getRandomSample(*data, minS);
//          TODO: implement this
//          get the model from the random sample
            Model curModel = estimateModelFromSample(consensusSet);
            for (int i = 0; i < size; i++) {
                if (agree(curModel, data[i], threshold)) {
                    consensusSet.push_back(data[i]);
                }
            }
            if (consensusSet.size() > maximalConsensusSet.size()) {
                maximalConsensusSet = consensusSet;
//              update N
                N = log(1 - probability) / log(1 - pow(consensusSet.size() / (double) size, minS));
            }
        }
        inliers = &maximalConsensusSet;
        model = estimateModelFromSample(maximalConsensusSet);
    }

    //given a good sample, this function estimates a model, [both linear and circular, then chooses the best]
    // TODO: Implement the Model class (and use inheritance for circle/linear models ?)
    // TODO: (circle and linear). Test this separately also.
    Model estimateModelFromSample(const std::vector<dataType> &consensusDataSample) const {
        Model result;
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
    static std::vector<dataType> getRandomSample(const std::vector<dataType> &data, int sampleSize) {
        std::vector<dataType> result;
        if (data.empty() || sampleSize < 1) return result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(0, data.size() - 1);
        for (int i = 0; i < sampleSize; i++) {
            result.push_back(data[distribution(gen)]);
        }
        return result;
    }
};


#endif //RANSAC_RANSAC_H
