#ifndef H_COLOR
#define H_COLOR

#include <algorithm>
#include <boost/serialization/access.hpp>

namespace taets {

class Color
{
	private:
    unsigned char m_rgb[3];

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & m_rgb;
    }
	
	public:
	Color() {
        m_rgb[0] = m_rgb[1] = m_rgb[2] = 0;
	}
	
    Color(unsigned char _luminance) {
		m_rgb[0] = m_rgb[1] = m_rgb[2] = _luminance;
	}
	
    Color(unsigned char _rgb[3]) {
		m_rgb[0] = _rgb[0];
		m_rgb[1] = _rgb[1];
		m_rgb[2] = _rgb[2];
	}

    Color(unsigned char _r, unsigned char _g, unsigned char _b) {
		m_rgb[0] = _r;
		m_rgb[1] = _g;
		m_rgb[2] = _b;
	}

    static Color fromRGBf(float _r, float _g, float _b) {
        return Color(_r * 255.0, _g * 255.0, _b * 255.0);
    }

    /**
     * Gibt den Rotwert der Farbe zurück
     */
    unsigned char red() const {
		return m_rgb[0];
	}

    /**
     * Gibt den Grünwert der Color zurück
     */
    unsigned char green() const {
		return m_rgb[1];
	}
	
    /**
     * Gibt den Blauwert der Farbe zurück
     */
    unsigned char blue() const {
		return m_rgb[2];
	}

    /**
     * Gibt den Rotwert der Farbe als float zurück
     */
    float redf() {
        return red() / 255.0;
    }

    /**
     * Gibt den Grünwert der Farbe als float zurück
     */
    float greenf() {
        return green() / 255.0;
    }

    /**
     * Gibt den Blauwert der Farbe als float zurück
     */
    float bluef() {
        return blue() / 255.0;
    }

    /**
     * Gibt den Grauwert der Farbe zurück.
     */
    unsigned char value() {
        return std::max(std::max(red(), green()), blue());
    }

    /**
     * Gibt den Grauwert der Farbe als float zurück.
     */
    float valuef() {
        return value() / 255.0;
    }
	
    /**
     * Setzt den Rotwert der Farbe
     */
    void setRed(unsigned char _r) {
		m_rgb[0] = _r;
	}
	
    /**
     * Setzt den Grünwert der Farbe
     */
    void setGreen(unsigned char _g) {
		m_rgb[1] = _g;
	}
	
    /**
     * Setzt den Blauwert der Farbe
     */
    void setBlue(unsigned char _b) {
		m_rgb[2] = _b;
	}

    /**
     * Setzt den Rotwert der Farbe per float Variable
     */
    void setRedf(float _r) {
        setRed(_r * 255.0);
    }

    /**
     * Setzt den Grünwert der Farbe per float Variable
     */
    void setGreenf(float _r) {
        setGreen(_r * 255.0);
    }

    /**
     * Setzt den Blauwert der Farbe per float Variable
     */
    void setBluef(float _r) {
        setBlue(_r * 255.0);
    }

};

} //end namespace TAETS

inline bool operator==(const taets::Color& l, const taets::Color& r) {
    return l.red() == r.red() && l.green() == r.green() && l.blue() == r.blue();
}

inline bool operator!=(const taets::Color& l, const taets::Color& r) {
    return !(l == r);
}

#endif //H_COLOR
