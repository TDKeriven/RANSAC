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
    mergeimages(Mat img0, Mat img1, Mat img2, double probability, int minS, double threshold, int nbit) :
            img0(img0), img1(img1), img2(img2), probability(probability), minS(minS), threshold(threshold), nbit(nbit) {
    }


    Mat domergeimages() {
        int nbimg = 3;

        //on stocke les images dans imgs
        vector<Mat> imgs;
        imgs.push_back(img0);
        imgs.push_back(img1);
        imgs.push_back(img2);

        vector<Mat> homographies;

        for (int i = 0; i < nbimg; i++) {
            homographies.push_back(gethomography(imgs[i], imgs[i + 1]));
        }

        Mat H = Mat::eye(homographies[0].size(),homographies[0].type());

        pair<Mat, Mat> res(H, H);

        Mat I=imgs[0];
        for (unsigned int i = 0; i < homographies.size(); i++){
            cout << "Merging step : " << i << endl;
            cout << "Multiplying homographies" << endl;
            H = res.second*homographies[i];
            cout << "Merging images" << endl;
            res = merge_images(I, imgs[i + 1], H,false);
            I = res.first;
            cout << I.at<Vec3b>(0, 0) << endl;
            //imshow("added image : "+to_string(i+1), I1);
            //waitKey(0);
        }

        resize(I, I, Size(0, 0), 0.7, 0.7);
        return I;
    }


    Mat gethomography(Mat im1, Mat im2) {
        //transforme les images en N&B et les stock da,s gimgs

        Mat gimg1, gimg2;
        cvtColor(im1, gimg1, CV_BGR2GRAY);
        cvtColor(im2, gimg2, CV_BGR2GRAY);

        Mat desc1, desc2;
        vector<KeyPoint> keypoints1, keypoints2;

        cout << "Computing key points" << endl;
        //Ptr<AKAZE> akaze = AKAZE::create();
        Ptr<Feature2D> akaze = AKAZE::create("akaze");

        akaze->detect(gimg1, keypoints1, cv::Mat());
        akaze->compute(gimg1, keypoints1, desc1);

        akaze->detect(gimg2, keypoints2, cv::Mat());
        akaze->compute(gimg2, keypoints2, desc2);


        vector<Mat> keypointsofimg(2);
        drawKeypoints(im1, keypoints1, keypointsofimg[1], Scalar::all(-1), DrawMatchesFlags::DEFAULT);
        drawKeypoints(im2, keypoints2, keypointsofimg[2], Scalar::all(-1), DrawMatchesFlags::DEFAULT);

        /*Calcul du matching par force brute*/
        BFMatcher matcher(NORM_HAMMING);
        vector<vector<DMatch>> nn_matches;
        matcher.knnMatch(desc1, desc2, nn_matches, 2);

        //Mat raw_matches_img;
        //drawMatches(imgs[1], keypoints[1],imgs[2],keypoints[2], nn_matches, raw_matches_img);
        //imshow("Raw matches", raw_matches_img);
        //waitKey(0);

        vector<Point2f> img1;
        vector<Point2f> img2;

        vector<KeyPoint> matched1, matched2;

        for (unsigned int i = 0; i < nn_matches.size(); i++) {
            img1.push_back(keypoints1[nn_matches[i][0].queryIdx].pt);
            img2.push_back(keypoints2[nn_matches[i][0].trainIdx].pt);

            matched1.push_back(keypoints1[nn_matches[i][0].queryIdx]);
            matched2.push_back(keypoints2[nn_matches[i][0].trainIdx]);
        }

        cout << "Computing Ransac" << endl;
        /*Calcul du Ransac*/

        /*On cree le tableau dynamique de donnees et on l'alloue */
        const int dataSize = nn_matches.size();

        pair<Point2f, Point2f> *data = new pair<Point2f, Point2f>[dataSize];

        for (int i = 0; i < dataSize; i++) {
            data[i] = pair<Point2f, Point2f>(img1[i], img2[i]);
        }

        bool *inliers = new bool[dataSize];

        /*Passage dans l'algorithme generique*/
        Mat H;

        return H;
    }

    pair<Mat,Mat> merge_images(Mat& I1, Mat& I2, const Mat& H,const bool& pano){
        //On calcule les bords
        int x_max, x_min, y_max, y_min;
        compute_extr(I1, I2, H, x_max, x_min, y_max, y_min);

        //On en deduit la taille de l'image cible
        int len = (int) (x_max - x_min);
        int width = (int) (y_max - y_min);

        //On regarde ou il faut placer l'image 1 par rapport a la 2
        int i1_x = max(0,-x_min), i1_y = max(0,-y_min);

        Mat final(Size(len, width), CV_8UC3);

        //On ne va pas toucher a I1 donc on lui alloue pile la place dans un rectangle of interest et on la recopiera
        Mat roi1(final, Rect(i1_x, i1_y, I1.cols, I1.rows));

        /* On cree un mask */
        Mat gsI2(I2.rows, I2.cols, CV_8U);

        /*
        Le masque sert a n'envoyer que I2 dans l'image finale (et pas les bords noirs dus a la transformation)
        L'offset permet de supprimer les bords noirs qui apparaissent (je ne sais pas trop pourquoi) autrement
        */
        int offset = 0;

        for (int i = 0; i < gsI2.rows; i++){
            for (int j = 0; j < gsI2.cols; j++){
                if ((i< offset) || (i >= I2.rows - offset) || (j < offset) || (j >= I2.cols - offset)){
                    gsI2.at<uchar>(i, j) = 0;
                }
                else{
                    gsI2.at<uchar>(i, j) = 255;
                }
            }
        }

        Mat mask2(len, width, CV_8U);

        /*Ajout d'une translation a l'homographie pour supprimer les index negatifs (qui font perdre des pixels) */
        Mat TH = offset_matrix(H, i1_x, i1_y);

        /* warp l'image blanche/noire pour creer un masque */
        warpPerspective(gsI2, mask2, TH, Size(len, width));

        /* Copier I1 dans l'image finale*/
        I1.copyTo(roi1);

        /* Envoyer I2 dans une image intermediaire */
        Mat bigI2(Size(len, width), CV_8UC3);
        warpPerspective(I2, bigI2, TH, Size(len, width));

        /*copier l'image intermediaire en ne gardant que la zone d'I2 grace au mask */
        bigI2.copyTo(final, mask2);

        /*
        Tentative pour rogner l'image finale en enlevant les bandes noires sur les cotes
        int xt, xb, yl, yr;
        rogner_image(final, xt, yl, xb, yr);

        len = yr - yl + 2;
        width = xb - xt + 2;

        cout << 0.5*(xb + xt + 1) << " ; " << 0.5*(yl + yr + 1) << endl;
        cout << xb << " : " << xt << " : " << yl << " : " << yr << endl;
        */

        /*Si on est dans le cas du panorama (translation selon une seule direction), rogner le haut et le bas de l'image*/
        if (pano){
            cout << I1.rows << ";" << i1_y << " par rapport a " << final.rows << endl;
            int len=final.cols;
            Mat roi_pano(final, Rect(0, i1_y, final.cols, I1.rows));
            Mat cut_final(Size(len, I1.rows),final.type());
            roi_pano.copyTo(cut_final);
            //imshow("roi",roi_pano);
            //waitKey(0);

            return pair<Mat, Mat>(cut_final, offset_matrix(TH, 0, -i1_y));
        }

        //getRectSubPix(final, Size(len, width), Point2f(0.5*(yl + yr + 1), 0.5*(xb + xt + 1) ), cut_final);

        //return pair<Mat, Mat>(cut_final, offset_matrix(TH,xt,yl));
        return pair<Mat, Mat>(final, TH);
    }

    inline void compute_extr(Mat& I1, Mat& I2,const Mat& H, int& x_max, int& x_min, int& y_max, int& y_min){

        const float offset = 0.;

        //On cree les points extremes
        vector<Point2f> extr_points;
        extr_points.push_back(Point2f(offset, offset));
        extr_points.push_back(Point2f(offset, (float)I2.rows - offset));
        extr_points.push_back(Point2f((float) I2.cols - offset, offset));
        extr_points.push_back(Point2f((float) I2.cols-offset, (float) I2.rows-offset));

        //On calcule leur image par l'homographie
        vector<Point2f> new_extr = compute_extreme_points(extr_points, H);
        //cout << I1.cols <<" , " << I1.rows << endl;

        //On retourne les coordonnees extremes de la nouvelle image
        x_max = max((float)I1.cols, max(max(max(new_extr[2].x, new_extr[3].x), new_extr[1].x), new_extr[0].x));
        y_max = max((float)I1.rows,max(max(max(new_extr[2].y, new_extr[3].y), new_extr[1].y), new_extr[0].y));

        x_min = min((float) 0, min(min(min(new_extr[2].x, new_extr[3].x), new_extr[1].x), new_extr[0].x));
        y_min = min((float) 0, min(min(min(new_extr[2].y, new_extr[3].y), new_extr[1].y), new_extr[0].y));
        //cout << x_max << " ; " << y_max << " ; " << x_min << " ; " << y_min << endl;
    }

    Mat offset_matrix(const Mat& H,const double& ox,const double& oy){
        Mat T = Mat::eye(3,3,H.type());

        T.at<double>(0, 2) = ox;
        T.at<double>(1, 2) = oy;

        return T*H;
    }

    inline vector<Point2f> compute_extreme_points(vector<Point2f>& inputs,const Mat& H){
        vector<Point2f> transformed;
        for (unsigned int i = 0; i < inputs.size(); i++){
            Point2f p2 = inputs[i];
            float sub = (float) (H.at<double>(2, 0)*p2.x + H.at<double>(2, 1)*p2.y + H.at<double>(2, 2));
            transformed.push_back(Point2f (
                    (float) (H.at<double>(0, 0)*p2.x + H.at<double>(0, 1)*p2.y + H.at<double>(0, 2)) / sub,
                    (float) (H.at<double>(1, 0)*p2.x + H.at<double>(1, 1)*p2.y + H.at<double>(1, 2)) / sub
            ));
            cout << transformed[i] << endl;
        }
        return transformed;
    }
};
#endif //RANSAC_MERGE_H
