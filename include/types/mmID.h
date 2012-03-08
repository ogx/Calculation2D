#ifndef mmIDH
#define mmIDH

#include <types/mmBasic.h>
#include <serialization/mmSerializer.h>

class mmID {
public:
	mmID(void) : iID(0) {}
	explicit mmID(mmUInt const p_iID) : iID(p_iID) {} 
	mmID operator ++ (void) {++iID; return *this; }
	bool operator == (mmID const & p_sID) const { return iID == p_sID.iID; }
	bool operator != (mmID const & p_sID) const { return iID != p_sID.iID; }
	bool operator < (mmID const & p_sID) const { return iID < p_sID.iID; }
	// serialization support
	friend class mmSerializer<mmID>;
public:
	static mmID const invalid;
private:
	mmUInt iID; 
};

#endif