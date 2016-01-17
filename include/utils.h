//
// Created by geoffrey on 09/01/16.
//

#ifndef RANSAC_UTILS_H
#define RANSAC_UTILS_H

#include <opencv2/core.hpp>


// sampling with remplacement.
//TODO: sample with removal and not replacement
inline std::vector<cv::Point2f> &getRandomSample(std::vector<cv::Point2f>& data, int dataSize, std::vector<cv::Point2f> &sample,
                                          int sampleSize) {
    std::vector<cv::Point2f> result;
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


#endif //RANSAC_UTILS_H
