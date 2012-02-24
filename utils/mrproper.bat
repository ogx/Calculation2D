@ECHO OFF
ECHO # Calculation2D Mr. Proper Solution Cleaner #

REM ver. 20120224
REM ask user yes/no question
REM TODO: use log parameter optionally

REM go up
cd /d ..

REM ask user to continue
ECHO About to remove all intermediate output files in
ECHO.
ECHO %CD%
ECHO.

CHOICE /C YN /M "(Y)es you want to continue or (N)o you don't? "

IF NOT "%ERRORLEVEL%"=="1" GOTO :DONT_WANNA

REM to stop log output
REM set logfile="NUL"

set logfile="mrproper.log"

ECHO -####################- >> %logfile%
date /T >> %logfile% 
time /T >> %logfile% 
ECHO ---------------------- >> %logfile%

REM targets
REM del /S *.exe >> %logfile%  2>&1
REM del /S *.dll >> %logfile%  2>&1
REM del /S *.lib >> %logfile%  2>&1

del /S *.obj >> %logfile%  2>&1
del /S *.pch >> %logfile%  2>&1
del /S *.exp >> %logfile%  2>&1
del /S *.res >> %logfile%  2>&1
del /S *.bin >> %logfile%  2>&1
del /S *.idb >> %logfile%  2>&1
del /S *.pdb >> %logfile%  2>&1
del /S *.suo >> %logfile%  2>&1
del /S *.ncb >> %logfile%  2>&1
del /S *.ilk >> %logfile%  2>&1

REM del /S *.user >> %logfile%  2>&1
del /S *.embed.manifest >> %logfile%  2>&1
del /S *.embed.manifest.res >> %logfile%  2>&1
del /S *.intermediate.manifest >> %logfile%  2>&1
del /S *.dep >> %logfile%  2>&1
del /S *.sdf >> %logfile%  2>&1
del /S *.tlog >> %logfile%  2>&1

del /S BuildLog.htm >> %logfile%  2>&1


ECHO ---------------------- >> %logfile%

GOTO THE_END

:DONT_WANNA

ECHO.
ECHO formatting c: - just kidding, script aborted.
ECHO.

:THE_END

timeout 10