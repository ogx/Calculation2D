#include "mmCalculationServer.h"
#include <interfaces\mmInterfaceInitializers.h>
#include <memory>
#include "json\json.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace mmImages;


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
	std::auto_ptr<mmImageStructureI> image(mmInterfaceInitializers::CreateDefaultImageStructure(NULL, NULL));

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
			break;
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
		method_info[L"author"] = L"Anonymous"; // TODO: support for author in plugin info
		method_info[L"params"] = Json::Value(Json::objectValue); // TODO: either translate params to JSON or switch to JSON in plugins
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
			Json::Value result(Json::objectValue);
			result[L"image_structure"] = Json::Value(Json::arrayValue); // TODO: form output image_structure
			result[L"image_structure"].resize(image_structure->GetImageCount());
			result[L"params"];// TODO: form output parameters
			response[L"result"] = result;

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

	image_structure = mmInterfaceInitializers::CreateDefaultImageStructure(NULL, NULL);
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
		mmUInt width = image_param[L"image"][L"width"].asUInt();
		mmUInt height = image_param[L"image"][L"height"].asUInt();
		std::wstring name = image_param[L"name"].asString();

		mmImages::mmImageI* image = image_structure->CreateImage(name, width, height, mmImages::mmImageI::mmP24);
		//image->SetPixels(); // TODO: copy the image from JSON array
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

// 		switch(mmtype)
// 		{
// 		case mmXML::g_eXMLReal:
// 			break;
// 		case mmXML::g_eXMLInt:
// 			break;
// 		case mmXML::g_eXMLString:
// 			break;
// 		case mmXML::g_eXMLBool:
// 			break;
// 		case mmXML::g_eXMLImageName:
// 			break;
// 		case mmXML::g_eXMLDataLayerName:
// 			break;
// 		default:
// 			;
// 		};

		mmXML::mmXMLNodeI* _v_sParam = _v_sRootNode->AddChild(g_pAutoCalcXML_Params_Param_Node);
		mmXML::mmXMLNodeI* _v_sParamName = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamName_Node);
		mmXML::mmXMLNodeI* _v_sParamType = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamType_Node);
		mmXML::mmXMLNodeI* _v_sParamValue = _v_sParam->AddChild(g_pAutoCalcXML_Params_ParamValue_Node);

		_v_sParamName->SetText(name);
		_v_sParamType->SetText(mmXML::GetTypeTransition(mmtype));
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
