#pragma once
#include <images\mmImagesStructure.h>
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
	// server interface
	Json::Value GetMethods();
	Json::Value GetStatus();
	Json::Value RunCalculationMethod(Json::Value&);

	// method params conversion
	Json::Value Params_XML2JSON(mmString const & params_xml);
	mmString Params_JSON2XML(Json::Value const & params_json);
	
	// image data encoding/deconding
	Json::Value LayerToJSON(mmImages::mmLayerI const * layer);
	void LayerFromJSON(Json::Value const & json, mmImages::mmLayerI * layer);

	// calculation environment
	mmImages::mmImagesCalculationManagement calc_mgr;
	mmImages::mmImagesCalculationMethodContainerForWindows methods_mgr;
	mmFactories::mmUtilsFactoryI* utils_factory;
	mmImages::mmImageStructure* image_structure;
	mmImages::mmImagesCalculationMethodI* calculation_method;

	// utils
	std::map<std::wstring, mmXML::mmXMLDataType> param_type_lookup;
	Json::Value success_response, failure_response;
	Json::Value FailureResponse(std::wstring const & error);
};

