#include <iostream>
#include <string>
#include <vector>
#include "opencv/opencv_transformers.hpp"
#include "color.hpp"

using namespace std;
using namespace cv;

#ifndef IMAGE_TRANSFORMER
#define IMAGE_TRANSFORMER

class Transformer {

    protected:
        ImageInterface  input_img;

    public:
        Transformer(ImageInterface input_img_i):  input_img(input_img_i){}
        virtual ImageInterface transform()  = 0;
        virtual string get_transformation_name() = 0;
        virtual ~Transformer() {} // Virtual destructor (important for polymorphism)
};

class EdgeTransformer: public Transformer{

    public:
        EdgeTransformer(ImageInterface i_img):
        Transformer(i_img){}
        ImageInterface  transform() override {
            return canny_edge_detector(input_img);}
        string  get_transformation_name() override {
            return  "EdgeTransformer" ;
        }
};

class EdgeEnhancerTransformer: public Transformer{

    public:
        EdgeEnhancerTransformer(ImageInterface i_img):
        Transformer(i_img){}
        ImageInterface  transform() override {
            return edge_enhancer(input_img);}
        string  get_transformation_name() override {
            return "EdgeEnhancerTransformer" ;
        }

};

class ComplementTransformer: public Transformer{

    public:
        ComplementTransformer(ImageInterface i_img):
        Transformer(i_img) {}
        ImageInterface transform() override {
        return  complement(input_img);
    }
        string  get_transformation_name() override {
            return  "ComplementTransformer" ;
        }

};

class ContrastTransformer: public Transformer{

    public:
        ContrastTransformer(ImageInterface i_img):
        Transformer(i_img) {}
        ImageInterface transform() override {
            return  contrast_enhancer(input_img);
    }
        string get_transformation_name() override {
            return  "ContrastTransformer"  ;
        }
};

class OverlayTransformer: public Transformer{

    private:
        ImageInterface second_input_img;

    public:
        OverlayTransformer(ImageInterface i_img, ImageInterface second_img_i):
        Transformer(i_img),  second_input_img(second_img_i) {}
        ImageInterface transform() override {

            return ImageInterface(overlay(input_img, second_input_img));
    }
        string  get_transformation_name() override {
            return  "OverlayTransformer" ;
        }
};

class PainterlyTransformer: public Transformer{

    public:
        PainterlyTransformer(ImageInterface i_img):
        Transformer(i_img) {}
        ImageInterface transform() override {

            ImageInterface tr1 = canny_edge_detector(input_img);
            ImageInterface tr2  = edge_enhancer(tr1);
            return  ImageInterface(overlay(input_img, tr2));
    }
        string get_transformation_name() override {
            return  "PainterlyTransformer" ;
        }
};

class PainterlyBilateralTransformer: public Transformer{

    public:
        PainterlyBilateralTransformer(ImageInterface i_img):
        Transformer(i_img) {}
        ImageInterface transform() override {
            ImageInterface img_painting = bilateral_filter(input_img);
            return  img_painting;
    }
        string get_transformation_name() override {
            return  "PainterlyBilateralTransformer" ;
        }
};

class ColorShiftTransformer: public Transformer{

    public:
        ColorShiftTransformer(ImageInterface i_img):
        Transformer(i_img) {}
        ImageInterface transform() override {

        return  color_shift(input_img, 30);

    }
        string  get_transformation_name() override {
            return "ColorShiftTransformer" ;
        }
};

class WhiteTransformer: public Transformer{

    public:
        WhiteTransformer(ImageInterface i_img):
        Transformer(i_img) {}
        ImageInterface transform() override {

        return  white(input_img);

    }
        string  get_transformation_name() override {
            return "WhiteTransformer" ;
        }
};

class BlackTransformer: public Transformer{

    public:
        BlackTransformer(ImageInterface i_img):
        Transformer(i_img) {}
        ImageInterface transform() override {

        return  black(input_img);
    }
        string  get_transformation_name() override {
            return "BlackTransformer" ;
        }
};

class GrayScaleTransformer: public Transformer{

    public:
        GrayScaleTransformer(ImageInterface i_img):
        Transformer(i_img) {}
        ImageInterface transform() override {

        return  gray_scale(input_img);
    }
        string  get_transformation_name() override {
            return "GrayScaleTransformer" ;
        }
};


enum class TransformerType {
    Border,
    EnhancedBorder,
    Contrast,
    Complement,
    Painterly,
    Bilateral,
    ColorShift,
    White,
    Black,
    GrayScale
};

class TransformerFactory {
public:
    static Transformer* create_transformer(const TransformerType type, ImageInterface first_img) {

        Transformer* transformer;
        switch (type){

            case TransformerType::Black:
                transformer = new  BlackTransformer(first_img);
                break;
            case TransformerType::White:
                transformer = new  WhiteTransformer(first_img);
                break;
            case TransformerType::GrayScale:
                transformer = new  GrayScaleTransformer(first_img);
                break;
            case TransformerType::Complement:
                transformer = new  ComplementTransformer(first_img);
                break;
            case TransformerType::ColorShift:
                transformer = new ColorShiftTransformer(first_img);
                break;
            case TransformerType::Contrast:
                transformer = new ContrastTransformer(first_img);
                break;
            case TransformerType::Border:
                transformer = new EdgeTransformer(first_img);
                break;
            case TransformerType::EnhancedBorder:
                transformer = new EdgeEnhancerTransformer(first_img);
                break;
            case TransformerType::Painterly:
                transformer = new PainterlyTransformer(first_img);
                break;
            case TransformerType::Bilateral:
                transformer = new PainterlyBilateralTransformer(first_img);
                break;
            default:
                transformer = nullptr;
        }

        return transformer;
    }
};

#endif