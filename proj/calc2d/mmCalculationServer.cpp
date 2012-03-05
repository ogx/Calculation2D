#include "mmCalculationServer.h"
#include <interfaces\mmInterfaceInitializers.h>
#include <memory>
#include "json\json.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "mmCodecBase64.h"

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
	param_type_lookup[L"real"] = mmXML::g_eXMLReal;
	param_type_lookup[L"int"] = mmXML::g_eXMLInt;
	param_type_lookup[L"string"] = mmXML::g_eXMLString;
	param_type_lookup[L"bool"] = mmXML::g_eXMLBool;
	param_type_lookup[L"image"] = mmXML::g_eXMLImageName;
	param_type_lookup[L"layer"] = mmXML::g_eXMLDataLayerName;
	param_type_lookup[L"unknown"] = mmXML::g_eXMLUnknownDataType;

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
	while(1)
	{
		is >> input;
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
		method_info[L"params"][L"out"] = Params_XML2JSON(method_infos[i].sAutoParams.sOutParams);
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
			response[L"status"] = L"calculation";
		else
		{
			response[L"status"] = L"finished";
			Json::Value& result = response[L"result"];

			// send image structure
			Json::Value& isr = result[L"image_structure"] = Json::Value(Json::arrayValue);
			mmImages::mmImageI* image = NULL;
			while(image = image_structure->FindImage(image))
			{
				Json::Value image_json(Json::objectValue);
				image_json[L"name"] = image->GetName();
				Json::Value& image_json_image = image_json[L"image"] = Json::Value(Json::objectValue);
				Json::Value& image_json_layers = image_json[L"data_layers"] = Json::Value(Json::arrayValue);

				image_json_image[L"width"] = image->GetWidth();
				image_json_image[L"height"] = image->GetHeight();
				image_json_image[L"red"] = LayerToJSON(image->GetChannel(0));
				if(image->GetPixelType() >= mmImageI::mmP24)
				{
					image_json_image[L"green"] = LayerToJSON(image->GetChannel(1));
					image_json_image[L"blue"] = LayerToJSON(image->GetChannel(2));
				}
				else // TODO: doing small steps - remove this overhead!
					image_json_image[L"green"] = image_json_image[L"blue"] = image_json_image[L"red"];

				isr.append(image_json);
			}

			// TODO: send output parameters some day
			//result[L"params"];

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
	Json::Value& image_structure_json = params[L"image_structure"];
	Json::Value& method_id = params[L"method"];
	Json::Value response = success_response;

	image_structure = new mmImages::mmImageStructure(NULL);
	utils_factory = mmInterfaceInitializers::CreateUtilsFactory();
	calculation_method = methods_mgr.InitializeImagesCalculationMethod(method_id.asString());

	// prep params
	mmString params_xml = Params_JSON2XML(params[L"method_params"]);
	mmImages::mmImagesCalculationMethodI::sCalculationMethodParams method_info = 
		calculation_method->GetCalculationMethodInfo();
	mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams params_struct = 
		method_info.sAutoParams;
	wcscpy_s(params_struct.sInParams, params_xml.c_str());

	// prep image structure
	for(int i=0, n=image_structure_json.size(); i<n; ++i)
	{
		Json::Value& image_param = image_structure_json[i];
		Json::Value& image_param_image = image_param[L"image"];
		mmUInt width = image_param_image[L"width"].asUInt();
		mmUInt height = image_param_image[L"height"].asUInt();
		std::wstring name = image_param[L"name"].asString();

		mmImages::mmImageI* image = image_structure->CreateImage(name, width, height, mmImages::mmImageI::mmP24);
		LayerFromJSON(image_param_image[L"red"], image->GetChannel(0));
		LayerFromJSON(image_param_image[L"green"], image->GetChannel(1));
		LayerFromJSON(image_param_image[L"blue"], image->GetChannel(2));
	}

	// run the plugin
	calc_mgr.CalculateImages(calculation_method, image_structure, &params_struct);

	response[L"message"] = L"The calculation has been queued.";
	return response;
}

Json::Value mmCalculationServer::Params_XML2JSON( mmString const & params_xml )
{
	// get child nodes with parameters
	mmXML::mmXMLDocI* _v_sInputXML = mmInterfaceInitializers::CreateXMLDocument(NULL);
	_v_sInputXML->ParseXMLBuffer(params_xml);
	mmXML::mmXMLNodeI* _v_sRootNode = _v_sInputXML->GetXMLRootNode();
	std::vector<mmXML::mmXMLNodeI*> v_vrChildNodes = _v_sRootNode->GetChildren();

	// add a corresponding edit/combo for each parameter depending on its type
	mmXML::mmXMLDataType v_eDataType;
	mmString v_tName, v_tValue;
	Json::Value converted(Json::arrayValue);
	for( std::vector<mmXML::mmXMLNodeI*>::iterator v_vriI = v_vrChildNodes.begin(); v_vriI != v_vrChildNodes.end(); ++v_vriI) {
		v_tName = (*v_vriI)->FindChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node)->GetText();
		v_tValue = (*v_vriI)->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->GetText();
		v_eDataType = mmXML::GetTypeTransition((*v_vriI)->FindChild( mmImages::g_pAutoCalcXML_Params_ParamType_Node)->GetText());

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
		case mmXML::g_eXMLReal:
			param[L"value"] = StringToMMReal(v_tValue);
			break;
		case mmXML::g_eXMLInt:
			param[L"value"] = StringToMMInt(v_tValue);
			break;
		case mmXML::g_eXMLString:
			param[L"value"] = StringToMMReal(v_tValue);
			break;
		case mmXML::g_eXMLBool:
			param[L"value"] = v_tValue.compare(g_pAutoCalcXML_Params_ParamType_BoolValue_YES) == 0;
			break;
		case mmXML::g_eXMLImageName:
			param[L"value"] = v_tValue;
			break;
		case mmXML::g_eXMLDataLayerName:
			param[L"value"] = v_tValue;
			break;
		default:
			param[L"value"] = v_tValue;
		}
		converted.append(param);
	}

	delete _v_sInputXML;
	return converted;
}

mmString mmCalculationServer::Params_JSON2XML(Json::Value const & params_json)
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
		mmXML::mmXMLDataType mmtype = param_type_lookup[type];

		mmXML::mmXMLNodeI* _v_sParam = _v_sRootNode->AddChild(g_pAutoCalcXML_Params_Param_Node);
		mmXML::mmXMLNodeI* _v_sParamName = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamName_Node);
		mmXML::mmXMLNodeI* _v_sParamType = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamType_Node);
		mmXML::mmXMLNodeI* _v_sParamValue = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamValue_Node);

		_v_sParamName->SetText(name);
		_v_sParamType->SetText(mmXML::GetTypeTransition(mmtype));
		if(mmtype == mmXML::g_eXMLBool)
			_v_sParamValue->SetText(value.compare(L"true") == 0 ? mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES : mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_NO);
		else
			_v_sParamValue->SetText(value);
	}

	mmString params_xml = _v_sInputXML->SaveToXMLBuffer();
	delete _v_sInputXML;
	return params_xml;
}

Json::Value mmCalculationServer::FailureResponse( std::wstring const & error )
{
	Json::Value result = failure_response;
	result[L"error"] = error;
	return result;
}

Json::Value mmCalculationServer::LayerToJSON( mmImages::mmLayerI const * layer )
{
	Json::Value json(Json::objectValue);

	json[L"name"] = layer->GetName();
	json[L"id"] = layer->GetID().ancpainalxnkalisxnaosnx_you_cant_type_it_so_you_wont_use_it();
	json[L"default"] = layer->GetDefaultValue();

	mmUInt width = (json[L"width"] = layer->GetWidth()).asUInt();
	mmUInt height = (json[L"height"] = layer->GetHeight()).asUInt();

	std::vector<mmReal> buf(width*height);
	mmRect rect(0, 0, width, height);
	layer->GetPixels(&buf[0], rect);

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

void mmCalculationServer::LayerFromJSON( Json::Value const & json, mmImages::mmLayerI * layer )
{
	layer->SetName(json[L"name"].asString());

	mmUInt width = json[L"width"].asUInt();
	mmUInt height = json[L"height"].asUInt();
	Json::Value const & values = json[L"values"];

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
