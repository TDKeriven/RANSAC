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



        //imshow("keypoints I1", img_keypoints_1);
        //imshow("keypoints I2", img_keypoints_2);
        //waitKey(0);

        cout << "Computing matching" << endl;
        /*Calcul du matching par force brute*/
        BFMatcher matcher(NORM_HAMMING);
        vector<vector<DMatch>> nn_matches;
        matcher.knnMatch(desc[0], desc[1], nn_matches, 2);

        //Mat raw_matches_img;
        //drawMatches(imgs[1], keypoints[1],imgs[2],keypoints[2], nn_matches, raw_matches_img);
        //imshow("Raw matches", raw_matches_img);
        //waitKey(0);

        vector<Point2f> img1;
        vector<Point2f> img2;

        vector<KeyPoint> matched1, matched2;

        for (unsigned int i = 0; i < nn_matches.size(); i++)
        {
            img1.push_back(keypoints[1][nn_matches[i][0].queryIdx].pt);
            img2.push_back(keypoints[2][nn_matches[i][0].trainIdx].pt);

            matched1.push_back(keypoints[1][nn_matches[i][0].queryIdx]);
            matched2.push_back(keypoints[2][nn_matches[i][0].trainIdx]);
        }


        cout << "Computing RANSAC" << endl;
        /*Calcul du RANSAC*/

        /*On cree le tableau dynamique de donnees et on l'alloue */
        const int dataSize = nn_matches.size();

        pair<Point2f, Point2f>* data = new pair<Point2f, Point2f>[dataSize];

        for (int i = 0; i < dataSize; i++){
            data[i] = pair<Point2f, Point2f>(img1[i], img2[i]);
        }

        bool* inliers = new bool[dataSize];

        /*Passage dans l'algorithme generique*/
       // ransac<pair<Point2f, Point2f>, homography> my_ransac(data, data_size);
        my_ransac.getInliers(inliers, max_err_thresh,nb_iters,min_goodpoints);

        ransachomography rsc(data, dataSize, probability, minS, threshold, nbit)
        rsc.getInliers();

        /*On recupere deux tableaux avec les inliers dans les images 1 et 2*/
        vector<Point2f> filtered_img1, filtered_img2;
        int nb_inliers = 0;

        vector<KeyPoint> inliers1, inliers2;
        vector<DMatch> good_matches;

        for (int i = 0; i < dataSize; i++){
            if (inliers[i]){
                filtered_img1.push_back(img1[i]);
                filtered_img2.push_back(img2[i]);
                inliers1.push_back(matched1[i]);
                inliers2.push_back(matched2[i]);
                good_matches.push_back(DMatch(nb_inliers, nb_inliers, 0));
                nb_inliers++;
            }
        }

        /* On sort si aucun match n'a ete trouve */
        if (filtered_img1.size() == 0) {
            cout << "No points matched, try a superior error threshold" << endl;
            return Mat::zeros(3, 3, CV_32F);
        }

        cout << filtered_img1.size() << " points remaining, computing homography" << endl;

        /*On calcule l'homographie par la methode classique (moindres carres) sur les inliers du RANSAC */
        Mat H = findHomography(filtered_img2, filtered_img1, 0);

        /*
        cout << "Drawing inliers" << endl;

        Mat res;
        drawMatches(I1, inliers1, I2, inliers2, good_matches, res);
        imshow("Inliers", res);
        waitKey(0);
        */

        return H;
    }
};


#endif //RANSAC_MERGE_H
