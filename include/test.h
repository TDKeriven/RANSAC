//
// Created by geoffrey on 31/12/15.
//

#ifndef RANSAC_TEST_H
#define RANSAC_TEST_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "ransac.h"
#include <vector>
#include "homography.h"
#include "merge.h"

void testSample();

//generates a dataset of size dataSize in the array data
void generateData(int dataSize, std::vector<Point2f> data);

void testLinearRegression();

void testRansac();

void testBinomial();

void testImageDisplay();

void testransachomography();

#endif //RANSAC_TEST_H
