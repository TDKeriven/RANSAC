//
// Created by pily on 14/01/16.
//

#ifndef RANSAC_MERGE_H
#define RANSAC_MERGE_H

#include <vector>
#include <random>
#include <opencv2/core.hpp>
#include "linearmodel.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class mergeimages {
private:
    Mat img0;
    Mat img1;
    Mat img2;
    double probability; //probability for drawing an outlier free subset
    int minS; //minimal number of data element to correctly estimate the model
    double threshold; //Threshold which defines if a data element, di, agrees with model M.
    int nbit;

public:
    mergeimages(Mat img0, Mat img1, Mat img2, double probability, int minS, double threshold, int nbit):
            img0(img0),img1(img1),img2(img2),probability(probability), minS(minS), threshold(threshold),nbit(nbit){
    }


    Mat domergeimages(){
        int nbimg=3;

        //on stocke les images dans imgs
        vector<Mat> imgs;
        imgs.push_back(img0);
        imgs.push_back(img1);
        imgs.push_back(img2);


        //transforme les images en N&B et les stock da,s gimgs
        vector<Mat> gimgs(nbimg);
        for (int i=0;i<nbimg;i++){
            cvtColor(imgs[i], gimgs[i], CV_BGR2GRAY);
        }



        cout<< "c'est quoin desc??" <<endl;
        vector<Mat> desc(nbimg);
        vector<vector<KeyPoint>> keypoints(nbimg);


        //REPRENDRE ICI !!!
        cout << "Computing key points" << endl;
        Ptr<Feature2D> akaze=AKAZE::create("AKAZE");

        for (int i=0;i<nbimg;i++){
             akaze->detect(gimgs[i],keypoints[i],cv::Mat() );
            akaze->compute(gimgs[i],keypoints[i],desc[i]);
        }

        cout << "drawkeypiint" << endl;
        vector<Mat> keypointsofimg(nbimg);
        for (int i=0;i<nbimg;i++){
            drawKeypoints(imgs[i], keypoints[i], keypointsofimg[i], Scalar::all(-1), DrawMatchesFlags::DEFAULT);
        }

        return keypointsofimg[0];

        //imshow("keypoints I1", img_keypoints_1);
        //imshow("keypoints I2", img_keypoints_2);
        //waitKey(0);

        cout << "Computing matching" << endl;
        /*Calcul du matching par force brute*/
     /*   BFMatcher matcher(NORM_HAMMING);
        vector<vector<DMatch>> nn_matches;
        matcher.knnMatch(desc1, desc2, nn_matches, 2);

        //Mat raw_matches_img;
        //drawMatches(I1, kpts1, I2, kpts2, nn_matches, raw_matches_img);
        //imshow("Raw matches", raw_matches_img);
        //waitKey(0);

        vector<Point2f> img1;
        vector<Point2f> img2;

        vector<KeyPoint> matched1, matched2;

        for (unsigned int i = 0; i < nn_matches.size(); i++)
        {
            img1.push_back(kpts1[nn_matches[i][0].queryIdx].pt);
            img2.push_back(kpts2[nn_matches[i][0].trainIdx].pt);

            matched1.push_back(kpts1[nn_matches[i][0].queryIdx]);
            matched2.push_back(kpts2[nn_matches[i][0].trainIdx]);
        }
        */
    }
};


#endif //RANSAC_MERGE_H
