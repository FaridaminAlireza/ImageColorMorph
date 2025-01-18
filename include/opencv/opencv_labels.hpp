#include <cmath>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>
#include <string>
#include <tuple>
#include <vector>

#ifndef CUSTOM_L
#define CUSTOM_L

void  draw_label_for_histogram(cv::Mat image, std::string text,
                            BGRInterface block_color, int top_left_x,
                            int top_left_y, double relative_frequency) {

  int font_face = cv::FONT_HERSHEY_SIMPLEX;
  double font_scale = 0.7;
  int thickness = 2;
  cv::Scalar text_color(0, 0, 0);
  int baseline = 0;

  // Define the block position.
  cv::Point blockTopLeft(top_left_x, top_left_y);
  cv::Point blockBottomRight(int(50 * relative_frequency), top_left_y + 50);

  // Draw the block (rectangle)
  cv::rectangle(image, blockTopLeft, blockBottomRight, block_color.toScalar(), cv::FILLED);

  cv::Point text_origin(top_left_x + 27, top_left_y + 27);
  cv::putText(image, text, text_origin, font_face, font_scale, text_color,
              thickness);

}

void  draw_label_for_color_morph(cv::Mat image, std::string text, cv::Scalar blockColor) {

  cv::Point text_origin(10, 30);
  int font_face = cv::FONT_HERSHEY_SIMPLEX;
  cv::Scalar text_color(255, 255, 255);
  double font_scale = 0.7;
  int thickness = 4;
  int baseline = 0;

  // Get the text size to determine the block size
  cv::Size text_size =
      cv::getTextSize(text, font_face, font_scale, thickness, &baseline);

  cv::Point blockTopLeft(5, 5);
  cv::Point blockBottomRight(5 + text_size.width + 30,
                            5 + text_size.height + 30);
  cv::rectangle(image, blockTopLeft, blockBottomRight, blockColor, cv::FILLED);

  cv::putText(image, text, text_origin, font_face, font_scale, text_color,
              thickness);

}

#endif