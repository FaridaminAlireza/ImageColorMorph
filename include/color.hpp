#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "opencv_interface.hpp"
using namespace std;

#ifndef COLOR
#define COLOR

struct Color {

    string name;
    BGRInterface bgr;

    bool operator<(const Color& other) const {

    if (name != other.name) {
      return name < other.name;
    }

    return bgr.getBlue() < other.bgr.getBlue() ||
          (bgr.getBlue() == other.bgr.getBlue() && bgr.getGreen() < other.bgr.getGreen()) ||
          (bgr.getBlue() == other.bgr.getBlue() && bgr.getGreen() == other.bgr.getGreen() &&
          bgr.getRed() < other.bgr.getRed()
          );
  }
};

class ColorUtils {

  public:
  static vector<Color> get_colour_names(string colour_type) {
  //  Well known colors
    vector<Color> known_colors = {
      {"Black",BGRInterface(0, 0, 0)},
      {"White", BGRInterface(255, 255, 255)},
      {"Red", BGRInterface(0, 0, 255)},
      {"Green", BGRInterface(0, 255, 0)},
      {"Blue", BGRInterface(255, 0, 0)},
      {"Yellow", BGRInterface(0, 255, 255)},
      {"Cyan", BGRInterface(255, 255, 0)},
      {"Magenta", BGRInterface(255, 0, 255)},
      {"Gray", BGRInterface(128, 128, 128)},
      {"Dark Gray", BGRInterface(64, 64, 64)},
      {"Light Gray", BGRInterface(192, 192, 192)},
      {"Orange", BGRInterface(0, 165, 255)},
      {"Pink", BGRInterface(203, 192, 255)},
      {"Purple", BGRInterface(128, 0, 128)},
      {"Brown", BGRInterface(42, 42, 165)},
      {"Lime", BGRInterface(0, 255, 128)},
      {"Olive", BGRInterface(0, 128, 128)},
      {"Navy", BGRInterface(128, 0, 0)},
      {"Maroon", BGRInterface(0, 0, 128)},
      {"Teal", BGRInterface(128, 128, 0)},
      {"Beige", BGRInterface(220, 245, 245)},
      {"Ivory", BGRInterface(240, 255, 255)},
      {"Lavender", BGRInterface(250, 230, 230)},
      {"Gold", BGRInterface(0, 215, 255)},
      {"Silver", BGRInterface(192, 192, 192)},
      {"Coral", BGRInterface(80, 127, 255)},
      {"Turquoise", BGRInterface(208, 224, 64)},
      {"Indigo", BGRInterface(130, 0, 75)},
      {"Violet", BGRInterface(238, 130, 238)},
      {"Salmon", BGRInterface(114, 128, 250)},
      {"Khaki", BGRInterface(140, 230, 240)},
      {"Crimson", BGRInterface(60, 20, 220)},
      {"Aqua", BGRInterface(255, 255, 128)},
      {"Light Blue", BGRInterface(230, 216, 173)},
      {"Midnight Blue", BGRInterface(112, 25, 25)},
      {"Peach", BGRInterface(180, 218, 255)},
      {"Mint", BGRInterface(170, 255, 189)},
      {"Periwinkle", BGRInterface(204, 204, 255)},
      {"Plum", BGRInterface(221, 160, 221)},
      {"Rose", BGRInterface(128, 0, 255)},
      {"Sky Blue", BGRInterface(235, 206, 135)},
      {"Tan", BGRInterface(180, 229, 210)},
      {"Seashell", BGRInterface(238, 245, 255)},
      {"Slate Gray", BGRInterface(90, 128, 112)},
      {"Tomato", BGRInterface(71, 99, 255)},
      {"Wheat", BGRInterface(179, 222, 245)},
      {"Chocolate", BGRInterface(30, 105, 210)},
      {"Firebrick", BGRInterface(34, 34, 178)},
      {"Forest Green", BGRInterface(34, 139, 34)},
      {"Gainsboro", BGRInterface(220, 220, 220)},
      {"Honeydew", BGRInterface(240, 255, 240)},
      {"Hot Pink", BGRInterface(180, 105, 255)},
      {"Light Coral", BGRInterface(128, 128, 240)},
      {"Lemon Chiffon", BGRInterface(205, 250, 255)},
      {"Light Cyan", BGRInterface(255, 255, 224)},
      {"Medium Purple", BGRInterface(219, 112, 147)},
      {"Pale Green", BGRInterface(152, 251, 152)},
      {"Sandy Brown", BGRInterface(96, 164, 244)},
      {"Sienna", BGRInterface(45, 82, 160)},
      {"Spring Green", BGRInterface(127, 255, 0)},
      {"Thistle", BGRInterface(216, 191, 216)},
      {"Snow", BGRInterface(250, 250, 255)},
      {"Pale Violet Red", BGRInterface(147, 112, 219)},
      {"Medium Slate Blue", BGRInterface(238, 104, 123)},
      {"Medium Spring Green", BGRInterface(154, 255, 250)},
      {"Pale Goldenrod", BGRInterface(170, 232, 238)},
      {"Papaya Whip", BGRInterface(213, 239, 255)},
      {"Orchid", BGRInterface(214, 112, 218)},
      {"Old Lace", BGRInterface(230, 245, 253)},
      {"Medium Turquoise", BGRInterface(204, 209, 72)},
      {"Light Slate Gray", BGRInterface(144, 153, 119)},
      {"Light Steel Blue", BGRInterface(222, 196, 176)},
      {"Linen", BGRInterface(240, 245, 250)},
      {"Light Goldenrod Yellow", BGRInterface(210, 250, 250)},
      {"Lawn Green", BGRInterface(0, 252, 124)},
      {"Deep Pink", BGRInterface(147, 20, 255)},
      {"Dark Olive Green", BGRInterface(47, 107, 85)},
      {"Dark Orange", BGRInterface(0, 140, 255)},
      {"Dark Orchid", BGRInterface(204, 50, 153)},
      {"Deep Sky Blue", BGRInterface(255, 191, 0)},
      {"Dodger Blue", BGRInterface(255, 144, 30)},
      {"Indian Red", BGRInterface(92, 92, 205)},
      {"Lavender Blush", BGRInterface(245, 240, 255)},
      {"Light Pink", BGRInterface(193, 182, 255)},
      {"Light Salmon", BGRInterface(122, 160, 255)},
      {"Moccasin", BGRInterface(181, 228, 255)},
      {"Medium Aquamarine", BGRInterface(170, 205, 102)},
      {"Medium Blue", BGRInterface(205, 0, 0)},
      {"Medium Orchid", BGRInterface(211, 85, 186)},
      {"Olive Drab", BGRInterface(35, 142, 107)}};

  // Basic colors
  vector<Color> basic_colors = {
      {"Red", BGRInterface(0, 0, 255)},       {"Green", BGRInterface(0, 255, 0)},
      {"Blue", BGRInterface(255, 0, 0)},      {"Yellow", BGRInterface(0, 255, 255)},
      {"Cyan", BGRInterface(255, 255, 0)},    {"Magenta", BGRInterface(255, 0, 255)},
      {"White", BGRInterface(255, 255, 255)}, {"Black", BGRInterface(0, 0, 0)}};

  if (colour_type == "basic_colours") {
    return basic_colors;
  } else {
    return known_colors;
  }
}

static double get_color_distance(const BGRInterface& color1, const BGRInterface& color2) {
  return sqrt(pow(color1.getBlue() - color2.getBlue(), 2) +
              pow(color1.getGreen() - color2.getGreen(), 2) +
              pow(color1.getRed() - color2.getRed(), 2)
  );
}

static Color get_closest_color(const BGRInterface& pixel_color, const vector<Color>& known_colors) {

  double minDistance = numeric_limits<double>::max();
  Color closestColor;

  for (const auto& known_color : known_colors) {
    double distance = ColorUtils::get_color_distance(pixel_color, known_color.bgr);
    if (distance < minDistance) {
      minDistance = distance;
      closestColor = known_color;
    }
  }

  return closestColor;
}

};
#endif