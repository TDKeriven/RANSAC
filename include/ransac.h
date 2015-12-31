//
// Created by geoffrey on 30/12/15.
//

#ifndef RANSAC_RANSAC_H
#define RANSAC_RANSAC_H

#include <vector>
#include <random>
//TODO: add OpenCV lib use to process images and points (for the modelling)

template<typename dataType>
class RANSAC {
private:
    std::vector<dataType> *data; //pointer to the data we want to get a model for
    int size; // data size
    double probability; //probability for drawing an outlier free subset
    int minS; //minimal number of data element to correctly estimate the model
    double threshold; //Threshold which defines if a data element, di, agrees with model M.

public:
    RANSAC<dataType>(std::vector<dataType> *data, double probability, int minS, double threshold) :
            data(data), probability(probability), minS(minS) {
        this->data = data;
        size = data->size();
        estimateModel(); //we estimate the model on constructions
    }

    // main function of the algorithm
    // TODO: implement this
    void estimateModel() {
        std::vector<dataType> consensusSet;
        double N = binomial(minS, size);

        for (int i = 0; i < N; i++) {

        }

    }

    //given a good sample, this function estimates a model, [both linear and circular, then chooses the best]
    // TODO: choose data structure for the model. The model is represented by a Matrix in both cases
    void estimateModelFromSample(dataType *maxConsensusDataSample) { }

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
    static std::vector<dataType> getRandomSample(std::vector<dataType> &data, int sampleSize) {
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
