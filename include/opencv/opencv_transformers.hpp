#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv_interface.hpp"

using namespace cv;
using namespace std;

#ifndef OPEN_CV_TRANSFORMERS
#define OPEN_CV_TRANSFORMERS

ImageInterface canny_edge_detector(ImageInterface input_img) {

    // Convert to grayscale
    Mat gray_img;
    cvtColor(input_img.mat, gray_img, COLOR_BGR2GRAY);

    // Apply Gaussian blur
    Mat blurred_img;
    GaussianBlur(gray_img, blurred_img, Size(5, 5), 0);

    // Perform Canny edge detection
    Mat border_img;
    double threshold1 = 100, threshold2 = 200;
    Canny(blurred_img, border_img, threshold1, threshold2);

    // Increase contrast to the highest amount
    cv::Mat highContrastImg;
    double alpha = 0.5;  // Contrast factor (higher means more contrast)
    int beta = 0;        // Brightness offset (keep at 0 to only affect contrast)

    border_img.convertTo(highContrastImg, -1, alpha, beta);  // Apply contrast adjustment

    return  ImageInterface(border_img);
}

ImageInterface edge_enhancer(ImageInterface input_img){

    // Apply a Laplacian filter for edge enhancement
    cv::Mat laplacian;
    cv::Laplacian(input_img.mat, laplacian, CV_16S, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::Mat abs_laplacian;
    cv::convertScaleAbs(laplacian, abs_laplacian);  // Convert to absolute values

    // Add the Laplacian result to the original image for enhancement
    cv::Mat enhanced_img = input_img.mat + abs_laplacian;
    return ImageInterface(enhanced_img);
}

ImageInterface overlay(ImageInterface first_img_i, ImageInterface second_img_i) {


    Mat first_img = first_img_i.mat;
    Mat second_img = second_img_i.mat;

    resize(first_img, first_img, second_img.size());

    // Overlay the second image on the first image
    Mat overlayed_img = first_img.clone();
    overlayed_img.setTo(cv::Scalar(255, 200, 100), second_img);

    return ImageInterface(overlayed_img);
}

ImageInterface complement(ImageInterface input_image){

    Mat result_image = input_image.mat.clone();
    result_image.forEach<cv::Vec3b>([](cv::Vec3b &pixel, const int * position) {
            pixel[0] = 255 - pixel[0];
            pixel[1] = 255 - pixel[1];
            pixel[2] = 255 - pixel[2];
    });
    return ImageInterface(result_image);
}


ImageInterface white(ImageInterface input_image){

    Mat img = input_image.mat;
    Mat  white_image(img.size(), img.type(), cv::Scalar(255, 255, 255));

    return ImageInterface(white_image);
}

ImageInterface gray_scale(ImageInterface input_image){

    Mat img = input_image.mat;
    cv::Mat gray_image;
    cv::cvtColor(img, gray_image, cv::COLOR_BGR2GRAY);
    return ImageInterface(gray_image);
}

ImageInterface black(ImageInterface input_image){

    Mat img = input_image.mat;
    Mat black_image = cv::Mat::zeros(img.size(), img.type());

    return ImageInterface(black_image);
}

BGRInterface color_shift(const BGRInterface pixel, int hue_shift = 30) {

  // Convert the BGR pixel to a 1x1 Mat for HSV conversion
    cv::Mat bgr_mat(1, 1, CV_8UC3, pixel.toVec3b());
    cv::Mat hsv_mat;
    cv::cvtColor(bgr_mat, hsv_mat, cv::COLOR_BGR2HSV);

    cv::Vec3b hsv_pixel = hsv_mat.at<cv::Vec3b>(0, 0);

    // Adjust the Hue value clockwise
    int hue = hsv_pixel[0];
    hue = (hue + hue_shift) % 180;  // Keep hue in [0, 179]
    hsv_pixel[0] = static_cast<uchar>(hue);

    // Convert back to BGR
    hsv_mat.at<cv::Vec3b>(0, 0) = hsv_pixel;
    cv::Mat result_mat;
    cv::cvtColor(hsv_mat, result_mat, cv::COLOR_HSV2BGR);

    return BGRInterface(result_mat.at<cv::Vec3b>(0, 0));
}

ImageInterface color_shift(ImageInterface input_image, int hue_shift = 30){
    Mat result_image = input_image.mat.clone();
      result_image.forEach<cv::Vec3b>([hue_shift](cv::Vec3b &pixel, const int * position) {
        pixel = color_shift(BGRInterface(pixel), hue_shift).toVec3b();
    });

    return ImageInterface(result_image);
}

ImageInterface contrast_enhancer(ImageInterface input_image){
    Mat result_image = input_image.mat.clone();
    result_image.forEach<cv::Vec3b>([](cv::Vec3b &pixel, const int * position) {

            for (int i = 0; i < 3; i++) {
                int newValue = 2 * (pixel[i] - 128) + 128; // Double contrast formula
                pixel[i] = cv::saturate_cast<uchar>(newValue); // Ensure values remain in [0, 255]
        }
    });
    return ImageInterface(result_image);

}

ImageInterface bilateral_filter(ImageInterface bilateral_image_i){
            Mat img_painting = bilateral_image_i.mat.clone();
            bilateralFilter(bilateral_image_i.mat, img_painting, 15, 80, 80);
            return ImageInterface(img_painting);
    }

#endif