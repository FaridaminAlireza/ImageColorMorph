#include <opencv2/opencv.hpp>
#include <color_hist.hpp>
#include "opencv_interface.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {

    string image_path;

    if (argc == 2) {
        image_path = argv[1];
    } else {
        std::cerr << "No arguments passed!" << std::endl;
        return 1;
    }

    Mat img = imread(image_path, IMREAD_COLOR);

    filesystem::path file_path(image_path);
    string key = file_path.stem().string();
    string stats_image_path = "../data/stats/" + key + ".jpg";

    ImageInterface  histogram_img = ColorHist::plot_color_frequency(img, 100);
    imwrite(stats_image_path, histogram_img.mat);

    cout << "Histogram created in " << stats_image_path << endl ;
    return 0;
}