#include "mmCalculationServer.h"
#include <mmInterfaceInitializers.h>
#include <images\mmImagesCalculationManagement.h>
#include <images\mmImagesCalculationMethodContainerWindows.h>
#include <memory>
#include "json\json.h"
#include <iostream>
#include <stdlib.h>

using namespace mmImages;


mmCalculationServer::mmCalculationServer(void)
{
}


mmCalculationServer::~mmCalculationServer(void)
{
}

int mmCalculationServer::Serve()
{
	mmImagesCalculationManagement calc_mgr(3, NULL);
	mmImagesCalculationMethodContainerForWindows methods_mgr(NULL);
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

	//methods_mgr.GetAvailableImagesCalculationMethods();

	return 0;
}

Json::Value mmCalculationServer::GetMethods()
{
	Json::Value res(Json::arrayValue), method_info;
	char buf[32];
	for(Json::ArrayIndex i=0; i<3; ++i)
	{
		method_info = Json::Value(Json::objectValue);
		method_info["name"] = std::string("dummy method ") + itoa(i+1, buf, 10);
		method_info["id"] = i+1;
		method_info["description"] = "just a dummy for interface testing";
		method_info["author"] = "Janusz";
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
