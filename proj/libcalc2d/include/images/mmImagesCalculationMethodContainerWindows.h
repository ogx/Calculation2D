//******************************************************************************
//******************************************************************************
//
//  images calculation method Container for Windows OS class
//
//
//  Description: This header defines implementation of
//							 mmImages::mmImagesCalculationMethodContainerI interface
//							 for Windows OS using dlls.
//
//******************************************************************************
//******************************************************************************
#ifndef mmImagesCalculationMethodContainerForWindowsOSH
#define mmImagesCalculationMethodContainerForWindowsOSH

#include <interfaces\mmICalculationMethodContainer.h>
#include <log\mmLogSender.h>


////////////////////////////////////////////////////////////////////////////////
/// Definition of function type returning number of supported images
/// calculation method types.
////////////////////////////////////////////////////////////////////////////////
typedef mmInt (__stdcall *mmDLLGetSupportedImagesCalculationMethodCount)(void);

////////////////////////////////////////////////////////////////////////////////
/// Definition of function type returning structure
/// mmImages::mmImagesCalculationMethodI::sCalculationMethodParams
/// defining supported images calculation method.
////////////////////////////////////////////////////////////////////////////////
typedef mmImages::mmImagesCalculationMethodI::sCalculationMethodParams (__stdcall *mmDLLGetSupportedImagesCalculationMethodDef)(mmInt);

namespace mmImages
{
	////////////////////////////////////////////////////////////////////////////////
	/// Implementation of mmImages::mmImagesCalculationMethodContainerI interface for
	/// windows operating system with using of dll loading.
	////////////////////////////////////////////////////////////////////////////////
	class mmImagesCalculationMethodContainerForWindows: public mmImagesCalculationMethodContainerI, mmLog::mmLogSender
	{
		private:		// definitions
			////////////////////////////////////////////////////////////////////////////////
			/// Structure defining dll with images calculation method definition.
			////////////////////////////////////////////////////////////////////////////////
			typedef struct
			{
				////////////////////////////////////////////////////////////////////////////////
				/// DLL name with full path.
				////////////////////////////////////////////////////////////////////////////////
				mmString sDLLName;
				////////////////////////////////////////////////////////////////////////////////
				/// Definition of images calculation method supported by this DLL.
				////////////////////////////////////////////////////////////////////////////////
				std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> vImagesCalculationMethodDefs;
			} sImagesCalculationMethodInDLL;
		private:		// fields
			////////////////////////////////////////////////////////////////////////////////
			/// This vector stores available Images calculation method definitions.
			////////////////////////////////////////////////////////////////////////////////
			std::vector<sImagesCalculationMethodInDLL> m_vAvailableDLLs;

		private:		// methods
			////////////////////////////////////////////////////////////////////////////////
			/// This method fills all DLLs stored in application directory into m_vAvailableDLLs
			/// structure.
			////////////////////////////////////////////////////////////////////////////////
			void SearchForDLLLibraries(void);
			////////////////////////////////////////////////////////////////////////////////
			/// This method searches for DLLs supporting images calculation method
			/// interface and writes found calculation method names into m_vAvailableDLLs
			/// structure.
			////////////////////////////////////////////////////////////////////////////////
			void SearchForImagesCalculationMethodsInDLLLibraries(void);
		public:			// methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			/// @param[in] p_psMainWindow pointer to main window object
			/// @param[in] p_psLogReceiver pointer to log object
			////////////////////////////////////////////////////////////////////////////////
			mmImagesCalculationMethodContainerForWindows(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);

			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmImagesCalculationMethodContainerForWindows();

			std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> GetAvailableImagesCalculationMethods(void);
			std::map<mmString, mmString> GetMethodFileMapping(void) const;
			mmImages::mmImagesCalculationMethodI* InitializeImagesCalculationMethod(mmString const & p_sCalculationMethodName);
	};
};

#endif

