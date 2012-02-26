#include "mmCalculationServer.h"
#include <interfaces\mmInterfaceInitializers.h>
#include <memory>
#include "json\json.h"
#include <iostream>
#include <stdlib.h>

using namespace mmImages;


mmCalculationServer::mmCalculationServer(void) :
	calc_mgr(3, NULL),
	methods_mgr(NULL)
{
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
		else if(obj_in["cmd"].asString() == "getmethods")
			obj_out = this->GetStatus();
		else 
			obj_out = Json::Value("Unknown command!");

		os << fwriter.write(obj_out);
	}

	return 0;
}

Json::Value mmCalculationServer::GetMethods()
{
	typedef std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> method_infos_t;

	Json::Value res(Json::arrayValue), method_info;
	const size_t buf_size = INOUT_PARAMS_SIZE;
	size_t converted= 0;
	char buf[buf_size];

	method_infos_t method_infos = methods_mgr.GetAvailableImagesCalculationMethods();
	for(Json::ArrayIndex i=0, n=method_infos.size(); i<n; ++i)
	{
		method_info = Json::Value(Json::objectValue);
		wcstombs_s(&converted, buf, method_infos[i].sShortName, buf_size);
		method_info["name"] = buf;
		wcstombs_s(&converted, buf, method_infos[i].sIDName, buf_size);
		method_info["id"] = buf;
		wcstombs_s(&converted, buf, method_infos[i].sDescription, buf_size);
		method_info["description"] = buf;
		method_info["author"] = "Anonymous"; // TODO: support for author in plugin info
		method_info["params"] = Json::Value(Json::objectValue); // TODO: either translate params to JSON or switch to JSON in plugins
		wcstombs_s(&converted, buf, method_infos[i].sAutoParams.sInParams, buf_size);
		method_info["params"]["in"] = buf;
		wcstombs_s(&converted, buf, method_infos[i].sAutoParams.sOutParams, buf_size);
		method_info["params"]["out"] = buf;
		res[i] = method_info;
	}
	return res;
}

Json::Value mmCalculationServer::GetStatus()
{
	return Json::Value(Json::objectValue);
}

Json::Value mmCalculationServer::RunCalculationMethod( Json::Value& params )
{
	return Json::Value(Json::objectValue);
}
