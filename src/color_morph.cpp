#include <opencv2/opencv.hpp>
#include <iostream>
#include "image_transformer.hpp"
#include "color_morph.hpp"

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

    Mat first_img = imread(image_path);
    if (first_img.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    TransformerType  transformations[] = {
        TransformerType::GrayScale,
        TransformerType::Contrast,
        TransformerType::Painterly,
        TransformerType::Bilateral,
        TransformerType::GrayScale,
        TransformerType::Black
    };

    int counter = 0;
    string transformation_dir = "../data/transformations/";
    int clean_up_result = Utils::remove_files_from_directory(transformation_dir);

    for (const auto& t :transformations) {
            Transformer*  transformer  = TransformerFactory::create_transformer(
                t, ImageInterface( first_img));

            string transformation_name = transformer->get_transformation_name();
            ImageInterface  result_i  = transformer->transform();
            Mat result = result_i.mat;

            imwrite(transformation_dir + "T_" + std::to_string(counter++)
                + "_" + transformation_name +  ".jpg", result);
            imshow(transformer->get_transformation_name(),  result);
            waitKey(0);
        }
    destroyAllWindows();
    cv::waitKey(1);

    string frames_dir_path = "../data/frames/";

    filesystem::path file_path(image_path);
    string key = file_path.stem().string();
    string video_path = "../data/video/"+ key + ".mp4";

    int result = ColorMorph().color_morph(transformation_dir,  frames_dir_path, video_path);

    cout << "Video created in " << video_path <<endl ;
    return result;
}