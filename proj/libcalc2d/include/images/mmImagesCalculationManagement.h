//******************************************************************************
//******************************************************************************
//
//  Images calculation management class
//
//
//  Description: This header defines mmImages calculation management class
//
//******************************************************************************
//******************************************************************************
#ifndef mmIImagesCalculationManagementH
#define mmIImagesCalculationManagementH

#include <mmGlobalDefs.h>
#include <mmError.h>
#include <log\mmLogSender.h>
#include <interfaces\mmInterfaceInitializers.h>

namespace mmImages
{


	////////////////////////////////////////////////////////////////////////////////
	/// Class for images calculation management.
	////////////////////////////////////////////////////////////////////////////////
	class mmImagesCalculationManagement: public mmLog::mmLogSender,	 public mmThread::mmThreadExecutionI
	{
		private:
			bool m_bIsActiveCalculation;


			mmImages::mmImageStructureI* m_psImageStructure;
			std::vector<mmThread::mmThreadI*> m_vRunningThreads;
			mmInt m_iCalculationThreadCount;

			// control thread
			bool m_bTerminateControlThreadExecution;
			mmThread::mmThreadI* m_psControlThread;
			mmInt m_iThreadExecutionSleepTime;
			mmReal m_rProgress;
		private:
			void StopRunningThreads(void);
		public:
			mmImagesCalculationManagement(mmInt p_iCalcThreadCount, mmLog::mmLogReceiverI *p_psLogReceiver);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmImagesCalculationManagement();

			void CalculateImages(mmImages::mmImagesCalculationMethodI* p_psImagesCalculationMethod, mmImages::mmImageStructureI* p_psImageStructure,
						mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams = NULL);

			bool IsCalculating(void);

			bool Execute(void);
			void StopExecution(void);
			bool IsExecuting(void);
			mmReal GetProgress(void);

			void UserAction(mmString p_sXMLParams);
			void UserAction(wchar_t* p_pcXMLParamsBuffer,	mmInt p_iXMLParamsBufferSize);
	};
};

#endif
