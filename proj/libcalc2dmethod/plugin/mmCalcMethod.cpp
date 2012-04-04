#include <interfaces/mmInterfaceInitializers.h>
#include <mmOperatingSystemCalls.h>
#include <mmStringUtilities.h>
#include <plugin/mmCalcMethod.h>

mmImages::mmErrorHandlerI* g_psErrorHandler = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Additional tool class simplifying edition of method's parameters 
////////////////////////////////////////////////////////////////////////////////

mmImages::mmCalcMethod::mmCalcMethod(mmLog::mmLogReceiverI *p_psLogReceiver, mmString p_sClassName) :
	mmLog::mmLogSender(p_sClassName,p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	swprintf_s(m_sCMParams.sShortName, L"Generic calculation method");
	swprintf_s(m_sCMParams.sIDName, L"{C13390E5-EBA6-404d-8706-B07FFE01C52F}");
	swprintf_s(m_sCMParams.sDescription, L"Generic calculation method");
	m_sCMParams.bIsMultithreaded = true;

	m_psThreadSynchEL.reset(mmInterfaceInitializers::CreateExclusiveLock(NULL));

	m_rProgress = 0.0;
	m_bIsExecuting = false;
	m_bStopExecution = false;

	m_psImageStructure = NULL;

	m_iRowsCountInBlock = 100;

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}
//---------------------------------------------------------------------------
mmImages::mmCalcMethod::~mmCalcMethod()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));
	
	for(std::list<mmGenericParamI*>::iterator v_sParam = m_sInputParams.begin(); v_sParam != m_sInputParams.end(); ++v_sParam)
		delete *v_sParam;

	for(std::list<mmGenericParamI*>::iterator v_sParam = m_sOutputParams.begin(); v_sParam != m_sOutputParams.end(); ++v_sParam)
		delete *v_sParam;

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}
//---------------------------------------------------------------------------

void mmImages::mmCalcMethod::SerializeParameters()
{
	::wcscpy_s(m_sCMParams.sAutoParams.sInParams, mmImages::SerializeParameters(mmImages::g_pAutoCalcXML_INParams_Node, m_sInputParams).c_str());
	::wcscpy_s(m_sCMParams.sAutoParams.sOutParams, mmImages::SerializeParameters(mmImages::g_pAutoCalcXML_OUTParams_Node, m_sOutputParams).c_str());
}

void mmImages::mmCalcMethod::DeserializeParameters(mmImagesCalculationMethodI::sCalculationAutomationParams const & p_sAutomationParams)
{
	mmImages::DeserializeParameters(p_sAutomationParams.sInParams, m_sInputParams);
	mmImages::DeserializeParameters(p_sAutomationParams.sOutParams, m_sOutputParams);
}

mmImages::mmImagesCalculationMethodI::sCalculationMethodParams mmImages::mmCalcMethod::GetCalculationMethodInfo(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetCalculationMethodInfo"));

	// serialize parameters
	SerializeParameters();

	SendLogMessage(mmLog::debug,mmString(L"End GetCalculationMethodInfo"));

	return m_sCMParams;
}
//---------------------------------------------------------------------------

void mmImages::mmCalcMethod::SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImageStructure, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetCalculationMethodParameters"));

	m_psImageStructure = p_psImageStructure;

	m_rProgress = 0.0;
	//m_iThreadsCount = 0;
	m_bFinishImage = false;

	// deserialize parameters
	DeserializeParameters(*p_psAutomationParams);
	
	// prepare map with first available row for each image
	m_mNextRows.clear();
	std::vector<mmID> v_sImageIDs = GetImageIDs();
	for (mmUInt i = 0; i < v_sImageIDs.size(); ++i) {
		m_mNextRows[v_sImageIDs[i]] = 0;
	}

	SendLogMessage(mmLog::debug,mmString(L"End SetCalculationMethodParameters"));
}
//---------------------------------------------------------------------------
bool mmImages::mmCalcMethod::Execute(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Execute"));
	
	bool v_bResult = false;
	try {
		v_bResult = Calculate();
	} catch(mmError const & err) {
		if(g_psErrorHandler)
			g_psErrorHandler->HandleError(err.GetErrorString(), err.GetErrorCode());
		else
			throw;
	}

	SendLogMessage(mmLog::debug,mmString(L"End Execute"));

	return v_bResult;
}
//---------------------------------------------------------------------------
void mmImages::mmCalcMethod::StopExecution(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start StopExecution"));

	m_bStopExecution = true;

	SendLogMessage(mmLog::debug,mmString(L"End StopExecution"));
}
//---------------------------------------------------------------------------
bool mmImages::mmCalcMethod::IsExecuting(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start IsExecuting"));

	SendLogMessage(mmLog::debug,mmString(L"End IsExecuting"));

	return m_bIsExecuting;
}
//---------------------------------------------------------------------------
mmReal mmImages::mmCalcMethod::GetProgress(void)
{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetProgress"));

//	SendLogMessage(mmLog::debug,mmString(L"End GetProgress"));

	return m_rProgress;
}
//---------------------------------------------------------------------------
void mmImages::mmCalcMethod::ForEachImage(mmCalcKernelI* p_psKernel)
{
	mmInt v_iBlockHeight = 0;
	mmInt v_iNextRowIndex = 0;
	mmInt v_iHeight = 0;
	mmReal const v_rImageCount = static_cast<mmReal>(m_psImageStructure->GetImageCount());
	bool v_bNewImage = false;
	bool v_bFinishImage = false;

	m_psThreadSynchEL->Lock();
		//m_iThreadsCount++;
		v_iBlockHeight = m_iRowsCountInBlock;
	m_psThreadSynchEL->Unlock();
	
	mmImageI * v_psImage;
	mmInt v_iIndex;
	for (v_psImage = m_psImageStructure->FindImage(), v_iIndex = 0; NULL != v_psImage; v_psImage = m_psImageStructure->FindImage(v_psImage), ++v_iIndex ) {

		v_iHeight = v_psImage->GetHeight();

		m_psThreadSynchEL->Lock();
			v_iNextRowIndex = m_mNextRows[v_psImage->GetID()];
		m_psThreadSynchEL->Unlock();

		while (v_iNextRowIndex < v_iHeight) {
			m_psThreadSynchEL->Lock();
				v_iNextRowIndex = m_mNextRows[v_psImage->GetID()];
				m_mNextRows[v_psImage->GetID()] += v_iBlockHeight;
				m_rProgress = 100.0 * (static_cast<mmReal>(v_iNextRowIndex) / static_cast<mmReal>(v_iHeight)) * (static_cast<mmReal>(v_iIndex + 1) / v_rImageCount);
				if (m_mNextRows[v_psImage->GetID()] > v_iHeight) {
					m_mNextRows[v_psImage->GetID()] = v_iHeight;
					v_iBlockHeight = v_iHeight - v_iNextRowIndex;
				}
				if (v_iNextRowIndex == 0) {
					OnBeforeEachImage(v_psImage);
					v_bNewImage = true;
					m_bFinishImage = false;
				}
				else v_bNewImage = false;
			m_psThreadSynchEL->Unlock();

			(*p_psKernel)(v_psImage, 
				v_iNextRowIndex,
				v_iBlockHeight);
			
			m_psThreadSynchEL->Lock();
				v_iNextRowIndex = m_mNextRows[v_psImage->GetID()];
				if (v_iNextRowIndex >= v_iHeight && !m_bFinishImage) {
					v_bFinishImage = true;
					m_bFinishImage = true;
					OnAfterEachImage(v_psImage);
				}
				else {
					v_bFinishImage = false;
				}
			m_psThreadSynchEL->Unlock();
		}	
	}

	/*m_psThreadSynchEL->Lock();
		m_iThreadsCount--;
	m_psThreadSynchEL->Unlock();*/
}

std::vector<mmID> mmImages::mmCalcMethod::GetImageIDs()
{
	std::vector<mmID> v_vResultCloudIDs;

	// TODO: to trzeba zrobic jakos ladniej jesli uzupelniona zostanie funkcja GetXMLParameters()

	//m_psImageStructure->LockForRead();
	for(mmImageI * v_psImage = m_psImageStructure->FindImage(); NULL != v_psImage; v_psImage = m_psImageStructure->FindImage(v_psImage))
		v_vResultCloudIDs.push_back(v_psImage->GetID());
	//m_psImageStructure->UnlockFromRead();

	return v_vResultCloudIDs;
}

std::vector<mmString> mmImages::mmCalcMethod::GetImageNames()
{
	std::vector<mmString> v_vResultCloudNames;

	// TODO: to trzeba zrobic jakos ladniej jesli uzupelniona zostanie funkcja GetXMLParameters()

	//m_psImageStructure->LockForRead();
		for(mmImageI * v_psImage = m_psImageStructure->FindImage(); NULL != v_psImage; v_psImage = m_psImageStructure->FindImage(v_psImage))
			v_vResultCloudNames.push_back(v_psImage->GetName());
	//m_psImageStructure->UnlockFromRead();

	return v_vResultCloudNames;
}

std::vector<mmString> mmImages::mmCalcMethod::GetDLNames(mmImageI const * const p_psImage)
{
	std::vector<mmString> v_sResultDLNames;

	if(! p_psImage)
		return v_sResultDLNames;

	//v_psImage->LockForRead();
		for(mmLayerI * v_psLayer = p_psImage->FindLayer(); NULL != v_psLayer; v_psLayer = p_psImage->FindLayer(v_psLayer))
			v_sResultDLNames.push_back(v_psLayer->GetName());
	//v_psImage->UnlockFromRead();

	return v_sResultDLNames;
}
