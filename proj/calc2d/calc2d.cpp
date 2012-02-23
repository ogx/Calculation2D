// calc2d.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mmInterfaceInitializers.h>
#include <images\mmImagesCalculationManagement.h>
#include <images\mmImagesCalculationMethodContainerWindows.h>
#include <memory>


using namespace mmImages;

int _tmain(int argc, _TCHAR* argv[])
{
	mmImagesCalculationManagement calc_mgr(3, NULL);
	mmImagesCalculationMethodContainerForWindows methods_mgr(NULL);
	std::auto_ptr<mmImageStructureI> image(mmInterfaceInitializers::CreateDefaultImageStructure(NULL, NULL));

	//methods_mgr.GetAvailableImagesCalculationMethods();

	return 0;
}

