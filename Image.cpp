#include "Image.h"
#include <wand/MagickWand.h>
#include <string.h>
#include <stdio.h>

#define I(x, y) ((x * width) + y)

namespace ImgProc {

    Image::Image(const char* file) {
        red = NULL;
        blue = NULL;
        green = NULL;
        long y;

        MagickBooleanType status;
        MagickWand *image_wand;
        MagickPixelPacket pixel;
        PixelIterator *iterator;
        PixelWand **pixels;

        register long x;
        size_t w;

        MagickWandGenesis();
        image_wand=NewMagickWand();
        status=MagickReadImage(image_wand,file);
        if (status == MagickFalse) {
            showError(image_wand);
        }
        iterator=NewPixelIterator(image_wand);
        if ((iterator == (PixelIterator *) NULL)) {
            showError(image_wand);
        }
        height = MagickGetImageHeight(image_wand);
        width = MagickGetImageWidth(image_wand);
        setSize(width, height);
        for (y=0; y < height; y++) {
            pixels = PixelGetNextIteratorRow(iterator, &w);
            if (pixels == (PixelWand **) NULL) {
                break;
            }
            for (x=0; x < (long) width; x++) {
                PixelGetMagickColor(pixels[x],&pixel);
                red[I(y, x)] = pixel.red;
                green[I(y, x)] = pixel.green;
                blue[I(y, x)] = pixel.blue;
                alpha[I(y, x)] = pixel.opacity;
            }
        }
        if (y < (long) MagickGetImageHeight(image_wand)) {
            showError(image_wand);
        }
        iterator=DestroyPixelIterator(iterator);
        image_wand=DestroyMagickWand(image_wand);
        if (status == MagickFalse) {
            showError(image_wand);
        }
        MagickWandTerminus();
    }

    Image::Image(const ImgProc::Image& base) {
        red = NULL;
        blue = NULL;
        green = NULL;
        setSize(base.width, base.height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                this->setR(pos, base.getR(pos));
                this->setB(pos, base.getB(pos));
                this->setG(pos, base.getG(pos));
            }
        }
    }

    Image::Image(int width, int height) {
        red = NULL;
        blue = NULL;
        green = NULL;
        setSize(width, height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                this->setR(pos, 0);
                this->setB(pos, 0);
                this->setG(pos, 0);
            }
        }
    }

    void Image::showError(MagickWand* wand) { 
        char *description; 

        ExceptionType severity; 

        description=MagickGetException(wand,&severity); 
        fprintf(stderr, "%s %s %lu %sn", GetMagickModule(), description); 
        description=(char *) MagickRelinquishMemory(description); 
        exit(-1); 
    }


    //Image::Image(std::string file) {
    //}

    Image::~Image() {
        freeMemory();
    }

    void Image::save(const char* file) {
        MagickWand* writeWand;
        MagickPixelPacket pixel;
        PixelIterator *iterator;
        PixelWand *pWand = NULL;
        PixelWand **pixels;

        MagickWandGenesis();
        writeWand = NewMagickWand();
        if (writeWand == NULL) {
            printf("WTF\n"); 
        }

        pWand = NewPixelWand();
        PixelSetColor(pWand, "white");
        MagickNewImage(writeWand, width, height, pWand);
        //MagickSetFormat(writeWand, "PNG");
        //MagickSetSize(writeWand, width, height);
        int x, y;
        size_t w;
        int height = MagickGetImageHeight(writeWand);
        int width = MagickGetImageWidth(writeWand);
        iterator=NewPixelIterator(writeWand);
        for (y=0; y < height; y++) {
            pixels = PixelGetNextIteratorRow(iterator, &w);
            if (pixels == (PixelWand **) NULL) {
                break;
            }
            for (x=0; x < (long) width; x++) {
                Vector2 loc(y, x);
                PixelGetMagickColor(pixels[x],&pixel);
                pixel.red = getR(loc);
                pixel.green = getG(loc);
                pixel.blue = getB(loc);
                pixel.opacity = getA(loc);
                PixelSetMagickColor(pixels[x],&pixel);
            }
            (void) PixelSyncIterator(iterator);
        }
        if (y < (long) MagickGetImageHeight(writeWand)) {
            showError(writeWand);
        }
        MagickBooleanType status=MagickWriteImages(writeWand, file, MagickTrue);
        if (status == MagickFalse)
            showError(writeWand);
        iterator=DestroyPixelIterator(iterator);

        writeWand = DestroyMagickWand(writeWand);
        MagickWandTerminus();
    }
    float Image::getGrey(Vector2 position) const {
        float sum = getR(position);
        sum += getG(position);
        sum += getB(position);
        return sum / 3;
    }

    float Image::getR(Vector2 position) const {
        return red[I(position.x(), position.y())];
    }

    float Image::getG(Vector2 position) const {
        return green[I(position.x(), position.y())];
    }

    float Image::getB(Vector2 position) const {
        return blue[I(position.x(), position.y())];
    }

    float Image::getA(Vector2 position) const {
        return alpha[I(position.x(), position.y())];
    }

    float Image::get(Vector2 position, int channel) const {
        switch (channel) {
        case GRAY:
            return getGrey(position);
        case RED:
            return getR(position);
        case GREEN:
            return getG(position);
        case BLUE:
            return getB(position);
        case ALPHA:
            return getA(position);
        }
        return 0;
    }

    Color Image::get(Vector2 position) const {
        Color c(getR(position), getG(position), getB(position), getA(position));
        return c;
    }


    void Image::setGrey(Vector2 position, float value) {
        setR(position, value);
        setG(position, value);
        setB(position, value);
    }

    void Image::setR(Vector2 position, float value) {
        red[I(position.x(), position.y())] = value;
    }

    void Image::setG(Vector2 position, float value) {
        green[I(position.x(), position.y())] = value;
    }

    void Image::setB(Vector2 position, float value) {
        blue[I(position.x(), position.y())] = value;
    }

    void Image::setA(Vector2 position, float value) {
        alpha[I(position.x(), position.y())] = value;
    }

    void Image::set(Vector2 position, int channel, float value) {
        switch (channel) {
        case GRAY:
            setGrey(position, value);
        case RED:
            setR(position, value);
        case GREEN:
            setG(position, value);
        case BLUE:
            setB(position, value);
        case ALPHA:
            setA(position, value);
        }
    }

    void Image::set(Vector2 position, Color value) {
        setR(position, value.r);
        setG(position, value.g);
        setB(position, value.b);
        setA(position, value.a);
    }

    void Image::freeMemory() {
        if (red != NULL) {
            free(red);
        }
        if (blue != NULL) {
            free(blue);
        }
        if (green != NULL) {
            free(green);
        }
    }

    void Image::setSize(int width, int height) {
        freeMemory();
        this->width = width;
        this->height = height;
        int size = width * height;
        red = (float*)malloc(size * sizeof(float));
        blue = (float*)malloc(size * sizeof(float));
        green = (float*)malloc(size * sizeof(float));
        alpha = (float*)malloc(size * sizeof(float));
    }

    Image Image::operator+(Image base) {
        Image temp(getWidth(), getHeight());
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                temp.set(pos, get(pos) + base.get(pos));
            }
        }
        return temp;
    }

    Image Image::operator-(Image base) {
        Image temp(width, height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                temp.set(pos, get(pos) - base.get(pos));
            }
        }
        return temp;
    }

    Image &Image::operator=(Image base) {
        setSize(base.width, base.height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                set(pos, base.get(pos));
            }
        }
        return *this;
    }

    Image Image::operator+(Color base) {
        Image temp(getWidth(), getHeight());
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                temp.set(pos, get(pos) + base);
            }
        }
        return temp;
    }

    Image Image::operator-(Color base) {
        Image temp(width, height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                temp.set(pos, get(pos) - base);
            }
        }
        return temp;
    }

    Image &Image::operator=(Color base) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                set(pos, base);
            }
        }
        return *this;
    }
}
