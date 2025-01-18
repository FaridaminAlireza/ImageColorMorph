#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>
#include <random>
#include "opencv_interface.hpp"
#include <regex>

namespace fs = filesystem;
using namespace std;

#ifndef UTILS
#define UTILS


class Utils {

  public:
    static string format_precision(int num) {
  ostringstream oss;
  oss << fixed << setprecision(2) << num;
  return oss.str();
}

static vector<int> shuffle_indices(set<int> index_set) {
  vector<int> index_vector(index_set.begin(), index_set.end());

  random_device random_device;
  mt19937 random_generator(random_device());

  shuffle(index_vector.begin(), index_vector.end(), random_generator);
  return index_vector;
}

static int extract_number(const string& str_value) {
  regex number_pattern("(\\d+)");
  smatch match;
  if (regex_search(str_value, match, number_pattern)) {
    return stoi(match.str(0));  // Extract the first number found
  }
  return 0;
}

static vector<string> get_sorted_images(string image_directory_path){

  vector<string> image_names;

  for (const auto& entry : fs::directory_iterator(image_directory_path)) {
    string path = entry.path().string();
    if (path.ends_with(".png") || path.ends_with(".jpg") ||
        path.ends_with(".jpeg")) {
      image_names.push_back(path);
    }
  }

  if (image_names.empty()) {
    cerr << "No images found in the directory." << endl;
    return image_names;
  }

  sort(image_names.begin(), image_names.end(),
      [](const string& a, const string& b) {
        return Utils::extract_number(a) < Utils::extract_number(b);
      });

  return image_names;
}

static int build_video_from(string image_directory_path, string video_name) {

  vector<string> image_names = get_sorted_images(image_directory_path);

  cv::Mat first_frame = cv::imread(image_names[0]);

  if (first_frame.empty()) {
    cerr << "Failed to load the first image." << endl;
    return -1;
  }

  int width = first_frame.cols;
  int height = first_frame.rows;

  cv::VideoWriter video(video_name, cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                        9, cv::Size(width, height));

  if (!video.isOpened()) {
    cerr << "Failed to open video file for writing." << endl;
    return -1;
  }

  for (const auto& image_name : image_names) {
    cv::Mat frame = cv::imread(image_name);

    if (frame.empty()) {
      cerr << "Failed to load image: " << image_name << endl;
      continue;
    }

    if (frame.size() != first_frame.size()) {
      cv::resize(frame, frame, cv::Size(width, height));
    }
    video.write(frame);
  }

  video.release();

  cout << "Video saved as " << video_name << endl;
  return 0;
}

static int remove_files_from_directory(string directoryPath) {
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (fs::is_regular_file(entry.status())) {
                fs::remove(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}

static float euclidean_distance(BGRInterface& pixel_1, BGRInterface& pixel_2) {

    double sum_of_squares = 0;
    for (size_t i = 0; i < 3; ++i) {
        sum_of_squares += std::pow(pixel_1.at(i) - pixel_2.at(i), 2);
    }

    return std::sqrt(sum_of_squares);
}




};
#endif