#ifndef H_FLOATITEM
#define H_FLOATITEM

#include <DataItem.h>

namespace taets {

class FloatItem : public DataItem
{
	private:
	float *m_value;
	
	public:
	FloatItem(float *_value) : DataItem() {
		m_value = _value;
	}
	
    /**
     * Gibt den Wert des FloatItems zurück.
     */
	float value() const {
		return *m_value;
	}
	
    /**
     * Setzt den Wert des FloatItems.
     */
	void setValue(float _value) {
		*m_value = _value;
	}
};

} //end namespace taets

#endif //H_FLOATITEM
