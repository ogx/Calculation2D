#include <interfaces\mmInterfaceInitializers.h>

#include <synchronize\mmSynchronizeWindowsImpl.h>
#include <fileio\mmFileIOStd.h>
#include <log\mmLogFile.h>
#include <xml\mmXMLBB.h>

#include <thread\mmThreadWindowsImpl.h>
#include <images\mmImagesStructure.h>
#include <images\mmImagesCalculationMethodContainerWindows.h>

#include <factories\mmUtilsFactory.h>

#include <formats\mmMultiFormat.h>

mmSynchronize::mmExclusiveLockI* mmInterfaceInitializers::CreateExclusiveLock(mmLog::mmLogReceiverI *p_psLogReceiver)
{
	try
	{
		return (new mmSynchronize::mmExclusiveLockCSWindows(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
  };
}

mmSynchronize::mmReadWriteLockI* mmInterfaceInitializers::CreateReadWriteLock(mmLog::mmLogReceiverI *p_psLogReceiver)
{
	try
	{
		return (new mmSynchronize::mmReadWriteLockCSWindows(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}


mmFileIO::mmFileUtilsI* mmInterfaceInitializers::CreateFileUtils(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmFileIO::mmFileUtilsSTD(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmLog::mmLogReceiverI* mmInterfaceInitializers::CreateLogFile(mmString const & p_sLogFileName)
{
	try
	{
		return (new mmLog::mmLogFile(p_sLogFileName));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmLog::mmLogSenderI* mmInterfaceInitializers::CreateLogSender(mmString const & p_sClassName, void * const p_pClassPointer, mmLog::mmLogReceiverI * const p_psLogReceiver)
{
	try
	{
		return (new mmLog::mmLogSender(p_sClassName, p_psLogReceiver, p_pClassPointer));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmXML::mmXMLDocI* mmInterfaceInitializers::CreateXMLDocument(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmXML::mmXMLDocBB);
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmXML::mmXMLNodeI* mmInterfaceInitializers::CreateXMLNode(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmXML::mmXMLNodeBB);
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}
mmThread::mmThreadI* mmInterfaceInitializers::CreateDefaultThread(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmThread::mmThreadWindows(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmImages::mmImagesCalculationMethodContainerI* mmInterfaceInitializers::CreateDefaultImagesCalculationMethodContainer(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmImages::mmImagesCalculationMethodContainerForWindows(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmFactories::mmUtilsFactoryI* mmInterfaceInitializers::CreateUtilsFactory(void)
{
	try
	{
		return (new mmFactories::mmUtilsFactory);
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmFormats::mmFormatI* mmInterfaceInitializers::CreateRWFormat()
{
	try
	{
		return (new mmFormats::mmMultiFormat());
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}
