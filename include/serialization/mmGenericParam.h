#ifndef calc2dcmd_mmGenericParam_H_
#define calc2dcmd_mmGenericParam_H_

#include <algorithm>
#include <list>

#include <serialization/mmSerializer.h>

namespace mmImages {
	class mmGenericParamI {
	public:
		enum mmType {mmIntType=0, mmRealType, mmBoolType, mmStringType, mmImageType, mmImageNameType, mmLayerType, mmLayerNameType, mmRectType, mmPointType, mmListType, mmUnknownType};
	public:
		struct FindByName {
			FindByName(mmString const & p_sName) : sName(p_sName) {}
			bool operator () (mmGenericParamI const * const p_psParam) const {
				return p_psParam->GetNameString() == sName;
			}
		private:
			mmString const sName;
		};
	public:
		virtual mmString GetNameString(void) const = 0;
		virtual mmString GetTypeString(void) const = 0;
		virtual mmString GetValueString(void) const = 0;
		virtual void SetValueString(mmString const &) = 0;
		virtual ~mmGenericParamI(void) {}
	};

	template<class param_t>
	class mmGenericParam_base : public mmGenericParamI {
	public:
		mmGenericParam_base(mmString const & p_sName, mmType const p_eType, param_t & p_sValue) : m_sName(p_sName), m_eType(p_eType), m_sValue(p_sValue) {}
		virtual mmString GetTypeString(void) const {
			return ToString(m_eType);
		}
		virtual mmString GetNameString(void) const {
			return m_sName;
		}
		virtual mmString GetValueString(void) const {
			return ToString(m_sValue);
		}
		virtual void SetValueString(mmString const & p_sValue) {
			m_sValue = FromString<param_t>(p_sValue);
		}
	private:
		mmType const m_eType;
		mmString const m_sName;
		param_t & m_sValue;
	};

	template<class param_t>
	void BindParam(std::list<mmGenericParamI*> & p_sParams, mmString const & p_sName, mmGenericParamI::mmType const p_eType, param_t & p_sValue);

	extern mmString SerializeParameters(mmString const & p_sRootName, std::list<mmGenericParamI*> const & p_sParams);
	extern void DeserializeParameters(mmString const & p_sSerializedParams, std::list<mmGenericParamI*> & p_sParams);
};

template<class param_t>
inline void mmImages::BindParam(std::list<mmGenericParamI*> & p_sParams, mmString const & p_sName, mmGenericParamI::mmType const p_eType, param_t & p_sValue) {
	std::list<mmGenericParamI*>::iterator v_sParam;
	if((v_sParam = std::find_if(p_sParams.begin(), p_sParams.end(), mmGenericParamI::FindByName(p_sName))) != p_sParams.end()) {
		delete *v_sParam;
		*v_sParam = new mmGenericParam_base<param_t>(p_sName, p_eType, p_sValue);
	} else
		p_sParams.push_back(new mmGenericParam_base<param_t>(p_sName, p_eType, p_sValue));
}

template<>
class mmSerializer<mmImages::mmGenericParamI::mmType> {
public:
	static mmImages::mmGenericParamI::mmType FromString(mmString const & p_sString);
	static mmString ToString(mmImages::mmGenericParamI::mmType const & p_sValue);
};

#endif