function VerifyCalculation2DDir() {
  var bValid = true;
  var strProjectPath = window.external.FindSymbol('PROJECT_PATH');

  var fso = new ActiveXObject("Scripting.FileSystemObject");
  var strNewProjectPath = fso.GetParentFolderName(strProjectPath);

  var strPluginsDir = window.external.FindSymbol('PLUGINS_DIR');
  var strTestPluginsPath = strNewProjectPath + "\\" + strPluginsDir;

  if (!fso.FolderExists(strTestPluginsPath)) {
    window.external.ReportError(strTestPluginsPath + " - location not found. Please cancel and choose a valid Calculation 2D SDK directory!");
    bValid = false;
  }

  return bValid;
}