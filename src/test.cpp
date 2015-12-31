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
            std::vector<int> data;
            for (int i = 0; i < dSize; i++) {
                data.push_back(i);
            }
            std::vector<int> sample = RANSAC<int>::getRandomSample(data, sSize);
            for (int i = 0; i < sample.size(); i++) {
                std::cout << sample[i] << " ";
            }
            std::cout << std::endl;
        }
    }

}
