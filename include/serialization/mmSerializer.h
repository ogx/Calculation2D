#ifndef serialization_mmSerializer_H_
#define serialization_mmSerializer_H_

#include <types/mmBasic.h>

template<class value_t>
class mmSerializer {
public:
	static value_t FromString(mmString const & p_sString);
	static mmString ToString(value_t const & p_sValue);
};

template<class value_t>
value_t FromString(mmString const & p_sString) {
	return mmSerializer<value_t>::FromString(p_sString);
};

template<class value_t>
mmString ToString(value_t const & p_sValue) {
	return mmSerializer<value_t>::ToString(p_sValue);
};

#endif