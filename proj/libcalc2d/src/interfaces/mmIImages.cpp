#include <interfaces\mmIImages.h>

#include <limits>

mmUInt const mmImages::mmImageI::iInvalid = std::numeric_limits<mmUInt>::max();
mmUInt const mmImages::mmLayerI::iInvalid = std::numeric_limits<mmUInt>::max();

mmReal const mmImages::mmStats::rInvalid = std::numeric_limits<mmReal>::quiet_NaN();

mmImages::mmStats::mmStats(void) : rMin(rInvalid), rMax(rInvalid), rAverage(rInvalid), rStdDev(rInvalid)
{}

void mmImages::mmStats::Clear(void) {
	rMin = rInvalid;
	rMax = rInvalid;
	rAverage = rInvalid;
	rStdDev = rInvalid;
}

void mmImages::mmStats::Update(mmReal const p_prValues[], mmUInt const p_iSize) {
	if(p_iSize == 0)
		return;

	rAverage = 0.0;
	rStdDev = 0.0;
	rMin = std::numeric_limits<mmReal>::max();
	rMax = -std::numeric_limits<mmReal>::max();

	for(mmUInt v_iI = 0; v_iI < p_iSize; ++v_iI) {
		rAverage += p_prValues[v_iI];
		if(p_prValues[v_iI] < rMin)
			rMin = p_prValues[v_iI];
		if(p_prValues[v_iI] > rMax)
			rMax = p_prValues[v_iI];
	}
	rAverage /= static_cast<mmReal>(p_iSize);

	mmReal v_rTemp;
	for(mmUInt v_iI = 0; v_iI < p_iSize; ++v_iI) {
		v_rTemp = p_prValues[v_iI] - rAverage;
		rStdDev += v_rTemp * v_rTemp;
	}
	rStdDev = ::sqrt(rStdDev / static_cast<mmReal>(p_iSize));
}

const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_ImageName = L"Image name...";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_ImagesNames = L"Image names...";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_ImageCount_All = L"All";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_DataLayerName = L"Data layer name...";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_RealValue = L"Real value...";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_IntegerValue = L"Integer value...";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue = L"Yes/No value...";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES = L"YES";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_NO = L"NO";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_String = L"String value...";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_FileName = L"File name...";

const wchar_t* mmImages::g_pAutoCalcXML_INParams_Node = L"AutoINParams";
const wchar_t* mmImages::g_pAutoCalcXML_OUTParams_Node = L"AutoOUTParams";
const wchar_t* mmImages::g_pAutoCalcXML_Params_Param_Node = L"AutoParam";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamName_Node = L"AutoParamName";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamType_Node = L"AutoParamType";
const wchar_t* mmImages::g_pAutoCalcXML_Params_ParamValue_Node = L"AutoParamValue";
