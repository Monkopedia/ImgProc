#include "Image.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    ImgProc::Image image(argv[1]);

    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            Vector2 loc(i, j);
            image.setGrey(loc, 65535 - image.getGrey(loc));
        }
    }
    image.save(argv[2]);

    return 0;
}
