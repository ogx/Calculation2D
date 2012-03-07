#ifndef mmIDH
#define mmIDH

#include <mmGlobalDefs.h>

class mmStringConverters;

class mmID {
public:
	mmID(void) : iID(0) {}
	explicit mmID(mmUInt const p_iID) : iID(p_iID) {} 
	mmID operator ++ (void) {++iID; return *this; }
	bool operator == (mmID const & p_sID) const { return iID == p_sID.iID; }
	bool operator != (mmID const & p_sID) const { return iID != p_sID.iID; }
	bool operator < (mmID const & p_sID) const { return iID < p_sID.iID; }
	mmUInt ancpainalxnkalisxnaosnx_you_cant_type_it_so_you_wont_use_it(void) { return iID; }
public:
	static mmID const invalid;
private:
	mmUInt iID; 
};

#endif