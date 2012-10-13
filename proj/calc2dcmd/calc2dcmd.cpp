// calc2dcmd.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <tchar.h>

#include <console/mmClient.h>

int _tmain(int argc, _TCHAR* argv[])
{
	return mmConsole::mmClient(L"Calculation2D").Run();
}

