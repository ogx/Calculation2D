#include <interfaces/mmInterfaceInitializers.h>
#include <mmOperatingSystemCalls.h>
#include <mmStringUtilities.h>
#include <plugin/mmCalcMethod.h>

////////////////////////////////////////////////////////////////////////////////
/// Additional tool class simplifying edition of method's parameters 
////////////////////////////////////////////////////////////////////////////////
namespace mmImages {
	class mmCMParameter
	{
	public:
		mmCMParameter(): m_sName(L""), 
			m_eType(mmXML::g_eXMLUnknownDataType),
			m_bIsOutput(false) { }
		mmCMParameter(mmString p_sName, 
			mmXML::mmXMLDataType p_eType, 
			void* p_psValue,
			bool p_bIsOutput = false)
		{
			m_sName = p_sName;
			m_eType = p_eType;
			m_bIsOutput = p_bIsOutput;
			m_pValue = p_psValue;
		}
		~mmCMParameter() { }

		mmString m_sName;
		mmXML::mmXMLDataType m_eType;
		mmXML::mmXMLNodePosition m_sPosition;
		void* m_pValue;
		bool m_bIsOutput;
	};
}

mmImages::mmCalcMethod::mmCalcMethod(mmLog::mmLogReceiverI *p_psLogReceiver, mmString p_sClassName) :
	mmLog::mmLogSender(p_sClassName,p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	swprintf_s(m_sCMParams.sShortName, L"Generic calculation method");
	swprintf_s(m_sCMParams.sIDName, L"{C13390E5-EBA6-404d-8706-B07FFE01C52F}");
	swprintf_s(m_sCMParams.sDescription,L"Generic calculation method");
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

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}
//---------------------------------------------------------------------------
mmImages::mmImagesCalculationMethodI::sCalculationMethodParams mmImages::mmCalcMethod::GetCalculationMethodInfo(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetCalculationMethodInfo"));

	UpdateParameters();

	SendLogMessage(mmLog::debug,mmString(L"End GetCalculationMethodInfo"));

	return m_sCMParams;
}
//---------------------------------------------------------------------------
void mmImages::mmCalcMethod::SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImageStructure, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetCalculationMethodParameters"));

	m_psImageStructure = p_psImageStructure;

	if (p_psAutomationParams != NULL) {
		std::auto_ptr<mmXML::mmXMLDocI> v_psXMLDoc(mmInterfaceInitializers::CreateXMLDocument());
		v_psXMLDoc->ParseXMLBuffer(p_psAutomationParams->sInParams);
		for (mmUInt i = 0; i < m_vParameters.size(); ++i) {
			mmXML::GetValue(v_psXMLDoc.get(), m_vParameters[i].m_sPosition, m_vParameters[i].m_pValue);
		}
	}

	m_rProgress = 0.0;
	//m_iThreadsCount = 0;
	m_bFinishImage = false;
	
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

	bool v_bResult = Calculate();

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

void mmImages::mmCalcMethod::SetParam(mmString p_sName, mmXML::mmXMLDataType p_eType, void* p_psValue, bool p_bIsOutput)
{
	std::vector<mmCMParameter>::iterator v_it;
	for (v_it = m_vParameters.begin(); v_it != m_vParameters.end(); ++v_it) {
		if (p_sName.compare((*v_it).m_sName) == 0) {
			return;
		}
	}
	mmCMParameter v_sParam(p_sName, p_eType, p_psValue, p_bIsOutput);
	m_vParameters.push_back(v_sParam);
}

void mmImages::mmCalcMethod::UpdateParameters()
{
	// create XML document which stores input automation options
	std::auto_ptr<mmXML::mmXMLDocI> v_psXMLInDoc(mmInterfaceInitializers::CreateXMLDocument(GetLogReceiver()));
	mmXML::mmXMLPositionedNode v_sXMLInRootPosNode = mmXML::CreateAutomationInput(v_psXMLInDoc.get());

	// create XML document which stores output automation options
	std::auto_ptr<mmXML::mmXMLDocI> v_psXMLOutDoc(mmInterfaceInitializers::CreateXMLDocument(GetLogReceiver()));
	mmXML::mmXMLPositionedNode v_sXMLOutRootPosNode = mmXML::CreateAutomationOutput(v_psXMLOutDoc.get());
	
	std::vector<mmImages::mmCMParameter>::iterator v_it;
	for (v_it = m_vParameters.begin(); v_it != m_vParameters.end(); ++v_it) {
		if ((*v_it).m_bIsOutput) {
			(*v_it).m_sPosition = mmXML::AddParam(&v_sXMLOutRootPosNode,
																						(*v_it).m_sName,
																						(*v_it).m_eType,
																						(*v_it).m_pValue);
		}
		else {
			(*v_it).m_sPosition = mmXML::AddParam(&v_sXMLInRootPosNode,
																						(*v_it).m_sName,
																						(*v_it).m_eType,
																						(*v_it).m_pValue);
		}
	}
	
	mmXML::CopyInputParams(v_psXMLInDoc.get(), &m_sCMParams.sAutoParams);
	mmXML::CopyOutputParams(v_psXMLOutDoc.get(), &m_sCMParams.sAutoParams);
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
