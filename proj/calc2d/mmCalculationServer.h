#pragma once
#include <images\mmImagesStructure.h>
#include <images\mmImagesCalculationManagement.h>
#include <images\mmImagesCalculationMethodContainerWindows.h>
#include <serialization\mmGenericParam.h>
#include <interfaces\mmIFormat.h>
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
	Json::Value ImportImage( Json::Value& img_path );
	Json::Value ExportImage( Json::Value& params );
	Json::Value QueryImage( Json::Value& image );
	Json::Value QueryLayer( Json::Value& layer );
	Json::Value DataReceived( Json::Value& params );
	Json::Value SyncImages( Json::Value& img_struct );
	Json::Value SyncROI( Json::Value& img_struct );
	bool text_response;

	// method params conversion
	Json::Value Params_XML2JSON(mmString const & params_xml) const;
	mmString Params_JSON2XML(Json::Value const & params_json) const;
	
	// image data encoding/deconding
	Json::Value ImageLabelToJSON(mmImages::mmImageI* const image) const;
	
	// calculation environment
	mmImages::mmImagesCalculationManagement calc_mgr;
	mmImages::mmImagesCalculationMethodContainerForWindows methods_mgr;
	mmFactories::mmUtilsFactoryI* utils_factory;
	mmImages::mmImageStructure* image_structure;
	mmImages::mmImagesCalculationMethodI* calculation_method;

	// utils
	mmFormats::mmFormatI* m_psRWFormat;
	std::map<std::wstring, mmImages::mmGenericParamI::mmType> param_type_lookup;
	Json::Value success_response, failure_response;
	Json::Value FailureResponse(std::wstring const & error) const;
	Json::Value SuccessResponse( std::wstring const & message ) const;
};

