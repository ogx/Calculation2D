#ifndef calc2dcmd_mmParam_H_
#define calc2dcmd_mmParam_H_

#include <interfaces/mmIImages.h>
#include <serialization/mmGenericParam.h>

namespace mmCommands {
	struct mmParam {
	public:
		enum mmType {mmTypeMethod, mmTypeInt, mmTypeReal, mmTypeString, mmTypeBool, mmTypeImage, mmTypeLayer, mmTypeFile, mmTypeUnknown};
	public:
		mmParam(void) : eType(mmTypeUnknown) {}
		mmParam(std::wstring const & p_sName, mmType const p_eType, bool const p_bOptional) : eType(p_eType), sName(p_sName), bOptional(p_bOptional) {}
	public:
		mmType eType;
		std::wstring sName;
		std::wstring sValue;
		bool bOptional;
	};

	mmParam::mmType TranslateType(mmImages::mmGenericParamI::mmType);
	bool TryConvert(mmParam const & p_sParam, std::wstring const & p_sPossibleValue, mmImages::mmImageStructureI const * const p_psImageStructure);
};

#endif