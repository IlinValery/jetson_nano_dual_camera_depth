//
// Created by valeryilin on 05.06.2020.
//

#include "depth_detector.h"

int DepthDetector::calculate_depth() {
    int max_disp = 160;
    double lambda = 8000.0;
    double sigma = 1.5;
    double vis_mult = 1.0;
    int wsize = 7;

    String left_im = "left_frame.jpg";
    String right_im = "right_frame.jpg";

    Mat left = imread(left_im, IMREAD_COLOR);
    if (left.empty()) {
        cout << "Cannot read image file: " << left_im;
        return -1;
    }

    Mat right = imread(right_im, IMREAD_COLOR);
    if (right.empty()) {
        cout << "Cannot read image file: " << right_im;
        return -1;
    }


    Mat left_for_matcher, right_for_matcher;
    Mat left_disp, right_disp;
    Mat filtered_disp, solved_disp, solved_filtered_disp;
    Mat conf_map = Mat(left.rows, left.cols, CV_8U);
    conf_map = Scalar(255);
    Rect ROI;
    Ptr<DisparityWLSFilter> wls_filter;
    double matching_time, filtering_time;
    double solving_time = 0;
    cout << max_disp << endl;
    max_disp = 48;
    if (max_disp <= 0 || max_disp % 16 != 0) {
        cout << "Incorrect max_disparity value: it should be positive and divisible by 16";
        return -1;
    }
    if (wsize <= 0 || wsize % 2 != 1) {
        cout << "Incorrect window_size value: it should be positive and odd";
        return -1;
    }


    max_disp /= 2;
    if (max_disp % 16 != 0)
        max_disp += 16 - (max_disp % 16);
    resize(left, left_for_matcher, Size(), 0.5, 0.5);
    resize(right, right_for_matcher, Size(), 0.5, 0.5);

    Ptr<StereoBM> left_matcher = StereoBM::create(max_disp, wsize);
    wls_filter = createDisparityWLSFilter(left_matcher);
    Ptr<StereoMatcher> right_matcher = createRightMatcher(left_matcher);

    cvtColor(left_for_matcher, left_for_matcher, COLOR_BGR2GRAY);
    cvtColor(right_for_matcher, right_for_matcher, COLOR_BGR2GRAY);

    matching_time = (double) getTickCount();
    left_matcher->compute(left_for_matcher, right_for_matcher, left_disp);
    right_matcher->compute(right_for_matcher, left_for_matcher, right_disp);
    matching_time = ((double) getTickCount() - matching_time) / getTickFrequency();


    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma);
    filtering_time = (double) getTickCount();
    wls_filter->filter(left_disp, left, filtered_disp, right_disp);
    filtering_time = ((double) getTickCount() - filtering_time) / getTickFrequency();
    //! [filtering]
    conf_map = wls_filter->getConfidenceMap();

    // Get the ROI that was used in the last filter call:
    ROI = wls_filter->getROI();
    resize(left_disp, left_disp, Size(), 2.0, 2.0);
    left_disp = left_disp * 2.0;
    ROI = Rect(ROI.x * 2, ROI.y * 2, ROI.width * 2, ROI.height * 2);


    //collect and print all the stats:
    cout.precision(2);
    cout << "Matching time:  " << matching_time << "s" << endl;
    cout << "Filtering time: " << filtering_time << "s" << endl;
    cout << endl;

    namedWindow("left", WINDOW_AUTOSIZE);
    imshow("left", left);
    Mat filtered_disp_vis;
    getDisparityVis(filtered_disp, filtered_disp_vis, vis_mult);
    namedWindow("filtered disparity", WINDOW_AUTOSIZE);
    imshow("filtered disparity", filtered_disp_vis);
    cout << filtered_disp.rows << endl;
    while (1) {
        char key = (char) waitKey(1);
        if (key == 27 || key == 'q' || key == 'Q') // 'ESC'
            break;
    }
    return 0;
}

cv::Mat DepthDetector::calculate_depth_from_frames(cv::Mat left, cv::Mat right) {
    // TODO
    return cv::Mat();
}