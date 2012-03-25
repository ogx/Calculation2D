#pragma once
#include <images\mmImagesStructure.h>
#include <images\mmImagesCalculationManagement.h>
#include <images\mmImagesCalculationMethodContainerWindows.h>
#include <serialization\mmGenericParam.h>
#include "json\json.h"
#include <map>
#include <set>

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
	typedef std::set<mmID> LayersToWrap;
	struct ImageToWrap 
	{
		LayersToWrap layers;
	};
	typedef std::map<mmID, ImageToWrap> ImagesToWrap;
	Json::Value Params_XML2JSON(
		mmString const & params_xml, 
		ImagesToWrap* images_to_wrap = NULL
		) const;
	mmString Params_JSON2XML(Json::Value const & params_json) const;
	
	// image data encoding/deconding
	Json::Value LayerToJSON(mmImages::mmLayerI const * layer) const;
	void LayerFromJSON(Json::Value const & json, mmImages::mmLayerI * layer) const;

	// calculation environment
	mmImages::mmImagesCalculationManagement calc_mgr;
	mmImages::mmImagesCalculationMethodContainerForWindows methods_mgr;
	mmFactories::mmUtilsFactoryI* utils_factory;
	mmImages::mmImageStructure* image_structure;
	mmImages::mmImagesCalculationMethodI* calculation_method;

	// utils
	std::map<std::wstring, mmImages::mmGenericParamI::mmType> param_type_lookup;
	Json::Value success_response, failure_response;
	Json::Value FailureResponse(std::wstring const & error) const;
	Json::Value WrapResults(
		mmImages::mmImageStructure const * image_structure,
		mmString const & output_params
		) const;
	void UnwrapArguments(
		Json::Value const & params_json,
		mmImages::mmImageStructure * image_structure,
		mmString & input_params
		) const;
};

