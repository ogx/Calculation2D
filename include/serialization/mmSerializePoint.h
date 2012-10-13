#ifndef serializatrion_mmPoint_H_
#define serializatrion_mmPoint_H_

#include <serialization/mmSerializer.h>
#include <math/mmMath.h>

template<>
class mmSerializer<mmMath::sPoint2D> {
public:
	static mmMath::sPoint2D FromString(mmString const & p_sString);
	static mmString ToString(mmMath::sPoint2D const & p_sValue);
};


#endif