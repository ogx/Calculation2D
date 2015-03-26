'/******************************************************************************
'/******************************************************************************
'/
'/  C2DPluginWizard Setup Script
'/
'/  Description: Helper script integrating Calculation2D Plugin Wizard with
'/               Visual Studio 2013. Copies wizard files to Visual C++ Wizards
'/               directory from Calculation2D SDK solution. 
'/
'/******************************************************************************
'/******************************************************************************

function FindAndReplace(strFilename, strFind, strReplace)
    Set inputFile = CreateObject("Scripting.FileSystemObject").OpenTextFile(strFilename, 1)
    strInputFile = inputFile.ReadAll
    inputFile.Close
    Set inputFile = Nothing
    Set outputFile = CreateObject("Scripting.FileSystemObject").OpenTextFile(strFilename,2,true)
    outputFile.Write Replace(strInputFile, strFind, strReplace)
    outputFile.Close
    Set outputFile = Nothing
end function 

Dim ws, strCurDir, fso
Set fso = CreateObject("Scripting.FileSystemObject")
Set ws = CreateObject("WScript.Shell")
home = ws.ExpandEnvironmentStrings("%USERPROFILE%")

curDir    = ws.CurrentDirectory
'ws.Run strCurDir & "\attribute.exe", 0


Set re = New RegExp
re.Pattern =  chr(34) & "ABSOLUTE_PATH = ([^" & chr(34) & "])+" & chr(34)

If Not fso.FolderExists(home & "\Documents\Visual Studio 2013\Wizards\") Then
  fso.CreateFolder home & "\Documents\Visual Studio 2013\Wizards\"
End If
fso.CopyFolder curDir & "\C2DPluginWizard\C2DPluginWiz", home & "\Documents\Visual Studio 2013\Wizards\C2D"

FindAndReplace home & "\Documents\Visual Studio 2013\Wizards\C2D\C2DPluginWiz.vsz", "C:\workspace\C2D\C2DPluginWizard", curDir & "\C2DPluginWizard"

WScript.Echo "Device Wizard for MS Visual Studio 2013 installed successfully."
