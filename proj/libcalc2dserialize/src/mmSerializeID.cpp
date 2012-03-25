#include <serialization/mmSerializeID.h>

#include <mmStringUtilities.h>

mmID mmSerializer<mmID>::FromString(mmString const & p_sString) {
	mmID v_sID;
	v_sID.iID = mmStringUtilities::StringToMMInt(p_sString);
	return v_sID;
}

mmString mmSerializer<mmID>::ToString(mmID const & p_sValue) {
	return mmStringUtilities::MMIntToString(p_sValue.iID);
}

mmImages::mmLayerI::sID mmSerializer<mmImages::mmLayerI::sID>::FromString(mmString const & p_sString) {
	size_t delim_pos = p_sString.find(L"|");
	mmImages::mmLayerI::sID v_sID = 
	{
		mmSerializer<mmID>::FromString(p_sString.substr(0, delim_pos)),
		mmSerializer<mmID>::FromString(p_sString.substr(delim_pos+1))
	};
	return v_sID;
}

mmString mmSerializer<mmImages::mmLayerI::sID>::ToString(mmImages::mmLayerI::sID const & p_sValue) {
	return mmSerializer<mmID>::ToString(p_sValue.m_sImageID) + 
	       L"|" + 
	       mmSerializer<mmID>::ToString(p_sValue.m_sLayerID);
}