#ifndef H_IMAGESERIES
#define H_IMAGESERIES

#include <DataSeries.h>
#include <Color.h>
#include <ImageItem.h>

#include <stdexcept>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/archive/basic_archive.hpp>

namespace taets {

class ImageSeries : public DataSeries
{
	private:
	unsigned int m_width, m_height;
	
	Color *m_data;
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void save(Archive &ar, const unsigned int version) const {
        (void)version;

		ar & boost::serialization::base_object<DataSeries>(*this);
		ar & m_width;
		ar & m_height;
		ar.save_binary(m_data, length() * width() * height() * sizeof(Color));
	}
	
	template<class Archive>
	void load(Archive &ar, const unsigned int version) {
        (void)version;

		ar & boost::serialization::base_object<DataSeries>(*this);
		ar & m_width;
		ar & m_height;
		m_data = new Color[length() * width() * height()];
		ar.load_binary(m_data, length() * width() * height() * sizeof(Color));
	}
	
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
	protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
	ImageSeries() : DataSeries() {
	}
	
	public:
	ImageSeries(unsigned int _length, unsigned int _width, unsigned int _height) : DataSeries(_length) {
		m_width = _width;
		m_height = _height;
        m_data = new Color[_length * _width * _height];
	}

    ~ImageSeries() {
        delete[] m_data;
    }
	
    /**
     * Gibt die Breite der Bilder zurück.
     */
	unsigned int width() const {
		return m_width;
	}
	
    /**
     * Gibt die Höhe der Bilder zurück.
     */
	unsigned int height() const {
		return m_height;
	}
	
    /**
     * Gibt eine Referenz auf den i-ten Wert als Bild zurück. Schmeißt eine Ausnahme, wenn der Index invalide ist.
     * @param [in] time_index Indes des Bildes
     */
	ImageItem* item(unsigned int _time_index) {
		if(_time_index >= length()) throw std::out_of_range("time_index too big");
		
		return new ImageItem(width(), height(), m_data + width() * height() * _time_index);
	}

    Color * data() {
        return m_data;
    }

    cv::Mat itemAsCVMatrix(unsigned int _time_index) {
        if(_time_index >= length()) throw std::out_of_range("time_index too big");

        return ImageDataHelper::createCVMatrix(width(), height(), m_data + width() * height() * _time_index);
    }
};

} //end namespace taets

#endif //H_IMAGESERIES
