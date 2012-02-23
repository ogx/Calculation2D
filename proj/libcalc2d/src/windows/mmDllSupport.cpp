#include <windows\mmDllSupport.h>

#include <windows.h>
#include <Imagehlp.h>
#pragma comment ( lib, "Imagehlp.lib" )

#include <mmStringUtilities.h>


mmString mmDLLSupport::FindSymbolInDLLExportTable(mmString p_sDLLName,
																									mmString p_sName)
{
	mmString v_sFullSymbol;

	LOADED_IMAGE v_sLoadedImage;

	std::string v_sCharStringDllName = mmStringUtilities::MMStringToCharString(p_sDLLName);
	std::string v_sCharStringName = mmStringUtilities::MMStringToCharString(p_sName);

	if(MapAndLoad(const_cast<char*>(v_sCharStringDllName.c_str()),
								"",
								&v_sLoadedImage,
								TRUE,
								TRUE))
	{
		PIMAGE_EXPORT_DIRECTORY v_sExpDir;
		v_sExpDir = (PIMAGE_EXPORT_DIRECTORY)(v_sLoadedImage.FileHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
		if(v_sExpDir != NULL)
		{
			v_sExpDir = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa(v_sLoadedImage.FileHeader, v_sLoadedImage.MappedAddress, (DWORD)v_sExpDir, 0);

			//PDWORD* v_pExpNames = (PDWORD *)ImageRvaToVa(v_sLoadedImage.FileHeader, v_sLoadedImage.MappedAddress, v_sExpDir->AddressOfNames, 0);
			PDWORD v_pExpNames = (PDWORD )ImageRvaToVa(v_sLoadedImage.FileHeader, v_sLoadedImage.MappedAddress, v_sExpDir->AddressOfNames, 0);

			mmInt v_iTemp;
			mmInt v_iExportSymbolCount = v_sExpDir->NumberOfNames;
			for (v_iTemp=0;v_iTemp<v_iExportSymbolCount;v_iTemp++)
			{
				PSTR v_psSymbolName = (PSTR)ImageRvaToVa(v_sLoadedImage.FileHeader, v_sLoadedImage.MappedAddress, (DWORD)*v_pExpNames, 0);
				if (v_psSymbolName != NULL) {

					std::string v_sSymbolName(v_psSymbolName);

					if(v_sSymbolName.find(v_sCharStringName) < v_sSymbolName.size())
					{
						v_sFullSymbol = mmStringUtilities::MMCharStringToMMString(v_sSymbolName);
						break;
					};
				}

				v_pExpNames++;
			};
		};
	};

	UnMapAndLoad(&v_sLoadedImage);

	return v_sFullSymbol;
}
