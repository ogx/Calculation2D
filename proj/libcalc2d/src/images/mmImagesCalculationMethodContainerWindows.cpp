#include <images\mmImagesCalculationMethodContainerWindows.h>

#include <interfaces\mmInterfaceInitializers.h>
#include <images\mmImagesCalculationMethodDLLBroker.h>
#include <windows\mmDllSupport.h>
#include <mmStringUtilities.h>

#include <windows.h>
#include <Imagehlp.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmImages::mmImagesCalculationMethodContainerForWindows
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmImages::mmImagesCalculationMethodContainerForWindows::mmImagesCalculationMethodContainerForWindows(mmLog::mmLogReceiverI *p_psLogReceiver): mmLog::mmLogSender(L"mmImages::mmImagesCalculationMethodContainerForWindows",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	SearchForDLLLibraries();
	SearchForImagesCalculationMethodsInDLLLibraries();

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmImages::mmImagesCalculationMethodContainerForWindows::~mmImagesCalculationMethodContainerForWindows()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> mmImages::mmImagesCalculationMethodContainerForWindows::GetAvailableImagesCalculationMethods(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetAvailableImagesCalculationMethods"));

	std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> v_vDefsOfKnownImagesCalculationMethods;

	std::size_t v_iDLL,v_iImp,v_iImpCount;
	std::size_t v_iDLLCount = m_vAvailableDLLs.size();
	for(v_iDLL=0; v_iDLL<v_iDLLCount; v_iDLL++)
	{
		v_iImpCount = m_vAvailableDLLs[v_iDLL].vImagesCalculationMethodDefs.size();

		for(v_iImp=0;v_iImp<v_iImpCount;v_iImp++)
		{
			v_vDefsOfKnownImagesCalculationMethods.push_back(m_vAvailableDLLs[v_iDLL].vImagesCalculationMethodDefs[v_iImp]);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetAvailableImagesCalculationMethods"));

	return v_vDefsOfKnownImagesCalculationMethods;
}

mmImages::mmImagesCalculationMethodI* mmImages::mmImagesCalculationMethodContainerForWindows::InitializeImagesCalculationMethod(mmString const & p_sCalculationMethodName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start InitializeImagesCalculationMethod"));

	mmImages::mmImagesCalculationMethodI* v_psInitializedImagesCalculationMethod = NULL;

	std::size_t v_iDLL,v_iImp,v_iImpCount;
	std::size_t v_iDLLCount = m_vAvailableDLLs.size();
	for(v_iDLL=0;v_iDLL<v_iDLLCount;v_iDLL++)
	{
		v_iImpCount = m_vAvailableDLLs[v_iDLL].vImagesCalculationMethodDefs.size();

		for(v_iImp=0;v_iImp<v_iImpCount;v_iImp++)
		{
			if(p_sCalculationMethodName.compare(m_vAvailableDLLs[v_iDLL].vImagesCalculationMethodDefs[v_iImp].sIDName) == 0)
			{
				v_psInitializedImagesCalculationMethod = new mmImages::mmImagesCalculationMethodDLLBroker(m_vAvailableDLLs[v_iDLL].sDLLName,
																																																								p_sCalculationMethodName,

																																																								GetLogReceiver());
			};
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End InitializeImagesCalculationMethod"));

	return v_psInitializedImagesCalculationMethod;
}

void mmImages::mmImagesCalculationMethodContainerForWindows::SearchForDLLLibraries(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SearchForDLLLibraries"));

	mmFileIO::mmFileUtilsI* v_psFileUtils = mmInterfaceInitializers::CreateFileUtils(this->GetLogReceiver());

	// clear existing detectors definition
	m_vAvailableDLLs.clear();

	sImagesCalculationMethodInDLL v_sDLLDef;

	mmString v_sAppDir = v_psFileUtils->GetApplicationDir();
	std::vector<mmFileIO::mmFileUtilsI::sDirElement> v_vApplicationDirElements;
	try {
		v_vApplicationDirElements = v_psFileUtils->GetDirElements(v_sAppDir,L"*.dll");
	} catch(mmError &) {}

	std::size_t v_iTemp;
	std::size_t v_iDirElementsCount = v_vApplicationDirElements.size();
	for(v_iTemp=0;v_iTemp<v_iDirElementsCount;v_iTemp++)
	{
		v_sDLLDef.sDLLName = v_sAppDir + mmString(L"\\") + v_vApplicationDirElements[v_iTemp].sName;
		m_vAvailableDLLs.push_back(v_sDLLDef);
	};

	delete v_psFileUtils;

	SendLogMessage(mmLog::debug,mmString(L"End SearchForDLLLibraries"));
}

std::map<mmString, mmString> mmImages::mmImagesCalculationMethodContainerForWindows::GetMethodFileMapping(void) const {
	std::map<mmString, mmString> v_sFileMapping;

	for(std::size_t v_iI = 0; v_iI < m_vAvailableDLLs.size(); ++v_iI)
		for(std::size_t v_iJ = 0; v_iJ < m_vAvailableDLLs[v_iI].vImagesCalculationMethodDefs.size(); ++v_iJ)
			v_sFileMapping[m_vAvailableDLLs[v_iI].vImagesCalculationMethodDefs[v_iJ].sIDName] = m_vAvailableDLLs[v_iI].sDLLName;

	return v_sFileMapping;
}

void mmImages::mmImagesCalculationMethodContainerForWindows::SearchForImagesCalculationMethodsInDLLLibraries(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SearchForImagesCalculationMethodsInDLLLibraries"));

	std::size_t v_iTemp;
	std::size_t v_iDirElementsCount = m_vAvailableDLLs.size();
	for(v_iTemp=0;v_iTemp<v_iDirElementsCount;v_iTemp++)
	{
		mmString v_sSymbolDLL_ImagesCalculationMethodsCount = mmDLLSupport::FindSymbolInDLLExportTable(m_vAvailableDLLs[v_iTemp].sDLLName,
																																																					mmString(L"mmDLLGetSupportedImagesCalculationMethodsCount"));

		mmString v_sSymbolDLL_ImagesCalculationMethodDef = mmDLLSupport::FindSymbolInDLLExportTable(m_vAvailableDLLs[v_iTemp].sDLLName,
																																																			 mmString(L"mmDLLGetSupportedImagesCalculationMethodDef"));

		if(v_sSymbolDLL_ImagesCalculationMethodsCount.size() > 0)
		{
			HINSTANCE v_hDLL = LoadLibrary(m_vAvailableDLLs[v_iTemp].sDLLName.c_str());

			mmString v_sDLLNameTemp = m_vAvailableDLLs[v_iTemp].sDLLName;

			mmDLLGetSupportedImagesCalculationMethodCount v_pfGetSupportedImagesCalculationMethodsCount = (mmDLLGetSupportedImagesCalculationMethodCount)GetProcAddress(v_hDLL,
																																																																																											 mmStringUtilities::MMStringToCharString(v_sSymbolDLL_ImagesCalculationMethodsCount).c_str());

			mmDLLGetSupportedImagesCalculationMethodDef v_pfGetSupportedImagesCalculationMethodDef = (mmDLLGetSupportedImagesCalculationMethodDef)GetProcAddress(v_hDLL,
																																																																																								mmStringUtilities::MMStringToCharString(v_sSymbolDLL_ImagesCalculationMethodDef).c_str());

			if(v_pfGetSupportedImagesCalculationMethodsCount != NULL)
			{
				mmImages::mmImagesCalculationMethodI::sCalculationMethodParams v_sCMP;
				mmInt v_iImp;
				mmInt v_iImpCount = v_pfGetSupportedImagesCalculationMethodsCount();
				//char v_pcTemp[256];

				for(v_iImp=0;v_iImp<v_iImpCount;v_iImp++)
				{
					v_sCMP = v_pfGetSupportedImagesCalculationMethodDef(v_iImp);

					m_vAvailableDLLs[v_iTemp].vImagesCalculationMethodDefs.push_back(v_sCMP);
				};
			};

			FreeLibrary(v_hDLL);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End SearchForImagesCalculationMethodsInDLLLibraries"));
}



