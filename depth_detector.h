//
// Created by valeryilin on 05.06.2020.
//

#ifndef JETSON_NANO_DUAL_CAMERA_DEPTH_DETECTOR_H
#define JETSON_NANO_DUAL_CAMERA_DEPTH_DETECTOR_H
#include <opencv2/opencv.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/ximgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace cv::ximgproc;
using namespace std;

class DepthDetector {
public:
    int calculate_depth();
    cv::Mat calculate_depth_from_frames(cv::Mat left, cv::Mat right);

private:

};


#endif //JETSON_NANO_DUAL_CAMERA_DEPTH_DETECTOR_H
