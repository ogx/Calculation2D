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