//
// Created by valeryilin on 05.06.2020.
//

#ifndef DEPTH_IMAGE_OPENCV_CAMERA_MANAGER_H
#define DEPTH_IMAGE_OPENCV_CAMERA_MANAGER_H

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>

class CameraManager {
public:
    CameraManager(const int &sensor_id,
                  const int &capture_width,
                  const int &capture_height,
                  const int &framerate,
                  const int &flip_method,
                  int* cur_index);


    void start();

    int stop();

    bool read(cv::Mat &mat);

    bool get_image(cv::Mat& out_img);

private:
    std::string get_camera_pipeline() const;

    void open();

    int release();

    void update_camera();

    std::string launcher = "";
    int sensor_id = 0;
    int capture_width = 1280;
    int capture_height = 720;
    int display_width = 1280;
    int display_height = 720;
    int framerate = 30;
    int flip_method = 0;
    int* cur_index_ptr;
    cv::VideoCapture capture;
    bool is_running = false;
    std::thread read_thread;
    cv::Mat img;
};


#endif //DEPTH_IMAGE_OPENCV_CAMERA_MANAGER_H
