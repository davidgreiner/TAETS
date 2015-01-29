#ifndef H_DATASERIES
#define H_DATASERIES

#include <DataItem.h>

#include <boost/serialization/access.hpp>
#include <boost/archive/basic_archive.hpp>

namespace taets {

/**
 * Diese Klasse stellt eine vom Programm erzeugte Datenreihe dar. Diese soll nur vom Projekt selbst in der richtigen Länge angelegt werden
 */
class DataSeries
{
	private:
	unsigned int m_length;
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {
        (void)version;

		ar & m_length;
	}
	
	protected:
	/**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
	DataSeries() {
	}
	
	public:
	DataSeries(unsigned int _length) {
		m_length = _length;
	}
	
	virtual ~DataSeries() {
	}
	
    /**
     * Gibt die Länge der Datenserie zurück
     */
	unsigned int length() const {
		return m_length;
	}
	
    /**
     * Gibt eine Referenz auf ein einzelnes Element der Datenserie zurück. Diese muss freigegeben werden, wenn sie nicht mehr gebraucht wird.
     * @param[in] time_index Index des Elements
     */
	virtual DataItem* item(unsigned int _time_index) = 0;
};

} //end namespace taets

#endif //H_DATASERIES
