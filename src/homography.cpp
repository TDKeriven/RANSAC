//
// Created by geoffrey on 13/01/16.
//

#include "homography.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>


//Use 4 points tu evaluate the Homography
Homography::Homography(const vector<pair<Point2f, Point2f>> matchPoints) {
    vector<Point2f> src, dst;

    for (unsigned int i = 0; i < 8; i++){
        src.push_back(matchPoints[i].first);
        dst.push_back(matchPoints[i].second);
    }
    //On utilise la fonction d'openCV mais sans utiliser leur RANSAC. 0 correspond a des moindres carres
    H = findHomography(dst, src,0);
}

double Homography::distance(const Point2f &point1,const Point2f &point2) const{
    float sub = (float) (H.at<double>(2, 0)*point2.x + H.at<double>(2, 1)*point2.y + H.at<double>(2, 2));

    if (sub == 0.){
        //cout << "Point a l'infini !" << endl;
        return DBL_MAX;
    }

    //H ne peut etre accedee qu'en double, il faut donc cast pour avoir des floats
    Point2f imp2(
            (float)(H.at<double>(0, 0)*point2.x + H.at<double>(0, 1)*point2.y + H.at<double>(0, 2)) / sub,
            (float)(H.at<double>(1, 0)*point2.x + H.at<double>(1, 1)*point2.y + H.at<double>(1, 2)) / sub
    );

    return pow(point1.x-imp2.x,2)+pow(point1.y-imp2.y,2);
}

bool Homography::agree(const Point2f &point1,const Point2f &point2, double threshold) const{
    return distance(point1,point2)<threshold;
}
