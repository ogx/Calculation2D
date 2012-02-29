#include "mmCalculationServer.h"
#include <interfaces\mmInterfaceInitializers.h>
#include <memory>
#include "json\json.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace mmImages;


const size_t buf_size = INOUT_PARAMS_SIZE;
size_t converted = 0;
char buf[buf_size];
wchar_t mmbuf[buf_size];
std::string mmstr2str(mmString const & mmstr)
{
	wcstombs_s(&converted, buf, mmstr.c_str(), buf_size);
	if(converted-1 != mmstr.size())
		return "--Error converting!--";
	return buf;
}
mmString str2mmstr(std::string const & str)
{
	mbstowcs_s(&converted, mmbuf, str.c_str(), buf_size);
	if(converted-1 != str.size())
		return L"--Error converting!--";
	return mmbuf;
}


mmCalculationServer::mmCalculationServer(void) :
	calc_mgr(3, NULL),
	methods_mgr(NULL),
	utils_factory(NULL),
	image_structure(NULL),
	calculation_method(NULL)
{
	param_type_lookup["real"] = mmXML::g_eXMLReal;
	param_type_lookup["int"] = mmXML::g_eXMLInt;
	param_type_lookup["string"] = mmXML::g_eXMLString;
	param_type_lookup["bool"] = mmXML::g_eXMLBool;
	param_type_lookup["image"] = mmXML::g_eXMLImageName;
	param_type_lookup["layer"] = mmXML::g_eXMLDataLayerName;
	param_type_lookup["unknown"] = mmXML::g_eXMLUnknownDataType;

	success_response = failure_response = Json::Value(Json::objectValue);
	success_response["success"] = true;
	failure_response["success"] = false;
	failure_response["error"] = "No error message provided.";
}

mmCalculationServer::~mmCalculationServer(void)
{
}

int mmCalculationServer::Serve()
{
	std::auto_ptr<mmImageStructureI> image(mmInterfaceInitializers::CreateDefaultImageStructure(NULL, NULL));

	std::istream& is = std::cin;
	std::ostream& os = std::cout;
	std::string input;
	Json::Reader reader;
	Json::FastWriter fwriter;
	Json::Value obj_in, obj_out;
	while(1)
	{
		is >> input;
		reader.parse(input, obj_in);
		if(obj_in["cmd"].asString() == "finalize")
			break;
		else if(obj_in["cmd"].asString() == "getmethods")
			obj_out = this->GetMethods();
		else if(obj_in["cmd"].asString() == "run")
			obj_out = this->RunCalculationMethod(obj_in["params"]);
		else if(obj_in["cmd"].asString() == "getstatus")
			obj_out = this->GetStatus();
		else 
			obj_out = FailureResponse("Unknown command.");

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
		method_info["name"] = mmstr2str(method_infos[i].sShortName);
		method_info["id"] = mmstr2str(method_infos[i].sIDName);
		method_info["description"] = mmstr2str(method_infos[i].sDescription);
		method_info["author"] = "Anonymous"; // TODO: support for author in plugin info
		method_info["params"] = Json::Value(Json::objectValue); // TODO: either translate params to JSON or switch to JSON in plugins
		method_info["params"]["in"] = Params_XML2JSON(method_infos[i].sAutoParams.sInParams);
		method_info["params"]["out"] = Params_XML2JSON(method_infos[i].sAutoParams.sOutParams);
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
			response["status"] = "calculation";
		else
		{
			response["status"] = "finished";
			Json::Value result(Json::objectValue);
			result["image_structure"] = Json::Value(Json::arrayValue); // TODO: form output image_structure
			result["image_structure"].resize(image_structure->GetImageCount());
			result["params"];// TODO: form output parameters
			response["result"] = result;
		}
		return response;
	}
	return FailureResponse("Nothing going on.");
}

Json::Value mmCalculationServer::RunCalculationMethod( Json::Value& params )
{
	Json::Value& image_structure_json = params["image_structure"];
	Json::Value& method_id = params["method"];
	Json::Value response = success_response;

	image_structure = mmInterfaceInitializers::CreateDefaultImageStructure(NULL, NULL);
	utils_factory = mmInterfaceInitializers::CreateUtilsFactory();
	calculation_method = methods_mgr.InitializeImagesCalculationMethod(str2mmstr(method_id.asString()));

	// prep params
	mmString params_xml = Params_JSON2XML(params["method_params"]);
	mmImages::mmImagesCalculationMethodI::sCalculationMethodParams method_info = 
		calculation_method->GetCalculationMethodInfo();
	mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams params_struct = 
		method_info.sAutoParams;
	wcscpy_s(params_struct.sInParams, params_xml.c_str());

	// prep image structure
	for(int i=0, n=image_structure_json.size(); i<n; ++i)
	{
		Json::Value& image_param = image_structure_json[i];
		mmUInt width = image_param["image"]["width"].asUInt();
		mmUInt height = image_param["image"]["height"].asUInt();
		std::string name = image_param["name"].asString();

		mmUInt image_id = image_structure->CreateImage(str2mmstr(name), width, height, mmImages::mmImageI::mmP24);
		mmImages::mmImageI* image = image_structure->GetImage(image_id);
		//image->SetPixels(); // TODO: copy the image from JSON array
	}

	// run the plugin
	calc_mgr.CalculateImages(calculation_method, image_structure, &params_struct);

	response["message"] = "The calculation has been queued.";
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

		std::string type = "unknown";
		for(auto it=param_type_lookup.begin(), end=param_type_lookup.end(); it!=end; ++it)
			if(it->second == v_eDataType)
			{
				type = it->first;
				break;
			}

		Json::Value param(Json::objectValue);
		param["name"] = mmstr2str(v_tName);
		param["type"] = type;
		switch( v_eDataType ) 
		{
		case mmXML::g_eXMLReal:
			param["value"] = StringToMMReal(v_tValue);
			break;
		case mmXML::g_eXMLInt:
			param["value"] = StringToMMInt(v_tValue);
			break;
		case mmXML::g_eXMLString:
			param["value"] = StringToMMReal(v_tValue);
			break;
		case mmXML::g_eXMLBool:
			param["value"] = v_tValue.compare(g_pAutoCalcXML_Params_ParamType_BoolValue_YES) == 0;
			break;
		case mmXML::g_eXMLImageName:
			param["value"] = mmstr2str(v_tValue);
			break;
		case mmXML::g_eXMLDataLayerName:
			param["value"] = mmstr2str(v_tValue);
			break;
		default:
			param["value"] = mmstr2str(v_tValue);
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
		std::string name = param["name"].asString();
		std::string type = param["type"].asString();
		std::string value = param["value"].asString();
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

		_v_sParamName->SetText(str2mmstr(name));
		_v_sParamType->SetText(str2mmstr(type));
		_v_sParamValue->SetText(str2mmstr(value));
	}

	mmString params_xml = _v_sInputXML->SaveToXMLBuffer();
	delete _v_sInputXML;
	return params_xml;
}

Json::Value mmCalculationServer::FailureResponse( std::string const & error )
{
	Json::Value result = failure_response;
	result["error"] = error;
	return result;
}
