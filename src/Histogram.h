#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "ImageItem.h"

namespace taets {

/**
 * Berechnet RGB- und Helligkeitshistogramme für ein ImageItem.
 */
class Histogram
{
public:
    Histogram() {
        _image = nullptr;
    }

    ~Histogram() {
        if(_image) delete _image;
    }

    ImageItem *image() {
        return _image;
    }

    /**
     * Das ImageItem wird automatisch gelöscht.
     */
    void setImage(ImageItem *image) {
        if(_image != image) {
            if(_image) delete _image;
            _image = image;
            recalculate();
        }
    }

    unsigned int redValue(unsigned int index) {
        if(index > 255)
            throw std::out_of_range("Histogram index out of range");
        return _values[index].r;
    }

    unsigned int greenValue(unsigned int index) {
        if(index > 255)
            throw std::out_of_range("Histogram index out of range");
        return _values[index].g;
    }

    unsigned int blueValue(unsigned int index) {
        if(index > 255)
            throw std::out_of_range("Histogram index out of range");
        return _values[index].b;
    }

    unsigned int luminanceValue(unsigned int index) {
        if(index > 255)
            throw std::out_of_range("Histogram index out of range");
        return _values[index].l;
    }

    unsigned int maxRedValue() {
        return _max_r;
    }

    unsigned int maxGreenValue() {
        return _max_g;
    }

    unsigned int maxBlueValue() {
        return _max_b;
    }

    unsigned int maxLuminanceValue() {
        return _max_l;
    }

private:
    void recalculate();

    ImageItem *_image;

    struct HistogramValue {
        unsigned int r, g, b, l;
    } _values[256];
    unsigned int _max_r, _max_g, _max_b, _max_l;
};

} //end namespace taets

#endif // HISTOGRAM_H
