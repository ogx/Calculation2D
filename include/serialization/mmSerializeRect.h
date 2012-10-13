#ifndef serialization_mmRect_H_
#define serialization_mmRect_H_

#include <types/mmRect.h>
#include <serialization/mmSerializer.h>

template<>
class mmSerializer<mmRect> {
public:
	static mmRect FromString(mmString const & p_sString);
	static mmString ToString(mmRect const & p_sValue);
};

#endif