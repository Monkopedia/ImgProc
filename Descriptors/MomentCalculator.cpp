#include "MomentCalculator.h"
#include "Image.h"
#include <math.h>

// Moments are only gray for now
namespace ImgProc {

    float MomentCalculator::getMoment(int p, int q) {
        for (unsigned int i = 0; i < momentXs.size(); i++) {
            if ((momentXs[i] == p) && (momentYs[i] == q)) {
                return moments[i];
            }
        }
        float ret = 0;

        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                float value = image->getGrey(pos);
                if (value != 0) {
                    unsigned int x = 1;
                    for (int pc = 0; pc < p; pc++) {
                        x *= pos.x();
                    }
                    unsigned int y = 1;
                    for (int qc = 0; qc < q; qc++) {
                        y *= pos.y();
                    }
                    ret += x * y * value;
                }
            }
        }

        momentXs.push_back(p);
        momentYs.push_back(q);
        moments.push_back(ret);
        return ret;
    }

    float MomentCalculator::getCentralMoment(int p, int q) {
        for (unsigned int i = 0; i < centralMomentXs.size(); i++) {
            if ((centralMomentXs[i] == p) && (centralMomentYs[i] == q)) {
                return centralMoments[i];
            }
        }

        float ret = 0;
        float xbar = getMoment(1, 0) / getMoment(0, 0);
        float ybar = getMoment(0, 1) / getMoment(0, 0);

        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                float value = image->getGrey(pos);
                if (value != 0) {
                    float x = 1;
                    float xv = (pos.x() - xbar);
                    for (int pc = 0; pc < p; pc++) {
                        x *= xv;
                    }
                    float y = 1;
                    float yv = (pos.y() - ybar);
                    for (int qc = 0; qc < q; qc++) {
                        y *= yv;
                    }
                    ret += x * y * value;
                }
            }
        }
         
        centralMomentXs.push_back(p);
        centralMomentYs.push_back(q);
        centralMoments.push_back(ret);
        return ret;
    }

    float MomentCalculator::getNormalMoment(int p, int q) {
        for (unsigned int i = 0; i < normalMomentXs.size(); i++) {
            if ((normalMomentXs[i] == p) && (normalMomentYs[i] == q)) {
                return normalMoments[i];
            }
        }

        float ret = getCentralMoment(p, q);
        float m = getMoment(0, 0);
        float exp = p + q;
        exp /= 2;
        exp++;
        ret = ret / pow(m, exp);

        normalMomentXs.push_back(p);
        normalMomentYs.push_back(q);
        normalMoments.push_back(ret);
        return ret;
    }

}
