#ifndef H_TESTDATA
#define H_TESTDATA

#include <DataSeries.h>

#include <map>
#include <vector>
#include <fstream>
#include <boost/serialization/map.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <stdexcept>

namespace taets {

class TestData
{
	private:
    unsigned int m_length;
	std::map<std::string, DataSeries*> m_data;
	
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        (void)version;

        ar & m_length;
		ar & m_data;
	}

    protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    TestData() {
    }
	
	public:
    TestData(unsigned int _length) {
        m_length = _length;
	}
	
	~TestData() {
        std::map<std::string, DataSeries*>::iterator it;
        for(it = m_data.begin(); it != m_data.end(); it++) {
            delete it->second;
        }
	}

    unsigned int length() const {
        return m_length;
    }
	
	void addDataSeries(std::string name, DataSeries *series) {
        if(series->length() != length()) {
            throw std::invalid_argument("DataSeries length does not match TestData length");
        }

		m_data.insert(std::pair<std::string, DataSeries*>(name, series));
    }
	
	DataSeries* dataSeries(std::string name) {
        return m_data.at(name);
    }

    template<class T>
    unsigned int countOfDataSeriesOfType() {
        unsigned int count = 0;

        std::map<std::string, DataSeries*>::iterator it;
        for(it = m_data.begin(); it != m_data.end(); it++) {
            if(typeid(*it->second) == typeid(T)) count++;
        }

        return count;
    }

    template<class T>
    T* dataSeriesOfType(std::string name) {
        DataSeries *series = dataSeries(name);

        if(typeid(*series) != typeid(T)) throw std::invalid_argument("requested series type does not match actual type");

        return static_cast<T*>(series);
    }

    template<class T>
    std::vector<std::string> dataSeriesNamesOfType() {
        std::vector<std::string> names;

        std::map<std::string, DataSeries*>::const_iterator it;
        for(it = m_data.begin(); it != m_data.end(); it++) {
            if(typeid(*it->second) == typeid(T)) names.push_back(it->first);
        }

        return names;
    }

    template<class T>
    std::list<std::pair<std::string, DataSeries*> > dataSeriesInformationOfType() {
        std::list<std::pair<std::string, DataSeries*> > list;

        std::map<std::string, DataSeries*>::iterator it;
        for(it = m_data.begin(); it != m_data.end(); it++) {
            if(typeid(*it->second) == typeid(T))
                list.push_back(std::pair<std::string, DataSeries*>(it->first, it->second));
        }

        return list;
    }

    std::string dataSeriesName(DataSeries *series) {
        std::map<std::string, DataSeries*>::iterator it;
        for(it = m_data.begin(); it != m_data.end(); it++) {
            if(it->second == series) return it->first;
        }
        throw std::runtime_error("DataSeries does not exist in this TestData instance");
    }
	
    void save(std::string filename) const;
	
    static TestData* load(std::string filename);
};

} //end namespace taets

#endif //H_TESTDATA
