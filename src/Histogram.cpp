#include "Histogram.h"

#include <cstring>
#include <cmath>

namespace taets {

void Histogram::recalculate() {
    if(!_image) return;

    memset(_values, 0, sizeof(HistogramValue) * 256);
    _max_r = _max_g = _max_b = _max_l = 0;

    unsigned int x, y;
    for(y = 0; y < _image->height(); y++) {
        for(x = 0; x < _image->width(); x++) {
            Color c = _image->pixel(x, y);

            unsigned int red_index = static_cast<unsigned int>(c.redf() * 255);
            unsigned int green_index = static_cast<unsigned int>(c.greenf() * 255);
            unsigned int blue_index = static_cast<unsigned int>(c.bluef() * 255);
            unsigned int luminance_index = static_cast<unsigned int>(c.valuef() * 255);

            _values[red_index].r++;
            if(_values[red_index].r > _max_r) _max_r = _values[red_index].r;

            _values[green_index].g++;
            if(_values[green_index].g > _max_g) _max_g = _values[green_index].g;

            _values[blue_index].b++;
            if(_values[blue_index].b > _max_b) _max_b = _values[blue_index].b;

            _values[luminance_index].l++;
            if(_values[luminance_index].l > _max_l) _max_l = _values[luminance_index].l;
        }
    }
}

} //end namespace taets
