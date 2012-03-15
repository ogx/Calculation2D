#ifndef serialization_mmBasicTypes_H_
#define serialization_mmBasicTypes_H_

#include <mmGlobalDefs.h>
#include <serialization/mmSerializer.h>

template<>
class mmSerializer<mmString> {
public:
	static mmString FromString(mmString const & p_sString);
	static mmString ToString(mmString const & p_sValue);
};


template<>
class mmSerializer<mmInt> {
public:
	static mmInt FromString(mmString const & p_sString);
	static mmString ToString(mmInt const & p_sValue);
};

template<>
class mmSerializer<mmUInt> {
public:
	static mmUInt FromString(mmString const & p_sString);
	static mmString ToString(mmUInt const & p_sValue);
};

template<>
class mmSerializer<mmReal> {
public:
	static mmReal FromString(mmString const & p_sString);
	static mmString ToString(mmReal const & p_sValue);
};

template<>
class mmSerializer<bool> {
public:
	static bool FromString(mmString const & p_sString);
	static mmString ToString(bool const & p_sValue);
};

#endif