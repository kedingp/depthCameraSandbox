#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <chrono>
#include <iostream>

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include "CameraInterface.h"
#include "DepthCamera.h"
#include "ParcelLocatorAlgo.h"
#include "helper.h"

int main()
{
    camera::DepthCamera depthCamera;
    depthCamera.start();

    const auto window_name = "Display Depth Image";
    const auto color_window_name = "Display Color Image";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(color_window_name, cv::WINDOW_AUTOSIZE);

    for (auto [frame, parcelLocator] =
             std::tuple(std::make_unique<const camera::Frame3D>(), parcel_locator::ParcelLocatorAlgo());
         cv::waitKey(0) != 32;) {
        frame = depthCamera.grabFrame();

        const auto parcel = parcelLocator.findParcel(*frame);
        cv::polylines(frame->color_image, parcel.topCorners, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);

        cv::imshow(window_name, frame->depth_image);
        cv::imshow(color_window_name, frame->color_image);
        std::cout << "Frame id is: " << frame->id << '\n';
    }

    return EXIT_SUCCESS;
}