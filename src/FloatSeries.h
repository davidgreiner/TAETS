#ifndef H_FLOATSERIES
#define H_FLOATSERIES

#include <DataSeries.h>
#include <FloatItem.h>

#include <stdexcept>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/archive/basic_archive.hpp>

namespace taets {

class FloatSeries : public DataSeries
{
	private:
    float *m_data;
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void save(Archive &ar, const unsigned int version) const {
        (void)version;

		ar & boost::serialization::base_object<DataSeries>(*this);
        ar.save_binary(m_data, length() * sizeof(float));
	}
	
	template<class Archive>
	void load(Archive &ar, const unsigned int version) {
        (void)version;

		ar & boost::serialization::base_object<DataSeries>(*this);
        m_data = new float[length()];
        ar.load_binary(m_data, length() * sizeof(float));
	}
	
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
	protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
	FloatSeries() : DataSeries() {
	}
	
	public:
	FloatSeries(unsigned int _length) : DataSeries(_length) {
        m_data = new float[length()];
	}
	
	~FloatSeries() {
        delete[] m_data;
	}
	
    /**
     * Gibt eine Referenz auf den i-ten Wert als Fließomma zurück. Schmeißt eine Ausnahme, wenn der Index invalide ist.
     * @param [in] time_index Indes des Fließkommawerts
     */
	FloatItem* item(unsigned int _time_index) {
		if(_time_index >= length()) throw std::out_of_range("time_index too big");
		
        return new FloatItem(m_data + _time_index);
	}

    float itemAsFloat(unsigned int _time_index) {
        if(_time_index >= length()) throw std::out_of_range("time_index too big");

        return m_data[_time_index];
    }

    void setFloatItem(unsigned int _time_index, float _value) {
        if(_time_index >= length()) throw std::out_of_range("time_index too big");

        m_data[_time_index] = _value;
    }
};

} //end namespace taets

#endif //H_FLOATSERIES
