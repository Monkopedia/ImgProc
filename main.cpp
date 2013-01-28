#include "Image.h"
#include <stdio.h>
#include "Invert.h"

int main(int argc, char* argv[]) {
    ImgProc::Image image(argv[1]);
    ImgProc::Image* proced;
    ImgProc::Invert inverter;
    ImgProc::Color color(0, 0, 0, 0);

//    proced = inverter.process(&image);
    image = (image + color);
    proced = &image;

    proced->save(argv[2]);

    delete proced;

    return 0;
}
