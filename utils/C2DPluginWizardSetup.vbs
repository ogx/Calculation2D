'/******************************************************************************
'/******************************************************************************
'/
'/  C2DPluginWizard Setup Script
'/
'/  Description: Helper script integrating Calculation2D Plugin Wizard with
'/               Visual Studio 9.0 / 10.0. Copies wizard files to Visual C++
'/               installation directory from Calculation2D SDK solution. 
'/
'/******************************************************************************
'/******************************************************************************

' create objects
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objApp = CreateObject("Shell.Application")

ShouldUninstall= False

' setup default paths
WizardLinkName = "C2D"
WizardFolderName = "C2DPluginWizard"
WizardInstallerName = "C2DPluginWizardSetup"
WizardTemplatesName = "C2DPluginWiz"

UtilsPath = objFSO.GetParentFolderName(WScript.ScriptFullName)
WizardPath = UtilsPath & "\" & WizardFolderName

WizardProjects2010Path = WizardPath & "\VCProjects2010"
WizardProjects2008Path = WizardPath & "\VCProjects2008"

Set objLog = objFSO.OpenTextFile( WScript.ScriptFullName & ".log", 8, True)
objLog.WriteLine("LOG START-------------------------")
objLog.WriteLine(Date() & " " & Time())
objLog.WriteLine(WScript.ScriptFullName)

' check if 'SolutionDir\utils\C2DPluginWizard' exists
If Not objFSO.FolderExists(WizardPath) Then
  objLog.WriteLine("ERROR. Missing " & WizardPath)
  Call Finish()
End If

'Re-launch this script with UAC
If WScript.Arguments.length =0 Then

  Question = "Yes - install" & vbCrLf & "No - remove"
  Answer = MsgBox(Question, vbYesNoCancel, WizardInstallerName)
  If Answer = vbCancel Then 
    objLog.WriteLine("Setup canceled by user")
    Call Finish()
  ElseIf Answer = vbYes Then
    Param = "install"
  ElseIf Answer = vbNo Then
    Param = "remove"
  End If

  objLog.WriteLine("Running without UAC")
  objApp.ShellExecute "wscript.exe", """" & _
  WScript.ScriptFullName & """ " & Param, "", "runas", 1
Else
  objLog.WriteLine("Running with UAC; Command = " & WScript.Arguments(0))
  ShouldUninstall = (WScript.Arguments(0) = "remove")
  Call FindVS()
End If

Sub FindVS()

  Keys = Array( _
  "HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\10.0\Setup\VC\ProductDir", _
  "HKLM\SOFTWARE\Wow6432Node\Microsoft\VCExpress\10.0\Setup\VC\ProductDir", _
  "HKLM\SOFTWARE\Microsoft\VisualStudio\10.0\Setup\VC\ProductDir", _
  "HKLM\SOFTWARE\Microsoft\VCExpress\10.0\Setup\VC\ProductDir")
  '"HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\9.0\Setup\VC\ProductDir", _
  '"HKLM\SOFTWARE\Wow6432Node\Microsoft\VCExpress\9.0\Setup\VC\ProductDir", _
  '"HKLM\SOFTWARE\Microsoft\VisualStudio\9.0\Setup\VC\ProductDir", _
  '"HKLM\SOFTWARE\Microsoft\VCExpress\9.0\Setup\VC\ProductDir")

  Set WSHShell = CreateObject("WScript.Shell")

  For Each Key In Keys

    On Error Resume Next
    VCPath = ""
    VCPath = WSHShell.RegRead(Key)
    objLog.WriteLine("Checking key: " & Key)
    On Error GoTo 0

    ' found VC path
    If Not VCPath = "" Then 
      objLog.WriteLine(" - FOUND: " & VCPath)

      'If InStr(VCPath, "10.0") > 0 Then 
        WizVersion = "VCProjects2010"
      'Else
      '  WizVersion = "VCProjects2008"
      'End If


      Call RunSetup(VCPath, WizVersion)
    End If

  Next
End Sub

'Call RunSetup("10.0", "VCProjects2010")
'Call RunSetup("9.0", "VCProjects2008")


Sub RunSetup(VCPath, WizVersion)

	WizardProjectsPath = WizardPath & "\" & WizVersion
	
	If objFSO.FolderExists(VCPath) Then
    objLog.WriteLine("Folder exists: " & VCPath)

		VCWizards = VCPath & "VCWizards\" & WizardLinkName

		If objFSO.FolderExists(VCPath & "Express") Then
      objLog.WriteLine("Detected VC Express edition")
			VCPath = VCPath & "Express\"
		End If

		VCProjects = VCPath & "VCProjects\" & WizardLinkName
		
				
			' wizard projects path
			Call SetupDir(VCProjects, WizardProjectsPath)
			
			' wizard templates path
			Call SetupDir(VCWizards, WizardPath)
  Else
      objLog.WriteLine("ERROR. Folder doesn't exist: " & VCPath)
	End If
End Sub


' hic sunt dracones

Const FA_REPARSE_POINT = &h400

''' Copies/Removes directory structure
Sub SetupDir(Destination, Source)
 
  If ShouldUninstall Then Operation = "Removing " Else Operation = "Creating "

  On Error Resume Next
  objLog.WriteLine(Operation & "...") 
  objLog.WriteLine("Source: " & Source) 
  objLog.WriteLine("Destination: " & Destination) 

  If ShouldUninstall Then
    If objFSO.FolderExists(Destination) Then
      IsSymbolic = (objFSO.GetFolder(Destination).Attributes And FA_REPARSE_POINT) = FA_REPARSE_POINT
      If IsSymbolic Then
        objLog.Writeline("Removing old symbolic link")
        Command = "/c rmdir """ & Destination & """"
        objApp.ShellExecute "cmd", Command, "", "", 0
      Else
        objLog.Writeline("DeleteFolder(Destination, False)")
        Call objFSO.DeleteFolder(Destination, False)
        Call CheckError()
      End If
    Else
      objLog.WriteLine("Folder doesn't exist (ignoring): " & Destination)
    End If
  Else ' installing
    If Not objFSO.FolderExists(Destination) Then
      'objLog.WriteLine("CreateFolder(Destination)")
      'Call objFSO.CreateFolder(Destination)
      'Call CheckError()
      objLog.WriteLine("CopyFolder(Source, Destination, False)")
      Call objFSO.CopyFolder(Source, Destination, False)
      Call CheckError()
    Else
      objLog.WriteLine("Already exists (ignoring): " & Destination)
    End If
  End If

  ' windows have a Kit Kat
  WScript.Sleep(1000)
	
  If objFSO.FolderExists(Destination) Xor ShouldUninstall Then  
    Result = "SUCCEEDED!" 
  Else  
    Result = "FAILED!"
  End If
    objLog.WriteLine(Operation & Location & Chr(13) & Result)

  On Error GoTo 0

End Sub

Call Finish()

Sub CheckError()
  If Err.Number <> 0 Then
    objLog.WriteLine(Err.Number & " Src: " & Err.Source & " Desc: " &  Err.Description)
    Err.Clear
  Else
   objLog.WriteLine("No errors so far...")
  End If
End Sub

Sub Finish
  objLog.WriteLine("LOG END***************************")
  objLog.Close
  Wscript.Quit
End Sub

