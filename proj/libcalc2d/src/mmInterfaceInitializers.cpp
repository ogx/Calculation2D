#include <mmInterfaceInitializers.h>
#include <synchronize\mmSynchronizeWindowsImpl.h>
#include <fileio\mmFileIOStd.h>
#include <log\mmLogFile.h>
#include <xml\mmXMLBB.h>

#include <thread\mmThreadWindowsImpl.h>
#include <images\mmImagesStructure.h>
#include <images\mmImagesCalculationMethodContainerWindows.h>

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

mmFileIO::mmTextFileReadI* mmInterfaceInitializers::CreateTextFileForRead(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmFileIO::mmTextFileReadSTD(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmFileIO::mmBinaryFileReadI* mmInterfaceInitializers::CreateBinaryFileForRead(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmFileIO::mmBinaryFileReadSTD(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmFileIO::mmTextFileWriteI* mmInterfaceInitializers::CreateTextFileForWrite(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmFileIO::mmTextFileWriteSTD(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}

mmFileIO::mmBinaryFileWriteI* mmInterfaceInitializers::CreateBinaryFileForWrite(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmFileIO::mmBinaryFileWriteSTD(p_psLogReceiver));
	}
	catch(std::bad_alloc)
	{
		throw mmError(mmeBadAlloc);
	};
}


mmLog::mmLogReceiverI* mmInterfaceInitializers::CreateLogFile(mmString p_sLogFileName)
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

mmXML::mmXMLDocI* mmInterfaceInitializers::CreateXMLDocument(mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmXML::mmXMLDocBB(p_psLogReceiver));
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
		return (new mmXML::mmXMLNodeBB(p_psLogReceiver));
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

mmImages::mmImageStructureI* mmInterfaceInitializers::CreateDefaultImageStructure(mmImages::mmImageI::mmCallbackI * p_psCallback, mmLog::mmLogReceiverI* p_psLogReceiver)
{
	try
	{
		return (new mmImages::mmImageStructure(p_psCallback));
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

