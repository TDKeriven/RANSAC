//
// Created by geoffrey on 31/12/15.
//

#include "test.h"


void testSample() {
    int sampleSize[5] = {10, 20, 40, 50, 100};
    int dataSize[6] = {0, 100, 200, 500, 1000, 10000};
    for (int m = 0; m < 5; m++) {
        for (int p = 0; p < 5; p++) {
            int sSize = sampleSize[m];
            int dSize = dataSize[p];
            Point2f data[dSize];
            for (int i = 0; i < dSize; i++) {
                data[i] = Point2f(i, i);
            }
            Point2f *sample;
            RANSAC::getRandomSample(data, dSize, sample, sSize);
            for (int i = 0; i < sSize; i++) {
                std::cout << sample[i] << " ";
            }
            std::cout << std::endl;
        }
    }

}

// generates data to data array. Must past the pointer to data (which is itself a pointer to Point2f)
//TODO: debug this...
void generateData(int dataSize, Point2f **data) {
    // perfectly linear data
    Point2f dataSample[dataSize];
    for (int i = 0; i < dataSize; i++) {
        dataSample[i] = Point2f(i, i);
    }
    for (int i = 0; i < dataSize; i++) std::cout << dataSample[i];
    std::cout << std::endl;
    *data = dataSample;
}


void testLinearRegression() {
    int dataSize = 100;
    Point2f **dataPtr = new Point2f*;
    generateData(dataSize, dataPtr);
    for (int i = 0; i < dataSize; i++) std::cout << *(*dataPtr + i);
    std::cout << std::endl;
    LinearModel lmodel(*dataPtr, dataSize);
//    std::cout << lmodel.getDistanceToOrigin();
}
