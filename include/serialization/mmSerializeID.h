#ifndef serialization_mmID_H_
#define serialization_mmID_H_

#include <serialization/mmSerializer.h>
#include <types/mmID.h>

template<>
class mmSerializer<mmID> {
public:
	static mmID FromString(mmString const & p_sString);
	static mmString ToString(mmID const & p_sValue);
};

#endif