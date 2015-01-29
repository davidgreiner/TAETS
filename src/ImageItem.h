#ifndef H_IMAGEITEM
#define H_IMAGEITEM

#include <DataItem.h>
#include <Color.h>
#include <ImageDataHelper.h>
#include <stdexcept>

namespace taets {

/**
 * Diese Klasse stellt eine Referenz auf ein Bild einer ImageSeries dar.
 */
class ImageItem : public DataItem
{
	private:
	unsigned int m_width, m_height;
	
	Color *m_pixels;
	
	public:
	ImageItem(unsigned int _width, unsigned int _height, Color *_pixels) {
		m_width = _width;
		m_height = _height;
		m_pixels = _pixels;
	}
	
    /**
     * Gibt die Breite des Bildes zurück
     */
	unsigned int width() const {
		return m_width;
	}
	
    /**
     * Gibt die Höhe des Bildes zurück
     */
	unsigned int height() const {
		return m_height;
	}

    Color pixel(unsigned int x, unsigned int y) {
        if(x >= m_width || y >= m_height)
            throw std::out_of_range("Pixel out of range");
        return m_pixels[y * m_width + x];
    }

    /**
     * Gibt eine OpenCV-Matrix des Bildes zurück
     */
    cv::Mat createCVMatrix() {
        return ImageDataHelper::createCVMatrix(m_width, m_height, m_pixels);
    }

    /**
     * Gibt eine QPixmap des Bildes zurück
     */
    QPixmap createQPixmap() {
        return ImageDataHelper::createQPixmap(m_width, m_height, m_pixels);
    }
};

} //end namespace taets

#endif //H_IMAGEITEM
