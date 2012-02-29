#pragma once
#include <images\mmImagesCalculationManagement.h>
#include <images\mmImagesCalculationMethodContainerWindows.h>
#include <mmXMLIOUtilities.h>
#include "json\json.h"
#include <map>

class mmCalculationServer
{
public:
	mmCalculationServer(void);
	~mmCalculationServer(void);

	int Serve();

private: 
	Json::Value GetMethods();
	Json::Value GetStatus();
	Json::Value RunCalculationMethod(Json::Value&);

	Json::Value Params_XML2JSON(mmString const & params_xml);
	mmString Params_JSON2XML(Json::Value const & params_json);

	mmImages::mmImagesCalculationManagement calc_mgr;
	mmImages::mmImagesCalculationMethodContainerForWindows methods_mgr;
	mmFactories::mmUtilsFactoryI* utils_factory;
	mmImages::mmImageStructureI* image_structure;
	mmImages::mmImagesCalculationMethodI* calculation_method;

	std::map<std::wstring, mmXML::mmXMLDataType> param_type_lookup;
	Json::Value success_response, failure_response;

	Json::Value FailureResponse(std::wstring const & error);
};

