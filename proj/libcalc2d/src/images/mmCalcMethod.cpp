
#pragma once

#include <mmInterfaceInitializers.h>
#include <mmOperatingSystemCalls.h>
#include <mmStringUtilities.h>
#include <plugin/mmCalcMethod.h>

mmImages::mmCalcMethod::mmCalcMethod(mmLog::mmLogReceiverI *p_psLogReceiver,
																		 mmString p_sClassName):
																		 mmLog::mmLogSender(p_sClassName,p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	swprintf(m_sCMParams.sShortName, L"Generic calculation method");
	swprintf(m_sCMParams.sIDName, L"{C13390E5-EBA6-404d-8706-B07FFE01C52F}");
	swprintf(m_sCMParams.sDescription,L"Generic calculation method");
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

	SendLogMessage(mmLog::debug,mmString(L"End GetCalculationMethodInfo"));

	return m_sCMParams;
}
//---------------------------------------------------------------------------
void mmImages::mmCalcMethod::SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImageStructure,
																														mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetCalculationMethodParameters"));

	m_psImageStructure = p_psImageStructure;

	if (p_psAutomationParams != NULL) {
		std::auto_ptr<mmXML::mmXMLDocI> v_psXMLDoc(mmInterfaceInitializers::CreateXMLDocument());
		v_psXMLDoc->ParseXMLBuffer(p_psAutomationParams->sInParams);
		for (mmUInt i = 0; i < m_vParameters.size(); ++i) {
			mmXML::GetValue(v_psXMLDoc.get(), m_vParameters[i].m_sPosition, m_vParameters[i].m_pValue.get());
		}
	}

	RetrieveParameters();
	m_rProgress = 0.0;
	m_iThreadsCount = 0;
	m_bFinishImage = false;
	
	// prepare map with first available row for each image
	m_mNextRows.clear();
	std::vector<mmString> v_sImageNames = GetImageNames();
	for (mmUInt i = 0; i < v_sImageNames.size(); ++i) {
		m_mNextRows[v_sImageNames[i]] = 0;
	}

	SendLogMessage(mmLog::debug,mmString(L"End SetCalculationMethodParameters"));
}
//---------------------------------------------------------------------------
void mmImages::mmCalcMethod::UserAction(mmString p_sXMLParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UserAction"));

	SendLogMessage(mmLog::debug,mmString(L"End UserAction"));
}
//---------------------------------------------------------------------------
void mmImages::mmCalcMethod::UserAction(wchar_t* p_pcXMLParamsBuffer, mmInt p_iXMLParamsBufferSize)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UserAction"));

	UserAction(p_pcXMLParamsBuffer);

	SendLogMessage(mmLog::debug,mmString(L"End UserAction"));
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
	//mmImageI* v_psLocalCurrentImage = NULL;
	//mmInt v_iLocalCurrentImage = 0;
	mmInt v_iBlockHeight = 0;
	mmInt v_iNextRowIndex = 0;
	mmInt v_iWidth = 0;
	mmInt v_iHeight = 0;
	mmInt v_iChannels = 0;
	std::vector<mmString> v_sImageNames = GetImageNames();
	bool v_bNewImage = false;
	bool v_bFinishImage = false;

	m_psThreadSynchEL->Lock();
		m_iThreadsCount++;
		v_iBlockHeight = m_iRowsCountInBlock;
	m_psThreadSynchEL->Unlock();
	
	for (mmUInt v_iIndex = 0; v_iIndex < m_psImageStructure->GetImageCount(); ++v_iIndex) {

		//m_psImageStructure->LockForRead();
		//v_psLocalCurrentImage = m_psImageStructure->GetImage(v_iIndex);
		//m_psImageStructure->UnlockFromRead();

		m_psThreadSynchEL->Lock();
			v_iNextRowIndex = m_mNextRows[v_sImageNames[v_iIndex]];
		m_psThreadSynchEL->Unlock();

		while (v_iNextRowIndex < v_iHeight) {
			m_psThreadSynchEL->Lock();
				v_iNextRowIndex = m_mNextRows[v_sImageNames[v_iIndex]];
				m_mNextRows[v_sImageNames[v_iIndex]] += v_iBlockHeight;
				m_rProgress = 100.0*((mmReal)v_iNextRowIndex/(mmReal)v_iHeight)*((mmReal)(v_iIndex + 1)/(mmReal)v_sImageNames.size());
				if (m_mNextRows[v_sImageNames[v_iIndex]] > v_iHeight) {
					m_mNextRows[v_sImageNames[v_iIndex]] = v_iHeight;
					v_iBlockHeight = v_iHeight - v_iNextRowIndex;
				}
				if (v_iNextRowIndex == 0) {
					ExecBeforeSingleImageCalc(v_iIndex /*v_psLocalCurrentImage*/);
					v_bNewImage = true;
					m_bFinishImage = false;
				}
				else v_bNewImage = false;
			m_psThreadSynchEL->Unlock();


			//(*p_psKernel)(v_psLocalCurrentImage, 
			//	v_iNextRowIndex,
			//	v_iBlockHeight);
			(*p_psKernel)(m_psImageStructure->GetImage(v_iIndex), 
				v_iNextRowIndex,
				v_iBlockHeight);
			
			m_psThreadSynchEL->Lock();
				v_iNextRowIndex = m_mNextRows[v_sImageNames[v_iIndex]];
				if (v_iNextRowIndex >= v_iHeight && !m_bFinishImage) {
					v_bFinishImage = true;
					m_bFinishImage = true;
					ExecAfterSingleImageCalc(v_iIndex /*v_psLocalCurrentImage*/);
				}
				else {
					v_bFinishImage = false;
				}
			m_psThreadSynchEL->Unlock();
		}	
	}

	m_psThreadSynchEL->Lock();
		m_iThreadsCount--;
		/*if (m_iThreadsCount <= 0) {
			m_psImageStructure->LockForRead();
				m_psImageStructure->UpdateImages();
			m_psImageStructure->UnlockFromRead();
		}*/
	m_psThreadSynchEL->Unlock();
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

const void* mmImages::mmCalcMethod::GetParam(mmString p_sName)
{
	std::vector<mmCMParameter>::iterator v_it;
	for (v_it = m_vParameters.begin(); v_it != m_vParameters.end(); ++v_it) {
		if (p_sName.compare((*v_it).m_sName) == 0) {
			return (*v_it).m_pValue.get();
		}
	}
	return NULL;
}

void mmImages::mmCalcMethod::GetParam(mmString p_sName, void* p_pValue)
{
	std::vector<mmCMParameter>::iterator v_it;

	for (v_it = m_vParameters.begin(); v_it != m_vParameters.end(); ++v_it) {
		if (p_sName.compare((*v_it).m_sName) == 0) {
			switch ((*v_it).m_eType) {
				case mmXML::g_eXMLReal:
					*(reinterpret_cast<mmReal*>(p_pValue)) = *(reinterpret_cast<mmReal*>((*v_it).m_pValue.get()));
					return;
				case mmXML::g_eXMLInt:
					*(reinterpret_cast<mmInt*>(p_pValue)) = *(reinterpret_cast<mmInt*>((*v_it).m_pValue.get()));
					return;
				case mmXML::g_eXMLBool:
					*(reinterpret_cast<bool*>(p_pValue)) = *(reinterpret_cast<bool*>((*v_it).m_pValue.get()));
					return;
				case mmXML::g_eXMLString:
				case mmXML::g_eXMLImageName:
				case mmXML::g_eXMLDataLayerName:						
					*(reinterpret_cast<mmString*>(p_pValue)) = *(reinterpret_cast<mmString*>((*v_it).m_pValue.get()));
					return;
			}
		}
	}
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
																						(*v_it).m_pValue.get());
		}
		else {
			(*v_it).m_sPosition = mmXML::AddParam(&v_sXMLInRootPosNode,
																						(*v_it).m_sName,
																						(*v_it).m_eType,
																						(*v_it).m_pValue.get());
		}
	}
	
	mmXML::CopyInputParams(v_psXMLInDoc.get(), &m_sCMParams.sAutoParams);
	mmXML::CopyOutputParams(v_psXMLOutDoc.get(), &m_sCMParams.sAutoParams);
}

std::vector<mmString> mmImages::mmCalcMethod::GetImageNames()
{
	std::vector<mmString> v_vResultCloudNames;

	// TODO: to trzeba zrobic jakos ladniej jesli uzupelniona zostanie funkcja GetXMLParameters()

	//m_psImageStructure->LockForRead();
		mmUInt v_iImageCount = m_psImageStructure->GetImageCount();
		for(mmUInt i = 0; i < v_iImageCount; ++i) {
			v_vResultCloudNames.push_back(m_psImageStructure->GetImage(i)->GetName());
		}
	//m_psImageStructure->UnlockFromRead();

	return v_vResultCloudNames;
}

std::vector<mmString> mmImages::mmCalcMethod::GetDLNames(mmUInt const p_iImage)
{
	std::vector<mmString> v_sResultDLNames;

	mmImageI* v_psImage = m_psImageStructure->GetImage(p_iImage);
	if(! v_psImage)
		return v_sResultDLNames;

	//v_psImage->LockForRead();
		mmUInt v_iDLCount = v_psImage->GetLayerCount();
		for(mmUInt i = 0; i < v_iDLCount; ++i) {
			v_sResultDLNames.push_back(v_psImage->GetLayer(i)->GetName());
		}
	//v_psImage->UnlockFromRead();

	return v_sResultDLNames;
}
