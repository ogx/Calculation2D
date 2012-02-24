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

WizardLinkName = "C2D"
WizardFolderName = "C2DPluginWizard"

' setup default paths
UtilsPath = fso.GetParentFolderName(WScript.ScriptFullName)
C2DWizardPath = UtilsPath & "\" & WizardFolderName

' check if 'utils\C2DPluginWizard' exists
If fso.FolderExists(C2DWizardPath) Then
    Wscript.Echo "Found " & C2DWizardPath
Else
    Wscript.Echo "ERROR. Missing " & C2DWizardPath
    Wscript.Quit
End If

Documents = WsShell.SpecialFolders("MyDocuments")
VSUserPath = Documents + "\Visual Studio 2010"

' check if 'C:\Users\student\Visual Studio 2010' exists
If fso.FolderExists(VSUserPath) Then
    Wscript.Echo "Found " & VSUserPath
Else
    Wscript.Echo "ERROR. Missing " & VSUserPath
    Wscript.Quit
End If

VSWizardsPath = VSUserPath & "\Wizards"


If fso.FolderExists(VSWizardsPath) Then
    ' found wizards folder
    LinkTarget = VSWizardsPath & "\" & WizardLinkName
    IsSymbolic = (fso.GetFolder(LinkTarget).Attributes And FA_REPARSE_POINT) = FA_REPARSE_POINT

    If IsSymbolic Then LinkOrFolder = "symbolic link" Else LinkOrFolder = "folder"

    ' confirm removal
    If Msgbox("The " & LinkOrFolder & " already exists: " & LinkTarget & ". Uninstall?",VBYesNo,"Remove confirmation") = vbYes Then
        sh.ShellExecute "cmd", "/c rmdir " & LinkTarget, "", "", 0
    End If

    WScript.Quit
Else
    
End If

'If Msgbox("WARN. The " & LinkOrFolder & " already exists: " & LinkTarget & ". Remove?",VBYesNo,"Remove confirmation") = vbYes Then
        
'End If




'If fso.FileExists(CalPath & "\ogxCalibration.sln") Then
'    WScript.Echo "OK. ogxCalibration.sln file found."
'Else
'    Set Folder = sh.BrowseForFolder(0, "Select destination folder (OGXCalibration)", 0, "")
'    
'    If Folder Is Nothing Then
'        WScript.Quit
'    End If
'    
'    CalPath = Folder.Self.Path
'    
'    If fso.FileExists(CalPath & "\ogxCalibration.sln") Then
'        WScript.Echo "OK. ogxCalibration.sln file found."
'    Else
'        WScript.Echo "WARN. ogxCalibration.sln file not found."    
'    End If
'    
'End If

'' link target full path
'LinkTarget = CalPath & "\" & MadmacLinkName

'' check if already exists
'If fso.FolderExists(LinkTarget) Then
'    IsSymbolic = (fso.GetFolder(LinkTarget).Attributes And FA_REPARSE_POINT) = FA_REPARSE_POINT
'    If IsSymbolic Then LinkOrFolder = "symbolic link" Else LinkOrFolder = "folder"
'    If Msgbox("WARN. The " & LinkOrFolder & " already exists: " & LinkTarget & ". Remove?",VBYesNo,"Remove confirmation") = vbYes Then
'        sh.ShellExecute "cmd", "/c rmdir " & LinkTarget, "", "", 0
'    Else
'        WScript.Quit
'    End If
'End If

'' find link destination
'Set Folder = sh.BrowseForFolder(0, "Select 3dmadmac_core folder", 0, "")
'If Folder Is Nothing Then
'    WScript.Quit
'End If

'MadmacPath = Folder.Self.Path

'If fso.FileExists(MadmacPath & "\3dmadmac.sln") Then
'    WScript.Echo "OK. 3dmadmac.sln file found."
'Else
'    WScript.Echo "WARN. 3dmadmac.sln file not found."    
'End If
'    
''create link with UAC
'Command = " /c mklink /D """ & LinkTarget & """ """ & MadmacPath & """"
''WScript.Echo Command
'sh.ShellExecute "cmd",  Command, "", "runas", 0

'' windows have a Kit Kat
'WScript.Sleep(1000)

'If fso.FolderExists(LinkTarget) Then
'    WScript.Echo "SUCCESS! " & LinkTarget & " created."
'Else
'    WScript.Echo "FAIL! " & LinkTarget & " not created."
'End If