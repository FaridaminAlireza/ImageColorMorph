#include <cmath>
#include <color.hpp>
#include <iostream>
#include "opencv/opencv_labels.hpp"
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "opencv_interface.hpp"
#include "util.hpp"

using namespace std;

#ifndef COLOR_HIST
#define COLOR_HIST

class ColorHist{
  public:
      template <typename T>
      static map<Color, T> get_color_frequency(ImageInterface image, vector<BGRInterface>& labels) {
      map<Color, T> color_freq;

      vector<Color> known_colors = ColorUtils:: get_colour_names("known_colours");

      for (int row = 0; row < image.row_nums(); row++) {
        for (int col = 0; col < image.col_nums(); col++) {

          BGRInterface pixel_bgr = BGRInterface(image.coord(row, col));

          Color closest_color =ColorUtils:: get_closest_color(pixel_bgr, known_colors);
          labels.push_back(closest_color.bgr);

          if (color_freq.count(closest_color) == 1) {
            color_freq[closest_color] += 1;
          } else {
            color_freq[closest_color] = 1;
          }
        }
      }

      return color_freq;
    }

      static void remove_colors_not_in_top_k_freq(map<Color, int>& color_freq, int top_k) {
      vector<pair<Color, int>> color_freq_pairs(color_freq.begin(),
                                                color_freq.end());

      sort(color_freq_pairs.begin(), color_freq_pairs.end(),
          [](const pair<Color, int>& a, const pair<Color, int>& b) {
            return a.second > b.second;  // Sort descending
          });

      int sum_freq = 0;
      for (const auto& pair : color_freq_pairs) {
        sum_freq += pair.second;
      }
      cout << "Pixel Nums: " << sum_freq << "\n";

      vector<Color> top_k_freq_colors;

      int item_counter = 0;

      vector<Color> items_to_remove;
      for (const auto& pair : color_freq_pairs) {
        double normalized_freq =
            (static_cast<double>(pair.second) / sum_freq) * 100;

        if (item_counter <= top_k && normalized_freq > 0.5) {
          cout << item_counter << " - bucket name: " << pair.first.name << "  ("
              <<  std::to_string(pair.first.bgr.getBlue()) << "," <<  std::to_string(pair.first.bgr.getGreen()) << "," <<  std::to_string(pair.first.bgr.getRed()) <<") "
              << normalized_freq << "%" <<endl;

          top_k_freq_colors.push_back(pair.first);
        } else {
          items_to_remove.push_back(pair.first);
        }
        item_counter++;
      }
      for (const auto& item : items_to_remove) {
        color_freq.erase(item);
      }
    }

      static tuple<vector<BGRInterface>, vector<BGRInterface>, int> get_top_k_freq_colors(ImageInterface  image, int top_k) {

      // TODO Apply caching

      map<Color, int> colour_dict;
      vector<BGRInterface> pixel_labels;
      colour_dict = ColorHist::get_color_frequency<int>(image, pixel_labels);

      ColorHist::remove_colors_not_in_top_k_freq(colour_dict, top_k);
      int adjusted_top_k = colour_dict.size();

      vector<BGRInterface> top_k_freq_colors;
      for (const auto& [key, value] : colour_dict) {
        top_k_freq_colors.push_back(key.bgr);
      }

      return make_tuple(top_k_freq_colors, pixel_labels, adjusted_top_k);
    }

      static ImageInterface plot_color_frequency(ImageInterface image, int top_k) {
  vector<BGRInterface> labels;

  map<Color, int> color_dict;
  map<Color, double> color_relative_freq;
  color_dict = ColorHist::get_color_frequency<int>(image, labels);

  int sum_freq = 0;
  for (const auto& [key, value] : color_dict) {
    sum_freq += (value);
  }

  ColorHist::remove_colors_not_in_top_k_freq(color_dict, top_k);
  int adjusted_top_k = color_dict.size();

  vector<BGRInterface> top_k_freq_colors;

  for (const auto& [key, value] : color_dict) {
    double normalized_freq = (static_cast<double>(value) / sum_freq) * 100;
    color_relative_freq[key] = normalized_freq;
    top_k_freq_colors.push_back(key.bgr);
  }

  vector<pair<Color, int>> color_freq_pairs(color_relative_freq.begin(),
                                            color_relative_freq.end());

  sort(color_freq_pairs.begin(), color_freq_pairs.end(),
      [](const pair<Color, int>& a, const pair<Color, int>& b) {
         return a.second > b.second;  // Sort descending
      });

  ImageInterface stats_img = image.clone_white();

  int item_counter = 1;

  for (const auto& pair : color_freq_pairs) {
    draw_label_for_histogram(stats_img.mat,
                            to_string(item_counter) + ". " + pair.first.name +
                                " (" + Utils::format_precision(pair.second) + " %)",
                            pair.first.bgr, 10, item_counter * 50,
                            pair.second);
    item_counter++;
  }

  return stats_img;
}

};

#endif