#include "ColorImage.h"

#include <stdio.h>

#include <sstream>
using namespace std;

namespace ImgProc {


    ColorImage::ColorImage(const char* file) : Image(1, 1) {
        image = NULL;
        this->loadFile(file);
    }

    ColorImage::ColorImage(const ImgProc::Image& base) : Image(1, 1){
        image = NULL;
        setSize(base.getWidth(), base.getHeight());
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                this->set(pos, base.get(pos));
            }
        }
    }
    ColorImage::ColorImage(int width, int height) : Image(width, height) {
        image = NULL;
        this->setSize(width, height);
    }

    ColorImage::~ColorImage() {

    }

    void ColorImage::freeMemory() {
        if (image != NULL) {
            free(image);
            image = NULL;
        }
        if (red != NULL) {
            free(red);
            red = NULL;
        }
        if (red != NULL) {
            free(red);
            red = NULL;
        }
        if (blue != NULL) {
            free(blue);
            blue = NULL;
        }
        if (alpha != NULL) {
            free(alpha);
            alpha = NULL;
        }
    }


float ColorImage::getR(Vector2 position) const {
    return image[this->I(position)].r;
}

float ColorImage::getG(Vector2 position) const {
    return image[this->I(position)].g;
}

float ColorImage::getB(Vector2 position) const {
    return image[this->I(position)].b;
}

float ColorImage::getA(Vector2 position) const {
    return image[this->I(position)].a;
}

Color ColorImage::get(Vector2 position) const {
    return image[this->I(position)];
}

void ColorImage::setR(Vector2 position, float value) {
    image[this->I(position)].r = value;
}

void ColorImage::setG(Vector2 position, float value) {
    image[this->I(position)].g = value;
}

void ColorImage::setB(Vector2 position, float value) {
    image[this->I(position)].b = value;
}

void ColorImage::setA(Vector2 position, float value) {
    image[this->I(position)].a = value;
}

void ColorImage::set(Vector2 position, Color value) {
    image[this->I(position)] = value;
}

void ColorImage::setSize(int width, int height) {
    freeMemory();
    this->width = width;
    this->height = height;
    int size = width * height;
    image = (Color*)malloc(size * sizeof(Color));
}


std::string ColorImage::toString() {
    stringstream stream("Color Image (");

    stream << width << "x" << height << " px)";
    return stream.str();
}



}
