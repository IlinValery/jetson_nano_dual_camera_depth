#include <iostream>
#include <opencv2/opencv.hpp>
#include "camera_manager.h"
#include "depth_detector.h"
#include <mutex>
#include <unistd.h>

using namespace cv;

void camera_problem(){
    std::mutex mutex;
    int index = 0;
    CameraManager cam_left(0, 640, 480, 30, 0, &index);
    CameraManager cam_right(1, 640, 480, 30, 0, &index);
    sleep(3);
    cam_left.start();
    cam_right.start();

    Mat image_left;
    Mat image_right;
    bool left_img_show = false;
    bool right_img_show = false;
    namedWindow("Image Left", WINDOW_AUTOSIZE);
    namedWindow("Image Right", WINDOW_AUTOSIZE);
    while (true) {
        if (!cam_left.get_image(image_left)) {
            std::cout << "Capture read for left camera error" << std::endl;
            break;
        }
        if (!cam_right.get_image(image_right)) {
            std::cout << "Capture read for right camera error" << std::endl;
            break;
        }

        if (!image_left.data) {
            std::cout << "No left image data \n";
            left_img_show = false;
        } else { left_img_show = true; }
        if (!image_right.data) {
            std::cout << "No right image data \n";
            right_img_show = false;
        } else { right_img_show = true; }
        if (left_img_show) {
            imshow("Image Left", image_left);
        }
        if (right_img_show) {
            imshow("Image Right", image_right);
        }

        int keycode = cv::waitKey(1) & 0xff; // escape
        if (keycode == 27) break;
    }
    cam_left.stop();
    cam_right.stop();
    sleep(3);
    cv::destroyAllWindows();
}

int main(int argc, char **argv) {
    bool cam = true;
    if (cam){
        camera_problem();
    } else {
        DepthDetector dd;
        dd.calculate_depth();
    }

    return 0;
}