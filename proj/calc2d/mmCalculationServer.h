#pragma once
#include "json\json-forwards.h"

class mmCalculationServer
{
public:
	mmCalculationServer(void);
	~mmCalculationServer(void);

	int Serve();

private: 
	Json::Value GetMethods();
	Json::Value GetStatus();
	Json::Value RunCalculationMethod(Json::Value&);
};

