#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "util.hpp"
#include "color_hist.hpp"
#include "opencv/opencv_labels.hpp"

using namespace std;
namespace fs = filesystem;

#ifndef BUILD_COLORIZE
#define BUILD_COLORIZE

class ColorMorph{

    private:

        int frame_counter = 0;

        vector<int> order_colors_by_neighborhood_distance(int center_nums, vector<BGRInterface> centers) {

            std::vector<std::vector<float>> distance_matrix(center_nums, std::vector<float>(center_nums,  0.0f));

        for (int i = 0; i < center_nums; ++i) {
            for (int j = 0; j < center_nums; ++j) {
                distance_matrix[i][j] = Utils::euclidean_distance(centers[i], centers[j]);
            }
        }

        // Order by neighbors distance
        vector<int> ordered_centers;
        vector<bool> visited(center_nums, false);

        int current_center = 0;  // Start from the first center
        while (ordered_centers.size() < center_nums) {
            if (visited[current_center]) continue;

            ordered_centers.push_back(current_center);
            visited[current_center] = true;

            // Find the nearest unvisited neighbor
            int next_center = -1;
            float min_distance = numeric_limits<float>::max();
            for (int neighbor = 0; neighbor < center_nums; ++neighbor) {
                if (!visited[neighbor] &&
                    distance_matrix[current_center][neighbor] <min_distance) {
                    min_distance = distance_matrix[current_center][neighbor];
                    next_center = neighbor;
                }
            }

            if (next_center != -1) {
            current_center = next_center;
            } else {
            break;  // No unvisited neighbors left
            }
    }
    return ordered_centers;
}
        int create_frames(ImageInterface  first_image, ImageInterface second_image, string new_image_path) {

        Mat first_image_1D = first_image.mat.reshape(1, first_image.mat.total());
        Mat  second_img_1D =  second_image.mat.reshape(1, second_image.mat.total());

        int top_k = 30;
        ImageInterface img = ImageInterface(first_image.mat.clone());
        auto [top_k_colours, labels_vec, center_nums] =
        ColorHist::get_top_k_freq_colors(img, top_k);
        cout << "Bucket Nums: " << center_nums << endl;

        vector<int> ordered_centers = order_colors_by_neighborhood_distance(center_nums, top_k_colours);

            Mat first_image_1D_transformed = first_image_1D.clone();
            int pixel_counter = 0;

            for (int center_index : ordered_centers) {
                cout << "Processing Bucket Index " << center_index << endl;

                BGRInterface center_color = top_k_colours[center_index];

                for (int k = 0; k < first_image.mat.total() ; k++) {
                        int i = k;

                        if (labels_vec[i] != center_color)  continue;

                        pixel_counter++;

                        Vec3f pixel_value  = second_img_1D.at<Vec3f>(i, 0);
                        first_image_1D.at<Vec3f>(i, 0) = pixel_value;

                        if (pixel_counter % 60000 == 0 || pixel_counter == (second_image.mat.total() -1 || pixel_counter == 0  ) ) {

                            Mat first_image_transformed =
                            first_image_1D.reshape(3, first_image.mat.rows);

                            string label_text = "(" +
                                                to_string(center_color.getBlue()) + "," +
                                                to_string(center_color.getGreen()) + "," +
                                                to_string(center_color.getRed()) +
                                                ")";

                            Scalar label_color(center_color.getBlue(),
                                                            center_color.getGreen() ,
                                                            center_color.getRed()
                            );

                            draw_label_for_color_morph(first_image_transformed, label_text, label_color);

                            imwrite(new_image_path + to_string(this->frame_counter++) + ".jpg", first_image_transformed);
                        }

                }
            }
            return 0;
        }

    public:
        ColorMorph(): frame_counter(0) {}

        int color_morph(string image_directory_path,  string new_image_dir_path, string video_path) {

        cout << "Start Processing ..." << endl;

        int clean_up_result = Utils::remove_files_from_directory(new_image_dir_path);

        vector<string> images = Utils::get_sorted_images(image_directory_path);
        if (images.size() <=1) {return 1;}
        string new_image_path = new_image_dir_path + "/_" ;

        for (int i=0; i<images.size()-1; i++){
            ImageInterface first_image = ImageInterface(imread(images[i]));
            ImageInterface second_image = ImageInterface(imread(images[i+1]));

        int frame_result = create_frames(first_image, second_image,  new_image_path);

        }
        cout << "Build Video ..." << endl;
        Utils::build_video_from(new_image_dir_path, video_path);

        cout << "Done!" << endl;
        return 0;
    }
};




#endif