//
// Created by geoffrey on 30/12/15.
//

#ifndef RANSAC_RANSACHOMOGRAPHY_H
#define RANSAC_RANSACHOMOGRAPHY_H

#include <vector>
#include <random>
#include <opencv2/core.hpp>
#include "linearmodel.h"
#include "homography.h"
#include <iostream>
#include "utils.h"

//TODO: add OpenCV lib use to process images and points (for the modelling)


template<class Model>
class Ransachomography {
private:
    std::vector<Point2f> data1;
    std::vector<Point2f> data2;
    int size; // data size
    double probability; //probability for drawing an outlier free subset
    int minS; //minimal number of data element to correctly estimate the model
    double threshold; //Threshold which defines if a data element, di, agrees with model M.
    int nbit; //nombre d'itérations nécessaire pour aboutir a un bon modèle (empirique)
    std::vector<Point2f> inliers1;
    std::vector<Point2f> inliers2;
    std::vector<Point2f> outliers1;
    std::vector<Point2f> outliers2;

public:
    Ransachomography(std::vector<Point2f> data1,std::vector<Point2f> data2, int dataSize, double probability, int minS, double threshold, int nbit) :
            data1(data1),data2(data2), probability(probability), minS(4), size(dataSize), nbit(nbit) {
        this->data1 = data1;
    }

    std::vector<Point2f> getInliers1() {
        return inliers1;
    }
    std::vector<Point2f> getInliers2() {
        return inliers2;
    }

    std::vector<Point2f> getOutliers1() {
        return outliers1;
    }
    std::vector<Point2f> getOutliers2() {
        return outliers2;
    }

    // main function of the algorithm
    Model estimateModel() {
        std::vector<Point2f> maximalConsensusSet1;
        std::vector<Point2f> maximalConsensusSet2;
        int maximalConsensusSize = 0;
        std::cout << "datesize is " << size << std::endl;

        for (int i = 0; i < nbit; i++) {
            int consensusSize = 0;
            std::vector<Point2f> consensusSet1;
            std::vector<Point2f> consensusSet2;
            std::vector<Point2f> curOutliers1;
            std::vector<Point2f> curOutliers2;
            std::vector<Point2f> sample1;
            std::vector<Point2f> sample2;
//            Choose a random sample of the right size, depending on the model
            getRandomSample(data1, size, sample1, minS);
            getRandomSample(data2, size, sample2, minS);
//          get the model from the random sample
            Model curModel(sample1,sample2);
            for (int j = 0; j < size; j++) {
                if (curModel.agree(data1[j],data2[j], threshold)) {
                    consensusSet1.push_back(data1[j]);
                    consensusSet2.push_back(data2[j]);
                    consensusSize++;
                } else {
                    curOutliers1.push_back(data1[j]);
                    curOutliers1.push_back(data2[j]);
                }
            }
            if (consensusSize > maximalConsensusSize) {
                maximalConsensusSet1 = consensusSet1;
                maximalConsensusSet2 = consensusSet2;
                maximalConsensusSize = consensusSize;
                outliers1 = curOutliers1;
                outliers2 = curOutliers2;
            }

        }
        inliers2 = maximalConsensusSet1;
        inliers1 = maximalConsensusSet2;

        Model result(maximalConsensusSet1,maximalConsensusSet2, maximalConsensusSize);
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


#endif //RANSAC_RANSACHOMOGRAPHY_H
