#ifndef calc2dcmd_mmGenericParam_H_
#define calc2dcmd_cmGenericParam_H_

#include <algorithm>
#include <list>

#include <interfaces/mmIXML.h>

#include <mmStringUtilities.h>
#include <mmXMLIOUtilities.h>

namespace mmImages {
	class mmGenericParamI {
	public:
		enum mmType {mmIntType=0, mmRealType, mmBoolType, mmStringType, mmImageType, mmImageNameType, mmLayerType, mmLayerNameType, mmRectType, mmPointType, mmUnknownType};
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

	// to remove with mmXMLIOUtilities
	extern mmGenericParamI::mmType GetTypeTransition(mmXML::mmXMLDataType const p_eType);
	extern mmXML::mmXMLDataType GetTypeTransition(mmGenericParamI::mmType const p_eType);

	template<class param_t> 
	inline param_t FromString(mmString const & p_sString);

	template<>
	extern mmString FromString<mmString>(mmString const & p_sString);
	template<>
	extern mmInt FromString<mmInt>(mmString const & p_sString);
	template<>
	extern mmReal FromString<mmReal>(mmString const & p_sString);
	template<>
	extern bool FromString<bool>(mmString const & p_sString);
	template<>
	extern mmRect FromString<mmRect>(mmString const & p_sString);
	template<>
	extern mmMath::sPoint2D FromString<mmMath::sPoint2D>(mmString const & p_sString);
	template<>
	extern mmGenericParamI::mmType FromString<mmGenericParamI::mmType>(mmString const & p_sString);

	template<class param_t>
	inline mmString ToString(param_t const & p_sValue);

	template<>
	extern mmString ToString<mmString>(mmString const & p_sValue);
	template<>
	extern mmString ToString<mmInt>(mmInt const & p_sValue);
	template<>
	extern mmString ToString<mmReal>(mmReal const & p_sValue);
	template<>
	extern mmString ToString<bool>(bool const & p_sValue);
	template<>
	extern mmString ToString<mmRect>(mmRect const & p_sValue);
	template<>
	extern mmString ToString<mmMath::sPoint2D>(mmMath::sPoint2D const & p_sValue);
	template<>
	extern mmString ToString<mmGenericParamI::mmType>(mmGenericParamI::mmType const & p_sValue);

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
	inline void BindParam(std::list<mmGenericParamI*> & p_sParams, mmString const & p_sName, mmGenericParamI::mmType const p_eType, param_t & p_sValue) {
		std::list<mmGenericParamI*>::iterator v_sParam;
		if((v_sParam = std::find_if(p_sParams.begin(), p_sParams.end(), mmGenericParamI::FindByName(p_sName))) != p_sParams.end()) {
			delete *v_sParam;
			*v_sParam = new mmGenericParam_base<param_t>(p_sName, p_eType, p_sValue);
		} else
			p_sParams.push_back(new mmGenericParam_base<param_t>(p_sName, p_eType, p_sValue));
	}

	extern void Serialize(mmGenericParamI const * const p_psParam, mmXML::mmXMLNodeI* const p_psOutputXML);	
	extern mmString SerializeParameters(mmString const & p_sRootName, std::list<mmGenericParamI*> const & p_sParams);
	extern bool Deserialize(mmXML::mmXMLNodeI* const p_psInputXML, mmGenericParamI * const p_psParam);
	extern void DeserializeParameters(mmString const & p_sSerializedParams, std::list<mmGenericParamI*> & p_sParams);
};

#endif