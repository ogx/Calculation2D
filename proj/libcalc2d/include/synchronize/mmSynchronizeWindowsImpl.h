//******************************************************************************
//******************************************************************************
//
//  Synchronization implementation for Windows systems
//
//
//  Description: Implementation of synchronization behavior for Windows
//							 operating systems basing on CriticalSection
//
//******************************************************************************
//******************************************************************************
#ifndef mmSynchronizeWindowsImplH
#define mmSynchronizeWindowsImplH

#include <windows.h>

#include <interfaces\mmISynchronize.h>
#include <log\mmLogSender.h>

namespace mmSynchronize
{
	////////////////////////////////////////////////////////////////////////////////
	/// Implements interface mmExclusiveLockI. It uses Windows CRITICAL_SECTION.
	////////////////////////////////////////////////////////////////////////////////
	class mmExclusiveLockCSWindows: public mmExclusiveLockI, mmLog::mmLogSender
	{
	private:  // fields
		////////////////////////////////////////////////////////////////////////////////
		/// Windows critical section.
		////////////////////////////////////////////////////////////////////////////////
		CRITICAL_SECTION m_sCriticalSection;
	public:   // methods
		////////////////////////////////////////////////////////////////////////////////
		/// Contructor initializes critical section object.
		///
		/// @param[in] p_psLogReceiver pointer to mmLogReceiverI object.
		////////////////////////////////////////////////////////////////////////////////
		mmExclusiveLockCSWindows(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
		 ////////////////////////////////////////////////////////////////////////////////
		/// Destructor. In case of unlocked state throws error
		///	mmError(mmeExclusiveLockUnlocked).
		////////////////////////////////////////////////////////////////////////////////
		~mmExclusiveLockCSWindows();

		virtual bool TryLock(void);
		virtual void Lock(void);
		virtual void Unlock(void);
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Implements interface mmReadWriteLockI. It uses mmExclusiveLockCSWindows
	///	and supports functionality for multiple reads and single exclusive write.
	////////////////////////////////////////////////////////////////////////////////
	class mmReadWriteLockCSWindows: public mmReadWriteLockI, mmLog::mmLogSender
	{
	private:  // fields
		////////////////////////////////////////////////////////////////////////////////
		/// Pointer to m5ExclusiveLockI object. This object is used for synchronizing
		///	state of m_iReadWriteCount counter.
		////////////////////////////////////////////////////////////////////////////////
		mmExclusiveLockI* m_psExclusiveLock;
		////////////////////////////////////////////////////////////////////////////////
		/// Counter for synchronization state:	= -1 - locked for write
		///        								= 0  - unlocked
		///        								= n  - locked for read by n-threads
		////////////////////////////////////////////////////////////////////////////////
		mmInt m_iReadWriteCount;
		  ////////////////////////////////////////////////////////////////////////////////
		/// Defines if any object waits for write access (TRUE) or not (FALSE).
		/// If TRUE then other objects waiting for read access are stopped.
		 ////////////////////////////////////////////////////////////////////////////////
		bool m_bWaitForWriteLock;
	public:   // methods
		////////////////////////////////////////////////////////////////////////////////
		/// Constructor initializes state of object. If allocation error occurs
		/// then throws exception mmError(mmeBadAlloc).
		///
		///@param[in] p_psLogReceiver pointer to mmLogReceiverI object.
		////////////////////////////////////////////////////////////////////////////////
		mmReadWriteLockCSWindows(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
		////////////////////////////////////////////////////////////////////////////////
		/// Destructor deinitialize m_psExclusiveLock object. In case of bad end
		/// conditions throws exception (if m_iReadWriteCount!=0 then throws
		/// mmError(mmeReadWriteLockBadEndLockCount), if m_bWaitForWriteLock=true then
		/// throws mmError(mmeReadWriteLockWaitForWrite)).
		////////////////////////////////////////////////////////////////////////////////
		~mmReadWriteLockCSWindows();

		virtual bool TryLockForRead(void);
		virtual void LockForRead(void);
		virtual void UnlockFromRead(void);

		virtual bool TryLockForWrite(void);
		virtual void LockForWrite(void);
		virtual void UnlockFromWrite(void);

		virtual mmInt GetLockState(bool* p_pbWaitForWrite);
	};
};

#endif
