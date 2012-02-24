//******************************************************************************
//******************************************************************************
//
//  DLL images Calculation Method Broker class
//
//
//  Description: This header defines implementation of
//							 mmImages::mmImagesCalculationMethodI interface for broker
//							 functionality between main application and DLL code
//							 (it is required because STL library fails in DLL).
//
//******************************************************************************
//******************************************************************************
#ifndef mmImagesCalculationMethodDLLBrokerH
#define mmImagesCalculationMethodDLLBrokerH

#include <interfaces\mmIImages.h>
#include <log\mmLogSender.h>

#include <mmExecutionContext.h>

#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
/// Definition of function type returning initialised specified images
/// calculation method.
////////////////////////////////////////////////////////////////////////////////
typedef mmImages::mmImagesCalculationMethodI* (__stdcall *mmDLLImagesCalculationMethod_Create)(mmExecutionContext*,const wchar_t*);

////////////////////////////////////////////////////////////////////////////////
/// Definition of function type freeing images calculation method object.
////////////////////////////////////////////////////////////////////////////////
typedef void (__stdcall *mmDLLImagesCalculationMethod_Destroy)(mmImages::mmImagesCalculationMethodI*);

namespace mmImages
{
	////////////////////////////////////////////////////////////////////////////////
	/// Implementation of mmImages::mmImagesCalculationMethodI interface for
	/// DLL broker.
	////////////////////////////////////////////////////////////////////////////////
	class mmImagesCalculationMethodDLLBroker: public mmImagesCalculationMethodI, public mmLog::mmLogSender
	{
		private:		// fields
			mmFactories::mmUtilsFactoryI * m_psUtilsFactory;

			HINSTANCE m_hDLLHandle;
			mmImages::mmImagesCalculationMethodI* m_psInitializedImagesCalculationMethod;


			mmDLLImagesCalculationMethod_Create m_psDLLImagesCalculationMethod_Create;
			mmDLLImagesCalculationMethod_Destroy m_psDLLImagesCalculationMethod_Destroy;
		public:			// methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			/// @param[in] p_sDLLName name of DLL with images calculation method
			///						 definition
			/// @param[in] p_sImagesCalculationMethodName name of images
			///						 calculation method to initialize
			/// @param[in] p_psLogReceiver pointer to log object
			////////////////////////////////////////////////////////////////////////////////
			mmImagesCalculationMethodDLLBroker(mmString const & p_sDLLName, mmString const & p_sImagesCalculationMethodName, mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmImagesCalculationMethodDLLBroker();

			mmImages::mmImagesCalculationMethodI::sCalculationMethodParams GetCalculationMethodInfo(void);
			void SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImageStructure, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams = NULL);

			void UserAction(mmString p_sXMLParams);
			void UserAction(wchar_t* p_pcXMLParamsBuffer, mmInt p_iXMLParamsBufferSize);

			bool Execute(void);
			void StopExecution(void);
			bool IsExecuting(void);
			mmReal GetProgress(void);
	};
};

#endif

