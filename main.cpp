#include "Image.h"
#include "ColorImage.h"
#include <stdio.h>
#include "Invert.h"
#include "MaskFilter.h"
#include "Color.h"
#include "Server.h"
#include "CommandInterface.h"

using namespace ImgProc;

int main(int argc, char* argv[]) {
    /*
    float m = 1.0 / 9;
    float mask[3][3] = {
        //{ 0, 0, 0},
        //{ 0, 1, 0},
        //{ 0, 0, 0},
        { m, m, m},
        { m, m, m},
        { m, m, m},
    };
    ImgProc::Color c(1, 1, 1);

    printf("Result: %lf\n", c.alphaBlend(32000, 16000, 65535, 65535));


    ImgProc::Image image(argv[1]);
    //ImgProc::ColorImage cimage(image);
    ImgProc::ColorImage cimage(image.getWidth(), image.getHeight());
    ImgProc::MaskFilter maskFilter(mask);

    maskFilter.process(&image, &cimage);

    cimage.save(argv[2]);
    */
    if (argc > 1) {
        CommandInterface c(argc, argv);
        c.runScript();
    } else {
        Server s;
        CommandInterface c(">");
        s.run();
    }

    return 0;
}
