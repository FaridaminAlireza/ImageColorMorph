#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

#ifndef INTERFACE
#define INTERFACE

struct Image {

    std::string  name;
    int sequence;
    Mat mat;

    Image(string name, int sequence, Mat mat){
      this->name = name;
      this->sequence = sequence;
      this->mat = mat;
    }

    void showInfo() {
        cout << "Name: " <<this->name << ", Sequence: " << this->sequence<<
        ", Mat size: " << this->mat.size << endl;
    }

};

class BGRInterface {
private:
    cv::Vec3b data;

public:

    BGRInterface() : data(0, 0, 0) {}
    BGRInterface(uchar b, uchar g, uchar r) : data(b, g, r) {}
    BGRInterface(int b, int g, int r) {
        data = cv::Vec3b(
                        static_cast<uchar>(b),
                        static_cast<uchar>(g),
                        static_cast<uchar>(r));
    }

    BGRInterface(const cv::Vec3b& vec) : data(vec) {}


    uchar getBlue() const { return data[0]; }
    uchar getGreen() const { return data[1]; }
    uchar getRed() const { return data[2]; }

    int at(int index){return data[index]; }

    void setBlue(uchar value) { data[0] = value; }
    void setGreen(uchar value) { data[1] = value; }
    void setRed(uchar value) { data[2] = value; }

    uchar& operator[](int index) { return data[index]; }
    const uchar& operator[](int index) const { return data[index]; }

    BGRInterface operator+(const BGRInterface& other) const {
        return BGRInterface(
            cv::saturate_cast<uchar>(data[0] + other.data[0]),
            cv::saturate_cast<uchar>(data[1] + other.data[1]),
            cv::saturate_cast<uchar>(data[2] + other.data[2])
        );
    }

    BGRInterface operator-(const BGRInterface& other) const {
        return BGRInterface(
            cv::saturate_cast<uchar>(data[0] - other.data[0]),
            cv::saturate_cast<uchar>(data[1] - other.data[1]),
            cv::saturate_cast<uchar>(data[2] - other.data[2])
        );
    }

     bool operator==(const BGRInterface& other) const {
        bool result =  ( (data[0] == other.data[0]) &&
                      (data[1] == other.data[1]) &&
                      (data[2] == other.data[2]) );
      return result;
    }

    bool operator!=(const BGRInterface& other) const {
        bool result =  ( (data[0] != other.data[0]) ||
                      (data[1] != other.data[1]) ||
                      (data[2] != other.data[2]) );
      return result;
    }

    BGRInterface operator*(double scalar) const {
        return BGRInterface(
            cv::saturate_cast<uchar>(data[0] * scalar),
            cv::saturate_cast<uchar>(data[1] * scalar),
            cv::saturate_cast<uchar>(data[2] * scalar)
        );
    }

  operator cv::Vec3b() const {
    return data;
}

    BGRInterface& operator=(const cv::Vec3b& vec) {
        data = vec;
        return *this;
    }

    cv::Vec3b toVec3b() const { return data; }

    cv::Scalar toScalar() const {
        return cv::Scalar(static_cast<double>(data[0]),
                          static_cast<double>(data[1]),
                          static_cast<double>(data[2]));
    }

    void print() const {
        std::cout << "Vec3b(" << (int)data[0] << ", " << (int)data[1] << ", " << (int)data[2] << ")\n";
    }
};


class ImageInterface {

  public:
    Mat mat;

  public:

    ImageInterface(Mat mat){
      this->mat = mat;
    }

    int row_nums(){
      return mat.rows;
    }

    int col_nums(){
      return mat.cols;
    }

    BGRInterface coord(int row, int col){
      return static_cast<BGRInterface>(mat.at<cv::Vec3b>(row, col));
    }


  ImageInterface clone_black() const {
      Mat blackImage(mat.size(), mat.type(), cv::Scalar(0, 0, 0));
        return  ImageInterface(blackImage);
    }

    ImageInterface clone_white() const {
      Mat blackImage(mat.size(), mat.type(), cv::Scalar(255, 255, 255));
        return  ImageInterface(blackImage);
    }

    void showInfo() {
        cout << " Mat size: " << this->mat.size << endl;
    }

};



#endif