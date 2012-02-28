#include <images\mmImagesCalculationMethodDLLBroker.h>

#include <mmExecutionContext.h>
#include <windows\mmDllSupport.h>
#include <mmStringUtilities.h>

#include <interfaces\mmInterfaceInitializers.h>

mmImages::mmImagesCalculationMethodDLLBroker::mmImagesCalculationMethodDLLBroker(mmString const & p_sDLLName, mmString const & p_sImagesCalculationMethodName, mmLog::mmLogReceiverI *p_psLogReceiver) :
	mmLog::mmLogSender(L"mmImages::mmImagesCalculationMethodDLLBroker", p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	// restoring functions symbols from DLL
	mmString v_sDLLSymbol_mmDLLImagesCalculationMethod_Create = mmDLLSupport::FindSymbolInDLLExportTable(p_sDLLName, L"mmDLLCreateImagesCalculationMethod");
	mmString v_sDLLSymbol_mmDLLImagesCalculationMethod_Destroy = mmDLLSupport::FindSymbolInDLLExportTable(p_sDLLName, L"mmDLLDestroyImagesCalculationMethod");

	// loading images calculation method dll
	m_hDLLHandle = LoadLibrary(p_sDLLName.c_str());

	// storing pointer to images calculation method functions
	m_psDLLImagesCalculationMethod_Create = (mmDLLImagesCalculationMethod_Create) GetProcAddress(m_hDLLHandle, mmStringUtilities::MMStringToCharString(v_sDLLSymbol_mmDLLImagesCalculationMethod_Create).c_str());
	m_psDLLImagesCalculationMethod_Destroy = (mmDLLImagesCalculationMethod_Destroy) GetProcAddress(m_hDLLHandle, mmStringUtilities::MMStringToCharString(v_sDLLSymbol_mmDLLImagesCalculationMethod_Destroy).c_str());

	// initialize images calculation method
	m_psUtilsFactory = mmInterfaceInitializers::CreateUtilsFactory();
	mmExecutionContext v_sExecutionContext = {p_psLogReceiver, m_psUtilsFactory};
	m_psInitializedImagesCalculationMethod = m_psDLLImagesCalculationMethod_Create(&v_sExecutionContext, p_sImagesCalculationMethodName.c_str());

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmImages::mmImagesCalculationMethodDLLBroker::~mmImagesCalculationMethodDLLBroker()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	delete m_psUtilsFactory;

	// destroy images calculation method
	m_psDLLImagesCalculationMethod_Destroy(m_psInitializedImagesCalculationMethod);

	// free images calculation method dll
	FreeLibrary(m_hDLLHandle);

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

mmImages::mmImagesCalculationMethodI::sCalculationMethodParams mmImages::mmImagesCalculationMethodDLLBroker::GetCalculationMethodInfo(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetCalculationMethodInfo"));

	mmImages::mmImagesCalculationMethodI::sCalculationMethodParams v_sCMP = m_psInitializedImagesCalculationMethod->GetCalculationMethodInfo();

	SendLogMessage(mmLog::debug,mmString(L"End GetCalculationMethodInfo"));

	return v_sCMP;
}

void mmImages::mmImagesCalculationMethodDLLBroker::SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImageStructure, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetCalculationMethodParameters"));

	m_psInitializedImagesCalculationMethod->SetCalculationMethodParameters(p_psImageStructure,p_psAutomationParams);

	SendLogMessage(mmLog::debug,mmString(L"End SetCalculationMethodParameters"));
}

void mmImages::mmImagesCalculationMethodDLLBroker::UserAction(mmString p_sXMLParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UserAction"));

	UserAction(const_cast<wchar_t*>(p_sXMLParams.c_str()),p_sXMLParams.size());

	SendLogMessage(mmLog::debug,mmString(L"End UserAction"));
}

void mmImages::mmImagesCalculationMethodDLLBroker::UserAction(wchar_t* p_pcXMLParamsBuffer, mmInt p_iXMLParamsBufferSize)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UserAction"));

	//m_psInitializedImagesCalculationMethod->UserAction(p_pcXMLParamsBuffer,p_iXMLParamsBufferSize);

	SendLogMessage(mmLog::debug,mmString(L"End UserAction"));
}

bool mmImages::mmImagesCalculationMethodDLLBroker::Execute(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Execute"));

	bool v_bRes = m_psInitializedImagesCalculationMethod->Execute();

	SendLogMessage(mmLog::debug,mmString(L"End Execute"));

	return v_bRes;
}

void mmImages::mmImagesCalculationMethodDLLBroker::StopExecution(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start StopExecution"));

	m_psInitializedImagesCalculationMethod->StopExecution();

	SendLogMessage(mmLog::debug,mmString(L"End StopExecution"));
}

bool mmImages::mmImagesCalculationMethodDLLBroker::IsExecuting(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start IsExecuting"));

	bool v_bIsExecuting = m_psInitializedImagesCalculationMethod->IsExecuting();

	SendLogMessage(mmLog::debug,mmString(L"End IsExecuting"));

	return v_bIsExecuting;
}

mmReal mmImages::mmImagesCalculationMethodDLLBroker::GetProgress(void)
{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetProgress"));

	mmReal v_rProgress = m_psInitializedImagesCalculationMethod->GetProgress();

//	SendLogMessage(mmLog::debug,mmString(L"End GetProgress"));

	return v_rProgress;
}


