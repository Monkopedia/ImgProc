#include "ColorImage.h"

#include <stdio.h>

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
    return image[I(position.x(), position.y())].r;
}

float ColorImage::getG(Vector2 position) const {
    return image[I(position.x(), position.y())].g;
}

float ColorImage::getB(Vector2 position) const {
    return image[I(position.x(), position.y())].b;
}

float ColorImage::getA(Vector2 position) const {
    return image[I(position.x(), position.y())].a;
}

Color ColorImage::get(Vector2 position) const {
    return image[I(position.x(), position.y())];
}

void ColorImage::setR(Vector2 position, float value) {
    image[I(position.x(), position.y())].r = value;
}

void ColorImage::setG(Vector2 position, float value) {
    image[I(position.x(), position.y())].g = value;
}

void ColorImage::setB(Vector2 position, float value) {
    image[I(position.x(), position.y())].b = value;
}

void ColorImage::setA(Vector2 position, float value) {
    image[I(position.x(), position.y())].a = value;
}

void ColorImage::set(Vector2 position, Color value) {
    image[I(position.x(), position.y())] = value;
}

void ColorImage::setSize(int width, int height) {
    freeMemory();
    this->width = width;
    this->height = height;
    int size = width * height;
    image = (Color*)malloc(size * sizeof(Color));
}






}
