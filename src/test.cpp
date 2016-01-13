//
// Created by geoffrey on 31/12/15.
//

#include "test.h"


using namespace std;
using namespace cv;

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

// generates data to data array. Must pass the pointer to data
void generateData(int dataSize, std::vector<Point2f> *data) {
    // perfectly linear data
    std::vector<Point2f> dataSample;
    for (int i = 0; i < dataSize; i++) {
        dataSample.push_back(Point2f(i, i + 2));
    }
    //one outlier :
    dataSample.push_back(Point2f(-10., 3.));
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
    std::cout << "Linear model should be : " << LinearModel(Point2f(-0.7, 0.7), 1.4) << std::endl;
}

void testRansac() {

    int dataSize = 100;
    std::vector<Point2f> data;
    generateData(dataSize, &data);
    double proba, threshold;
    int minS;
    std::cout << "probability ?" << std::endl;
    proba = 0.7;
    std::cout << "nb de donnees dans le modele ?" << std::endl;
    minS = 20;
    std::cout << "threshold ?" << std::endl;
    threshold = 0.5;
    RANSAC r = RANSAC(data, dataSize + 1, proba, minS, threshold);
    LinearModel model = r.estimateLinearModel();
    std::cout << "RANSAC found: " << model << std::endl;
    std::cout << "Inliers: " << std::endl;
    for (int i = 0; i < r.getInliers().size(); i++) {
        std::cout << r.getInliers()[i];
    }
    std::cout << std::endl;
    std::cout << "Outliers: " << std::endl;
    for (int i = 0; i < r.getOutliers().size(); i++) {
        std::cout << r.getOutliers()[i];
    }

}

void testBinomial() {
    for (int n = 1; n < 100; n += 10) {
        for (int k = 1; k < n; k += 5) {
            std::cout << k << ", " << n << ": " << RANSAC::binomial(n, k) << std::endl;
        }
    }
}

void testImageDisplay() {
    Mat image;
    image = imread("/home/geoffrey/RANSAC/data/pano1/image0006.jpg", IMREAD_UNCHANGED);
    // Check for invalid input
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return;
    }
    // Show our image inside it.
    imshow("Display window", image);

    waitKey(0); // Wait for a keystroke in the window
    return;
}