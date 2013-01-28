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
//#define QuantumScale  ((MagickRealType) 1.0/(MagickRealType) QuantumRange)
#define SigmoidalContrast(x) \
    (QuantumRange*(1.0/(1+exp(10.0*(0.5-QuantumScale*x)))-0.0066928509)*1.0092503)
#define ThrowWandException(wand) \
    { \
        char \
        *description; \
        \
        ExceptionType \
        severity; \
        \
        description=MagickGetException(wand,&severity); \
        (void) fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description); \
        description=(char *) MagickRelinquishMemory(description); \
        exit(-1); \
    }

    long y;

    MagickBooleanType status;
    MagickWand *image_wand;
    MagickPixelPacket pixel;
    PixelIterator *iterator;
    PixelWand **pixels;

    register long x;
    size_t w;

    //if (argc != 3) {
        //(void) fprintf(stdout,"Usage: %s image sigmoidal-image\n",argv[0]);
        //exit(0);
    //}
    /*
       Read an image.
       */
    MagickWandGenesis();
    image_wand=NewMagickWand();
    status=MagickReadImage(image_wand,file);
    if (status == MagickFalse) {
        ThrowWandException(image_wand);
    }
    /*
       Sigmoidal non-linearity contrast control.
       */
    iterator=NewPixelIterator(image_wand);
    if ((iterator == (PixelIterator *) NULL)) {
        ThrowWandException(image_wand);
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
        }
    }
    if (y < (long) MagickGetImageHeight(image_wand)) {
        ThrowWandException(image_wand);
    }
    iterator=DestroyPixelIterator(iterator);
    image_wand=DestroyMagickWand(image_wand);
    /*
       Write the image then destroy it.
       */
    if (status == MagickFalse) {
        ThrowWandException(image_wand);
    }
    MagickWandTerminus();
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
           PixelGetMagickColor(pixels[x],&pixel);
           pixel.red = red[I(y, x)];
           pixel.green = green[I(y, x)];
           pixel.blue = blue[I(y, x)];
           PixelSetMagickColor(pixels[x],&pixel);
       }
       (void) PixelSyncIterator(iterator);
   }
   if (y < (long) MagickGetImageHeight(writeWand)) {
       ThrowWandException(writeWand);
   }
   MagickBooleanType status=MagickWriteImages(writeWand, file, MagickTrue);
   if (status == MagickFalse)
       ThrowWandException(writeWand);
   iterator=DestroyPixelIterator(iterator);

   writeWand = DestroyMagickWand(writeWand);
   MagickWandTerminus();
}
float Image::getGrey(Vector2 position) {
    float sum = getR(position);
    sum += getG(position);
    sum += getB(position);
    return sum / 3;
}

float Image::getR(Vector2 position) {
    return red[I(position.x(), position.y())];
}

float Image::getG(Vector2 position) {
    return green[I(position.x(), position.y())];
}

float Image::getB(Vector2 position) {
    return blue[I(position.x(), position.y())];
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
}

}
