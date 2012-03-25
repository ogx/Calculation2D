#include "mmCalculationServer.h"
#include <interfaces\mmInterfaceInitializers.h>
#include <memory>
#include "json\json.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "mmCodecBase64.h"

#include <serialization/mmGenericParam.h>
#include <serialization/mmSerializeID.h>
#include <serialization/mmBasicSerialization.h>
#include <math/mmMath.h>

using namespace mmImages;


#define LAYER_TRANSPORT__TEXT_ARRAY 1
#define LAYER_TRANSPORT__BASE64 2
#define LAYER_TRANSPORT LAYER_TRANSPORT__BASE64


mmCalculationServer::mmCalculationServer(void) :
	calc_mgr(3, NULL),
	methods_mgr(NULL),
	utils_factory(NULL),
	image_structure(NULL),
	calculation_method(NULL)
{
	param_type_lookup[L"real"] = mmGenericParamI::mmRealType;
	param_type_lookup[L"int"] = mmGenericParamI::mmIntType;
	param_type_lookup[L"string"] = mmGenericParamI::mmStringType;
	param_type_lookup[L"bool"] = mmGenericParamI::mmBoolType;
	param_type_lookup[L"image"] = mmGenericParamI::mmImageType;
	param_type_lookup[L"image-name"] = mmGenericParamI::mmImageNameType;
	param_type_lookup[L"layer"] = mmGenericParamI::mmLayerType;
	param_type_lookup[L"layer-name"] = mmGenericParamI::mmLayerNameType;
	param_type_lookup[L"rect"] = mmGenericParamI::mmRectType;
	param_type_lookup[L"point"] = mmGenericParamI::mmPointType;
	param_type_lookup[L"list"] = mmGenericParamI::mmListType;
	param_type_lookup[L"unknown"] = mmGenericParamI::mmUnknownType;

	success_response = failure_response = Json::Value(Json::objectValue);
	success_response[L"success"] = true;
	failure_response[L"success"] = false;
	failure_response[L"error"] = L"No error message provided.";
}

mmCalculationServer::~mmCalculationServer(void)
{
}

int mmCalculationServer::Serve()
{
	std::auto_ptr<mmImageStructure> image(new mmImages::mmImageStructure(NULL));

	std::wistream& is = std::wcin;
	std::wostream& os = std::wcout;
	std::wstring input;
	Json::Reader reader;
	Json::FastWriter fwriter;
	Json::Value obj_in, obj_out;
	while(std::getline(is, input).good())
	{
		reader.parse(input, obj_in);
		if(obj_in[L"cmd"].asString() == L"finalize")
		{
			os << fwriter.write(success_response);
			break;
		}
		else if(obj_in[L"cmd"].asString() == L"getmethods")
			obj_out = this->GetMethods();
		else if(obj_in[L"cmd"].asString() == L"run")
			obj_out = this->RunCalculationMethod(obj_in[L"params"]);
		else if(obj_in[L"cmd"].asString() == L"getstatus")
			obj_out = this->GetStatus();
		else 
			obj_out = FailureResponse(L"Unknown command.");

		os << fwriter.write(obj_out);
	}

	return 0;
}

Json::Value mmCalculationServer::GetMethods()
{
	typedef std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> method_infos_t;

	Json::Value res(Json::arrayValue), method_info;

	method_infos_t method_infos = methods_mgr.GetAvailableImagesCalculationMethods();
	for(Json::ArrayIndex i=0, n=method_infos.size(); i<n; ++i)
	{
		method_info = Json::Value(Json::objectValue);
		method_info[L"name"] = method_infos[i].sShortName;
		method_info[L"id"] = method_infos[i].sIDName;
		method_info[L"description"] = method_infos[i].sDescription;
		method_info[L"author"] = Json::Value(Json::objectValue);
		method_info[L"author"][L"first_name"] = method_infos[i].sAuthorInfo.sFirstName;
		method_info[L"author"][L"last_name"] = method_infos[i].sAuthorInfo.sLastName;
		method_info[L"author"][L"id"] = method_infos[i].sAuthorInfo.iID;
		method_info[L"author"][L"email"] = method_infos[i].sAuthorInfo.sEmail;
		method_info[L"params"] = Json::Value(Json::objectValue);
		method_info[L"params"][L"in"] = Params_XML2JSON(method_infos[i].sAutoParams.sInParams);
		method_info[L"params"][L"out"] = Json::Value(Json::arrayValue);//Params_XML2JSON(method_infos[i].sAutoParams.sOutParams);
		res[i] = method_info;
	}
	return res;
}

Json::Value mmCalculationServer::GetStatus()
{
	if(calculation_method)
	{
		Json::Value response = success_response;
		if(calc_mgr.IsCalculating())
		{
			response[L"finished"] = false;
			response[L"status"] = L"calculation";
		}
		else
		{
			response[L"finished"] = true;
			response[L"status"] = L"finished";
			response[L"result"] = WrapResults(image_structure, 
				calculation_method->GetCalculationMethodInfo().sAutoParams.sOutParams);

			delete calculation_method; calculation_method = NULL;
			delete utils_factory; utils_factory = NULL;
			delete image_structure; image_structure = NULL;
		}
		return response;
	}
	return FailureResponse(L"Nothing going on.");
}

Json::Value mmCalculationServer::RunCalculationMethod( Json::Value& params )
{
	Json::Value& method_id = params[L"method"];
	Json::Value response = success_response;

	image_structure = new mmImages::mmImageStructure(NULL);
	utils_factory = mmInterfaceInitializers::CreateUtilsFactory();
	calculation_method = methods_mgr.InitializeImagesCalculationMethod(method_id.asString());

	mmString params_xml;
	UnwrapArguments(params, image_structure, params_xml);

	// prep params
	mmImages::mmImagesCalculationMethodI::sCalculationMethodParams method_info = 
		calculation_method->GetCalculationMethodInfo();
	mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams params_struct = 
		method_info.sAutoParams;
	wcscpy_s(params_struct.sInParams, params_xml.c_str());

	// run the plugin
	calc_mgr.CalculateImages(calculation_method, image_structure, &params_struct);

	response[L"message"] = L"The calculation has been queued.";
	return response;
}

Json::Value mmCalculationServer::Params_XML2JSON(mmString const & params_xml, ImagesToWrap* images_to_wrap) const
{
	// get child nodes with parameters
	mmXML::mmXMLDocI* _v_sInputXML = mmInterfaceInitializers::CreateXMLDocument(NULL);
	_v_sInputXML->ParseXMLBuffer(params_xml);
	mmXML::mmXMLNodeI* _v_sRootNode = _v_sInputXML->GetXMLRootNode();
	std::vector<mmXML::mmXMLNodeI*> v_vrChildNodes = _v_sRootNode->GetChildren();

	// add a corresponding edit/combo for each parameter depending on its type
	mmGenericParamI::mmType v_eDataType;
	mmString v_tName, v_tValue;
	Json::Value converted(Json::arrayValue);
	for( std::vector<mmXML::mmXMLNodeI*>::iterator v_vriI = v_vrChildNodes.begin(); v_vriI != v_vrChildNodes.end(); ++v_vriI) {
		v_tName = (*v_vriI)->FindChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node)->GetText();
		v_tValue = (*v_vriI)->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->GetText();
		v_eDataType = mmSerializer<mmGenericParamI::mmType>::FromString((*v_vriI)->FindChild( mmImages::g_pAutoCalcXML_Params_ParamType_Node)->GetText());

		std::wstring type = L"unknown";
		for(auto it=param_type_lookup.begin(), end=param_type_lookup.end(); it!=end; ++it)
			if(it->second == v_eDataType)
			{
				type = it->first;
				break;
			}

		Json::Value param(Json::objectValue);
		param[L"name"] = v_tName;
		param[L"type"] = type;
		switch( v_eDataType ) 
		{
		case mmGenericParamI::mmRealType:
			param[L"value"] = mmSerializer<mmReal>::FromString(v_tValue);
			break;
		case mmGenericParamI::mmIntType:
			param[L"value"] = mmSerializer<mmInt>::FromString(v_tValue);
			break;
		case mmGenericParamI::mmBoolType:
			param[L"value"] = mmSerializer<bool>::FromString(v_tValue);
			break;
		case mmGenericParamI::mmRectType:
			{
				mmRect rect = mmSerializer<mmRect>::FromString(v_tValue);
				param[L"value"] = Json::Value(Json::objectValue);
				param[L"value"][L"x"] = rect.iLeft;
				param[L"value"][L"y"] = rect.iTop;
				param[L"value"][L"width"] = rect.iWidth;
				param[L"value"][L"height"] = rect.iHeight;
			}
			break;
		case mmGenericParamI::mmPointType:
			{
				mmMath::sPoint2D point = mmSerializer<mmMath::sPoint2D>::FromString(v_tValue);
				param[L"value"] = Json::Value(Json::objectValue);
				param[L"value"][L"x"] = point.rX;
				param[L"value"][L"y"] = point.rY;
			}
			break;
		default:
			param[L"value"] = v_tValue;
		}
		converted.append(param);

		// note down images and layers
		if(images_to_wrap && image_structure)
		{
			if(v_eDataType == mmGenericParamI::mmImageType)
			{
				// Output image. Need to export all layers of that image.

				mmID id = mmSerializer<mmID>::FromString(v_tValue);
				ImageToWrap& itr = (*images_to_wrap)[id];
				mmImageI const * image = image_structure->GetImage(id);

				mmLayerI const * layer = NULL;
				while(layer = image->FindLayer(layer))
					itr.layers.insert(layer->GetID().m_sLayerID);
			}
			else if(v_eDataType == mmGenericParamI::mmLayerType)
			{
				// Output layer. Need to export this particular layer.

				mmImages::mmLayerI::sID lid = mmSerializer<mmImages::mmLayerI::sID>::FromString(v_tValue);
				mmImageI const * image = image_structure->GetImage(lid.m_sImageID);
				if(image && image->GetLayer(lid.m_sLayerID))
					(*images_to_wrap)[lid.m_sImageID].layers.insert(lid.m_sLayerID);
			}
			// TODO: elif mmLayerNameType - layers of given name across all images
		}
	}

	delete _v_sInputXML;
	return converted;
}

mmString mmCalculationServer::Params_JSON2XML(Json::Value const & params_json) const
{
	mmXML::mmXMLDocI* _v_sInputXML = mmInterfaceInitializers::CreateXMLDocument(NULL);
	_v_sInputXML->CreateXMLRootNode(g_pAutoCalcXML_INParams_Node);
	mmXML::mmXMLNodeI* _v_sRootNode = _v_sInputXML->GetXMLRootNode();

	for(int i=0, n=params_json.size(); i<n; ++i)
	{
		Json::Value const & param = params_json[i];
		std::wstring name = param[L"name"].asString();
		std::wstring type = param[L"type"].asString();
		std::wstring value = param[L"value"].asString();
		mmGenericParamI::mmType mmtype = param_type_lookup.at(type);

		mmXML::mmXMLNodeI* _v_sParam = _v_sRootNode->AddChild(g_pAutoCalcXML_Params_Param_Node);
		mmXML::mmXMLNodeI* _v_sParamName = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamName_Node);
		mmXML::mmXMLNodeI* _v_sParamType = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamType_Node);
		mmXML::mmXMLNodeI* _v_sParamValue = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamValue_Node);

		_v_sParamName->SetText(name);
		_v_sParamType->SetText(mmSerializer<mmGenericParamI::mmType>::ToString(mmtype));
		if(mmtype == mmGenericParamI::mmBoolType)
			_v_sParamValue->SetText(value.compare(L"true") == 0 ? mmSerializer<bool>::ToString(true) : mmSerializer<bool>::ToString(false));
		else
			_v_sParamValue->SetText(value);
	}

	mmString params_xml = _v_sInputXML->SaveToXMLBuffer();
	delete _v_sInputXML;
	return params_xml;
}

Json::Value mmCalculationServer::FailureResponse( std::wstring const & error ) const
{
	Json::Value result = failure_response;
	result[L"error"] = error;
	return result;
}

Json::Value mmCalculationServer::LayerToJSON(mmImages::mmLayerI const * layer) const
{
	Json::Value json(Json::objectValue);

	json[L"name"] = layer->GetName();
	json[L"id"] = ToString(layer->GetID());
	json[L"default_value"] = layer->GetDefaultValue();

	mmUInt width = (json[L"width"] = layer->GetWidth()).asUInt();
	mmUInt height = (json[L"height"] = layer->GetHeight()).asUInt();

	std::vector<mmReal> buf(width*height);
	mmRect rect(0, 0, width, height);
	layer->GetPixels(rect, &buf[0]);

#if LAYER_TRANSPORT == LAYER_TRANSPORT__TEXT_ARRAY

	Json::Value& values = json[L"values"] = Json::Value(Json::arrayValue);
	values.resize(width*height);
	for(mmUInt i=0, n=width*height; i<n; ++i)
		values[i] = buf[i];

#elif LAYER_TRANSPORT == LAYER_TRANSPORT__BASE64

	base64::byte_vect byte_buf(width*height);
	for(size_t i=0, n=width*height; i<n; ++i)
		byte_buf[i] = (unsigned char)(buf[i]*255.0 + 0.5 - 1e-6);
	std::string base64_buf = base64::encode(byte_buf);

	json[L"values"] = std::wstring(base64_buf.begin(), base64_buf.end());

#endif

	return json;
}

void mmCalculationServer::LayerFromJSON( Json::Value const & json, mmImages::mmLayerI * layer ) const
{
	layer->SetName(json[L"name"].asString());

	mmUInt width = json[L"width"].asUInt();
	mmUInt height = json[L"height"].asUInt();
	Json::Value const & values = json[L"values"];
	mmReal def_value = json[L"default_value"].asDouble(); // TODO: use

	std::vector<mmReal> buf(width*height);

#if LAYER_TRANSPORT == LAYER_TRANSPORT__TEXT_ARRAY

	for(mmUInt i=0, n=width*height; i<n; ++i)
		buf[i] = values[i].asDouble();

#elif LAYER_TRANSPORT == LAYER_TRANSPORT__BASE64

	std::wstring base64_buf = values.asString();
	base64::byte_vect byte_buf = base64::decode(std::string(base64_buf.begin(), base64_buf.end()));
	std::string test = base64::encode(byte_buf);
	for(size_t i=0, n=width*height; i<n; ++i)
		buf[i] = (mmReal)(byte_buf[i])/255.0;

#endif

	mmRect rect(0, 0, width, height);
	layer->SetPixels(rect, &buf[0]);
}

Json::Value mmCalculationServer::WrapResults( mmImages::mmImageStructure const * image_structure, mmString const & output_params ) const
{
	Json::Value result = Json::Value(Json::objectValue);

	// form output params
	ImagesToWrap images_to_wrap;
	result[L"params"] = Params_XML2JSON(output_params, &images_to_wrap);

	// form image structure
	Json::Value& isr = result[L"image_structure"] = Json::Value(Json::arrayValue);
	for(ImagesToWrap::const_iterator image_to_wrap=images_to_wrap.begin(), 
		end=images_to_wrap.end(); 
		image_to_wrap!=end; ++image_to_wrap)
	{
		mmImages::mmImageI* image = image_structure->GetImage(image_to_wrap->first);

		Json::Value image_json(Json::objectValue);
		image_json[L"id"] = ToString(image->GetID());
		image_json[L"name"] = image->GetName();
		Json::Value& image_json_channels = image_json[L"channels"] = Json::Value(Json::arrayValue);
		Json::Value& image_json_layers = image_json[L"data_layers"] = Json::Value(Json::arrayValue);

		image_json[L"width"] = image->GetWidth();
		image_json[L"height"] = image->GetHeight();
		image_json_channels[0] = LayerToJSON(image->GetChannel(0));
		if(image->GetPixelType() >= mmImageI::mmP24)
		{
			image_json_channels[1] = LayerToJSON(image->GetChannel(1));
			image_json_channels[2] = LayerToJSON(image->GetChannel(2));
		}
		else // TODO: doing small steps - remove this overhead!
			image_json_channels[2] = image_json_channels[1] = image_json_channels[0];

		// layers
		LayersToWrap const & layers_to_wrap = image_to_wrap->second.layers;
		for(LayersToWrap::const_iterator layer_to_wrap=layers_to_wrap.begin(), lend=layers_to_wrap.end(); 
			layer_to_wrap!=lend; ++layer_to_wrap)
		{
			mmImages::mmLayerI* layer = image->GetLayer(*layer_to_wrap);
			image_json_layers.append(LayerToJSON(layer));
		}

		isr.append(image_json);
	}

	return result;
}

void mmCalculationServer::UnwrapArguments( 
	Json::Value const & params_json, 
	mmImages::mmImageStructure * image_structure, 
	mmString & input_params 
	) const
{
	// prep image structure
	const Json::Value& image_structure_json = params_json[L"image_structure"];
	for(int i=0, n=image_structure_json.size(); i<n; ++i)
	{
		const Json::Value& image_param = image_structure_json[i];
		const Json::Value& channels_param = image_param[L"channels"];
		mmUInt width = image_param[L"width"].asUInt();
		mmUInt height = image_param[L"height"].asUInt();
		std::wstring name = image_param[L"name"].asString();
		int num_channels = channels_param.size();

		mmImages::mmImageI* image = image_structure->CreateImage(name, width, height, mmImages::mmImageI::mmPixelType(num_channels));
		
		for(int j=0; j<num_channels; ++j)
			LayerFromJSON(channels_param[j], image->GetChannel(j));

		// TODO: extract layers
	}

	// prep input params
	input_params = Params_JSON2XML(params_json[L"method_params"]);
}
