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
            std::vector<Point2f> data;
            for (int i = 0; i < dSize; i++) {
                data.push_back(Point2f(i, i));
            }
            std::vector<Point2f> sample;
            RANSAC::getRandomSample(data, dSize, sample, sSize);
            for (int i = 0; i < sSize; i++) {
                std::cout << sample[i] << " ";
            }
            std::cout << std::endl;
        }
    }

}

// generates data to data array. Must past the pointer to data (which is itself a pointer to Point2f)
void generateData(int dataSize, std::vector<Point2f> *data) {
    // perfectly linear data
    std::vector<Point2f> dataSample;
    for (int i = 0; i < dataSize; i++) {
        dataSample.push_back(Point2f(i, i));
    }
//    for (int i = 0; i < dataSize; i++) std::cout << dataSample[i];
//    std::cout << std::endl;
    *data = dataSample;
}


void testLinearRegression() {
    int dataSize = 100;
    std::vector<Point2f> data;
    generateData(dataSize, &data);
    std::cout << "Data is: " << std::endl;
    for (int i = 0; i < dataSize; i++) std::cout << data[i] << " ";
//    std::cout << std::endl;
    LinearModel lmodel(data, dataSize);
    std::cout << "Linear model found : " << lmodel << std::endl;
    std::cout << "Linear model should be : " << LinearModel(Point2f(1., 1.), 0) << std::endl;
}
