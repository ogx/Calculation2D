'/******************************************************************************
'/******************************************************************************
'/
'/  C2DPluginWizard Setup Script
'/
'/  Author: Jakub Krzes³owski
'/  Version: 2012.02.22
'/
'/  Description: Helper script integrating Calculation2D Plugin Wizard with
'/               Visual Studio 9.0 / 10.0. Creates a symbolic link in user space
'/               directory, connecting with Calculation2D SDK solution. 
'/               Once a link is created, it can be used for removing the Wizard.
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


' check if 'SolutionDir\utils\C2DPluginWizard' exists
If fso.FolderExists(WizardPath) Then
    'Wscript.Echo "Found " & WizardPath
Else
    Wscript.Echo "ERROR. Missing " & WizardPath
    Wscript.Quit
End If

VCPaths = Array( _
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC", _
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC", _
"C:\Program Files\Microsoft Visual Studio 10.0\VC", _
"C:\Program Files\Microsoft Visual Studio 9.0\VC")

Dim VisualStudioPath

' find visual studio
For Each VCPath In VCPaths
    'Wscript.Echo VCPath
    If fso.FolderExists(VCPath) Then 
        VisualStudioPath = VCPath
        Exit For
    End If
Next

If IsEmpty(VisualStudioPath) Then
    Wscript.Echo "ERROR. Missing " & VisualStudioPath
    Wscript.Quit
End If

' projects path
Call InstallerFlipFlop(VisualStudioPath & "\VCProjects\" & WizardLinkName, WizardPath)

' wizards (templates) path
Call InstallerFlipFlop(VisualStudioPath & "\VCWizards\" & WizardLinkName, WizardPath)


' hic sunt dracones

Sub InstallerFlipFlop(Location, Target)

    Uninstall = fso.FolderExists(Location)

    If Uninstall Then
        IsSymbolic = (fso.GetFolder(Location).Attributes And FA_REPARSE_POINT) = FA_REPARSE_POINT
        If IsSymbolic Then LinkOrFolder = "symbolic link" Else LinkOrFolder = "folder"
        Operation = "- found " & LinkOrFolder & ". Uninstall?"
    Else
        Operation = "- not Found. Install? " 
    End If

    Question = "Wizard installation " & Location & Chr(13) & Operation
    If Not Msgbox(Question, vbYesNo, WizardInstallerName) = vbYes Then Exit Sub

    ' create or remove link
    Call SymbolicDirLink(Location, Target, Uninstall)
End Sub

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
    If fso.FolderExists(Location) Xor Remove Then  Result = "SUCCEEDED!" Else  Result = "FAILED!"

    WScript.Echo Operation & Location & Chr(13) & Result
End Sub