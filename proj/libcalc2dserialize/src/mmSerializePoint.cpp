#include <serialization/mmSerializePoint.h>

#include <cstdlib>
#include <mmStringUtilities.h>

mmMath::sPoint2D mmSerializer<mmMath::sPoint2D>::FromString(mmString const & p_sString) {
	mmMath::sPoint2D v_sPoint = {0.0, 0.0};
	::swscanf_s(p_sString.c_str(), L"%lf|%lf", &v_sPoint.rX, &v_sPoint.rY);
	return v_sPoint;
}

mmString mmSerializer<mmMath::sPoint2D>::ToString(mmMath::sPoint2D const & p_sValue) {
	return mmStringUtilities::MMRealToString(p_sValue.rX) + L"|" + mmStringUtilities::MMRealToString(p_sValue.rY);
}

