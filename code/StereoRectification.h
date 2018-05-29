//
// Created by Chen on 2018/5/23.
//

#ifndef CODE_STEREORECTIFICATION_H
#define CODE_STEREORECTIFICATION_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;

void StereoRectification()
{
    string picList[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "11", "12", "13", "14"};
    string baseLeftPath = "../../img/left/left";
    string baseRightPath = "../../img/right/right";
    string rectifyPicNum = "11";
    Size imageSize;

    vector<cv::Point2f> corners[2];
    bool find[2];
    int nx = 9, ny = 6;
    vector<vector<cv::Point3f>> objectPoints;
    vector<Point3f> objp;
    vector<vector<cv::Point2f>> points[2];
    for (int i = 0; i < ny; i++)
        for (int j = 0; j < nx; j++)
            objp.push_back(cv::Point3f((float)(i), (float)(j), 0.f));

    // find point on chessboard
    for(auto pic : picList)
    {
        string leftPic = baseLeftPath + pic + ".jpg";
        string rightPic = baseRightPath + pic + ".jpg";
        Mat leftImg = imread(leftPic, 0);
        Mat rightImg = imread(rightPic, 0);
        find[0] = findChessboardCorners(leftImg, Size(nx, ny), corners[0]);
        find[1] = findChessboardCorners(rightImg, Size(nx, ny), corners[1]);
        if(find[0] && find[1])
        {
            objectPoints.push_back(objp);
            points[0].push_back(corners[0]);
            points[1].push_back(corners[1]);
        }
        imageSize = leftImg.size();
    }

    // calibrate
    Mat M1 = Mat::eye(3, 3, CV_64F);
    Mat M2 = Mat::eye(3, 3, CV_64F);
    Mat D1, D2, R, T, E, F;
//    calibrateCamera(objectPoints, points[0], imageSize, M1, D1, R, T);
    stereoCalibrate( objectPoints, points[0], points[1],
                     M1, D1, M2, D2, imageSize, R, T, E, F,
                     CALIB_FIX_ASPECT_RATIO | CALIB_ZERO_TANGENT_DIST | CALIB_SAME_FOCAL_LENGTH,
                     TermCriteria(cv::TermCriteria::COUNT | TermCriteria::EPS, 100, 1e-5));
//    cout << M1 << endl;
//    cout << D1 << endl;
//    cout << M2 << endl;
//    cout << D2 << endl;
//    cout << R << endl;
//    cout << T << endl;

    // rectify
    Mat R1, R2, P1, P2, map11, map12, map21, map22;
    stereoRectify(M1, D1, M2, D2, imageSize, R, T, R1, R2, P1, P2, noArray(), 0);
    initUndistortRectifyMap(M1, D1, R1, P1, imageSize, CV_16SC2, map11, map12);
    initUndistortRectifyMap(M2, D2, R2, P2, imageSize, CV_16SC2, map21, map22);

    Mat img1r, img2r;
    Mat img1 = imread("../../img/left/left" + rectifyPicNum + ".jpg", 0);
    Mat img2 = imread("../../img/right/right" + rectifyPicNum + ".jpg", 0);
    remap(img1, img1r, map11, map12, INTER_LINEAR);
    remap(img2, img2r, map21, map22, INTER_LINEAR);
//    imwrite("../../rectify/left02.jpg", img1r);
//    imwrite("../../rectify/right02.jpg", img2r);

    // show
    Mat pair;
    pair.create(imageSize.height, imageSize.width * 2, CV_8UC3);
    Mat part = pair.colRange(0, imageSize.width);
    cvtColor(img1r, part, COLOR_GRAY2BGR);
    part = pair.colRange(imageSize.width, imageSize.width * 2);
    cvtColor(img2r, part, COLOR_GRAY2BGR);
    for (int j = 0; j < imageSize.height; j += 16)
        line(pair, Point(0, j), Point(imageSize.width * 2, j), Scalar(0, 255, 0));
    if(access("../../img/rectify", F_OK))
        mkdir("../../img/rectify", S_IRWXU);
    imwrite("../../img/rectify/rectify" + rectifyPicNum + ".jpg", pair);
    imwrite("../../img/rectify/left" + rectifyPicNum + ".jpg", img1r);
    imwrite("../../img/rectify/right" + rectifyPicNum + ".jpg", img2r);
//    imshow("rectified", pair);
//    while(waitKey() != 'q');
}

#endif //CODE_STEREORECTIFICATION_H
