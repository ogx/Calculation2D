//******************************************************************************
//******************************************************************************
//
//  Global Interfaces Storage class
//
//
//  Description: Class with pointers to global interfaces initialized objects.
//							 It is used for exchange of main interface objects between
//							 core of the system and other components.
//
//******************************************************************************
//******************************************************************************
#ifndef mmGlobalInterfacesStorageH
#define mmGlobalInterfacesStorageH

#include <log\mmLogSender.h>


////////////////////////////////////////////////////////////////////////////////
/// Namespace for interface initializers.
////////////////////////////////////////////////////////////////////////////////
namespace mmInterfacesStorage
{
	//////////////////////////////////////////////////////////////////////////////////
	///// Class for storage of pointers into main interfaces classes.
	//////////////////////////////////////////////////////////////////////////////////
	//class mmGlobalInterfacesStorage: public mmLog::mmLogSender
	//{
	//	private:		// members
	//		////////////////////////////////////////////////////////////////////////////////
	//		/// Pointer for private storage of memory manager object.
	//		////////////////////////////////////////////////////////////////////////////////
	//		mmMemory::mmMemoryManagerI* m_psMemoryManager;

	//	public:			// methods
	//		////////////////////////////////////////////////////////////////////////////////
	//		/// This constructor initializes all private members.
	//		///
	//		/// @param[in] p_psMemoryManager pointer to memory manager object.
	//		/// @param[in] p_psLogReceiver pointer to log object.
	//		/// @param[in] p_psMainWindow pointer to main window object.
	//		////////////////////////////////////////////////////////////////////////////////
	//		mmGlobalInterfacesStorage( mmMemory::mmMemoryManagerI* p_psMemoryManager,  mmLog::mmLogReceiverI* p_psLogReceiver );

	//		////////////////////////////////////////////////////////////////////////////////
	//		/// Desctructor.
	//		////////////////////////////////////////////////////////////////////////////////
	//		~mmGlobalInterfacesStorage();

	//		////////////////////////////////////////////////////////////////////////////////
	//		/// Method returns pointer to memory manager object.
	//		///
	//		/// @return pointer to memory manager object
	//		////////////////////////////////////////////////////////////////////////////////
	//		mmMemory::mmMemoryManagerI* GetMemoryManager(void);

	//		////////////////////////////////////////////////////////////////////////////////
	//		/// Method returns pointer to log object.
	//		///
	//		/// @return pointer to log object
	//		////////////////////////////////////////////////////////////////////////////////
	//		mmLog::mmLogReceiverI* GetLogManager(void);

	//};
	struct mmGlobalInterfacesStorage {
		mmGlobalInterfacesStorage( mmLog::mmLogReceiverI* p_psLogReceiver );
		mmLog::mmLogReceiverI* GetLogManager(void);
		mmLog::mmLogReceiverI* GetLogReceiver(void);
	private:
		mmLog::mmLogReceiverI* m_psLogReceiver;
	};
};

#endif
