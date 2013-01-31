#include "Image.h"
#include <wand/MagickWand.h>
#include <string.h>
#include <stdio.h>


namespace ImgProc {

    Image::Image(const char* file) {
        red = NULL;
        blue = NULL;
        green = NULL;
        alpha = NULL;
        loadFile(file);
    }

    void Image::loadFile(const char* file) {
        int y;

        MagickBooleanType status;
        MagickWand *image_wand;
        MagickPixelPacket pixel;
        PixelIterator *iterator;
        PixelWand **pixels;

        register int x;
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
        this->setSize(width, height);
        for (y=0; y < height; y++) {
            pixels = PixelGetNextIteratorRow(iterator, &w);
            if (pixels == (PixelWand **) NULL) {
                break;
            }
            for (x=0; x < (long) width; x++) {
                PixelGetMagickColor(pixels[x],&pixel);
                Color c(pixel.red, pixel.green, pixel.blue, pixel.opacity);
                Vector2 pos(y, x);
                this->set(pos, c);
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
        alpha = NULL;
        setSize(base.width, base.height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                this->set(pos, base.get(pos));
            }
        }
    }

    Image::Image(int width, int height) {
        red = NULL;
        blue = NULL;
        green = NULL;
        alpha = NULL;
        Color c(0, 0, 0);
        this->setSize(width, height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                this->set(pos, c);
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
                pixel.red = this->getR(loc);
                pixel.green = this->getG(loc);
                pixel.blue = this->getB(loc);
                pixel.opacity = this->getA(loc);
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

    inline int Image::I(int x, int y) const {
        if (x < 0) x = 0;
        else if (x >= height) x = height - 1;
        if (y < 0) y = 0;
        else if (y >= width) y = width - 1;
        return ((x * width) + y);
    }

    float Image::getGrey(Vector2 position) const {
        float sum = this->getR(position);
        sum += this->getG(position);
        sum += this->getB(position);
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
            return this->getGrey(position);
        case RED:
            return this->getR(position);
        case GREEN:
            return this->getG(position);
        case BLUE:
            return this->getB(position);
        case ALPHA:
            return this->getA(position);
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
            this->setGrey(position, value);
        case RED:
            this->setR(position, value);
        case GREEN:
            this->setG(position, value);
        case BLUE:
            this->setB(position, value);
        case ALPHA:
            this->setA(position, value);
        }
    }

    void Image::set(Vector2 position, Color value) {
        this->setR(position, value.r);
        this->setG(position, value.g);
        this->setB(position, value.b);
        this->setA(position, value.a);
    }

    void Image::freeMemory() {
        if (red != NULL) {
            free(red);
            red = NULL;
        }
        if (blue != NULL) {
            free(blue);
            blue = NULL;
        }
        if (green != NULL) {
            free(green);
            green = NULL;
        }
        if (alpha != NULL) {
            free(alpha);
            alpha = NULL;
        }
    }

    void Image::setSize(int width, int height) {
        this->freeMemory();
        this->width = width;
        this->height = height;
        int size = width * height;
        red = (float*)malloc(size * sizeof(float));
        blue = (float*)malloc(size * sizeof(float));
        green = (float*)malloc(size * sizeof(float));
        alpha = (float*)malloc(size * sizeof(float));
    }

    Image Image::operator+(Image& base) {
        Image temp(getWidth(), getHeight());
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                temp.set(pos, get(pos) + base.get(pos));
            }
        }
        return temp;
    }

    Image Image::operator-(Image& base) {
        Image temp(width, height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                temp.set(pos, this->get(pos) - base.get(pos));
            }
        }
        return temp;
    }

    Image &Image::operator=(Image& base) {
        this->setSize(base.width, base.height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                this->set(pos, base.get(pos));
            }
        }
        return *this;
    }

    Image Image::operator+(Color& base) {
        Image temp(getWidth(), getHeight());
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                temp.set(pos, this->get(pos) + base);
            }
        }
        return temp;
    }

    Image Image::operator-(Color& base) {
        Image temp(width, height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                temp.set(pos, this->get(pos) - base);
            }
        }
        return temp;
    }

    Image &Image::operator=(Color& base) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Vector2 pos(i, j);
                set(pos, base);
            }
        }
        return *this;
    }
}
