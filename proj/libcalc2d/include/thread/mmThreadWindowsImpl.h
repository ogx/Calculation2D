//******************************************************************************
//******************************************************************************
//
//  Thread class (Windows implementation)
//
//
//  Description: Implementation of mmThreadI for Windows operating systems.
//
//******************************************************************************
//******************************************************************************
#ifndef mmThreadWindowsImplH
#define mmThreadWindowsImplH

#include <windows.h>

#include <mmError.h>

#include <interfaces\mmIThread.h>
#include <log\mmLogSender.h>
#include <interfaces\mmISynchronize.h>

namespace mmThread
{
  ////////////////////////////////////////////////////////////////////////////////
	/// This class implements mmThreads::mmThreadI interface. It uses Windows API.
	////////////////////////////////////////////////////////////////////////////////
	class mmThreadWindows: public mmThreadI, mmLog::mmLogSender
	{
    private:  // fields
      ////////////////////////////////////////////////////////////////////////////////
			/// Windows thread handle.
			////////////////////////////////////////////////////////////////////////////////
			HANDLE m_hWinThreadHandle;
			////////////////////////////////////////////////////////////////////////////////
			/// Thread state.
			////////////////////////////////////////////////////////////////////////////////
			eStatus m_eThreadStatus;
			////////////////////////////////////////////////////////////////////////////////
			/// Pointer to object with implementation mmThreadExecutionI which perform
			/// thread calculation.
			////////////////////////////////////////////////////////////////////////////////
			mmThreadExecutionI* m_psThreadExecutionClass;
			////////////////////////////////////////////////////////////////////////////////
			/// Synchronize object for thread data.
			////////////////////////////////////////////////////////////////////////////////
      mmSynchronize::mmExclusiveLockI* m_psExclusiveLock;
    private:  // methods
      ////////////////////////////////////////////////////////////////////////////////
			/// This method is always called during new thread creation. It calls
			/// ThreadExecute method for mmThreadWindows object pointed by
			/// p_pThreadClassPointer pointer.
      ///
			/// @param[in] p_pThreadClassPointer pointer to mmThreadWindows object
      /// @return state of thread.
      ////////////////////////////////////////////////////////////////////////////////
			static DWORD WINAPI ThreadRedirectFunction(LPVOID p_pThreadClassPointer);
			////////////////////////////////////////////////////////////////////////////////
			/// This method is called by ThreadRedirectFunction for call Execute method
			/// from mmThreadExecutionI object stored in m_psThreadExecutionClass.
			////////////////////////////////////////////////////////////////////////////////
			void ThreadExecute(void);
    public:   // methods
      ////////////////////////////////////////////////////////////////////////////////
			/// Constructor. In case of allocation error it throws mmError(mmeBadAlloc).
			///
      /// @param[in] p_psLogReceiver pointer to log object.
      ////////////////////////////////////////////////////////////////////////////////
      mmThreadWindows(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
      ////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmThreadWindows();

			void RegisterThreadExecutionClass(mmThreadExecutionI* p_psThreadExecutionObject);
			void Run(void);
			void Stop(void);
			eStatus GetStatus(void);
			mmReal GetProgress(void);
  };
};

#endif
