'/******************************************************************************
'/******************************************************************************
'/
'/  C2DPluginWizard Setup Script
'/
'/  Description: Helper script integrating Calculation2D Plugin Wizard with
'/               Visual Studio 9.0 / 10.0. Creates a symbolic link in user space
'/               directory, connecting with Calculation2D SDK solution. 
'/               Once a link is created, this script can be used for removing 
'/               the Wizard.
'/
'/******************************************************************************
'/******************************************************************************

' create objects
Set fso = CreateObject("Scripting.FileSystemObject")
Set sh = CreateObject("Shell.Application")
Set WsShell = Wscript.CreateObject("Wscript.Shell")


' setup default paths
WizardLinkName = "C2D"
WizardFolderName = "C2DPluginWizard"
WizardInstallerName = "C2DPluginWizardSetup"

UtilsPath = fso.GetParentFolderName(WScript.ScriptFullName)
WizardPath = UtilsPath & "\" & WizardFolderName

WizardProjects2010Path = WizardPath & "\VCProjects2010"
WizardProjects2008Path = WizardPath & "\VCProjects2008"


' check if 'SolutionDir\utils\C2DPluginWizard' exists
If Not fso.FolderExists(WizardPath) Then
    Wscript.Echo "ERROR. Missing " & WizardPath
    Wscript.Quit
End If

' x64 or x86 version?
ProgramFilesPath = "C:\Program Files (x86)"
If Not fso.FolderExists(ProgramFilesPath) Then
	ProgramFilesPath = "C:\Program Files"
End If

VSName = "Microsoft Visual Studio"

Call RunSetup("10.0", "VCProjects2010")
Call RunSetup("9.0", "VCProjects2008")

Sub RunSetup(VSVersion, WizardProjectsFolder)

	VCFoler = VSName & " " & VSVersion
	VCPath = ProgramFilesPath & "\" & VCFoler & "\VC"
	
	WizardProjectsPath = WizardPath & "\" & WizardProjectsFolder
	
	If fso.FolderExists(VCPath) Then
		If fso.FolderExists(VCPath & "\Express") Then
			VCProjects = VCPath & "\Express\VCProjects\" & WizardLinkName
		Else
			VCProjects = VCPath & "\VCProjects\" & WizardLinkName
		End If
		
		VCWizards = VCPath & "\VCWizards\" & WizardLinkName
		
		' uninstall if it exists
		Uninstall = fso.FolderExists(VCProjects)
		
		If Uninstall Then Operation = "Uninstall" Else Operation = "Install"
		
		Question = Operation & " " & WizardFolderName & " for " & VCFoler & "?"
		UserAccepted = (Msgbox(Question, vbYesNo, WizardInstallerName) = vbYes)
		
		If UserAccepted Then
		
			' wizard projects path
			Call SymbolicDirLink(VCProjects, WizardProjectsPath, Uninstall)
			
			' wizard templates path
			Call SymbolicDirLink(VCWizards, WizardPath, Uninstall)
			
		End If
	End If
End Sub


' hic sunt dracones

''' SymbolicDirLink - creates or removes specified symbolic link
Sub SymbolicDirLink(Location, Target, Remove)

    If Remove Then
        Command = "/c rmdir """ & Location & """"
    Else
        Command = " /c mklink /D """ & Location & """ """ & Target & """"
    End If

    ' run command with UAC
    'WScript.Echo Command
    sh.ShellExecute "cmd", Command, "", "runas", 0
    
    ' windows have a Kit Kat
    WScript.Sleep(1000)

    If Remove Then Operation = "Removing " Else Operation = "Creating "
	
    If fso.FolderExists(Location) Xor Remove Then  
      Result = "SUCCEEDED!" 
    Else  
      Result = "FAILED!"
		WScript.Echo Operation & Location & Chr(13) & Result
    End If
End Sub