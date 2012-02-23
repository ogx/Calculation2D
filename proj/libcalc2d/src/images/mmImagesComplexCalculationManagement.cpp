#include <images\mmImagesComplexCalculationManagement.h>

#include <mmStringUtilities.h>
#include <mmOperatingSystemCalls.h>

const wchar_t* mmImages::g_pComplexCalcXML_PatternContainer_Node = L"ComplexPatternContainer";

const wchar_t* mmImages::g_pComplexCalcXML_Pattern_Node = L"ComplexPattern";
	const wchar_t* mmImages::g_pComplexCalcXML_PatternName_Node = L"Name";
	const wchar_t* mmImages::g_pComplexCalcXML_PatternIDName_Node = L"IDName";
	const wchar_t* mmImages::g_pComplexCalcXML_PatternDescription_Node = L"Description";
	const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParams_Node = L"InputParams";
		const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParam_Node = L"InputParam";
		const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParam_Constant_Attr = L"ConstParam";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParamName_Node = L"InputParamName";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParamType_Node = L"InputParamType";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParamValue_Node = L"InputParamValue";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParamValueModifiedNodeName_Node = L"InputParamModified";
	const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputParams_Node = L"OutputParams";
		const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputParam_Node = L"OutputParam";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputParamName_Node = L"OutputParamName";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputParamType_Node = L"OutputParamType";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputParamValue_Node = L"OutputParamValue";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputParamValueModifiedNodeName_Node = L"OutputParamModified";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputParamValueModifiedSecondNodeName_Node = L"SecondOutputParamModified";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node = L"OutputParamOperation";
	const wchar_t* mmImages::g_pComplexCalcXML_PatternDefinition_Node = L"PatternDefinition";
		const wchar_t* mmImages::g_pComplexCalcXML_PatternCalcMethod_Node = L"CalculationMethod";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternCalcMethodIDName_Node = L"CalculationMethodIDName";
		const wchar_t* mmImages::g_pComplexCalcXML_PatternInternalAction_Node = L"InternalAction";
			const wchar_t* mmImages::g_pComplexCalcXML_PatternInternalActionName_Node = L"InternalActionName";

// PB //
//const wchar_t* mmImages::g_pComplexCalcXML_Action_AddSimpleCalculationMethod = L"Add Calculation Method";
//const wchar_t* mmImages::g_pComplexCalcXML_Action_AddOutputValueFromExisting = L"Add Modified Output Value";
//const wchar_t* mmImages::g_pComplexCalcXML_Action_MixOutputValuesFromExisting = L"Mix Output Values";
//const wchar_t* mmImages::g_pComplexCalcXML_Action_AddInputValueFromExistingOrConst = L"Add Constant/Modified Input Value";
//const wchar_t* mmImages::g_pComplexCalcXML_Action_AddInputValueConstDecision = L"Add Input Decision Value";

// PB //
//const wchar_t* mmImages::g_pComplexCalcXML_Action_AddWhileLoop = L"Add WHILE loop";
//const wchar_t* mmImages::g_pComplexCalcXML_Action_AddEndWhile = L"Add END_WHILE";
//const wchar_t* mmImages::g_pComplexCalcXML_Action_CopyBinaryOutputToInputValue = L"Copy Decision From Output To Input Value";
//const wchar_t* mmImages::g_pComplexCalcXML_Action_CopyIntOutputToInputValue = L"Copy Int From Output To Input Value";
//const wchar_t* mmImages::g_pComplexCalcXML_Action_CopyRealOutputToInputValue = L"Copy Real From Output To Input Value";

// PB //
//const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParamUserSpecified = L"[User_Specified]";
//const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParamConstant_Real = L"[Constant_Real]";
//const wchar_t* mmImages::g_pComplexCalcXML_PatternInputParamConstant_Int = L"[Constant_Int]";

const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Addition = L"Addition";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Subtraction = L"Subtraction";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Multiplication = L"Multiplication";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Division = L"Division";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Power = L"Power";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Minimum = L"Minimum";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Maximum = L"Maximum";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Equal = L"Equal";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Less = L"Less";
const wchar_t* mmImages::g_pComplexCalcXML_PatternOutputFormula_Greater = L"Greater";

mmImages::mmImagesComplexCalculationManagement::mmImagesComplexCalculationManagement(	mmImages::mmImagesCalculationMethodContainerI* p_psImageCalculationMethodContainer,
																						mmImages::mmImagesCalculationManagement* p_psImageCalculationManagement,
																						mmLog::mmLogReceiverI *p_psLogReceiver):
																						mmLog::mmLogSender(L"mmImages::mmImagesComplexCalculationManagement",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	m_psImageCalculationMethodContainer = p_psImageCalculationMethodContainer;
	m_psImageCalculationManagement = p_psImageCalculationManagement;

	m_vSimpleCalculationMethodsInfo = p_psImageCalculationMethodContainer->GetAvailableImagesCalculationMethods();

	m_psXMLCurrentPatternDefinition = NULL;
	m_bIsNew = true;
	m_psXMLCalculatedPatternDefinition = NULL;

	m_sComplexPatternContainerXMLFile = mmOperatingSystem::GetApplicationDirectory() +
										mmString(L"\\ComplexPatterns.icpxml");
	m_psXMLComplexPatternContainer = NULL;
	LoadComplexPatternsFromFile(m_sComplexPatternContainerXMLFile);

// PB //
//	m_hUIActiveDialog = -1;

	m_bIsActiveCalculation = false;

	m_psStateChangeReceiver = NULL;

	//m_psThreadSynchronizationEL = mmInterfaceInitializers::CreateExclusiveLock(GetLogReceiver());
	m_psThreadSynchronizationEL = mmInterfaceInitializers::CreateExclusiveLock(NULL);

	m_iThreadExecutionSleepTime = 10;
	//m_psControlThread = mmInterfaceInitializers::CreateDefaultThread(GetLogReceiver());
	m_psControlThread = mmInterfaceInitializers::CreateDefaultThread(NULL);
	m_psControlThread->RegisterThreadExecutionClass(this);
	m_psControlThread->Run();

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmImages::mmImagesComplexCalculationManagement::~mmImagesComplexCalculationManagement()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	m_psControlThread->Stop();
	while(m_psControlThread->GetStatus() != mmThread::mmThreadI::finished)
	{
		mmOperatingSystem::StopThread(1);
	};
	delete m_psControlThread;

	if( m_psXMLComplexPatternContainer )
		delete m_psXMLComplexPatternContainer;

	delete m_psThreadSynchronizationEL;

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

mmInt mmImages::mmImagesComplexCalculationManagement::GetCountOfComplexCalculationPatterns(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetCountOfComplexCalculationPatterns"));

	mmInt v_iCCPCount = 0;

	std::vector<mmXML::mmXMLNodeI*> v_vCCPNodes = m_psXMLComplexPatternContainer->GetXMLRootNode()->GetChilds();
	v_iCCPCount = v_vCCPNodes.size();

	SendLogMessage(mmLog::debug,mmString(L"End GetCountOfComplexCalculationPatterns"));

	return v_iCCPCount;
}

mmImages::mmImagesComplexCalculationManagement::sComplexCalculationInfo
	mmImages::mmImagesComplexCalculationManagement::GetComplexCalculationPatternInfo(mmInt p_iPatternID)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetComplexCalculationPatternInfo"));

	mmImages::mmImagesComplexCalculationManagement::sComplexCalculationInfo v_sCCPI;

	v_sCCPI.sName = m_psXMLComplexPatternContainer->
									GetXMLRootNode()->
									GetChild(p_iPatternID)->
									FindChild(mmImages::g_pComplexCalcXML_PatternName_Node)->
									GetText();
	v_sCCPI.sIDName = m_psXMLComplexPatternContainer->
										GetXMLRootNode()->
										GetChild(p_iPatternID)->
										FindChild(mmImages::g_pComplexCalcXML_PatternIDName_Node)->
										GetText();
	v_sCCPI.sDescription = m_psXMLComplexPatternContainer->
												 GetXMLRootNode()->
												 GetChild(p_iPatternID)->
												 FindChild(mmImages::g_pComplexCalcXML_PatternDescription_Node)->
												 GetText();

	SendLogMessage(mmLog::debug,mmString(L"End GetComplexCalculationPatternInfo"));

	return v_sCCPI;
}

void mmImages::mmImagesComplexCalculationManagement::ExecuteComplexCalculationPattern(	mmString p_sPatternIDName,
																						mmImages::mmImageStructureI* p_psImageStructure)
{
	SendLogMessage(mmLog::debug,mmString(L"Start ExecuteComplexCalculationPattern"));

	sComplexCalculationDefinition v_sCCD;
	v_sCCD.sIDName = p_sPatternIDName;
	v_sCCD.psImageStructure = p_psImageStructure;

	// add pattern to execute list
	m_psThreadSynchronizationEL->Lock();
		m_vCalculationSheme.push_back(v_sCCD);
	m_psThreadSynchronizationEL->Unlock();

	SendLogMessage(mmLog::debug,mmString(L"End ExecuteComplexCalculationPattern"));
}

void mmImages::mmImagesComplexCalculationManagement::RegisterStateChangeReceiver(mmImagesComplexCalculationManagementStateChangeReceiverI* p_psStateChangeReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start RegisterStateChangeReceiver"));

	m_psStateChangeReceiver = p_psStateChangeReceiver;

	SendLogMessage(mmLog::debug,mmString(L"End RegisterStateChangeReceiver"));
}

void mmImages::mmImagesComplexCalculationManagement::UnregisterStateChangeReceiver(mmImagesComplexCalculationManagementStateChangeReceiverI* p_psStateChangeReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UnregisterStateChangeReceiver"));

	m_psStateChangeReceiver = NULL;

	SendLogMessage(mmLog::debug,mmString(L"End UnregisterStateChangeReceiver"));
}

bool mmImages::mmImagesComplexCalculationManagement::IsCalculating(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start IsCalculating"));

	SendLogMessage(mmLog::debug,mmString(L"End IsCalculating"));

	return m_bIsActiveCalculation;
}

bool mmImages::mmImagesComplexCalculationManagement::Execute(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Execute"));

	mmReal v_rCalculationStart;

	m_bTerminateControlThreadExecution = false;

	while(!m_bTerminateControlThreadExecution)
	{
		if(m_bIsActiveCalculation)
		{
			m_bIsActiveCalculation = !ManageCurrentComplexCalculationPatternExecution();
		}
		else
		{
			// check if are some patterns to execute
			m_psThreadSynchronizationEL->Lock();
				if(m_vCalculationSheme.size() > 0)
				{
					m_sCurrentComplexCalculation = m_vCalculationSheme.front();
					m_vCalculationSheme.pop_front();

					m_psXMLCalculatedPatternDefinition = GetComplexCalculationPatternDefinition(m_sCurrentComplexCalculation.sIDName);
					m_iCurrentComplexCalculationItem = -1;
				};
			m_psThreadSynchronizationEL->Unlock();

			// execute them
			if(m_psXMLCalculatedPatternDefinition != NULL)
			{
// PB //
//				InitializeCurrentComplexCalculationPatternExecution();

				m_bIsActiveCalculation = !ManageCurrentComplexCalculationPatternExecution();
			};
		};

		mmOperatingSystem::StopThread(m_iThreadExecutionSleepTime);
	};

	SendLogMessage(mmLog::debug,mmString(L"End Execute"));

	return true;
}

void mmImages::mmImagesComplexCalculationManagement::StopExecution(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start StopExecution"));

	m_bTerminateControlThreadExecution = true;

	SendLogMessage(mmLog::debug,mmString(L"End StopExecution"));
}

bool mmImages::mmImagesComplexCalculationManagement::IsExecuting(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start IsExecuting"));

	SendLogMessage(mmLog::debug,mmString(L"End IsExecuting"));

	return !m_bTerminateControlThreadExecution;
}

mmReal mmImages::mmImagesComplexCalculationManagement::GetProgress(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetProgress"));

	SendLogMessage(mmLog::debug,mmString(L"End GetProgress"));

	return 0.0;
}

// PB //
//mmImages::mmImagesCalculationMethodI::sCalculationMethodParams*
//	mmImages::mmImagesComplexCalculationManagement::GetCalculationMethodInfo(mmString p_sCalculationMethodIDName)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetCalculationMethodInfo"));
//
//	mmInt v_iCMCount = m_vSimpleCalculationMethodsInfo.size();
//	for(mmInt v_iCM=0;v_iCM<v_iCMCount;v_iCM++)
//	{
//		if(p_sCalculationMethodIDName.compare(m_vSimpleCalculationMethodsInfo[v_iCM].sIDName) == 0)
//		{
//			return &m_vSimpleCalculationMethodsInfo[v_iCM];
//		};
//	};
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetCalculationMethodInfo"));
//
//	return NULL;
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::GetCalculationMethodName(mmString p_sCalculationMethodIDName)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetCalculationMethodName"));
//
//	mmString v_sOutName;
//
//	mmInt v_iCMCount = m_vSimpleCalculationMethodsInfo.size();
//	for(mmInt v_iCM=0;v_iCM<v_iCMCount;v_iCM++)
//	{
//		if(p_sCalculationMethodIDName.compare(m_vSimpleCalculationMethodsInfo[v_iCM].sIDName) == 0)
//		{
//			v_sOutName = m_vSimpleCalculationMethodsInfo[v_iCM].sShortName;
//		};
//	};
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetCalculationMethodName"));
//
//	return v_sOutName;
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::UpdateInputOutputParameters(mmString p_psInOutParams)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start UpdateInputOutputParameters"));
//
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// parse string
//	v_psXMLDoc->ParseXMLBuffer(p_psInOutParams);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//
//	std::vector<mmXML::mmXMLNodeI*> v_vInputParams = v_psRootNode->GetChild(1)->GetChilds();
//	mmInt v_iIPCount = v_vInputParams.size();
//	for(mmInt v_iIP=1;v_iIP<v_iIPCount;v_iIP++)
//	{
//		std::vector<mmXML::sXMLAttribute> v_vOrigAttrParams = v_vInputParams[v_iIP]->GetAttributes();
//
//		if(v_vOrigAttrParams.size() > 0)
//		{
//			if(v_vOrigAttrParams[1].sValue.compare(mmImages::g_pAutoCalcXML_Params_ParamType_ImagesNames) == 0)
//			{
//
//				std::vector<mmString> v_vClNamesVals =
//					mmStringUtilities::MMStringToVectorOfMMString(	v_vInputParams[v_iIP]->
//																	FindChild(mmUI::g_pUIXML_EL_DEF_Value)->
//																	GetText());
//
//				// DEBUG
//				//mmInt v_iNCount = v_vClNamesVals.size();
//				//for(mmInt v_iN=0;v_iN<v_iNCount;v_iN++)
//				//{
//				//	SendLogMessage(mmLog::info,mmString(L"XXXXXXXXXX ") +v_vClNamesVals[v_iN]);
//				//};
//
//				if(v_vClNamesVals[0].compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudCount_All) != 0)
//				{
//					mmInt v_iCCount = mmStringUtilities::StringToMMInt(v_vClNamesVals[0]);
//					for(mmInt v_iC=0;v_iC<v_iCCount;v_iC++)
//					{
//			mmString v_sTemp = v_vClNamesVals[v_iC+1];
//
//						if(v_vClNamesVals[v_iC+1].compare(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified) == 0)
//						{
//							mmString v_sNewInputParamName = GetFromUIReadableInputParameterName(L"Write input parameter name",
//																																									v_psRootNode->GetChild(0)->GetText(),
//																																									v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Name)->GetText() +
//																																									mmString(L" - Cloud ") +
//																																									mmStringUtilities::MMIntToString(v_iC+1));
//
//							v_vClNamesVals[v_iC+1] = v_sNewInputParamName;
//
//							mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParam_Node);
//								// add name
//								mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Name = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node);
//								v_psNewPattern_InputValues_NewNode_Name->SetText(v_sNewInputParamName);
//								// add type
//								mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Type = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node);
//								v_psNewPattern_InputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName);
//								// add value
//								mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Value = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node);
//								v_psNewPattern_InputValues_NewNode_Value->SetText(v_sNewInputParamName);
//						};
//					};
//
//					mmString v_sFinalCloudNames = v_vClNamesVals[0];
//					mmInt v_iNCount = v_vClNamesVals.size();
//					for(mmInt v_iN=1;v_iN<v_iNCount;v_iN++)
//					{
//						v_sFinalCloudNames += mmString(L" ") +
//																	v_vClNamesVals[v_iN];
//					};
//					v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->SetText(v_sFinalCloudNames);
//				};
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//			{
//				if(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText().compare(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified) == 0)
//				{
//					mmString v_sNewInputParamName = GetFromUIReadableInputParameterName(L"Write input parameter name",
//																																							v_psRootNode->GetChild(0)->GetText(),
//																																							v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Name)->GetText());
//
//					v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->SetText(v_sNewInputParamName);
//
//					mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParam_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Name = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node);
//						v_psNewPattern_InputValues_NewNode_Name->SetText(v_sNewInputParamName);
//						// add type
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Type = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node);
//						v_psNewPattern_InputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Value = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node);
//						v_psNewPattern_InputValues_NewNode_Value->SetText(v_sNewInputParamName);
//				};
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//			{
//				if(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText().compare(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified) == 0)
//				{
//					mmString v_sNewInputParamName = GetFromUIReadableInputParameterName(L"Write input parameter name",
//																																							v_psRootNode->GetChild(0)->GetText(),
//																																							v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Name)->GetText());
//
//					v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->SetText(v_sNewInputParamName);
//
//					mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParam_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Name = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node);
//						v_psNewPattern_InputValues_NewNode_Name->SetText(v_sNewInputParamName);
//						// add type
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Type = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node);
//						v_psNewPattern_InputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName);
//						// add value
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Value = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node);
//						v_psNewPattern_InputValues_NewNode_Value->SetText(v_sNewInputParamName);
//				};
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//			{
//				if(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText().compare(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified) == 0)
//				{
//					mmString v_sNewInputParamName = GetFromUIReadableInputParameterName(L"Write input parameter name",
//																																							v_psRootNode->GetChild(0)->GetText(),
//																																							v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Name)->GetText());
//
//					v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->SetText(v_sNewInputParamName);
//
//					mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParam_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Name = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node);
//						v_psNewPattern_InputValues_NewNode_Name->SetText(v_sNewInputParamName);
//						// add type
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Type = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node);
//						v_psNewPattern_InputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName);
//						// add value
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Value = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node);
//						v_psNewPattern_InputValues_NewNode_Value->SetText(v_sNewInputParamName);
//				};
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//			{
//				if(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText().compare(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified) == 0)
//				{
//					mmString v_sNewInputParamName = GetFromUIReadableInputParameterName(L"Write input parameter name",
//																																							v_psRootNode->GetChild(0)->GetText(),
//																																							v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Name)->GetText());
//
//					v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->SetText(v_sNewInputParamName);
//
//					mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParam_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Name = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node);
//						v_psNewPattern_InputValues_NewNode_Name->SetText(v_sNewInputParamName);
//						// add type
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Type = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node);
//						v_psNewPattern_InputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Value = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node);
//						v_psNewPattern_InputValues_NewNode_Value->SetText(v_sNewInputParamName);
//				};
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//			{
//				if(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText().compare(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified) == 0)
//				{
//					mmString v_sNewInputParamName = GetFromUIReadableInputParameterName(L"Write input parameter name",
//																																							v_psRootNode->GetChild(0)->GetText(),
//																																							v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Name)->GetText());
//
//					v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->SetText(v_sNewInputParamName);
//
//					mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParam_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Name = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node);
//						v_psNewPattern_InputValues_NewNode_Name->SetText(v_sNewInputParamName);
//						// add type
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Type = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node);
//						v_psNewPattern_InputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psNewPattern_InputValues_NewNode_Value = v_psNewPattern_InputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node);
//						v_psNewPattern_InputValues_NewNode_Value->SetText(v_sNewInputParamName);
//				};
//			};
//		};
//	};
//
//	std::vector<mmXML::mmXMLNodeI*> v_vOutputParams = v_psRootNode->GetChild(2)->GetChilds();
//	mmInt v_iOPCount = v_vOutputParams.size();
//	for(mmInt v_iOP=1;v_iOP<v_iOPCount;v_iOP++)
//	{
//		std::vector<mmXML::sXMLAttribute> v_vOrigAttrParams = v_vOutputParams[v_iOP]->GetAttributes();
//
//		if(v_vOrigAttrParams.size() > 0)
//		{
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//			{
//				mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParam_Node);
//					// add name
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Name = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node);
//					v_psNewPattern_OutputValues_NewNode_Name->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//					// add type
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Type = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node);
//					v_psNewPattern_OutputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue);
//					// add value
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Value = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamValue_Node);
//					v_psNewPattern_OutputValues_NewNode_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//			{
//				mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParam_Node);
//					// add name
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Name = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node);
//					v_psNewPattern_OutputValues_NewNode_Name->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//					// add type
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Type = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node);
//					v_psNewPattern_OutputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName);
//					// add value
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Value = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamValue_Node);
//					v_psNewPattern_OutputValues_NewNode_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//			{
//				mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParam_Node);
//					// add name
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Name = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node);
//					v_psNewPattern_OutputValues_NewNode_Name->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//					// add type
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Type = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node);
//					v_psNewPattern_OutputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName);
//					// add value
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Value = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamValue_Node);
//					v_psNewPattern_OutputValues_NewNode_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//			{
//				mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParam_Node);
//					// add name
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Name = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node);
//					v_psNewPattern_OutputValues_NewNode_Name->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//					// add type
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Type = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node);
//					v_psNewPattern_OutputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue);
//					// add value
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Value = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamValue_Node);
//					v_psNewPattern_OutputValues_NewNode_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//			};
//			if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//			{
//				mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParams_Node)->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParam_Node);
//					// add name
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Name = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node);
//					v_psNewPattern_OutputValues_NewNode_Name->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//					// add type
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Type = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node);
//					v_psNewPattern_OutputValues_NewNode_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue);
//					// add value
//					mmXML::mmXMLNodeI* v_psNewPattern_OutputValues_NewNode_Value = v_psNewPattern_OutputValues_NewNode->AddChild(mmClouds::g_pComplexCalcXML_PatternOutputParamValue_Node);
//					v_psNewPattern_OutputValues_NewNode_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//			};
//		};
//	};
//
//	mmString v_sOutXML = v_psXMLDoc->SaveToXMLBuffer();
//
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End UpdateInputOutputParameters"));
//
//	return v_sOutXML;
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::GetFromUIReadableInputParameterName(	mmString p_sTitle,
//																								mmString p_sDescription,
//																								mmString p_sParamName)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetFromUIReadableInputParameterName"));
//
//	// create XML document which specifies new input parameter name
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// add dialog root node
//	v_psXMLDoc->CreateXMLRootNode(mmUI::g_pUIXML_EL_UserDialog);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//		// add visible name for dialog
//		mmXML::mmXMLNodeI* v_psRootNodeName = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//		v_psRootNodeName->SetText(p_sTitle);
//
//		// add input param group node
//		mmXML::mmXMLNodeI* v_psInParamGroupNode = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_Group);
//			// add visible name for group node
//			mmXML::mmXMLNodeI* v_psInParamGroupNodeName = v_psInParamGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//			v_psInParamGroupNodeName->SetText(p_sDescription);
//
//			// add name field
//			mmXML::mmXMLNodeI* v_psInParamName = v_psInParamGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psInParamNameName = v_psInParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psInParamNameName->SetText(p_sParamName);
//				// type
//				mmXML::mmXMLNodeI* v_psInParamNameType = v_psInParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psInParamNameType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// value
//				mmXML::mmXMLNodeI* v_psInParamNameValue = v_psInParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psInParamNameValue->SetText(L"");
//
//	mmString v_sOutXML = m_psMainWindow->ShowModalDialog(v_psXMLDoc->SaveToXMLBuffer(),NULL);
//
//	v_psXMLDoc->ParseXMLBuffer(v_sOutXML);
//	v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//
//	mmString v_sOutName = v_psRootNode->GetChild(1)->GetChild(1)->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText();
//
//  delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetFromUIReadableInputParameterName"));
//
//	return v_sOutName;
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::CreateNewOutputValueFromExistingUI(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start CreateNewOutputValueFromExistingUI"));
//
//	// create XML document which specifies new output parameter name
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// add dialog root node
//	v_psXMLDoc->CreateXMLRootNode(mmUI::g_pUIXML_EL_UserDialog);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//		// add visible name for dialog
//		mmXML::mmXMLNodeI* v_psRootNodeName = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//		v_psRootNodeName->SetText(L"Modify existing output value");
//
//		// add output param group node
//		mmXML::mmXMLNodeI* v_psGroupNode = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_Group);
//			// add visible name for group node
//			mmXML::mmXMLNodeI* v_psGroupNodeName = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//			v_psGroupNodeName->SetText(L"");
//
//			// add existing output value selection
//			mmXML::mmXMLNodeI* v_psOutValSelection = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psOutValSelectionName = v_psOutValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psOutValSelectionName->SetText(L" Select output value to modify: ");
//				// type
//				mmXML::mmXMLNodeI* v_psOutValSelectionType = v_psOutValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psOutValSelectionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// values range
//				mmXML::mmXMLNodeI* v_psOutValSelectionRange = v_psOutValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//				mmXML::mmXMLNodeI* v_psOutValSelectionRangeStringList = v_psOutValSelectionRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//				std::vector<mmXML::mmXMLNodeI*> v_vOutParams = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParams_Node)->GetChilds();
//				mmInt v_iOPCount = v_vOutParams.size();
//				for(mmInt v_iOP=0;v_iOP<v_iOPCount;v_iOP++)
//				{
//					if(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOutValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//					};
//					if(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOutValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//					};
//				};
//				std::vector<mmXML::mmXMLNodeI*> v_vInParams = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->GetChilds();
//				mmInt v_iIPCount = v_vInParams.size();
//				for(mmInt v_iIP=0;v_iIP<v_iIPCount;v_iIP++)
//				{
//					if(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOutValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//					};
//					if(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOutValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//					};
//				};
//				// value
//				mmXML::mmXMLNodeI* v_psOutValSelectionValue = v_psOutValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psOutValSelectionValue->SetText(L"");
//
//			// add formula input
//			mmXML::mmXMLNodeI* v_psFormula = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psFormulaName = v_psFormula->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psFormulaName->SetText(L" Set operation: ");
//				// type
//				mmXML::mmXMLNodeI* v_psFormulaType = v_psFormula->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psFormulaType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// values range
//				mmXML::mmXMLNodeI* v_psFormulaRange = v_psFormula->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//				mmXML::mmXMLNodeI* v_psFormulaRangeStringList = v_psFormulaRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//					mmXML::mmXMLNodeI* v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Multiplication);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Division);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Power);
//				// value
//				mmXML::mmXMLNodeI* v_psFormulaValue = v_psFormula->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psFormulaValue->SetText(L"");
//
//			// add modification input
//			mmXML::mmXMLNodeI* v_psMultVal = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psMultValName = v_psMultVal->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psMultValName->SetText(L" Set modification value: ");
//				// type
//				mmXML::mmXMLNodeI* v_psMultValType = v_psMultVal->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psMultValType->SetText(mmUI::g_pUIXML_EL_DEF_Type_Real);
//				// value
//				mmXML::mmXMLNodeI* v_psMultValValue = v_psMultVal->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psMultValValue->SetText(L"1.0");
//
//			// add name for new value
//			mmXML::mmXMLNodeI* v_psNewParamName = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psNewParamNameName = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psNewParamNameName->SetText(L" Set new name for output value: ");
//				// type
//				mmXML::mmXMLNodeI* v_psNewParamNameType = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psNewParamNameType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// value
//				mmXML::mmXMLNodeI* v_psNewParamNameValue = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psNewParamNameValue->SetText(L"");
//
//	mmString v_sOutXML = v_psXMLDoc->SaveToXMLBuffer();
//
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End CreateNewOutputValueFromExistingUI"));
//
//	return v_sOutXML;
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::CreateNewOutputValueFromMixOfTwoExistingUI(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start CreateNewOutputValueFromMixOfTwoExistingUI"));
//
//	// create XML document which specifies new output parameter name
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// add dialog root node
//	v_psXMLDoc->CreateXMLRootNode(mmUI::g_pUIXML_EL_UserDialog);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//		// add visible name for dialog
//		mmXML::mmXMLNodeI* v_psRootNodeName = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//		v_psRootNodeName->SetText(L"Mix two existing output values");
//
//		// add output param group node
//		mmXML::mmXMLNodeI* v_psGroupNode = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_Group);
//			// add visible name for group node
//			mmXML::mmXMLNodeI* v_psGroupNodeName = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//			v_psGroupNodeName->SetText(L"");
//
//			// add existing output 1st value selection
//			mmXML::mmXMLNodeI* v_psOut1ValSelection = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psOut1ValSelectionName = v_psOut1ValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psOut1ValSelectionName->SetText(L" Select 1st output value to modify: ");
//				// type
//				mmXML::mmXMLNodeI* v_psOut1ValSelectionType = v_psOut1ValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psOut1ValSelectionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// values range
//				mmXML::mmXMLNodeI* v_psOut1ValSelectionRange = v_psOut1ValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//				mmXML::mmXMLNodeI* v_psOut1ValSelectionRangeStringList = v_psOut1ValSelectionRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//				std::vector<mmXML::mmXMLNodeI*> v_vOutParams = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParams_Node)->GetChilds();
//				mmInt v_iOPCount = v_vOutParams.size();
//				for(mmInt v_iOP=0;v_iOP<v_iOPCount;v_iOP++)
//				{
//					if(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOut1ValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//					};
//					if(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOut1ValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//					};
//				};
//				std::vector<mmXML::mmXMLNodeI*> v_vInParams = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->GetChilds();
//				mmInt v_iIPCount = v_vInParams.size();
//				for(mmInt v_iIP=0;v_iIP<v_iIPCount;v_iIP++)
//				{
//					if(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOut1ValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//					};
//					if(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOut1ValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//					};
//				};
//				// value
//				mmXML::mmXMLNodeI* v_psOut1ValSelectionValue = v_psOut1ValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psOut1ValSelectionValue->SetText(L"");
//
//			// add existing output 2nd value selection
//			mmXML::mmXMLNodeI* v_psOut2ValSelection = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psOut2ValSelectionName = v_psOut2ValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psOut2ValSelectionName->SetText(L" Select 2nd output value to modify: ");
//				// type
//				mmXML::mmXMLNodeI* v_psOut2ValSelectionType = v_psOut2ValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psOut2ValSelectionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// values range
//				mmXML::mmXMLNodeI* v_psOut2ValSelectionRange = v_psOut2ValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//				mmXML::mmXMLNodeI* v_psOut2ValSelectionRangeStringList = v_psOut2ValSelectionRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//				for(mmInt v_iOP=0;v_iOP<v_iOPCount;v_iOP++)
//				{
//					if(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOut2ValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//					};
//					if(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOut2ValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vOutParams[v_iOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//					};
//				};
//				for(mmInt v_iIP=0;v_iIP<v_iIPCount;v_iIP++)
//				{
//					if(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOut2ValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//					};
//					if(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psOut2ValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//					};
//				};
//				// value
//				mmXML::mmXMLNodeI* v_psOut2ValSelectionValue = v_psOut2ValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psOut2ValSelectionValue->SetText(L"");
//
//			// add formula input
//			mmXML::mmXMLNodeI* v_psFormula = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psFormulaName = v_psFormula->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psFormulaName->SetText(L" Set operation: ");
//				// type
//				mmXML::mmXMLNodeI* v_psFormulaType = v_psFormula->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psFormulaType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// values range
//				mmXML::mmXMLNodeI* v_psFormulaRange = v_psFormula->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//				mmXML::mmXMLNodeI* v_psFormulaRangeStringList = v_psFormulaRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//					mmXML::mmXMLNodeI* v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Addition);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Subtraction);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Multiplication);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Division);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Minimum);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Maximum);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Equal);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Less);
//					v_psUIStringListElement = v_psFormulaRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternOutputFormula_Greater);
//				// value
//				mmXML::mmXMLNodeI* v_psFormulaValue = v_psFormula->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psFormulaValue->SetText(L"");
//
//			// add name for new value
//			mmXML::mmXMLNodeI* v_psNewParamName = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psNewParamNameName = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psNewParamNameName->SetText(L" Set new name for output value: ");
//				// type
//				mmXML::mmXMLNodeI* v_psNewParamNameType = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psNewParamNameType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// value
//				mmXML::mmXMLNodeI* v_psNewParamNameValue = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psNewParamNameValue->SetText(L"");
//
//	mmString v_sOutXML = v_psXMLDoc->SaveToXMLBuffer();
//
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End CreateNewOutputValueFromMixOfTwoExistingUI"));
//
//	return v_sOutXML;
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::CreateNewConstantInputValueUI(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start CreateNewConstantInputValueUI"));
//
//	// create XML document which specifies new input parameter name
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// add dialog root node
//	v_psXMLDoc->CreateXMLRootNode(mmUI::g_pUIXML_EL_UserDialog);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//		// add visible name for dialog
//		mmXML::mmXMLNodeI* v_psRootNodeName = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//		v_psRootNodeName->SetText(L"Create/Modify existing input value");
//
//		// add input param group node
//		mmXML::mmXMLNodeI* v_psGroupNode = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_Group);
//			// add visible name for group node
//			mmXML::mmXMLNodeI* v_psGroupNodeName = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//			v_psGroupNodeName->SetText(L"");
//
//			// add existing input value selection
//			mmXML::mmXMLNodeI* v_psInValSelection = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psInValSelectionName = v_psInValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psInValSelectionName->SetText(L" Select input value: ");
//				// type
//				mmXML::mmXMLNodeI* v_psInValSelectionType = v_psInValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psInValSelectionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// values range
//				mmXML::mmXMLNodeI* v_psInValSelectionRange = v_psInValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//				mmXML::mmXMLNodeI* v_psInValSelectionRangeStringList = v_psInValSelectionRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//				std::vector<mmXML::mmXMLNodeI*> v_vInParams = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->GetChilds();
//				mmInt v_iIPCount = v_vInParams.size();
//				for(mmInt v_iIP=0;v_iIP<v_iIPCount;v_iIP++)
//				{
//					if(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psInValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//					};
//					if(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//					{
//						mmXML::mmXMLNodeI* v_psUIStringListElement = v_psInValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//						v_psUIStringListElement->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//					};
//				};
//				mmXML::mmXMLNodeI* v_psUIStringListElement = v_psInValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//				v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamConstant_Real);
//				v_psUIStringListElement = v_psInValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//				v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamConstant_Int);
//				// value
//				mmXML::mmXMLNodeI* v_psInValSelectionValue = v_psInValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psInValSelectionValue->SetText(L"");
//
//			// add multiplication input
//			mmXML::mmXMLNodeI* v_psMultVal = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psMultValName = v_psMultVal->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psMultValName->SetText(L" Set modification/constant value: ");
//				// type
//				mmXML::mmXMLNodeI* v_psMultValType = v_psMultVal->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psMultValType->SetText(mmUI::g_pUIXML_EL_DEF_Type_Real);
//				// value
//				mmXML::mmXMLNodeI* v_psMultValValue = v_psMultVal->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psMultValValue->SetText(L"1.0");
//
//			// add name for new value
//			mmXML::mmXMLNodeI* v_psNewParamName = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psNewParamNameName = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psNewParamNameName->SetText(L" Set new name for input value: ");
//				// type
//				mmXML::mmXMLNodeI* v_psNewParamNameType = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psNewParamNameType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// value
//				mmXML::mmXMLNodeI* v_psNewParamNameValue = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psNewParamNameValue->SetText(L"");
//
//	mmString v_sOutXML = v_psXMLDoc->SaveToXMLBuffer();
//
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End CreateNewConstantInputValueUI"));
//
//	return v_sOutXML;
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::CreateNewConstantInputDecisionUI(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start CreateNewConstantInputDecisionUI"));
//
//	// create XML document which specifies new input parameter name
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// add dialog root node
//	v_psXMLDoc->CreateXMLRootNode(mmUI::g_pUIXML_EL_UserDialog);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//		// add visible name for dialog
//		mmXML::mmXMLNodeI* v_psRootNodeName = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//		v_psRootNodeName->SetText(L"Create input constant decision");
//
//		// add input param group node
//		mmXML::mmXMLNodeI* v_psGroupNode = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_Group);
//			// add visible name for group node
//			mmXML::mmXMLNodeI* v_psGroupNodeName = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//			v_psGroupNodeName->SetText(L"");
//
//			// add existing input value selection
//			mmXML::mmXMLNodeI* v_psInValSelection = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psInValSelectionName = v_psInValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psInValSelectionName->SetText(L" Select input value: ");
//				// type
//				mmXML::mmXMLNodeI* v_psInValSelectionType = v_psInValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psInValSelectionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// values range
//				mmXML::mmXMLNodeI* v_psInValSelectionRange = v_psInValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//				mmXML::mmXMLNodeI* v_psInValSelectionRangeStringList = v_psInValSelectionRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//					mmXML::mmXMLNodeI* v_psUIStringListElement = v_psInValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue_YES);
//					v_psUIStringListElement = v_psInValSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//					v_psUIStringListElement->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue_NO);
//				// value
//				mmXML::mmXMLNodeI* v_psInValSelectionValue = v_psInValSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psInValSelectionValue->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue_YES);
//
//			// add name for new value
//			mmXML::mmXMLNodeI* v_psNewParamName = v_psGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//				// name
//				mmXML::mmXMLNodeI* v_psNewParamNameName = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//				v_psNewParamNameName->SetText(L" Set new name for input value: ");
//				// type
//				mmXML::mmXMLNodeI* v_psNewParamNameType = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//				v_psNewParamNameType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//				// value
//				mmXML::mmXMLNodeI* v_psNewParamNameValue = v_psNewParamName->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//				v_psNewParamNameValue->SetText(L"");
//
//	mmString v_sOutXML = v_psXMLDoc->SaveToXMLBuffer();
//
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End CreateNewConstantInputDecisionUI"));
//
//	return v_sOutXML;
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::AddSingleCalculationMethodIntoPattern(	mmString p_sCalculationMethodParams,
//																							mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddSingleCalculationMethodIntoPattern"));
//
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// parse string
//	v_psXMLDoc->ParseXMLBuffer(p_sCalculationMethodParams);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//
//	mmXML::mmXMLNodeI* v_psXMLCCNewMethod = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->FindChild(mmImages::g_pComplexCalcXML_PatternDefinition_Node)->AddChild(mmImages::g_pComplexCalcXML_PatternCalcMethod_Node);
//		// add name of CM
//		mmXML::mmXMLNodeI* v_psXMLCCNewMethodName = v_psXMLCCNewMethod->AddChild(mmImages::g_pComplexCalcXML_PatternCalcMethodIDName_Node);
//		v_psXMLCCNewMethodName->SetText(v_psRootNode->FindChild(mmUI::g_pUIXML_EL_DEF_Name)->GetText());
//
//		// adding input params node
//		mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParams = v_psXMLCCNewMethod->AddChild(mmClouds::g_pAutoCalcXML_INParams_Node);
//
//		// add CM input params
//		std::vector<mmXML::mmXMLNodeI*> v_vInputParams = v_psRootNode->GetChild(1)->GetChilds();
//		mmInt v_iIPCount = v_vInputParams.size();
//		for(mmInt v_iIP=1;v_iIP<v_iIPCount;v_iIP++)
//		{
//			std::vector<mmXML::sXMLAttribute> v_vOrigAttrParams = v_vInputParams[v_iIP]->GetAttributes();
//
//			if(v_vOrigAttrParams.size() > 0)
//			{
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam = v_psXMLCCNewMethod_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Name = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLCCNewMethod_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLCCNewMethod_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Type = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Value = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam = v_psXMLCCNewMethod_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Name = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLCCNewMethod_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLCCNewMethod_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Type = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Value = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam = v_psXMLCCNewMethod_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Name = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLCCNewMethod_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLCCNewMethod_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Type = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Value = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam = v_psXMLCCNewMethod_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Name = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLCCNewMethod_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLCCNewMethod_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Type = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Value = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam = v_psXMLCCNewMethod_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Name = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLCCNewMethod_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLCCNewMethod_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Type = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_InParam_Value = v_psXMLCCNewMethod_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//			};
//		};
//
//		// adding output params node
//		mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParams = v_psXMLCCNewMethod->AddChild(mmClouds::g_pAutoCalcXML_OUTParams_Node);
//
//		std::vector<mmXML::mmXMLNodeI*> v_vOutputParams = v_psRootNode->GetChild(2)->GetChilds();
//		mmInt v_iOPCount = v_vOutputParams.size();
//		for(mmInt v_iOP=1;v_iOP<v_iOPCount;v_iOP++)
//		{
//			std::vector<mmXML::sXMLAttribute> v_vOrigAttrParams = v_vOutputParams[v_iOP]->GetAttributes();
//
//			if(v_vOrigAttrParams.size() > 0)
//			{
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam = v_psXMLCCNewMethod_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Name = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLCCNewMethod_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Type = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Value = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam = v_psXMLCCNewMethod_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Name = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLCCNewMethod_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Type = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Value = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam = v_psXMLCCNewMethod_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Name = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLCCNewMethod_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Type = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Value = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam = v_psXMLCCNewMethod_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Name = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLCCNewMethod_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Type = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Value = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam = v_psXMLCCNewMethod_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Name = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLCCNewMethod_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Type = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLCCNewMethod_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLCCNewMethod_OutParam_Value = v_psXMLCCNewMethod_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLCCNewMethod_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//			};
//		};
//
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End AddSingleCalculationMethodIntoPattern"));
//}

void mmImages::mmImagesComplexCalculationManagement::LoadComplexPatternsFromFile(mmString p_sCPFile)
{
	SendLogMessage(mmLog::debug,mmString(L"Start LoadComplexPatternsFromFile"));

	delete m_psXMLComplexPatternContainer;
	m_psXMLComplexPatternContainer = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());

	if(mmOperatingSystem::IsExistingFile(p_sCPFile))
	{
		m_psXMLComplexPatternContainer->ParseXMLFile(p_sCPFile);
	}
	else
	{
		m_psXMLComplexPatternContainer->CreateXMLRootNode(mmImages::g_pComplexCalcXML_PatternContainer_Node);
	};

	SendLogMessage(mmLog::debug,mmString(L"End LoadComplexPatternsFromFile"));
}

// PB ??
//void mmImages::mmImagesComplexCalculationManagement::SaveComplexPatternsIntoFile(mmString p_sCPFile)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start SaveComplexPatternsIntoFile"));
//
//	m_psXMLComplexPatternContainer->SaveToXMLFile(p_sCPFile);
//
//	SendLogMessage(mmLog::debug,mmString(L"End SaveComplexPatternsIntoFile"));
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::AddComplexPatternIntoContainer(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddComplexPatternIntoContainer"));
//
//	/*
//	std::vector<mmXML::mmXMLNodeI*> v_vPatternCMs = 	p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->
//														FindChild(mmClouds::g_pComplexCalcXML_PatternDefinition_Node)->
//														GetChilds();
//
//	if(v_vPatternCMs.size() > 0)
//	{
//		// check name of pattern; if this name exists, add extension _YYYYMMDDHHMMSS
//		mmString v_sNewPatternName = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->
//																 FindChild(mmClouds::g_pComplexCalcXML_PatternName_Node)->
//																 GetText();
//		std::vector<mmXML::mmXMLNodeI*> v_vCCPatterns = m_psXMLComplexPatternContainer->GetXMLRootNode()->GetChilds();
//		mmInt v_iCCPCount = v_vCCPatterns.size();
//		for(mmInt v_iCCP=0;v_iCCP<v_iCCPCount;v_iCCP++)
//		{
//			if(v_vCCPatterns[v_iCCP]->FindChild(mmClouds::g_pComplexCalcXML_PatternName_Node)->GetText().compare(v_sNewPatternName) == 0)
//			{
//				v_sNewPatternName += 	mmString(L" ") +
//										mmStringUtilities::GetCurrentDateTimeString();
//
//				break;
//			};
//		};
//		p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->FindChild(mmClouds::g_pComplexCalcXML_PatternName_Node)->SetText(v_sNewPatternName);
//
//		m_psXMLComplexPatternContainer->GetXMLRootNode()->AddChildWithStructure(p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode());
//
//		SaveComplexPatternsIntoFile(m_sComplexPatternContainerXMLFile);
//
//		if(m_psStateChangeReceiver != NULL)
//		{
//			m_psStateChangeReceiver->ListOfComplexCalculationPatternsUpdated();
//		};
//	};
//	*/
//
//	m_psXMLComplexPatternContainer->GetXMLRootNode()->AddChildWithStructure(p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode());
//
//	SaveComplexPatternsIntoFile(m_sComplexPatternContainerXMLFile);
//
//	if(m_psStateChangeReceiver != NULL)
//	{
//		m_psStateChangeReceiver->ListOfComplexCalculationPatternsUpdated();
//	};
//
//	SendLogMessage(mmLog::debug,mmString(L"End AddComplexPatternIntoContainer"));
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::UpdateComplexPatternInContainer(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start UpdateComplexPatternInContainer"));
//
//	std::vector<mmXML::mmXMLNodeI*> v_vPatternCMs = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->
//																									FindChild(mmImages::g_pComplexCalcXML_PatternDefinition_Node)->
//																									GetChilds();
//
//	if(v_vPatternCMs.size() > 0)
//	{
//		// find pattern with given name
//		mmString v_sNewPatternIDName = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->
//																	 FindChild(mmImages::g_pComplexCalcXML_PatternIDName_Node)->
//																	 GetText();
//		std::vector<mmXML::mmXMLNodeI*> v_vCCPatterns = m_psXMLComplexPatternContainer->GetXMLRootNode()->GetChilds();
//		mmInt v_iCCPCount = v_vCCPatterns.size();
//		for(mmInt v_iCCP=0;v_iCCP<v_iCCPCount;v_iCCP++)
//		{
//			if(v_vCCPatterns[v_iCCP]->FindChild(mmImages::g_pComplexCalcXML_PatternIDName_Node)->GetText().compare(v_sNewPatternIDName) == 0)
//			{
//				v_vCCPatterns[v_iCCP]->RemoveAllChilds();
//
//				std::vector<mmXML::mmXMLNodeI*> v_vCurrentPatternChilds = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->GetChilds();
//				mmInt v_iCCount = v_vCurrentPatternChilds.size();
//				for(mmInt v_iC=0;v_iC<v_iCCount;v_iC++)
//				{
//					v_vCCPatterns[v_iCCP]->AddChildWithStructure(v_vCurrentPatternChilds[v_iC]);
//				};
//
//				break;
//			};
//		};
//
//		SaveComplexPatternsIntoFile(m_sComplexPatternContainerXMLFile);
//
//		if(m_psStateChangeReceiver != NULL)
//		{
//			m_psStateChangeReceiver->ListOfComplexCalculationPatternsUpdated();
//		};
//	};
//
//	SendLogMessage(mmLog::debug,mmString(L"End UpdateComplexPatternInContainer"));
//}

mmXML::mmXMLNodeI* mmImages::mmImagesComplexCalculationManagement::GetComplexCalculationPatternDefinition(mmString p_sCCPIDName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetComplexCalculationPatternDefinition"));

	std::vector<mmXML::mmXMLNodeI*> v_vCCPatterns = 	m_psXMLComplexPatternContainer->GetXMLRootNode()->
														GetChilds();

	mmInt v_iCCPCount = v_vCCPatterns.size();
	for(mmInt v_iCCP=0;v_iCCP<v_iCCPCount;v_iCCP++)
	{
		if(v_vCCPatterns[v_iCCP]->FindChild(mmImages::g_pComplexCalcXML_PatternIDName_Node)->GetText().compare(p_sCCPIDName) == 0)
		{
			return v_vCCPatterns[v_iCCP];
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetComplexCalculationPatternDefinition"));

	return NULL;
}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::AddInternalWHILELoop(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddInternalWHILELoop"));
//
//	// define input-output params
//	// create XML document which stores input automation options
//	mmXML::mmXMLDocI* v_psINXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psINXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_INParams_Node);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psINXMLDoc->GetXMLRootNode();
//		// add while control variable param
//		mmXML::mmXMLNodeI* v_psWHILEControlVariable = v_psRootNode->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node);
//			// name
//			mmXML::mmXMLNodeI* v_psWHILEControlVariableName = v_psWHILEControlVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node);
//			v_psWHILEControlVariableName->SetText(L"Continue WHILE loop?");
//			// type
//			mmXML::mmXMLNodeI* v_psWHILEControlVariableType = v_psWHILEControlVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node);
//			v_psWHILEControlVariableType->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue);
//			// value
//			mmXML::mmXMLNodeI* v_psWHILEControlVariableValue = v_psWHILEControlVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
//			v_psWHILEControlVariableValue->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES);
//
//	mmString v_sInParams = v_psINXMLDoc->SaveToXMLBuffer();
//	delete v_psINXMLDoc;
//
//	// create XML document which stores output automation options
//	mmXML::mmXMLDocI* v_psOUTXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psOUTXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_OUTParams_Node);
//
//	mmString v_sOutParams = v_psOUTXMLDoc->SaveToXMLBuffer();
//	delete v_psOUTXMLDoc;
//
//	// user set values of params
//	bool v_bNoParams = false;
//	mmString v_sCMInOutParams = GetUIDefinitionForInternalAction(	mmImages::g_pComplexCalcXML_Action_AddWhileLoop,
//																	v_sInParams,
//																	v_sOutParams,
//																	&v_bNoParams);
//
//	mmString v_sSCMDef = v_sCMInOutParams;
//	if(!v_bNoParams)
//	{
//		v_sSCMDef = m_psMainWindow->ShowModalDialog(v_sCMInOutParams,this);
//	};
//
//	// update in/out params of pattern
//	mmString v_sSCMInOutDef = UpdateInputOutputParameters(v_sSCMDef);
//
//	// add internal action into calculation pattern
//	AddInternalMethodIntoPattern(v_sSCMInOutDef,
//															 m_psXMLCurrentPatternDefinition);
//
//	SendLogMessage(mmLog::debug,mmString(L"End AddInternalWHILELoop"));
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::AddInternalENDWHILELoop(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddInternalENDWHILELoop"));
//
//	// define input-output params
//	// create XML document which stores input automation options
//	mmXML::mmXMLDocI* v_psINXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psINXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_INParams_Node);
//
//	mmString v_sInParams = v_psINXMLDoc->SaveToXMLBuffer();
//	delete v_psINXMLDoc;
//
//	// create XML document which stores output automation options
//	mmXML::mmXMLDocI* v_psOUTXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psOUTXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_OUTParams_Node);
//
//	mmString v_sOutParams = v_psOUTXMLDoc->SaveToXMLBuffer();
//	delete v_psOUTXMLDoc;
//
//	// user set values of params
//	bool v_bNoParams = false;
//	mmString v_sCMInOutParams = GetUIDefinitionForInternalAction(mmImages::g_pComplexCalcXML_Action_AddEndWhile,
//																															 v_sInParams,
//																															 v_sOutParams,
//																															 &v_bNoParams);
//
//	mmString v_sSCMDef = v_sCMInOutParams;
//	if(!v_bNoParams)
//	{
//		v_sSCMDef = m_psMainWindow->ShowModalDialog(v_sCMInOutParams,this);
//	};
//
//	// update in/out params of pattern
//	mmString v_sSCMInOutDef = UpdateInputOutputParameters(v_sSCMDef);
//
//	// add internal action into calculation pattern
//	AddInternalMethodIntoPattern(v_sSCMInOutDef,
//															 m_psXMLCurrentPatternDefinition);
//
//	SendLogMessage(mmLog::debug,mmString(L"End AddInternalENDWHILELoop"));
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::AddInternalCopyBinaryOutToInVariable(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddInternalCopyBinaryOutToInVariable"));
//
//	// define input-output params
//	// create XML document which stores input automation options
//	mmXML::mmXMLDocI* v_psINXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psINXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_INParams_Node);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psINXMLDoc->GetXMLRootNode();
//		// add selection of surce output variable
//		mmXML::mmXMLNodeI* v_psSelSourceVariable = v_psRootNode->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node);
//			// name
//			mmXML::mmXMLNodeI* v_psSelSourceVariableName = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node);
//			v_psSelSourceVariableName->SetText(L"Source Binary Output Variable: ");
//			// type
//			mmXML::mmXMLNodeI* v_psSelSourceVariableType = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node);
//			v_psSelSourceVariableType->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue);
//			// value
//			mmXML::mmXMLNodeI* v_psSelSourceVariableValue = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
//			v_psSelSourceVariableValue->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES);
//		// add selection of destination input variable
//		mmXML::mmXMLNodeI* v_psSelDestinationVariable = v_psRootNode->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node);
//			// name
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableName = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node);
//			v_psSelDestinationVariableName->SetText(L"Destination Binary Input Variable: ");
//			// type
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableType = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node);
//			v_psSelDestinationVariableType->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue);
//			// value
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableValue = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
//			v_psSelDestinationVariableValue->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES);
//
//	mmString v_sInParams = v_psINXMLDoc->SaveToXMLBuffer();
//	delete v_psINXMLDoc;
//
//	// create XML document which stores output automation options
//	mmXML::mmXMLDocI* v_psOUTXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psOUTXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_OUTParams_Node);
//
//	mmString v_sOutParams = v_psOUTXMLDoc->SaveToXMLBuffer();
//	delete v_psOUTXMLDoc;
//
//	// user set values of params
//	bool v_bNoParams = false;
//	mmString v_sCMInOutParams = GetUIDefinitionForInternalAction(	mmImages::g_pComplexCalcXML_Action_CopyBinaryOutputToInputValue,
//																	v_sInParams,
//																	v_sOutParams,
//																	&v_bNoParams);
//
//	mmString v_sSCMDef = v_sCMInOutParams;
//	if(!v_bNoParams)
//	{
//		v_sSCMDef = m_psMainWindow->ShowModalDialog(v_sCMInOutParams,this);
//	};
//
//	// update in/out params of pattern
//	mmString v_sSCMInOutDef = UpdateInputOutputParameters(v_sSCMDef);
//
//	// add internal action into calculation pattern
//	AddInternalMethodIntoPattern(v_sSCMInOutDef,
//															 m_psXMLCurrentPatternDefinition);
//
//	SendLogMessage(mmLog::debug,mmString(L"End AddInternalCopyBinaryOutToInVariable"));
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::AddInternalCopyIntOutToInVariable(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddInternalCopyIntOutToInVariable"));
//
//	// define input-output params
//	// create XML document which stores input automation options
//	mmXML::mmXMLDocI* v_psINXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psINXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_INParams_Node);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psINXMLDoc->GetXMLRootNode();
//		// add selection of surce output variable
//		mmXML::mmXMLNodeI* v_psSelSourceVariable = v_psRootNode->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node);
//			// name
//			mmXML::mmXMLNodeI* v_psSelSourceVariableName = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node);
//			v_psSelSourceVariableName->SetText(L"Source Integer Output Variable: ");
//			// type
//			mmXML::mmXMLNodeI* v_psSelSourceVariableType = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node);
//			v_psSelSourceVariableType->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_IntegerValue);
//			// value
//			mmXML::mmXMLNodeI* v_psSelSourceVariableValue = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
//			v_psSelSourceVariableValue->SetText(L"");
//		// add selection of destination input variable
//		mmXML::mmXMLNodeI* v_psSelDestinationVariable = v_psRootNode->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node);
//			// name
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableName = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node);
//			v_psSelDestinationVariableName->SetText(L"Destination Integer Input Variable: ");
//			// type
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableType = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node);
//			v_psSelDestinationVariableType->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_IntegerValue);
//			// value
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableValue = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
//			v_psSelDestinationVariableValue->SetText(L"");
//
//	mmString v_sInParams = v_psINXMLDoc->SaveToXMLBuffer();
//	delete v_psINXMLDoc;
//
//	// create XML document which stores output automation options
//	mmXML::mmXMLDocI* v_psOUTXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psOUTXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_OUTParams_Node);
//
//	mmString v_sOutParams = v_psOUTXMLDoc->SaveToXMLBuffer();
//	delete v_psOUTXMLDoc;
//
//	// user set values of params
//	bool v_bNoParams = false;
//	mmString v_sCMInOutParams = GetUIDefinitionForInternalAction(mmImages::g_pComplexCalcXML_Action_CopyIntOutputToInputValue,
//																															 v_sInParams,
//																															 v_sOutParams,
//																															 &v_bNoParams);
//
//	mmString v_sSCMDef = v_sCMInOutParams;
//	if(!v_bNoParams)
//	{
//		v_sSCMDef = m_psMainWindow->ShowModalDialog(v_sCMInOutParams,this);
//	};
//
//	// update in/out params of pattern
//	mmString v_sSCMInOutDef = UpdateInputOutputParameters(v_sSCMDef);
//
//	// add internal action into calculation pattern
//	AddInternalMethodIntoPattern(v_sSCMInOutDef,
//															 m_psXMLCurrentPatternDefinition);
//
//	SendLogMessage(mmLog::debug,mmString(L"End AddInternalCopyIntOutToInVariable"));
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::AddInternalCopyRealOutToInVariable(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddInternalCopyRealOutToInVariable"));
//
//	// define input-output params
//	// create XML document which stores input automation options
//	mmXML::mmXMLDocI* v_psINXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psINXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_INParams_Node);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psINXMLDoc->GetXMLRootNode();
//		// add selection of surce output variable
//		mmXML::mmXMLNodeI* v_psSelSourceVariable = v_psRootNode->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node);
//			// name
//			mmXML::mmXMLNodeI* v_psSelSourceVariableName = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node);
//			v_psSelSourceVariableName->SetText(L"Source Real Output Variable: ");
//			// type
//			mmXML::mmXMLNodeI* v_psSelSourceVariableType = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node);
//			v_psSelSourceVariableType->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_RealValue);
//			// value
//			mmXML::mmXMLNodeI* v_psSelSourceVariableValue = v_psSelSourceVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
//			v_psSelSourceVariableValue->SetText(L"");
//		// add selection of destination input variable
//		mmXML::mmXMLNodeI* v_psSelDestinationVariable = v_psRootNode->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node);
//			// name
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableName = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node);
//			v_psSelDestinationVariableName->SetText(L"Destination Real Input Variable: ");
//			// type
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableType = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node);
//			v_psSelDestinationVariableType->SetText(mmImages::g_pAutoCalcXML_Params_ParamType_RealValue);
//			// value
//			mmXML::mmXMLNodeI* v_psSelDestinationVariableValue = v_psSelDestinationVariable->AddChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node);
//			v_psSelDestinationVariableValue->SetText(L"");
//
//	mmString v_sInParams = v_psINXMLDoc->SaveToXMLBuffer();
//	delete v_psINXMLDoc;
//
//	// create XML document which stores output automation options
//	mmXML::mmXMLDocI* v_psOUTXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psOUTXMLDoc->CreateXMLRootNode(mmImages::g_pAutoCalcXML_OUTParams_Node);
//
//	mmString v_sOutParams = v_psOUTXMLDoc->SaveToXMLBuffer();
//	delete v_psOUTXMLDoc;
//
//	// user set values of params
//	bool v_bNoParams = false;
//	mmString v_sCMInOutParams = GetUIDefinitionForInternalAction(mmImages::g_pComplexCalcXML_Action_CopyRealOutputToInputValue,
//																															 v_sInParams,
//																															 v_sOutParams,
//																															 &v_bNoParams);
//
//	mmString v_sSCMDef = v_sCMInOutParams;
//	if(!v_bNoParams)
//	{
//		v_sSCMDef = m_psMainWindow->ShowModalDialog(v_sCMInOutParams,this);
//	};
//
//	// update in/out params of pattern
//	mmString v_sSCMInOutDef = UpdateInputOutputParameters(v_sSCMDef);
//
//	// add internal action into calculation pattern
//	AddInternalMethodIntoPattern(v_sSCMInOutDef,
//															 m_psXMLCurrentPatternDefinition);
//
//	SendLogMessage(mmLog::debug,mmString(L"End AddInternalCopyRealOutToInVariable"));
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::GetUIDefinitionForInternalAction(	mmString p_sInternalActionName,
//																							mmString p_sIAInParams,
//																							mmString p_sIAOutParams,
//																							bool* p_pbNoParams)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetUIDefinitionForInternalAction"));
//
//	*p_pbNoParams = false;
//
//	// input params XML
//	mmXML::mmXMLDocI* v_psXMLDoc_InputParams = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psXMLDoc_InputParams->ParseXMLBuffer(p_sIAInParams);
//	mmXML::mmXMLNodeI* v_psRootNode_InputParams = v_psXMLDoc_InputParams->GetXMLRootNode();
//	std::vector<mmXML::mmXMLNodeI*> v_vInputParams = v_psRootNode_InputParams->GetChilds();
//
//	// output params XML
//	mmXML::mmXMLDocI* v_psXMLDoc_OutputParams = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	v_psXMLDoc_OutputParams->ParseXMLBuffer(p_sIAOutParams);
//	mmXML::mmXMLNodeI* v_psRootNode_OutputParams = v_psXMLDoc_OutputParams->GetXMLRootNode();
//	std::vector<mmXML::mmXMLNodeI*> v_vOutputParams = v_psRootNode_OutputParams->GetChilds();
//
//	// input values
//	std::vector<mmXML::mmXMLNodeI*> v_vAvailableInputValues = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmImages::g_pComplexCalcXML_PatternInputParams_Node)->GetChilds();
//	mmInt v_iIVCount = v_vAvailableInputValues.size();
//
//	// output values
//	std::vector<mmXML::mmXMLNodeI*> v_vAvailableOutputValues = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParams_Node)->GetChilds();
//	mmInt v_iOVCount = v_vAvailableOutputValues.size();
//
//	// create XML document which specifies new calculation pattern wizard
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// add dialog root node
//	v_psXMLDoc->CreateXMLRootNode(mmUI::g_pUIXML_EL_UserDialog);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//		// add visible name for dialog
//		mmXML::mmXMLNodeI* v_psRootNodeName = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//		v_psRootNodeName->SetText(p_sInternalActionName);
//
//		// add input params group node
//		mmXML::mmXMLNodeI* v_psInParamsGroupNode = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_Group);
//			// add visible name for group node
//			mmXML::mmXMLNodeI* v_psInParamsGroupNodeName = v_psInParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//			v_psInParamsGroupNodeName->SetText(L" Input Params: ");
//
//			mmInt v_iIPCount = v_vInputParams.size();
//			for(mmInt v_iIP=0;v_iIP<v_iIPCount;v_iIP++)
//			{
//				mmString v_sName = v_vInputParams[v_iIP]->FindChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node)->GetText();
//				mmString v_sType = v_vInputParams[v_iIP]->FindChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node)->GetText();
//				mmString v_sInitValue = v_vInputParams[v_iIP]->FindChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node)->GetText();
//
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudNames) == 0)
//				{
//					// add cloud names
//					mmXML::mmXMLNodeI* v_psCloudCount = v_psInParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psCloudCount->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psCloudCount->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psCloudCount->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sInitValue);
//						// name
//						mmXML::mmXMLNodeI* v_psCloudCountName = v_psCloudCount->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psCloudCountName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psCloudCountType = v_psCloudCount->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psCloudCountType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// value
//						mmXML::mmXMLNodeI* v_psCloudCountValue = v_psCloudCount->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psCloudCountValue->SetText(v_sInitValue);
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//				{
//					// add binary decision
//					mmXML::mmXMLNodeI* v_psBinaryDecision = v_psInParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psBinaryDecision->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psBinaryDecision->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psBinaryDecision->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sInitValue);
//						// name
//						mmXML::mmXMLNodeI* v_psBinaryDecisionName = v_psBinaryDecision->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psBinaryDecisionName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psBinaryDecisionType = v_psBinaryDecision->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psBinaryDecisionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// values range
//						mmXML::mmXMLNodeI* v_psBinaryDecisionRange = v_psBinaryDecision->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//							mmXML::mmXMLNodeI* v_psBinaryDecisionRangeStringList = v_psBinaryDecisionRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//							for(mmInt v_iIV=0;v_iIV<v_iIVCount;v_iIV++)
//							{
//								if(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psBinaryDecisionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//								};
//							};
//							for(mmInt v_iOV=0;v_iOV<v_iOVCount;v_iOV++)
//							{
//								if(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psBinaryDecisionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//								};
//							};
//							mmXML::mmXMLNodeI* v_psUIStringListElement = v_psBinaryDecisionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//							v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//						// value
//						mmXML::mmXMLNodeI* v_psBinaryDecisionValue = v_psBinaryDecision->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psBinaryDecisionValue->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//				{
//					// add cloud name
//					mmXML::mmXMLNodeI* v_psCloud = v_psInParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psCloud->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psCloud->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psCloud->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sInitValue);
//						// name
//						mmXML::mmXMLNodeI* v_psCloudName = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psCloudName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psCloudType = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psCloudType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// values range
//						mmXML::mmXMLNodeI* v_psCloudRange = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//							mmXML::mmXMLNodeI* v_psCloudRangeStringList = v_psCloudRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//							for(mmInt v_iIV=0;v_iIV<v_iIVCount;v_iIV++)
//							{
//								if(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psCloudRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//								};
//							};
//							for(mmInt v_iOV=0;v_iOV<v_iOVCount;v_iOV++)
//							{
//								if(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psCloudRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//								};
//							};
//							mmXML::mmXMLNodeI* v_psUIStringListElement = v_psCloudRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//							v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//						// value
//						mmXML::mmXMLNodeI* v_psCloudValue = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psCloudValue->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//				{
//					// add Data Layer name
//					mmXML::mmXMLNodeI* v_psDataLayer = v_psInParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psDataLayer->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psDataLayer->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psDataLayer->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sInitValue);
//						// name
//						mmXML::mmXMLNodeI* v_psDataLayerName = v_psDataLayer->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psDataLayerName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psDataLayerType = v_psDataLayer->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psDataLayerType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// values range
//						mmXML::mmXMLNodeI* v_psDataLayerRange = v_psDataLayer->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//							mmXML::mmXMLNodeI* v_psDataLayerRangeStringList = v_psDataLayerRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//							for(mmInt v_iIV=0;v_iIV<v_iIVCount;v_iIV++)
//							{
//								if(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psDataLayerRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//								};
//							};
//							for(mmInt v_iOV=0;v_iOV<v_iOVCount;v_iOV++)
//							{
//								if(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psDataLayerRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//								};
//							};
//							mmXML::mmXMLNodeI* v_psUIStringListElement = v_psDataLayerRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//							v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//						// value
//						mmXML::mmXMLNodeI* v_psDataLayerValue = v_psDataLayer->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psDataLayerValue->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//				{
//					// add int value
//					mmXML::mmXMLNodeI* v_psInteger = v_psInParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psInteger->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psInteger->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psInteger->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sInitValue);
//						// name
//						mmXML::mmXMLNodeI* v_psIntegerName = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psIntegerName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psIntegerType = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psIntegerType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// values range
//						mmXML::mmXMLNodeI* v_psIntegerRange = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//							mmXML::mmXMLNodeI* v_psIntegerRangeStringList = v_psIntegerRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//							for(mmInt v_iIV=0;v_iIV<v_iIVCount;v_iIV++)
//							{
//								if(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psIntegerRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//								};
//							};
//							for(mmInt v_iOV=0;v_iOV<v_iOVCount;v_iOV++)
//							{
//								if(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psIntegerRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//								};
//							};
//							mmXML::mmXMLNodeI* v_psUIStringListElement = v_psIntegerRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//							v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//						// value
//						mmXML::mmXMLNodeI* v_psIntegerValue = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psIntegerValue->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//				{
//					// add real value
//					mmXML::mmXMLNodeI* v_psReal = v_psInParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psReal->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psReal->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psReal->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sInitValue);
//						// name
//						mmXML::mmXMLNodeI* v_psRealName = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psRealName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psRealType = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psRealType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// values range
//						mmXML::mmXMLNodeI* v_psRealValueRange = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//							mmXML::mmXMLNodeI* v_psRealValueRangeStringList = v_psRealValueRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//							for(mmInt v_iIV=0;v_iIV<v_iIVCount;v_iIV++)
//							{
//								if(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psRealValueRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//								};
//							};
//							for(mmInt v_iOV=0;v_iOV<v_iOVCount;v_iOV++)
//							{
//								if(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//								{
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psRealValueRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//								};
//							};
//							mmXML::mmXMLNodeI* v_psUIStringListElement = v_psRealValueRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//							v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//						// value
//						mmXML::mmXMLNodeI* v_psRealValue = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psRealValue->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//				};
//			};
//
//		// add output params group node
//		mmXML::mmXMLNodeI* v_psOutParamsGroupNode = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_Group);
//			// add visible name for group node
//			mmXML::mmXMLNodeI* v_psOutParamsGroupNodeName = v_psOutParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//			v_psOutParamsGroupNodeName->SetText(L" Output Params: ");
//
//			mmInt v_iOPCount = v_vOutputParams.size();
//			for(mmInt v_iOP=0;v_iOP<v_iOPCount;v_iOP++)
//			{
//				mmString v_sName = v_vOutputParams[v_iOP]->FindChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node)->GetText();
//				mmString v_sType = v_vOutputParams[v_iOP]->FindChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node)->GetText();
//				mmString v_sValue = v_vOutputParams[v_iOP]->FindChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node)->GetText();
//
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//				{
//					// add cloud name
//					mmXML::mmXMLNodeI* v_psBinaryDecision = v_psOutParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psBinaryDecision->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psBinaryDecision->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psBinaryDecision->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sValue);
//						// name
//						mmXML::mmXMLNodeI* v_psBinaryDecisionName = v_psBinaryDecision->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psBinaryDecisionName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psBinaryDecisionType = v_psBinaryDecision->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psBinaryDecisionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// value
//						mmXML::mmXMLNodeI* v_psBinaryDecisionValue = v_psBinaryDecision->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psBinaryDecisionValue->SetText(L"");
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//				{
//					// add binary decision
//					mmXML::mmXMLNodeI* v_psCloud = v_psOutParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psCloud->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psCloud->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psCloud->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sValue);
//						// name
//						mmXML::mmXMLNodeI* v_psCloudName = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psCloudName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psCloudType = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psCloudType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// value
//						mmXML::mmXMLNodeI* v_psCloudValue = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psCloudValue->SetText(L"");
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//				{
//					// add Data Layer name
//					mmXML::mmXMLNodeI* v_psDataLayer = v_psOutParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psDataLayer->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psDataLayer->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psDataLayer->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sValue);
//						// name
//						mmXML::mmXMLNodeI* v_psDataLayerName = v_psDataLayer->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psDataLayerName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psDataLayerType = v_psDataLayer->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psDataLayerType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// value
//						mmXML::mmXMLNodeI* v_psDataLayerValue = v_psDataLayer->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psDataLayerValue->SetText(L"");
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//				{
//					// add int value
//					mmXML::mmXMLNodeI* v_psInteger = v_psOutParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psInteger->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psInteger->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psInteger->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sValue);
//						// name
//						mmXML::mmXMLNodeI* v_psIntegerName = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psIntegerName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psIntegerType = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psIntegerType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// value
//						mmXML::mmXMLNodeI* v_psIntegerValue = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psIntegerValue->SetText(L"");
//				};
//				if(v_sType.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//				{
//					// add real value
//					mmXML::mmXMLNodeI* v_psReal = v_psOutParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//						// add orig attributes
//						v_psReal->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamName_Node,v_sName);
//						v_psReal->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamType_Node,v_sType);
//						v_psReal->AddAttribute(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node,v_sValue);
//						// name
//						mmXML::mmXMLNodeI* v_psRealName = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psRealName->SetText(v_sName);
//						// type
//						mmXML::mmXMLNodeI* v_psRealType = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//						v_psRealType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//						// value
//						mmXML::mmXMLNodeI* v_psRealValue = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//						v_psRealValue->SetText(L"");
//				};
//			};
//
//	if( (v_iIPCount == 0) && (v_iOPCount == 0) )
//	{
//		*p_pbNoParams = true;
//	};
//
//	mmString v_sOutDialog = v_psXMLDoc->SaveToXMLBuffer();
//
//	delete v_psXMLDoc_InputParams;
//	delete v_psXMLDoc_OutputParams;
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetUIDefinitionForInternalAction"));
//
//	return v_sOutDialog;
//}

// PB //
//mmString mmImages::mmImagesComplexCalculationManagement::GetUIDefinitionForMultiSelectValues(mmString p_sUIData)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddInternalCreationOfSimplifiedCloudStructureIntoPattern"));
//
//  // input values
//	std::vector<mmXML::mmXMLNodeI*> v_vAvailableInputValues = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmImages::g_pComplexCalcXML_PatternInputParams_Node)->GetChilds();
//	mmInt v_iIVCount = v_vAvailableInputValues.size();
//
//	// output values
//	std::vector<mmXML::mmXMLNodeI*> v_vAvailableOutputValues = m_psXMLCurrentPatternDefinition->GetXMLRootNode()->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParams_Node)->GetChilds();
//	mmInt v_iOVCount = v_vAvailableOutputValues.size();
//
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// parse string
//	v_psXMLDoc->ParseXMLBuffer(p_sUIData);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//
//	// in params values
//	std::vector<mmXML::mmXMLNodeI*> v_vInParams = v_psRootNode->GetChild(1)->GetChilds();
//	mmInt v_iInPCount = v_vInParams.size();
//	for(mmInt v_iInP=1;v_iInP<v_iInPCount;v_iInP++)
//	{
//		mmString v_sInParamType = v_vInParams[v_iInP]->GetAttributeValue(mmImages::g_pAutoCalcXML_Params_ParamType_Node);
//
//		if(v_sInParamType.compare(mmImages::g_pAutoCalcXML_Params_ParamType_ImagesNames) == 0)
//		{
//			mmString v_sMultiCloudSelectionValue = v_vInParams[v_iInP]->
//																						 FindChild(mmUI::g_pUIXML_EL_DEF_Value)->
//																						 GetText();
//
//			// if not all selected
//			if(v_sMultiCloudSelectionValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudCount_All) != 0)
//			{
//				// create UI for cloud names
//				mmInt v_iCloudToSelectCount = mmStringUtilities::StringToMMInt(v_sMultiCloudSelectionValue);
//
//				// create XML document which specifies multi cloud selection
//				mmXML::mmXMLDocI* v_psXMLDocUI = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//				//mmXML::mmXMLDocI* v_psXMLDocUI = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//				// add dialog root node
//				v_psXMLDocUI->CreateXMLRootNode(mmUI::g_pUIXML_EL_UserDialog);
//				mmXML::mmXMLNodeI* v_psRootNodeUI = v_psXMLDocUI->GetXMLRootNode();
//					// add visible name for dialog
//					mmXML::mmXMLNodeI* v_psRootNodeUIName = v_psRootNodeUI->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//					v_psRootNodeUIName->SetText(L" Select clouds: ");
//
//					// add cloud selection group node
//					mmXML::mmXMLNodeI* v_psClSelGroupNode = v_psRootNodeUI->AddChild(mmUI::g_pUIXML_EL_Group);
//						// add visible name for group node
//						mmXML::mmXMLNodeI* v_psClSelGroupNodeName = v_psClSelGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//						v_psClSelGroupNodeName->SetText(L" Clouds: ");
//
//						for(mmInt v_iC=0;v_iC<v_iCloudToSelectCount;v_iC++)
//						{
//							// add cloud name selection
//							mmXML::mmXMLNodeI* v_psCloud = v_psClSelGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//								// name
//								mmXML::mmXMLNodeI* v_psCloudName = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//								v_psCloudName->SetText(mmString(L" Cloud ") +
//																			 mmStringUtilities::MMIntToString(v_iC+1) +
//																			 mmString(L": "));
//								// type
//								mmXML::mmXMLNodeI* v_psCloudType = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//								v_psCloudType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//								// values range
//								mmXML::mmXMLNodeI* v_psCloudRange = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//									mmXML::mmXMLNodeI* v_psCloudRangeStringList = v_psCloudRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//									for(mmInt v_iIV=0;v_iIV<v_iIVCount;v_iIV++)
//									{
//										if(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//										{
//											mmXML::mmXMLNodeI* v_psUIStringListElement = v_psCloudRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//											v_psUIStringListElement->SetText(v_vAvailableInputValues[v_iIV]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//										};
//									};
//									for(mmInt v_iOV=0;v_iOV<v_iOVCount;v_iOV++)
//									{
//										if(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamType_Node)->GetText().compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//										{
//											mmXML::mmXMLNodeI* v_psUIStringListElement = v_psCloudRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//											v_psUIStringListElement->SetText(v_vAvailableOutputValues[v_iOV]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText());
//										};
//									};
//									mmXML::mmXMLNodeI* v_psUIStringListElement = v_psCloudRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//									v_psUIStringListElement->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//									// value
//									mmXML::mmXMLNodeI* v_psCloudValue = v_psCloud->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//									v_psCloudValue->SetText(mmClouds::g_pComplexCalcXML_PatternInputParamUserSpecified);
//						};
//
//				// show dialog
//				mmString v_sInDialog = v_psXMLDocUI->SaveToXMLBuffer();
//				delete v_psXMLDocUI;
//
//				mmString v_sOutDialog = m_psMainWindow->ShowModalDialog(v_sInDialog);
//
//				// parse result
//				mmXML::mmXMLDocI* v_psXMLDocUIRes = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//				//mmXML::mmXMLDocI* v_psXMLDocUIRes = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//				// parse string
//				v_psXMLDocUIRes->ParseXMLBuffer(v_sOutDialog);
//				mmXML::mmXMLNodeI* v_psRootNodeUIRes = v_psXMLDocUIRes->GetXMLRootNode();
//
//				std::vector<mmXML::mmXMLNodeI*> v_vResClouds = v_psRootNodeUIRes->GetChild(1)->GetChilds();
//				for(mmInt v_iC=0;v_iC<v_iCloudToSelectCount;v_iC++)
//				{
//					mmString v_sCloudNameToAdd = mmString(L" ") +
//																			 v_vResClouds[v_iC+1]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText();
//
//					v_sMultiCloudSelectionValue += v_sCloudNameToAdd;
//				};
//
//				v_vInParams[v_iInP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->SetText(v_sMultiCloudSelectionValue);
//				delete v_psXMLDocUIRes;
//			};
//		};
//	};
//
//	mmString v_sOutRes = v_psXMLDoc->SaveToXMLBuffer();
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetUIDefinitionForInternalAction"));
//
//	return v_sOutRes;
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::AddInternalMethodIntoPattern(	mmString p_sInOutParams,
//																					mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start AddInternalCreationOfSimplifiedCloudStructureIntoPattern"));
//
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// parse string
//	v_psXMLDoc->ParseXMLBuffer(p_sInOutParams);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//
//	mmXML::mmXMLNodeI* v_psXMLNewInternalAction = p_psXMLComplexCalculationPatternDefinition->GetXMLRootNode()->
//													FindChild(mmImages::g_pComplexCalcXML_PatternDefinition_Node)->
//													AddChild(mmImages::g_pComplexCalcXML_PatternInternalAction_Node);
//		// add name of IA
//		mmXML::mmXMLNodeI* v_psXMLNewInternalActionName = v_psXMLNewInternalAction->AddChild(mmImages::g_pComplexCalcXML_PatternInternalActionName_Node);
//
//		v_psXMLNewInternalActionName->SetText(v_psRootNode->FindChild(mmUI::g_pUIXML_EL_DEF_Name)->GetText());
//
//		// adding input params node
//		mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParams = v_psXMLNewInternalAction->AddChild(mmImages::g_pAutoCalcXML_INParams_Node);
//
//		// add IA input params
//		std::vector<mmXML::mmXMLNodeI*> v_vInputParams = v_psRootNode->GetChild(1)->GetChilds();
//		mmInt v_iIPCount = v_vInputParams.size();
//		for(mmInt v_iIP=1;v_iIP<v_iIPCount;v_iIP++)
//		{
//			std::vector<mmXML::sXMLAttribute> v_vOrigAttrParams = v_vInputParams[v_iIP]->GetAttributes();
//
//			if(v_vOrigAttrParams.size() > 0)
//			{
//				if(v_vOrigAttrParams[1].sValue.compare(mmImages::g_pAutoCalcXML_Params_ParamType_ImagesNames) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam = v_psXMLNewInternalAction_InParams->AddChild(mmImages::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Name = v_psXMLNewInternalAction_InParam->AddChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLNewInternalAction_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLNewInternalAction_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Type = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudNames);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Value = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam = v_psXMLNewInternalAction_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Name = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLNewInternalAction_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLNewInternalAction_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Type = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Value = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam = v_psXMLNewInternalAction_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Name = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLNewInternalAction_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLNewInternalAction_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Type = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Value = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam = v_psXMLNewInternalAction_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Name = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLNewInternalAction_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLNewInternalAction_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Type = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Value = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam = v_psXMLNewInternalAction_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Name = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLNewInternalAction_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLNewInternalAction_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Type = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Value = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//				{
//					// adding input param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam = v_psXMLNewInternalAction_InParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Name = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						//v_psXMLNewInternalAction_InParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						v_psXMLNewInternalAction_InParam_Name->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Type = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_InParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_InParam_Value = v_psXMLNewInternalAction_InParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_InParam_Value->SetText(v_vInputParams[v_iIP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//			};
//		};
//
//		// adding output params node
//		mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParams = v_psXMLNewInternalAction->AddChild(mmClouds::g_pAutoCalcXML_OUTParams_Node);
//
//		std::vector<mmXML::mmXMLNodeI*> v_vOutputParams = v_psRootNode->GetChild(2)->GetChilds();
//		mmInt v_iOPCount = v_vOutputParams.size();
//		for(mmInt v_iOP=1;v_iOP<v_iOPCount;v_iOP++)
//		{
//			std::vector<mmXML::sXMLAttribute> v_vOrigAttrParams = v_vOutputParams[v_iOP]->GetAttributes();
//
//			if(v_vOrigAttrParams.size() > 0)
//			{
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam = v_psXMLNewInternalAction_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Name = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLNewInternalAction_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Type = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Value = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam = v_psXMLNewInternalAction_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Name = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLNewInternalAction_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Type = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Value = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam = v_psXMLNewInternalAction_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Name = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLNewInternalAction_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Type = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Value = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam = v_psXMLNewInternalAction_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Name = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLNewInternalAction_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Type = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Value = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//				if(v_vOrigAttrParams[1].sValue.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//				{
//					// adding output param
//					mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam = v_psXMLNewInternalAction_OutParams->AddChild(mmClouds::g_pAutoCalcXML_Params_Param_Node);
//						// add name
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Name = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamName_Node);
//						v_psXMLNewInternalAction_OutParam_Name->SetText(v_vOrigAttrParams[0].sValue);
//						// add type
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Type = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamType_Node);
//						v_psXMLNewInternalAction_OutParam_Type->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue);
//						// add value
//						mmXML::mmXMLNodeI* v_psXMLNewInternalAction_OutParam_Value = v_psXMLNewInternalAction_OutParam->AddChild(mmClouds::g_pAutoCalcXML_Params_ParamValue_Node);
//						v_psXMLNewInternalAction_OutParam_Value->SetText(v_vOutputParams[v_iOP]->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText());
//				};
//			};
//		};
//
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End AddInternalCreationOfSimplifiedCloudStructureIntoPattern"));
//}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::InitializeCurrentComplexCalculationPatternExecution(void)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start InitializeCurrentComplexCalculationPatternExecution"));
//
//// PB //
////	// set UI for user input values
////	GetComplexCalculationPatternInputParamsFromUI(m_psXMLCalculatedPatternDefinition);
//
//	SendLogMessage(mmLog::debug,mmString(L"End InitializeCurrentComplexCalculationPatternExecution"));
//}

bool mmImages::mmImagesComplexCalculationManagement::ManageCurrentComplexCalculationPatternExecution(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start ManageCurrentComplexCalculationPatternExecution"));

	bool v_bIsFinishedCalculation = true;

	// check if m_psCoPCalculationManagement object is calculating
	bool v_bIsCalculating = m_psImageCalculationManagement->IsCalculating();

	// check if there is a next simple calc method
	if(v_bIsCalculating)
	{
		v_bIsFinishedCalculation = false;
	}
	else
	{
		std::vector<mmXML::mmXMLNodeI*> v_vCalcMethods =	m_psXMLCalculatedPatternDefinition->
															FindChild(mmImages::g_pComplexCalcXML_PatternDefinition_Node)->
															GetChilds();

		// intialize output params
		if(m_iCurrentComplexCalculationItem > -1)
		{
// PB //
			UpdateOutputParamsOfSimpleCalculationMethod(	m_psXMLCalculatedPatternDefinition,
															m_iCurrentComplexCalculationItem);

			delete m_psCurrentCalculationMethod;
			m_psCurrentCalculationMethod = NULL;
		};

		// check next calculation method number
		m_iCurrentComplexCalculationItem++;
		if(static_cast<mmInt>(v_vCalcMethods.size()) > m_iCurrentComplexCalculationItem)
		{
			// check if it is calculation method or internal action
			mmString v_sOperationType = v_vCalcMethods[m_iCurrentComplexCalculationItem]->GetName();

			// internal action
			if(v_sOperationType.compare(mmImages::g_pComplexCalcXML_PatternInternalAction_Node) == 0)
			{
				mmString v_sInternalActionName = v_vCalcMethods[m_iCurrentComplexCalculationItem]->FindChild(mmImages::g_pComplexCalcXML_PatternInternalActionName_Node)->GetText();

// PB //
//				if( (v_sInternalActionName.compare(mmImages::g_pComplexCalcXML_Action_CopyBinaryOutputToInputValue) == 0) ||
//						(v_sInternalActionName.compare(mmImages::g_pComplexCalcXML_Action_CopyIntOutputToInputValue) == 0) ||
//						(v_sInternalActionName.compare(mmImages::g_pComplexCalcXML_Action_CopyRealOutputToInputValue) == 0) )
//				{
//					// initialize input params
//					mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams v_sCAP;
//					v_sCAP = InitializeParamsOfSimpleCalculationMethod(	m_psXMLCalculatedPatternDefinition,
//																		m_iCurrentComplexCalculationItem);
//
//					// copy values
//					mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//					//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//					v_psXMLDoc->ParseXMLBuffer(v_sCAP.sInParams);
//					mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//
//					mmString v_sSourceValue = v_psRootNode->
//																		GetChild(0)->
//																		FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->
//																		GetText();
//					mmString v_sDestName = v_psRootNode->
//																 GetChild(1)->
//																 FindChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node)->
//																 GetText();
//
//					mmString v_sLogText = mmString(L"CopyValue ") +
//																v_sDestName +
//																mmString(L"=") +
//																v_sSourceValue;
//					SendLogMessage(mmLog::user,v_sLogText);
//
//					SetValueOfParameter(m_psXMLCalculatedPatternDefinition,v_sDestName,v_sSourceValue);
//				};

// PB //
//				if(v_sInternalActionName.compare(mmImages::g_pComplexCalcXML_Action_AddEndWhile) == 0)
//				{
//					// search for while
//					mmInt v_iStartWhileIndex = -1;
//					mmInt v_iWhileAndEndWhileCount = 0;
//					mmInt v_iCMSize = v_vCalcMethods.size();
//					for(mmInt v_iCM=m_iCurrentComplexCalculationItem-1;v_iCM>=0;v_iCM--)
//					{
//						if(v_vCalcMethods[v_iCM]->GetName().compare(mmImages::g_pComplexCalcXML_PatternInternalAction_Node) == 0)
//						{
//							mmString v_sIAName = v_vCalcMethods[v_iCM]->
//												 FindChild(mmImages::g_pComplexCalcXML_PatternInternalActionName_Node)->
//												 GetText();
//
//							if(v_sIAName.compare(mmImages::g_pComplexCalcXML_Action_AddEndWhile) == 0)
//							{
//								v_iWhileAndEndWhileCount++;
//							};
//							if(v_sIAName.compare(mmImages::g_pComplexCalcXML_Action_AddWhileLoop) == 0)
//							{
//								if(v_iWhileAndEndWhileCount > 0)
//								{
//									v_iWhileAndEndWhileCount--;
//								}
//								else
//								{
//									v_iStartWhileIndex = v_iCM-1;
//									break;
//								};
//							};
//						};
//					};
//					m_iCurrentComplexCalculationItem = v_iStartWhileIndex;
//				};

// PB //
//				if(v_sInternalActionName.compare(mmImages::g_pComplexCalcXML_Action_AddWhileLoop) == 0)
//				{
//					// initialize input params
//					mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams v_sCAP;
//					v_sCAP = InitializeParamsOfSimpleCalculationMethod(	m_psXMLCalculatedPatternDefinition,
//																		m_iCurrentComplexCalculationItem);
//
//					// while condition
//					mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//					//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//					v_psXMLDoc->ParseXMLBuffer(v_sCAP.sInParams);
//					mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//
//					bool v_bContinueWhile = false;
//					if(v_psRootNode->
//						 GetChild(0)->
//						 FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->
//						 GetText().compare(mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES) == 0)
//					{
//						v_bContinueWhile = true;
//					};
//
//					if(!v_bContinueWhile)
//					{
//						// search for end while
//						mmInt v_iEndWhileNextIndex = -1;
//						mmInt v_iWhileAndEndWhileCount = 0;
//						mmInt v_iCMSize = v_vCalcMethods.size();
//						for(mmInt v_iCM=m_iCurrentComplexCalculationItem+1;v_iCM<v_iCMSize;v_iCM++)
//						{
//							if(v_vCalcMethods[v_iCM]->GetName().compare(mmImages::g_pComplexCalcXML_PatternInternalAction_Node) == 0)
//							{
//								mmString v_sIAName = v_vCalcMethods[v_iCM]->
//																		 FindChild(mmImages::g_pComplexCalcXML_PatternInternalActionName_Node)->
//																		 GetText();
//
//								if(v_sIAName.compare(mmImages::g_pComplexCalcXML_Action_AddWhileLoop) == 0)
//								{
//									v_iWhileAndEndWhileCount++;
//								};
//								if(v_sIAName.compare(mmImages::g_pComplexCalcXML_Action_AddEndWhile) == 0)
//								{
//									if(v_iWhileAndEndWhileCount > 0)
//									{
//										v_iWhileAndEndWhileCount--;
//									}
//									else
//									{
//										v_iEndWhileNextIndex = v_iCM;
//
//										break;
//									};
//								};
//							};
//						};
//
//						m_iCurrentComplexCalculationItem = v_iEndWhileNextIndex;
//					};
//				};
			};

			// simple calculation method
			if(v_sOperationType.compare(mmImages::g_pComplexCalcXML_PatternCalcMethod_Node) == 0)
			{
				//mmOperatingSystem::StopThread(50);
				m_psCurrentCalculationMethod = m_psImageCalculationMethodContainer->InitializeImagesCalculationMethod(	v_vCalcMethods[m_iCurrentComplexCalculationItem]->
																														FindChild(mmImages::g_pComplexCalcXML_PatternCalcMethodIDName_Node)->
																														GetText());

				// initialize input params
				mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams v_sCAP;
				v_sCAP = InitializeParamsOfSimpleCalculationMethod(	m_psXMLCalculatedPatternDefinition,
																	m_iCurrentComplexCalculationItem);

				// start calculation
				m_psImageCalculationManagement->CalculateImages(	m_psCurrentCalculationMethod,
																	m_sCurrentComplexCalculation.psImageStructure,
																	&v_sCAP);
			};

			v_bIsFinishedCalculation = false;
		};
	};

	// clear calculation members
	if(v_bIsFinishedCalculation)
	{
		m_psXMLCalculatedPatternDefinition = NULL;
	};

	SendLogMessage(mmLog::debug,mmString(L"End ManageCurrentComplexCalculationPatternExecution"));

	return v_bIsFinishedCalculation;
}

// PB //
//void mmImages::mmImagesComplexCalculationManagement::GetComplexCalculationPatternInputParamsFromUI(mmXML::mmXMLNodeI* p_psComplexCalculationDefinition)
//{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetComplexCalculationPatternInputParamsFromUI"));
//
//	mmInt v_iValuesToSetCount = 0;
//
//	// create XML document which specifies calculation pattern input params UI dialog
//	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
//	//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
//
//	// add dialog root node
//	v_psXMLDoc->CreateXMLRootNode(mmUI::g_pUIXML_EL_UserDialog);
//	mmXML::mmXMLNodeI* v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//		// add visible name for dialog
//		mmXML::mmXMLNodeI* v_psRootNodeName = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//		v_psRootNodeName->SetText(p_psComplexCalculationDefinition->FindChild(mmClouds::g_pComplexCalcXML_PatternName_Node)->GetText() +
//															mmString(L" - Input Paramters Definition"));
//
//		// add global params group node
//		mmXML::mmXMLNodeI* v_psInputParamsGroupNode = v_psRootNode->AddChild(mmUI::g_pUIXML_EL_Group);
//			// add visible name for group node
//			mmXML::mmXMLNodeI* v_psInputParamsGroupNodeName = v_psInputParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//			v_psInputParamsGroupNodeName->SetText(L" Params: ");
//
//			std::vector<mmXML::mmXMLNodeI*> v_vInParams = p_psComplexCalculationDefinition->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParams_Node)->GetChilds();
//			mmInt v_iIPCount = v_vInParams.size();
//			for(mmInt v_iIP=0;v_iIP<v_iIPCount;v_iIP++)
//			{
//				std::vector<mmXML::sXMLAttribute> v_sAttributes = v_vInParams[v_iIP]->GetAttributes();
//				mmXML::mmXMLNodeI* v_psXMLModifiedValueNode = v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamValueModifiedNodeName_Node);
//
//				if( (v_sAttributes.size() == 0) &&
//						(v_psXMLModifiedValueNode == NULL) )
//				{
//					mmString v_sNodeTypeStr = v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamType_Node)->GetText();
//
//					if(v_sNodeTypeStr.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_CloudName) == 0)
//					{
//						// add cloud selection
//						mmXML::mmXMLNodeI* v_psCloudSelection = v_psInputParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//							// name
//							mmXML::mmXMLNodeI* v_psCloudSelectionName = v_psCloudSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//							v_psCloudSelectionName->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//							// type
//							mmXML::mmXMLNodeI* v_psCloudSelectionType = v_psCloudSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//							v_psCloudSelectionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//							// values range
//							mmXML::mmXMLNodeI* v_psCloudSelectionValueRange = v_psCloudSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//							mmXML::mmXMLNodeI* v_psCloudSelectionValueRangeStringList = v_psCloudSelectionValueRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//							mmInt v_iCCount = m_sCurrentComplexCalculation.psCloudStructure->GetCloudsCount();
//							for(mmInt v_iC=0;v_iC<v_iCCount;v_iC++)
//							{
//								mmXML::mmXMLNodeI* v_psUIStringListElement = v_psCloudSelectionValueRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//								v_psUIStringListElement->SetText(m_sCurrentComplexCalculation.psCloudStructure->GetDirectionalCloudOfPointsName(v_iC));
//							};
//							// value
//							mmXML::mmXMLNodeI* v_psCloudSelectionValue = v_psCloudSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//							v_psCloudSelectionValue->SetText(mmUI::g_pUIXML_Value_SELECT);
//					};
//					if(v_sNodeTypeStr.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_DataLayerName) == 0)
//					{
//						// add groups data layer selection
//						mmXML::mmXMLNodeI* v_psDLSelection = v_psInputParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//							// name
//							mmXML::mmXMLNodeI* v_psDLSelectionName = v_psDLSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//							v_psDLSelectionName->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//							// type
//							mmXML::mmXMLNodeI* v_psDLSelectionType = v_psDLSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//							v_psDLSelectionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//							// values range
//							mmXML::mmXMLNodeI* v_psDLSelectionValueRange = v_psDLSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//							mmXML::mmXMLNodeI* v_psDLSelectionValueRangeStringList = v_psDLSelectionValueRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//							std::vector<mmClouds::mmCloudsOfPointsStructureI::sDataLayerInfo> v_vDLInfo = m_sCurrentComplexCalculation.psCloudStructure->GetDataLayersInfo();
//							mmInt v_iDLCount = v_vDLInfo.size();
//							for(mmInt v_iDL=0;v_iDL<v_iDLCount;v_iDL++)
//							{
//								mmXML::mmXMLNodeI* v_psUIStringListElement = v_psDLSelectionValueRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//								v_psUIStringListElement->SetText(v_vDLInfo[v_iDL].sName);
//							};
//							// value
//							mmXML::mmXMLNodeI* v_psDLSelectionValue = v_psDLSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//							v_psDLSelectionValue->SetText(mmUI::g_pUIXML_Value_SELECT);
//					};
//					if(v_sNodeTypeStr.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_RealValue) == 0)
//					{
//						// add real value input
//						mmXML::mmXMLNodeI* v_psReal = v_psInputParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//							// name
//							mmXML::mmXMLNodeI* v_psRealName = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//							v_psRealName->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//							// type
//							mmXML::mmXMLNodeI* v_psRealType = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//							v_psRealType->SetText(mmUI::g_pUIXML_EL_DEF_Type_Real);
//							// value
//							mmXML::mmXMLNodeI* v_psRealValue = v_psReal->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//							v_psRealValue->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node)->GetText());
//					};
//					if(v_sNodeTypeStr.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_IntegerValue) == 0)
//					{
//						// add integer value input
//						mmXML::mmXMLNodeI* v_psInteger = v_psInputParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//							// name
//							mmXML::mmXMLNodeI* v_psIntegerName = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//							v_psIntegerName->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//							// type
//							mmXML::mmXMLNodeI* v_psIntegerType = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//							v_psIntegerType->SetText(mmUI::g_pUIXML_EL_DEF_Type_Int);
//							// value
//							mmXML::mmXMLNodeI* v_psIntegerValue = v_psInteger->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//							v_psIntegerValue->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node)->GetText());
//					};
//					if(v_sNodeTypeStr.compare(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue) == 0)
//					{
//						// add binary decision input value selection
//						mmXML::mmXMLNodeI* v_psBoolSelection = v_psInputParamsGroupNode->AddChild(mmUI::g_pUIXML_EL_SingleValue);
//							// name
//							mmXML::mmXMLNodeI* v_psBoolSelectionName = v_psBoolSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Name);
//							v_psBoolSelectionName->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamName_Node)->GetText());
//							// type
//							mmXML::mmXMLNodeI* v_psBoolSelectionType = v_psBoolSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Type);
//							v_psBoolSelectionType->SetText(mmUI::g_pUIXML_EL_DEF_Type_String);
//							// values range
//							mmXML::mmXMLNodeI* v_psBoolSelectionRange = v_psBoolSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Range);
//							mmXML::mmXMLNodeI* v_psBoolSelectionRangeStringList = v_psBoolSelectionRange->AddChild(mmUI::g_pUIXML_EL_DEF_StringList);
//								mmXML::mmXMLNodeI* v_psUIStringListElement = v_psBoolSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//								v_psUIStringListElement->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue_YES);
//								v_psUIStringListElement = v_psBoolSelectionRangeStringList->AddChild(mmUI::g_pUIXML_EL_DEF_StringListElement);
//								v_psUIStringListElement->SetText(mmClouds::g_pAutoCalcXML_Params_ParamType_BoolValue_NO);
//							// value
//							mmXML::mmXMLNodeI* v_psBoolSelectionValue = v_psBoolSelection->AddChild(mmUI::g_pUIXML_EL_DEF_Value);
//							v_psBoolSelectionValue->SetText(v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node)->GetText());
//					};
//
//					v_iValuesToSetCount++;
//				};
//			};
//
//	mmString v_sOutDefinition = v_psXMLDoc->SaveToXMLBuffer();
//
//	if(v_iValuesToSetCount > 0)
//	{
//		// show dialog
//		mmString v_sUserInput = m_psMainWindow->ShowModalDialog(v_sOutDefinition,this);
//
//		// parse output
//		v_psXMLDoc->ParseXMLBuffer(v_sUserInput);
//		v_psRootNode = v_psXMLDoc->GetXMLRootNode();
//
//		v_iValuesToSetCount = 0;
//		for(mmInt v_iIP=0;v_iIP<v_iIPCount;v_iIP++)
//		{
//			std::vector<mmXML::sXMLAttribute> v_sAttributes = v_vInParams[v_iIP]->GetAttributes();
//			mmXML::mmXMLNodeI* v_psXMLModifiedValueNode = v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamValueModifiedNodeName_Node);
//
//			if( (v_sAttributes.size() == 0) &&
//					(v_psXMLModifiedValueNode == NULL) )
//			{
//				v_vInParams[v_iIP]->FindChild(mmClouds::g_pComplexCalcXML_PatternInputParamValue_Node)->SetText(	v_psRootNode->
//																													GetChild(1)->
//																													GetChild(1+v_iValuesToSetCount)->
//																													FindChild(mmUI::g_pUIXML_EL_DEF_Value)->
//																													GetText());
//
//				v_iValuesToSetCount++;
//			};
//		};
//	};
//
//	delete v_psXMLDoc;
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetComplexCalculationPatternInputParamsFromUI"));
//}

mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams mmImages::mmImagesComplexCalculationManagement::InitializeParamsOfSimpleCalculationMethod(mmXML::mmXMLNodeI* p_psXMLCalcPatternDefinition,
																																																																																						mmInt p_iCalculationMethodToInitializeID)
{
	SendLogMessage(mmLog::debug,mmString(L"Start InitializeParamsOfSimpleCalculationMethod"));

	mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams v_sCAP;

	mmXML::mmXMLNodeI* v_psXMLCalcMethod = p_psXMLCalcPatternDefinition->
																				 FindChild(mmImages::g_pComplexCalcXML_PatternDefinition_Node)->
																				 GetChild(p_iCalculationMethodToInitializeID);

	mmXML::mmXMLNodeI* v_psXMLCalcMethod_InParams = v_psXMLCalcMethod->FindChild(mmImages::g_pAutoCalcXML_INParams_Node);
	std::vector<mmXML::mmXMLNodeI*> v_vCMInParams = v_psXMLCalcMethod_InParams->GetChilds();
	mmInt v_iCMIPCount = v_vCMInParams.size();
	for(mmInt v_iCMIP=0;v_iCMIP<v_iCMIPCount;v_iCMIP++)
	{
		mmString v_sValue;
		mmString v_sName = v_vCMInParams[v_iCMIP]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node)->GetText();

		mmString v_sParamType = v_vCMInParams[v_iCMIP]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamType_Node)->GetText();

		if(v_sParamType.compare(mmImages::g_pAutoCalcXML_Params_ParamType_ImagesNames) == 0)
		{
			if(v_sName.compare(mmImages::g_pAutoCalcXML_Params_ParamType_ImageCount_All) != 0)
			{
				std::vector<mmString> v_vClNames = mmStringUtilities::MMStringToVectorOfMMString(v_sName);
				mmInt v_iCCount = mmStringUtilities::StringToMMInt(v_vClNames[0]);
				v_sValue = v_vClNames[0];
				for(mmInt v_iC=0;v_iC<v_iCCount;v_iC++)
				{
					v_sValue += mmString(L" ") +
											GetValueOfParameter(p_psXMLCalcPatternDefinition,
																					v_vClNames[v_iC+1]);
				};
			};
		}
		else
		{
			v_sValue = GetValueOfParameter(p_psXMLCalcPatternDefinition,
																		 v_vCMInParams[v_iCMIP]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamName_Node)->GetText());
		};

		v_vCMInParams[v_iCMIP]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->SetText(v_sValue);

		SendLogMessage(mmLog::user,mmString(L"InParam Name=") +
															 v_sName +
															 mmString(L" Value=") +
															 v_sValue);
	};

	mmXML::mmXMLDocI* v_psINXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
	v_psINXMLDoc->CopyXMLRootNodeStructure(v_psXMLCalcMethod_InParams);
	mmString v_sInParams = v_psINXMLDoc->SaveToXMLBuffer();
	mmInt v_iStrSize = v_sInParams.copy(v_sCAP.sInParams,INOUT_PARAMS_SIZE,0);
	v_sCAP.sInParams[v_iStrSize] = 0;
	delete v_psINXMLDoc;

	mmXML::mmXMLNodeI* v_psXMLCalcMethod_OutParams = v_psXMLCalcMethod->FindChild(mmImages::g_pAutoCalcXML_OUTParams_Node);
	//std::vector<mmXML::mmXMLNodeI*> v_vCMOutParams = v_psXMLCalcMethod_OutParams->GetChilds();
	//mmInt v_iCMOPCount = v_vCMOutParams.size();

	mmXML::mmXMLDocI* v_psOUTXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
	v_psOUTXMLDoc->CopyXMLRootNodeStructure(v_psXMLCalcMethod_OutParams);
	mmString v_sOutParams = v_psOUTXMLDoc->SaveToXMLBuffer();
	v_iStrSize = v_sOutParams.copy(v_sCAP.sOutParams,INOUT_PARAMS_SIZE,0);
	v_sCAP.sOutParams[v_iStrSize] = 0;
	delete v_psOUTXMLDoc;

	SendLogMessage(mmLog::debug,mmString(L"End InitializeParamsOfSimpleCalculationMethod"));

	return v_sCAP;
}

// PB //
void mmImages::mmImagesComplexCalculationManagement::UpdateOutputParamsOfSimpleCalculationMethod(	mmXML::mmXMLNodeI* p_psXMLCalcPatternDefinition,
																									mmInt p_iCalculationMethodToUpdateID)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UpdateOutputParamsOfSimpleCalculationMethod"));

	if(m_psCurrentCalculationMethod != NULL)
	{
		mmImages::mmImagesCalculationMethodI::sCalculationMethodParams v_sCMP = m_psCurrentCalculationMethod->GetCalculationMethodInfo();
		mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
		//mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(NULL);
		v_psXMLDoc->ParseXMLBuffer(v_sCMP.sAutoParams.sOutParams);
		std::vector<mmXML::mmXMLNodeI*> v_vCurrentCMOutParams = v_psXMLDoc->GetXMLRootNode()->GetChilds();

		mmXML::mmXMLNodeI* v_psXMLCalcMethod = p_psXMLCalcPatternDefinition->
																					 FindChild(mmImages::g_pComplexCalcXML_PatternDefinition_Node)->
																					 GetChild(p_iCalculationMethodToUpdateID);

		mmXML::mmXMLNodeI* v_psXMLCalcMethod_OutParams = v_psXMLCalcMethod->FindChild(mmImages::g_pAutoCalcXML_OUTParams_Node);
		std::vector<mmXML::mmXMLNodeI*> v_vCMOutParams = v_psXMLCalcMethod_OutParams->GetChilds();
		mmInt v_iCMOPCount = v_vCMOutParams.size();
		for(mmInt v_iCMOP=0;v_iCMOP<v_iCMOPCount;v_iCMOP++)
		{
			SetValueOfParameter(p_psXMLCalcPatternDefinition,
													v_vCMOutParams[v_iCMOP]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->GetText(),
													v_vCurrentCMOutParams[v_iCMOP]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->GetText());

			mmString v_sName = v_vCMOutParams[v_iCMOP]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->GetText();
			mmString v_sValue = v_vCurrentCMOutParams[v_iCMOP]->FindChild(mmImages::g_pAutoCalcXML_Params_ParamValue_Node)->GetText();

			SendLogMessage(mmLog::user,mmString(L"OutParam Name=") +
																 v_sName +
																 mmString(L" Value=") +
																 v_sValue);
		};

		delete v_psXMLDoc;
	};

	SendLogMessage(mmLog::debug,mmString(L"End UpdateOutputParamsOfSimpleCalculationMethod"));
}

mmString mmImages::mmImagesComplexCalculationManagement::GetValueOfParameter(mmXML::mmXMLNodeI* p_psXMLCalcPatternDefinition,
																																										 mmString p_sParamName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetValueOfParameter"));

	mmString v_sOutValue;

	mmXML::mmXMLNodeI* v_psXMLCalcPattern_InParams = p_psXMLCalcPatternDefinition->FindChild(mmImages::g_pComplexCalcXML_PatternInputParams_Node);
	std::vector<mmXML::mmXMLNodeI*> v_vCPInParams = v_psXMLCalcPattern_InParams->GetChilds();
	mmInt v_iCPIPCount = v_vCPInParams.size();
	for(mmInt v_iCMIP=0;v_iCMIP<v_iCPIPCount;v_iCMIP++)
	{
		if(p_sParamName.compare(v_vCPInParams[v_iCMIP]->FindChild(mmImages::g_pComplexCalcXML_PatternInputParamName_Node)->GetText()) == 0)
		{
			mmXML::mmXMLNodeI* v_psXMLCalcPattern_InParam_ModifiedNode =
				v_vCPInParams[v_iCMIP]->FindChild(mmImages::g_pComplexCalcXML_PatternInputParamValueModifiedNodeName_Node);

			if(v_psXMLCalcPattern_InParam_ModifiedNode == NULL)
			{
				v_sOutValue = v_vCPInParams[v_iCMIP]->FindChild(mmImages::g_pComplexCalcXML_PatternInputParamValue_Node)->GetText();
			}
			else
			{
				mmString v_sModifierValue = GetValueOfParameter(p_psXMLCalcPatternDefinition,v_psXMLCalcPattern_InParam_ModifiedNode->GetText());

				std::vector<mmXML::sXMLAttribute> v_vAttributes = v_vCPInParams[v_iCMIP]->GetAttributes();

				if(v_vAttributes.size() == 0)
				{
					v_sOutValue = mmStringUtilities::MMRealToString(mmStringUtilities::StringToMMReal(v_vCPInParams[v_iCMIP]->FindChild(mmImages::g_pComplexCalcXML_PatternInputParamValue_Node)->GetText()) *
																													mmStringUtilities::StringToMMReal(v_sModifierValue));
				}
				else
				{
					v_sOutValue = GetValueOfParameter(p_psXMLCalcPatternDefinition,v_psXMLCalcPattern_InParam_ModifiedNode->GetText());
				};
			};

			break;
		};
	};

	if(v_sOutValue.size() == 0)
	{
		mmXML::mmXMLNodeI* v_psXMLCalcPattern_OutParams = p_psXMLCalcPatternDefinition->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParams_Node);
		std::vector<mmXML::mmXMLNodeI*> v_vCPOutParams = v_psXMLCalcPattern_OutParams->GetChilds();
		mmInt v_iCPOPCount = v_vCPOutParams.size();
		for(mmInt v_iCMOP=0;v_iCMOP<v_iCPOPCount;v_iCMOP++)
		{
			if(p_sParamName.compare(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamName_Node)->GetText()) == 0)
			{
				mmXML::mmXMLNodeI* v_psXMLCalcPattern_OutParam_ModifiedNode = v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamValueModifiedNodeName_Node);
				mmXML::mmXMLNodeI* v_psXMLCalcPattern_OutParam_Modified2ndNode = v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamValueModifiedSecondNodeName_Node);

				if(v_psXMLCalcPattern_OutParam_ModifiedNode == NULL)
				{
					v_sOutValue = v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamValue_Node)->GetText();
				}
				else
				{
					if(v_psXMLCalcPattern_OutParam_Modified2ndNode == NULL)
					{
						mmString v_sModifierValue = GetValueOfParameter(p_psXMLCalcPatternDefinition,v_psXMLCalcPattern_OutParam_ModifiedNode->GetText());

						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Multiplication) == 0)
						{
							v_sOutValue = mmStringUtilities::MMRealToString(mmStringUtilities::StringToMMReal(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamValue_Node)->GetText()) *
																															mmStringUtilities::StringToMMReal(v_sModifierValue));
						}
						else
						{
							if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Division) == 0)
							{
								v_sOutValue = mmStringUtilities::MMRealToString(mmStringUtilities::StringToMMReal(v_sModifierValue)/
																																mmStringUtilities::StringToMMReal(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamValue_Node)->GetText()));
							}
							else
							{
								//mmReal v_rArg = mmStringUtilities::StringToMMReal(v_sModifierValue);
								//mmReal v_rPow = mmStringUtilities::StringToMMReal(v_vCPOutParams[v_iCMOP]->FindChild(mmClouds::g_pComplexCalcXML_PatternOutputParamValue_Node)->GetText());
								//mmReal v_rRes = pow(v_rArg,v_rPow);

								v_sOutValue = mmStringUtilities::MMRealToString(pow(mmStringUtilities::StringToMMReal(v_sModifierValue),
																																		mmStringUtilities::StringToMMReal(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamValue_Node)->GetText())));
							};
						};
					}
					else
					{
						mmString v_sModified1stValue = GetValueOfParameter(p_psXMLCalcPatternDefinition,v_psXMLCalcPattern_OutParam_ModifiedNode->GetText());

						mmString v_sModified2ndValue = GetValueOfParameter(p_psXMLCalcPatternDefinition,v_vCPOutParams[v_iCMOP]->
																																														FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamValueModifiedSecondNodeName_Node)->
																																														GetText());

						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Addition) == 0)
						{
							v_sOutValue = mmStringUtilities::MMRealToString(mmStringUtilities::StringToMMReal(v_sModified1stValue) +
																															mmStringUtilities::StringToMMReal(v_sModified2ndValue));
						};
						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Subtraction) == 0)
						{
							v_sOutValue = mmStringUtilities::MMRealToString(mmStringUtilities::StringToMMReal(v_sModified1stValue) -
																															mmStringUtilities::StringToMMReal(v_sModified2ndValue));
						};
						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Multiplication) == 0)
						{
							v_sOutValue = mmStringUtilities::MMRealToString(mmStringUtilities::StringToMMReal(v_sModified1stValue) *
																															mmStringUtilities::StringToMMReal(v_sModified2ndValue));
						};
						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Division) == 0)
						{
							v_sOutValue = mmStringUtilities::MMRealToString(mmStringUtilities::StringToMMReal(v_sModified1stValue) /
																															mmStringUtilities::StringToMMReal(v_sModified2ndValue));
						};
						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Minimum) == 0)
						{
							mmReal v_r1stValue = mmStringUtilities::StringToMMReal(v_sModified1stValue);
							mmReal v_r2ndValue = mmStringUtilities::StringToMMReal(v_sModified2ndValue);

							if(v_r1stValue < v_r2ndValue)
							{
								v_sOutValue = v_sModified1stValue;
							}
							else
							{
								v_sOutValue = v_sModified2ndValue;
							};
						};
						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Maximum) == 0)
						{
							mmReal v_r1stValue = mmStringUtilities::StringToMMReal(v_sModified1stValue);
							mmReal v_r2ndValue = mmStringUtilities::StringToMMReal(v_sModified2ndValue);

							if(v_r1stValue > v_r2ndValue)
							{
								v_sOutValue = v_sModified1stValue;
							}
							else
							{
								v_sOutValue = v_sModified2ndValue;
							};
						};
						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Equal) == 0)
						{
							mmReal v_r1stValue = mmStringUtilities::StringToMMReal(v_sModified1stValue);
							mmReal v_r2ndValue = mmStringUtilities::StringToMMReal(v_sModified2ndValue);

							if(v_r1stValue == v_r2ndValue)
							{
								v_sOutValue = mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES;
							}
							else
							{
								v_sOutValue = mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_NO;
							};
						};
						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Greater) == 0)
						{
							mmReal v_r1stValue = mmStringUtilities::StringToMMReal(v_sModified1stValue);
							mmReal v_r2ndValue = mmStringUtilities::StringToMMReal(v_sModified2ndValue);

							if(v_r1stValue > v_r2ndValue)
							{
								v_sOutValue = mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES;
							}
							else
							{
								v_sOutValue = mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_NO;
							};
						};
						if(v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamOperation_Node)->GetText().compare(mmImages::g_pComplexCalcXML_PatternOutputFormula_Less) == 0)
						{
							mmReal v_r1stValue = mmStringUtilities::StringToMMReal(v_sModified1stValue);
							mmReal v_r2ndValue = mmStringUtilities::StringToMMReal(v_sModified2ndValue);

							if(v_r1stValue < v_r2ndValue)
							{
								v_sOutValue = mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_YES;
							}
							else
							{
								v_sOutValue = mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue_NO;
							};
						};
					};
				};

				break;
			};
		};
	};

	SendLogMessage(mmLog::info,mmString(L"GetValueOfParameter ")+
														 p_sParamName +
														 mmString(L"=") +
														 v_sOutValue);

	SendLogMessage(mmLog::debug,mmString(L"End GetValueOfParameter"));

	return v_sOutValue;
}

void mmImages::mmImagesComplexCalculationManagement::SetValueOfParameter(	mmXML::mmXMLNodeI* p_psXMLCalcPatternDefinition,
																			mmString p_sParamName,
																			mmString p_sParamValue)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetValueOfParameter"));

	mmXML::mmXMLNodeI* v_psXMLCalcPattern_OutParams =
		p_psXMLCalcPatternDefinition->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParams_Node);
	std::vector<mmXML::mmXMLNodeI*> v_vCPOutParams = v_psXMLCalcPattern_OutParams->GetChilds();
	mmInt v_iCPOPCount = v_vCPOutParams.size();
	for(mmInt v_iCMOP=0;v_iCMOP<v_iCPOPCount;v_iCMOP++)
	{
		if(p_sParamName.compare(v_vCPOutParams[v_iCMOP]->
														FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamName_Node)->
														GetText()) == 0)
		{
			v_vCPOutParams[v_iCMOP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParamValue_Node)->SetText(p_sParamValue);
		};
	};

	mmXML::mmXMLNodeI* v_psXMLCalcPattern_InParams =
		p_psXMLCalcPatternDefinition->FindChild(mmImages::g_pComplexCalcXML_PatternInputParams_Node);
	std::vector<mmXML::mmXMLNodeI*> v_vCPInParams = v_psXMLCalcPattern_InParams->GetChilds();
	mmInt v_iCPIPCount = v_vCPInParams.size();
	for(mmInt v_iCMIP=0;v_iCMIP<v_iCPIPCount;v_iCMIP++)
	{
		if(p_sParamName.compare(v_vCPInParams[v_iCMIP]->FindChild(mmImages::g_pComplexCalcXML_PatternInputParamName_Node)->GetText()) == 0)
		{
			v_vCPInParams[v_iCMIP]->FindChild(mmImages::g_pComplexCalcXML_PatternInputParamValue_Node)->SetText(p_sParamValue);
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End SetValueOfParameter"));
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
mmString mmImages::mmImagesComplexCalculationManagement::GetInputParamsOfComplexCalculationPattern(mmString p_sCCPIDName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetInputParamsOfComplexCalculationPattern"));

	mmString v_sOutValue = L"";

	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
	v_psXMLDoc->CopyXMLRootNodeStructure(GetInputParamsOfCCPattern(p_sCCPIDName));
	v_sOutValue = v_psXMLDoc->SaveToXMLBuffer();

	SendLogMessage(mmLog::debug,mmString(L"End GetInputParamsOfComplexCalculationPattern"));
	return v_sOutValue;
}
//---------------------------------------------------------------------------
mmString mmImages::mmImagesComplexCalculationManagement::GetOutputParamsOfComplexCalculationPattern(mmString p_sCCPIDName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetOutputParamsOfComplexCalculationPattern"));

	mmString v_sOutValue = L"";

	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
	v_psXMLDoc->CopyXMLRootNodeStructure(GetOutputParamsOfCCPattern(p_sCCPIDName));
	v_sOutValue = v_psXMLDoc->SaveToXMLBuffer();

	SendLogMessage(mmLog::debug,mmString(L"End GetOutputParamsOfComplexCalculationPattern"));
	return v_sOutValue;
}
//---------------------------------------------------------------------------
mmString mmImages::mmImagesComplexCalculationManagement::SetInputParamsOfComplexCalculationPattern(mmString p_sCCPIDName, mmString p_sNewParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetInputParamsOfComplexCalculationPattern"));

	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
	v_psXMLDoc->ParseXMLBuffer(p_sNewParams);

	mmXML::mmXMLNodeI* v_psXMLNode;
	v_psXMLNode = GetInputParamsOfCCPattern(p_sCCPIDName);
	mmXML::mmXMLNodeI* v_psXMLParent = v_psXMLNode->GetParent();
	delete v_psXMLNode;
	mmString mtemp = v_psXMLDoc->GetXMLRootNode()->GetName();
	v_psXMLParent->AddChildWithStructure(v_psXMLDoc->GetXMLRootNode());

	SendLogMessage(mmLog::debug,mmString(L"End SetInputParamsOfComplexCalculationPattern"));
	return GetInputParamsOfComplexCalculationPattern(p_sCCPIDName);
}
//---------------------------------------------------------------------------
mmString mmImages::mmImagesComplexCalculationManagement::SetOutputParamsOfComplexCalculationPattern(mmString p_sCCPIDName, mmString p_sNewParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start SetOutputParamsOfComplexCalculationPattern"));

	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
	v_psXMLDoc->ParseXMLBuffer(p_sNewParams);

	mmXML::mmXMLNodeI* v_psXMLNode;
	v_psXMLNode = GetOutputParamsOfCCPattern(p_sCCPIDName);
	mmXML::mmXMLNodeI* v_psXMLParent = v_psXMLNode->GetParent();
	delete v_psXMLNode;
	mmString mtemp = v_psXMLDoc->GetXMLRootNode()->GetName();
	v_psXMLParent->AddChildWithStructure(v_psXMLDoc->GetXMLRootNode());

	SendLogMessage(mmLog::debug,mmString(L"End SetOutputParamsOfComplexCalculationPattern"));
	return GetOutputParamsOfComplexCalculationPattern(p_sCCPIDName);
}
//---------------------------------------------------------------------------
mmXML::mmXMLNodeI* mmImages::mmImagesComplexCalculationManagement::GetInputParamsOfCCPattern(mmString p_sCCPIDName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetInputParamsOfCCPattern"));

	std::vector<mmXML::mmXMLNodeI*> v_vCCPatterns = m_psXMLComplexPatternContainer->GetXMLRootNode()->GetChilds();

	mmInt v_iCCPCount = v_vCCPatterns.size();
	for(mmInt v_iCCP=0;v_iCCP<v_iCCPCount;v_iCCP++)
	{
		if(v_vCCPatterns[v_iCCP]->FindChild(mmImages::g_pComplexCalcXML_PatternIDName_Node)->GetText().compare(p_sCCPIDName) == 0)
		{
			mmXML::mmXMLNodeI* v_vCCPInputParams = v_vCCPatterns[v_iCCP]->FindChild(mmImages::g_pComplexCalcXML_PatternInputParams_Node);

			SendLogMessage(mmLog::debug,mmString(L"End GetInputParamsOfCCPattern"));

			return v_vCCPInputParams;
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetInputParamsOfCCPattern"));

	return NULL;
}
//---------------------------------------------------------------------------
mmXML::mmXMLNodeI* mmImages::mmImagesComplexCalculationManagement::GetOutputParamsOfCCPattern(mmString p_sCCPIDName)
{
	SendLogMessage(mmLog::debug,mmString(L"Start GetOutputParamsOfCCPattern"));

	std::vector<mmXML::mmXMLNodeI*> v_vCCPatterns = m_psXMLComplexPatternContainer->GetXMLRootNode()->GetChilds();

	mmInt v_iCCPCount = v_vCCPatterns.size();
	for(mmInt v_iCCP=0;v_iCCP<v_iCCPCount;v_iCCP++)
	{
		if(v_vCCPatterns[v_iCCP]->FindChild(mmImages::g_pComplexCalcXML_PatternIDName_Node)->GetText().compare(p_sCCPIDName) == 0)
		{
			mmXML::mmXMLNodeI* v_vCCPInputParams = v_vCCPatterns[v_iCCP]->FindChild(mmImages::g_pComplexCalcXML_PatternOutputParams_Node);

			SendLogMessage(mmLog::debug,mmString(L"End GetOutputParamsOfCCPattern"));

			return v_vCCPInputParams;
		};
	};

	SendLogMessage(mmLog::debug,mmString(L"End GetOutputParamsOfCCPattern"));

	return NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
