#include "Image.h"
#include <cmath>

void Image::lighten() {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            if (getPixel(i,j).l + 0.1 > 1)
                getPixel(i,j).l = 1;
            else
                getPixel(i,j).l += 0.1;
        }
    }
}

void Image::lighten(double amount) {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            if (getPixel(i,j).l + amount > 1)
                getPixel(i,j).l = 1;
            else if (getPixel(i,j).l + amount < 0)
                getPixel(i,j).l = 0;
            else
                getPixel(i,j).l += amount;
        }
    }
}

void Image::darken() {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            if (getPixel(i,j).l - 0.1 < 0)
                getPixel(i,j).l = 0;
            else
                getPixel(i,j).l -= 0.1;
        }
    }
}

void Image::darken(double amount) {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            if (getPixel(i,j).l - amount < 0)
                getPixel(i,j).l = 0;
            else if (getPixel(i,j).l - amount > 1)
                getPixel(i,j).l = 1;
            else
                getPixel(i,j).l -= amount;
        }
    }
}

void Image::saturate() {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            if (getPixel(i,j).s + 0.1 > 1)
                getPixel(i,j).s = 1;
            else
                getPixel(i,j).s += 0.1;
        }
    }
}

void Image::saturate(double amount) {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            if (getPixel(i,j).s + amount > 1)
                getPixel(i,j).s = 1;
            else if (getPixel(i,j).s + amount < 0)
                getPixel(i,j).s = 0;
            else
                getPixel(i,j).s += amount;
        }
    }
}

void Image::desaturate() {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            if (getPixel(i,j).s - 0.1 < 0)
                getPixel(i,j).s = 0;
            else
                getPixel(i,j).s -= 0.1;
        }
    }
}

void Image::desaturate(double amount) {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            if (getPixel(i,j).s - amount < 0)
                getPixel(i,j).s = 0;
            else if (getPixel(i,j).s - amount > 1)
                getPixel(i,j).s = 1;
            else
                getPixel(i,j).s -= amount;
        }
    }
}

void Image::grayscale() {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            getPixel(i,j).s = 0;
        }
    }
}

void Image::rotateColor(double degrees) {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            getPixel(i,j).h += degrees;
            if (degrees < 0 && getPixel(i,j).h < 0)
                getPixel(i,j).h += 360; 
            else if (degrees > 0 && getPixel(i,j).h >= 360)
                getPixel(i,j).h -= 360;
        }
    }
}

void Image::illinify() {
    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
            double orange_dis = std::abs(11 - getPixel(i,j).h);
            double blue_dis = std::abs(216 - getPixel(i,j).h);
            if (orange_dis >= blue_dis) {
                getPixel(i,j).h = 216;
            } else {
                getPixel(i,j).h = 11;
            }
        }
    }
}

void Image::scale(double factor) {
    PNG* oldImage = new PNG(*this);
    resize(width() * factor, height() * factor);

    for (unsigned int i = 0; i < width(); i++) {
        for (unsigned int j = 0; j < height(); j++) {
           int x = std::floor(i / factor);
           int y = std::floor(j / factor);
           getPixel(i, j) = oldImage->getPixel(x, y);
        }
    }

    delete oldImage;
}

void Image::scale(unsigned w, unsigned h) {
    if (w == width() && h == height())
        return;
    else if ((double) w / (double) width() < (double) h / (double) height()) 
        scale((double) w / width());
    else if ((double) h / (double) height() < (double) w / (double) width()) 
        scale((double) h / (double) height());
} 