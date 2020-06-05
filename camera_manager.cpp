//
// Created by valeryilin on 05.06.2020.
//

#include "camera_manager.h"

CameraManager::CameraManager(const int &sensor_id, const int &capture_width, const int &capture_height,
                             const int &framerate,
                             const int &flip_method, int *cur_index_ptr) {
    this->sensor_id = sensor_id;
    this->capture_width = capture_width;
    this->capture_height = capture_height;
    this->display_width = capture_width;
    this->display_height = capture_height;
    this->framerate = framerate;
    this->flip_method = flip_method;
    this->cur_index_ptr = cur_index_ptr;
    this->open();
}

std::string CameraManager::get_camera_pipeline() const {
    return "nvarguscamerasrc sensor-id=" + std::to_string(this->sensor_id) +
           " ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(this->capture_width) + ", height=(int)" +
           std::to_string(this->capture_height) + ", format=(string)NV12, framerate=(fraction)" +
           std::to_string(this->framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(this->flip_method) + " ! video/x-raw, width=(int)" +
           std::to_string(this->display_width) + ", height=(int)" +
           std::to_string(this->display_height) +
           ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

void CameraManager::open() {
    this->capture.open(this->get_camera_pipeline(), cv::CAP_GSTREAMER);
    if (!this->capture.isOpened()) {
        std::cerr << "Failed to open camera." << std::endl;
        exit(-1);
    }
}

int CameraManager::stop() {
    this->is_running = false;
    this->release();
}

int CameraManager::release() {
    if (this->capture.isOpened()) {
        this->capture.release();
    }
    this->read_thread.join();
}

bool CameraManager::read(cv::Mat &mat) {
    return this->capture.read(mat);
}

void CameraManager::update_camera() {
    while (this->is_running) {
        try {
            if (*this->cur_index_ptr == this->sensor_id) {
                std::cout << "Update camera: " << this->sensor_id << " index: " << *this->cur_index_ptr << std::endl;
                if (this->read(this->img)){
                    *this->cur_index_ptr = (*this->cur_index_ptr + 1) % 2;
                }

            }
        } catch (cv::Exception er) {
            std::cerr << er.what() << std::endl;
        }
    }
}

void CameraManager::start() {
    if (this->is_running) {
        std::cerr << "Video capturing is already running\n";
    }
    if (this->capture.isOpened()) {
        std::cout << "Opening camera: " << this->sensor_id << std::endl;
        this->is_running = true;
        std::thread new_thread(&CameraManager::update_camera, this);
        this->read_thread.swap(new_thread);
        this->read_thread.detach();
    }
}

bool CameraManager::get_image(cv::Mat &out_img) {
    out_img = this->img;
    return true;
}
