#include <serialization/mmSerializeRect.h>

#include <mmStringUtilities.h>

mmRect mmSerializer<mmRect>::FromString(mmString const & p_sString) {
	mmRect v_sRect;
	::swscanf_s(p_sString.c_str(), L"%d|%d|%d|%d", &v_sRect.iLeft, &v_sRect.iTop, &v_sRect.iWidth, &v_sRect.iHeight);
	return v_sRect;
}

mmString mmSerializer<mmRect>::ToString(mmRect const & p_sValue) {
	return mmStringUtilities::MMIntToString(p_sValue.iLeft) + L"|" + mmStringUtilities::MMIntToString(p_sValue.iTop) + L"|" + mmStringUtilities::MMIntToString(p_sValue.iWidth) + L"|" + mmStringUtilities::MMIntToString(p_sValue.iHeight);
}

