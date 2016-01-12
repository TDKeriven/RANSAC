//
// Created by geoffrey on 31/12/15.
//

#ifndef RANSAC_TEST_H
#define RANSAC_TEST_H

#include <iostream>
#include "ransac.h"
#include <vector>

void testSample();

//generates a dataset of size dataSize in the array data
void generateData(int dataSize, std::vector<Point2f> data);

void testLinearRegression();

void testRansac();

void testBinomial();

#endif //RANSAC_TEST_H
