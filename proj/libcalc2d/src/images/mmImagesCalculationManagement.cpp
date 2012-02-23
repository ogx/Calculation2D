#include <images\mmImagesCalculationManagement.h>

#include <mmStringUtilities.h>
#include <mmOperatingSystemCalls.h>



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmImages::mmImagesCalculationManagement
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmImages::mmImagesCalculationManagement::mmImagesCalculationManagement(mmInt p_iCalcThreadCount, mmLog::mmLogReceiverI *p_psLogReceiver = NULL):
	mmLog::mmLogSender(L"mmImages::mmImagesCalculationManagement",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));


	m_bIsActiveCalculation = false;
	m_iCalculationThreadCount = p_iCalcThreadCount;

	m_iThreadExecutionSleepTime = 10;
	//m_psControlThread = mmInterfaceInitializers::CreateDefaultThread(GetLogReceiver());
	m_psControlThread = mmInterfaceInitializers::CreateDefaultThread(NULL);
	m_psControlThread->RegisterThreadExecutionClass(this);
	m_psControlThread->Run();

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}

mmImages::mmImagesCalculationManagement::~mmImagesCalculationManagement()
{
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	m_psControlThread->Stop();
	while(m_psControlThread->GetStatus() != mmThread::mmThreadI::finished)
	{
		mmOperatingSystem::StopThread(1);
	};
	delete m_psControlThread;

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}


void mmImages::mmImagesCalculationManagement::StopRunningThreads(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start StopRunningThreads"));

	mmInt v_iActiveThreadCount = m_vRunningThreads.size();
	for(mmInt v_iT=0;v_iT<v_iActiveThreadCount;v_iT++)
	{
		m_vRunningThreads[v_iT]->Stop();
	};

	SendLogMessage(mmLog::debug,mmString(L"End StopRunningThreads"));
}

bool mmImages::mmImagesCalculationManagement::IsCalculating(void)
{
//	SendLogMessage(mmLog::debug,mmString(L"Start IsCalculating"));

//	SendLogMessage(mmLog::debug,mmString(L"End IsCalculating"));

	return m_bIsActiveCalculation;
}


void mmImages::mmImagesCalculationManagement::CalculateImages(	mmImages::mmImagesCalculationMethodI* p_psImagesCalculationMethod,
																mmImages::mmImageStructureI* p_psImageStructure,
																mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start CalculateImages"));

	p_psImagesCalculationMethod->SetCalculationMethodParameters(	p_psImageStructure,
																	p_psAutomationParams);

	m_bIsActiveCalculation = true;

	m_psImageStructure = p_psImageStructure;
	if(p_psImagesCalculationMethod->GetCalculationMethodInfo().bIsMultithreaded)
	{
		//m_psImageStructure->InitializeForMultithreadedCalculation(m_iCalculationThreadCount);

		for(mmInt v_iT=0;v_iT<m_iCalculationThreadCount;v_iT++)
		{
			//mmThread::mmThreadI* v_psThread = mmInterfaceInitializers::CreateDefaultThread(GetLogReceiver());
			mmThread::mmThreadI* v_psThread = mmInterfaceInitializers::CreateDefaultThread(NULL);
			v_psThread->RegisterThreadExecutionClass(p_psImagesCalculationMethod);
			v_psThread->Run();

			m_vRunningThreads.push_back(v_psThread);
		};
	}
	else
	{
  	//m_psImageStructure->InitializeForMultithreadedCalculation(1);

		mmThread::mmThreadI* v_psThread = mmInterfaceInitializers::CreateDefaultThread(NULL);
		v_psThread->RegisterThreadExecutionClass(p_psImagesCalculationMethod);
		v_psThread->Run();

		m_vRunningThreads.push_back(v_psThread);
	};

	SendLogMessage(mmLog::debug,mmString(L"End CalculateImages"));
}

bool mmImages::mmImagesCalculationManagement::Execute(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Execute"));

	mmInt v_iThreadExecutionProgressUpdateFromLastTime = 0;

	mmReal v_rPrevProgress = 0.0;

	mmReal v_rCalculationStart = 0.0;

	m_bTerminateControlThreadExecution = false; 

	while(!m_bTerminateControlThreadExecution)
	{
		if(m_bIsActiveCalculation)
		{
			mmInt v_iActiveThreadCount = m_vRunningThreads.size();
			mmInt v_iWorkingThreadCount = 0;

			// check working threads
			if(v_iActiveThreadCount > 0)
			{
				mmReal v_rProgress = 0.0;
				bool v_bIsCalculating = false;
				for(mmInt v_iT=0;v_iT<v_iActiveThreadCount;v_iT++)
				{
					mmThread::mmThreadI::eStatus v_eTS = m_vRunningThreads[v_iT]->GetStatus();

					if( (v_eTS != mmThread::mmThreadI::stopped) &&
							(v_eTS != mmThread::mmThreadI::finished) )
					{
						v_bIsCalculating = true;
						v_iWorkingThreadCount++;
					};

					v_rProgress += m_vRunningThreads[v_iT]->GetProgress();
				};
				//v_rProgress = v_rProgress/static_cast<mmReal>(v_iActiveThreadCount);
				v_rProgress = v_rProgress/static_cast<mmReal>(v_iWorkingThreadCount);
				//v_rProgress = m_vRunningThreads[0]->GetProgress();
				if(v_rProgress >= v_rPrevProgress)
				{
					v_rPrevProgress = v_rProgress;
					m_rProgress = v_rProgress;
				}
				else
				{
					v_rProgress = v_rPrevProgress;
					m_rProgress = v_rPrevProgress;
				};


				if(!v_bIsCalculating)
				{


					mmReal v_rActCalculationTime = mmOperatingSystem::GetApplicationTime();
                                        mmReal v_rElapsed = v_rActCalculationTime - v_rCalculationStart;

                                        SendLogMessage(mmLog::user,mmString(L"Operation ") + mmString(L": ") +
                                        	mmStringUtilities::MMRealToString(v_rElapsed,3) + mmString(L"s"));


					for(mmInt v_iT=0;v_iT<v_iActiveThreadCount;v_iT++)
					{
						delete m_vRunningThreads[v_iT];
						//m_vRunningThreads.clear();
					};
					m_vRunningThreads.clear();

					v_rPrevProgress = 0.0;
					m_rProgress = 100.0;

					//m_psImageStructure->DeinitializeForMultithreadedCalculation();

					m_bIsActiveCalculation = false;
				};
			};
		};

		mmOperatingSystem::StopThread(m_iThreadExecutionSleepTime);

		v_iThreadExecutionProgressUpdateFromLastTime += m_iThreadExecutionSleepTime;
	};

	SendLogMessage(mmLog::debug,mmString(L"End Execute"));

	return true;
}

void mmImages::mmImagesCalculationManagement::StopExecution(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start StopExecution"));

	m_bTerminateControlThreadExecution = true;

	SendLogMessage(mmLog::debug,mmString(L"End StopExecution"));
}

bool mmImages::mmImagesCalculationManagement::IsExecuting(void)
{
	SendLogMessage(mmLog::debug,mmString(L"Start IsExecuting"));

	SendLogMessage(mmLog::debug,mmString(L"End IsExecuting"));

	return !m_bTerminateControlThreadExecution;
}

mmReal mmImages::mmImagesCalculationManagement::GetProgress(void)
{
//	SendLogMessage(mmLog::debug,mmString(L"Start GetProgress"));
//
//	SendLogMessage(mmLog::debug,mmString(L"End GetProgress"));

	return m_rProgress;
}

void mmImages::mmImagesCalculationManagement::UserAction(mmString p_sXMLParams)
{
	SendLogMessage(mmLog::debug,mmString(L"Start UserAction"));

	// create XML document which specifies user action response
	mmXML::mmXMLDocI* v_psXMLDoc = mmInterfaceInitializers::CreateXMLDocument(this->GetLogReceiver());
	v_psXMLDoc->ParseXMLBuffer(p_sXMLParams);
	mmXML::mmXMLNodeI* v_psUIRootNode = v_psXMLDoc->GetXMLRootNode();

//	// parsing user actions
//	mmString v_sUserActionID = v_psUIRootNode->FindChild(mmUI::g_pUIXML_EL_DEF_Value)->GetText();
//
//	if(v_sUserActionID.compare(mmImages::g_pXML_Progress_CancelButton) == 0)
//	{
//		// stop threads
//		StopRunningThreads();
//	};

	SendLogMessage(mmLog::debug,mmString(L"End UserAction"));
}

void mmImages::mmImagesCalculationManagement::UserAction(wchar_t* p_pcXMLParamsBuffer, mmInt p_iXMLParamsBufferSize )
{
	SendLogMessage(mmLog::debug,mmString(L"Start UserAction"));

	UserAction(p_pcXMLParamsBuffer);

	SendLogMessage(mmLog::debug,mmString(L"End UserAction"));
}

