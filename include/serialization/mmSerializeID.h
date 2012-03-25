#ifndef serialization_mmID_H_
#define serialization_mmID_H_

#include <serialization/mmSerializer.h>
#include <types/mmID.h>
#include <interfaces/mmIImages.h>

template<>
class mmSerializer<mmID> {
public:
	static mmID FromString(mmString const & p_sString);
	static mmString ToString(mmID const & p_sValue);
};

template<>
class mmSerializer<mmImages::mmLayerI::sID> {
public:
	static mmImages::mmLayerI::sID FromString(mmString const & p_sString);
	static mmString ToString(mmImages::mmLayerI::sID const & p_sValue);
};

#endif