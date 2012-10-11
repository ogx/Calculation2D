#include "mmCalculationServer.h"

#include <interfaces\mmInterfaceInitializers.h>

#include <serialization/mmGenericParam.h>
#include <serialization/mmSerializeID.h>
#include <serialization/mmBasicSerialization.h>

#include <mmStringUtilities.h>

#include <memory>
#include "json\json.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

using namespace mmImages;

mmCalculationServer::mmCalculationServer(void) :
	calc_mgr(3, NULL),
	methods_mgr(NULL),
	utils_factory(NULL),
	calculation_method(NULL),
	text_response(true)
{
	param_type_lookup[L"real"] = mmGenericParamI::mmRealType;
	param_type_lookup[L"int"] = mmGenericParamI::mmIntType;
	param_type_lookup[L"string"] = mmGenericParamI::mmStringType;
	param_type_lookup[L"bool"] = mmGenericParamI::mmBoolType;
	param_type_lookup[L"image"] = mmGenericParamI::mmImageNameType;
	param_type_lookup[L"layer"] = mmGenericParamI::mmLayerNameType;
	param_type_lookup[L"unknown"] = mmGenericParamI::mmUnknownType;

	success_response = failure_response = Json::Value(Json::objectValue);
	success_response[L"success"] = true;
	failure_response[L"success"] = false;
	failure_response[L"error"] = L"No error message provided.";

	image_structure = new mmImages::mmImageStructure(NULL);

	m_psRWFormat = mmInterfaceInitializers::CreateRWFormat();
}

mmCalculationServer::~mmCalculationServer(void)
{
	delete image_structure;
	delete m_psRWFormat;
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
		else if(obj_in[L"cmd"].asString() == L"load")
			obj_out = ImportImage(obj_in[L"params"]);
		else if(obj_in[L"cmd"].asString() == L"sync_roi")
			obj_out = SyncROI(obj_in[L"params"]);
		else if(obj_in[L"cmd"].asString() == L"sync_images")
			obj_out = SyncImages(obj_in[L"params"]);
		else if(obj_in[L"cmd"].asString() == L"query_image")
			obj_out = QueryImage(obj_in[L"params"]);
		else if(obj_in[L"cmd"].asString() == L"query_layer")
			obj_out = QueryLayer(obj_in[L"params"]);
		else if(obj_in[L"cmd"].asString() == L"data_received")
			obj_out = DataReceived(obj_in[L"params"]);
		else if(obj_in[L"cmd"].asString() == L"run")
			obj_out = this->RunCalculationMethod(obj_in[L"params"]);
		else if(obj_in[L"cmd"].asString() == L"getstatus")
			obj_out = this->GetStatus();
		else 
			obj_out = FailureResponse(L"Unknown command.");

		if (text_response) os << fwriter.write(obj_out);
	}

	return 0;
}

Json::Value mmCalculationServer::GetMethods()
{
	typedef std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> method_infos_t;

	Json::Value res(Json::arrayValue), method_info, response;

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
		method_info[L"author"][L"id"] = mmSerializer<int>::ToString( method_infos[i].sAuthorInfo.iID );
		method_info[L"author"][L"email"] = method_infos[i].sAuthorInfo.sEmail;
		method_info[L"params"] = Json::Value(Json::objectValue);
		method_info[L"params"][L"in"] = Params_XML2JSON(method_infos[i].sAutoParams.sInParams);
		method_info[L"params"][L"out"] = Params_XML2JSON(method_infos[i].sAutoParams.sOutParams);
		res[i] = method_info;
	}
	response[L"methods"] = res;
	text_response = true;
	return response;
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

			delete calculation_method; calculation_method = NULL;
			delete utils_factory; utils_factory = NULL;
		}
		text_response = true;
		return response;
	}
	text_response = true;
	return FailureResponse(L"Nothing going on.");
}

Json::Value mmCalculationServer::RunCalculationMethod( Json::Value& params )
{
	Json::Value& method_id = params[L"id"];
	Json::Value response = success_response;

	utils_factory = mmInterfaceInitializers::CreateUtilsFactory();
	calculation_method = methods_mgr.InitializeImagesCalculationMethod(method_id.asString());

	// prep input params
	mmString params_xml = Params_JSON2XML(params[L"params"][L"in"]);

	// prep params
	mmImages::mmImagesCalculationMethodI::sCalculationMethodParams method_info = 
		calculation_method->GetCalculationMethodInfo();
	mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams params_struct = 
		method_info.sAutoParams;
	wcscpy_s(params_struct.sInParams, params_xml.c_str());

	// run the plugin
	calc_mgr.CalculateImages(calculation_method, image_structure, &params_struct);

	response[L"status"] = L"The calculation has been queued.";
	text_response = true;
	return response;
}

Json::Value mmCalculationServer::ImportImage( Json::Value& img_path )
{
	Json::Value response = success_response;
	std::wstring path = img_path[L"path"].asString();

	mmString v_sName = (path.size() > 1 ? path : path.substr(path.find_last_of(L'\\') + 1));
	v_sName = v_sName.substr(path.find_last_of(L'\\') + 1);

	if(! m_psRWFormat->Read(path, image_structure, v_sName)) {
		return FailureResponse(L"Reading file failed.");
	}

	Json::Value image_array(Json::arrayValue);
	image_array.append(ImageLabelToJSON(image_structure->FindImage(NULL, v_sName)));
	response[L"images_structure"] = image_array;

	text_response = true;
	return response;
}

Json::Value mmCalculationServer::ImageLabelToJSON(mmImages::mmImageI* const image) const
{
	Json::Value image_json;
	Json::Value layer_json;
	image_json[L"name"] = image->GetName();
	image_json[L"width"] = image->GetWidth();
	image_json[L"height"] = image->GetHeight();
	image_json[L"id"] = mmSerializer<mmID>::ToString(image->GetID());
	image_json[L"layers"] = Json::Value(Json::arrayValue);
	Json::Value& layers = image_json[L"layers"];

	mmImages::mmLayerI* current_layer = NULL;
	mmImages::mmLayerI* previous_layer = NULL;

	do {
		current_layer = image->FindLayer(previous_layer);
		if (current_layer != NULL) {
			layer_json[L"name"] = current_layer->GetName();
			layer_json[L"id"] = mmSerializer<mmID>::ToString(current_layer->GetID());
			layer_json[L"owner"] = image->GetName();
			layers.append(layer_json);
		}
		previous_layer = current_layer;
	}
	while (current_layer != NULL);

	return image_json;
}

Json::Value mmCalculationServer::QueryImage( Json::Value& image_json )
{
	Json::Value response = success_response;

	mmImages::mmImageI* image = image_structure->FindImage(NULL, image_json[L"name"].asString());
	if (image != NULL) {
		int width = image->GetWidth();
		int height = image->GetHeight();
		mmImages::mmImageI::mmPixelType pixel_type = image->GetPixelType();
		unsigned int pixel_size = pixel_type;
		if (width == image_json[L"width"].asInt() && height == image_json[L"height"].asInt()) {
			mmReal* pixels = new mmReal[width*height*pixel_size];
			int* out_buffer = new int[width*height];
			std::fill(out_buffer, out_buffer + width*height, 0);
			char dot = 0;

			switch (pixel_type) {
			case mmImages::mmImageI::mmP8 :
				image->GetPixels(mmRect(0, 0, width, height), (mmPixel8*)pixels);
				for (int i = 0; i < width*height; ++i) {
					dot = (char)(pixels[i]*255);
					out_buffer[i] = 0xFF000000 | dot << 16 | dot << 8 | dot;
				}
				break;
			case mmImages::mmImageI::mmP24 :
				image->GetPixels(mmRect(0, 0, width, height), (mmPixel24*)pixels);
				for (int i = 0; i < width*height; ++i) {
					out_buffer[i] = 0xFF000000 | (int)(pixels[3*i]*255) << 16 | (int)(pixels[3*i+1]*255) << 8 | (int)(pixels[3*i+2]*255);
				}
				break;
			case mmImages::mmImageI::mmP32 :
				image->GetPixels(mmRect(0, 0, width, height), (mmPixel24*)pixels);
				for (int i = 0; i < width*height; ++i) {
					out_buffer[i] = (int)(pixels[4*i]*255) << 24 | (int)(pixels[4*i+1]*255) << 16 | (int)(pixels[4*i+2]*255) << 8 | (int)(pixels[4*i+3]*255);
				}
				break;
			}

			// enter binary mode
			_setmode(_fileno( stdout ), _O_BINARY);
			fwrite(out_buffer, sizeof(int), width*height, stdout);
			fflush(stdout);
			// leave binary mode
			_setmode(_fileno( stdout ), _O_TEXT);
		}
	}
	text_response = false;
	return response;
}

Json::Value mmCalculationServer::QueryLayer( Json::Value& layer_json )
{
	Json::Value response = success_response;

	mmImages::mmImageI* image = image_structure->FindImage(NULL, layer_json[L"owner"].asString());
	if (image != NULL) {
		mmImages::mmLayerI* layer = image->FindLayer(NULL, layer_json[L"name"].asString());
		if (layer != NULL) {
			int width = image->GetWidth();
			int height = image->GetHeight();
			mmImages::mmImageI::mmPixelType pixel_type = image->GetPixelType();
			unsigned int pixel_size = pixel_type;
			mmReal* pixels = new mmReal[width*height];
			char* out_buffer = new char[width*height];
			std::fill(out_buffer, out_buffer + width*height, 0);
			char dot = 0;
			layer->GetPixels(mmRect(0, 0, width, height), pixels);
			mmReal max_val = *(std::max_element(pixels, pixels + width*height));
			mmReal min_val = *(std::min_element(pixels, pixels + width*height));
			for (int i = 0; i < width*height; ++i) {
				out_buffer[i] = (char)(255.0*(pixels[i] - min_val)/(max_val - min_val));
			}
			// enter binary mode
			_setmode(_fileno( stdout ), _O_BINARY);
			fwrite(out_buffer, sizeof(char), width*height, stdout);
			// leave binary mode
			_setmode(_fileno( stdout ), _O_TEXT);
		}
	}
	text_response = false;
	return response;
}

Json::Value mmCalculationServer::DataReceived( Json::Value& params )
{
	Json::Value response = success_response;
	response[L"status"] = L"Data reception confirmed";
	text_response = true;
	return response;
}

Json::Value mmCalculationServer::SyncImages( Json::Value& img_struct )
{
	Json::Value response = success_response;
	Json::Value image_array(Json::arrayValue);
	Json::Value client_images = img_struct[L"images_structure"];

	// We receive info on current images structure and use it to update images structure according to images remaining in the client
	// The assumption is that we do not check which images were altered by the calculation method because it would require
	// complicated logic. Instead we send all remaining images data to the client.

	mmImages::mmImageI* current_image = NULL;
	mmImages::mmImageI* previous_image = NULL;

	do {
		current_image = image_structure->FindImage(previous_image);
		if (current_image != NULL) {
			image_array.append(ImageLabelToJSON(current_image));
		}
		previous_image = current_image;
	}
	while (current_image != NULL);
	response[L"images_structure"] = image_array;
	text_response = true;
	return response;
}

Json::Value mmCalculationServer::SyncROI( Json::Value& img_struct )
{
	Json::Value response = success_response;
	response[L"status"] = L"regions of interest synchronized";

	Json::Value img_array = Json::Value(Json::arrayValue);
	img_array = img_struct[L"images_structure"];

	for (unsigned int i = 0; i < img_array.size(); ++i) {
		int id = img_array[i][L"id"].asInt();
		int top = img_array[i][L"roi"][L"top"].asInt();
		int left = img_array[i][L"roi"][L"left"].asInt();
		int width = img_array[i][L"roi"][L"width"].asInt();
		int height = img_array[i][L"roi"][L"height"].asInt();
		mmImages::mmImageI* image = image_structure->GetImage(mmID(id));
		if (image != NULL) image->SetRegionOfInterest(mmRect(left, top, width, height));
	}

	// if client deleted any image or layer it has to be removed from structure, so lets find if any image was destroyed
	mmImages::mmImageI* current_image = NULL;
	mmImages::mmImageI* previous_image = NULL;
	mmImages::mmLayerI* current_layer = NULL;
	mmImages::mmLayerI* previous_layer = NULL;
	bool image_has_match = false;
	bool layer_has_match = false;

	do {
		current_image = image_structure->FindImage(previous_image);
		if (current_image != NULL) {
			// check if current image was deleted in client and remove it from structure if so
			image_has_match = false;
			for (unsigned int i = 0; i < img_array.size(); ++i) {
				if (current_image->GetID() == mmID(img_array[i][L"id"].asInt())) {
					image_has_match = true;
					// check if any layer of the current image was deleted in client and remove it from structure if so
					Json::Value layer_array = img_array[i][L"layers"];
					do {
						current_layer = current_image->FindLayer(previous_layer);
						if (current_layer != NULL) {
							layer_has_match = false;
							for (unsigned int j = 0; j < layer_array.size(); ++j) {
								if (current_layer->GetID() == mmID(layer_array[j][L"id"].asInt())) {
									layer_has_match = true;
									break;
								}
							}
							if (!layer_has_match) current_image->DeleteLayer(current_layer->GetID());
						}
						previous_layer = current_layer;
					}
					while (current_layer != NULL);
					break;
				}
			}
			if (!image_has_match) image_structure->DeleteImage(current_image->GetID());
		}
		previous_image = current_image;
	}
	while (current_image != NULL);

	text_response = true;
	return response;
}

Json::Value mmCalculationServer::Params_XML2JSON( mmString const & params_xml ) const
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
		param[L"value"] = v_tValue;
		converted.append(param);
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
