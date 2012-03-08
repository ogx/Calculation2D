#ifndef serialization_mmSerializeList_H_
#define serialization_mmSerializeList_H_

#include <serialization/mmSerializer.h>

#include <list>

template<class val_t>
class mmSerializer<std::list<val_t>> {
public:
	static std::list<val_t> FromString(mmString const & p_sString)
	{
		std::basic_stringstream<mmString::value_type, mmString::traits_type> v_sStream(p_sString);
		std::list<val_t> v_sItems;
		mmString v_sItemString;
		do {
			std::getline(v_sStream, v_sItemString, L'|');
			v_sItems.push_back(mmSerializer<val_t>::FromString(v_sItemString));
		} while( ! v_sStream.eof());
		return v_sItems;
	}
	static mmString ToString(std::list<val_t> const & p_sValue) {
		if(p_sValue.empty())
			return L"";
		
		mmString v_sString = mmSerializer<val_t>::ToString(p_sValue);
		for(std::list<val_t>::const_iterator v_sItem = ++p_sValue.begin(), v_sItem != p_sValue.end(); ++v_sItem)
			v_sString += L"|" + mmSerializer<val_t>::ToString(*p_sValue);
		return v_sString;
	}
};

#endif