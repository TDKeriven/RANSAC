#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;
using namespace cv;

Mat float2byte(const Mat& If) {
	double minVal, maxVal;
	minMaxLoc(If, &minVal, &maxVal);
	Mat Ib;
	If.convertTo(Ib, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
	return Ib;
}

int main() {

	Mat A = imread("../IMG_0045.JPG");
	Mat B = imread("../IMG_0046.JPG");
	imshow("A", A);
	imshow("B", B);

	waitKey(0);

	vector<KeyPoint> kpts1, kpts2;
	Mat desc1, desc2;

	Ptr<AKAZE> akaze = AKAZE::create();
	akaze->detectAndCompute(A, noArray(), kpts1, desc1);
	akaze->detectAndCompute(B, noArray(), kpts2, desc2);

	Mat KA(A.rows, A.cols, CV_8U);
	Mat KB(B.rows, B.cols, CV_8U);

	drawKeypoints(A, kpts1, KA);
	drawKeypoints(B, kpts2, KB);

	BFMatcher matcher(NORM_HAMMING);
	vector< vector<DMatch> > nn_matches;
	matcher.knnMatch(desc1, desc2, nn_matches, 1);

	imshow("Key Points A", KA);
	imshow("Key Points B", KB);

	waitKey(0);

	Mat Matches(A.rows + B.rows, A.cols + B.cols, CV_8U);

	drawMatches(A, kpts1, B, kpts2, nn_matches, Matches);
	imshow("Matches", Matches);

	waitKey(0);

	vector<Point2f> mA;
	vector<Point2f> mB;

	for (int i = 0; i < nn_matches.size(); i++){
		mA.push_back(kpts1[nn_matches[i][0].queryIdx].pt);
		mB.push_back(kpts2[nn_matches[i][0].trainIdx].pt);
	}

	double Seuil_Ransac = 10;

	Mat homography = findHomography(mA, mB, RANSAC, Seuil_Ransac);

	vector<DMatch> inliers;

	for (int i = 0; i < nn_matches.size(); i++){
		if (nn_matches[i][0].distance < Seuil_Ransac)
			inliers.push_back(nn_matches[i][0]);
	}

	Mat Inliers(A.rows + B.rows, A.cols + B.cols, CV_8U);
	drawMatches(A, kpts1, B, kpts2, inliers, Inliers);
	imshow("Inliers", Inliers);
	waitKey(0);

	Mat transformed(B.rows, B.cols, CV_8U);
	warpPerspective(B, transformed, homography, transformed.size());


	Mat image;
	hconcat(A, transformed, image);
	imshow("image", image);

	waitKey(0);
	return 0;
}
