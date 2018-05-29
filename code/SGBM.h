//
// Created by Chen on 2018/5/29.
//

#ifndef CODE_SGBM_H
#define CODE_SGBM_H

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void SGBM()
{
    string picNum = "08";
    Mat left = imread("../../img/rectify/left" + picNum + ".jpg", IMREAD_GRAYSCALE);
    Mat right = imread("../../img/rectify/right" + picNum + ".jpg", IMREAD_GRAYSCALE);
    Mat disp;

    int mindisparity = 0;
    int ndisparities = 64;
    int SADWindowSize = 3;

    //SGBM
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(mindisparity, ndisparities, SADWindowSize);
    int P1 = 8 * left.channels() * SADWindowSize * SADWindowSize;
    int P2 = 32 * left.channels() * SADWindowSize * SADWindowSize;
    sgbm->setP1(P1);
    sgbm->setP2(P2);

    sgbm->setPreFilterCap(15);
    sgbm->setUniquenessRatio(10);
    sgbm->setSpeckleRange(2);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setDisp12MaxDiff(1);
    //sgbm->setMode(cv::StereoSGBM::MODE_HH);

    sgbm->compute(left, right, disp);

    disp.convertTo(disp, CV_32F, 1.0 / 16);                //除以16得到真实视差值
    Mat disp8U = Mat(disp.rows, disp.cols, CV_8UC1);       //显示
    normalize(disp, disp8U, 0, 255, NORM_MINMAX, CV_8UC1);

    imwrite("../../img/sgbm/sgbm" + picNum + ".jpg", disp8U);
}

#endif //CODE_SGBM_H
