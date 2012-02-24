#include <mmStringUtilities.h>

#include <stdio.h>

#include <locale>

mmString mmStringUtilities::BoolToString(bool p_bInput)
{
	if(p_bInput)
	{
		return mmString(L"true");
	};

	return mmString(L"false");
}

mmString mmStringUtilities::MMIntToString(mmInt p_iInput,
																					mmInt p_iLength)
{
	wchar_t v_tcTemp[256];

	if(p_iLength == 0)
	{
		swprintf_s(v_tcTemp,L"%d",p_iInput);
	}
	else
	{
		wchar_t v_tcTempFormat[16];

		swprintf_s(v_tcTempFormat,L"%s0%dd",L"%",p_iLength);
		swprintf_s(v_tcTemp,v_tcTempFormat,p_iInput);
	};

	return mmString(v_tcTemp);
}

mmString mmStringUtilities::MMRealToString(mmReal p_rInput,mmInt p_iPrecision)
{
	wchar_t v_tcTemp[256];
	wchar_t v_tcTempFormat[16];

	if(p_iPrecision >= 0)
	{
		swprintf_s(v_tcTempFormat,L"%s.%df",L"%",p_iPrecision);
	}
	else
	{
		swprintf_s(v_tcTempFormat,L"%sf",L"%");
	};

	swprintf_s(v_tcTemp,v_tcTempFormat,p_rInput);

	return mmString(v_tcTemp);
}

mmString mmStringUtilities::PointerToString(void* p_pInput)
{
	wchar_t v_tcTemp[256];

	swprintf_s(v_tcTemp,L"%p",p_pInput);

	return mmString(v_tcTemp);
}

mmInt mmStringUtilities::StringToMMInt(mmString p_sStr)
{
	int v_iTemp;

	swscanf_s(p_sStr.c_str(),L"%d",&v_iTemp);

	return v_iTemp;
}

mmReal mmStringUtilities::StringToMMReal(mmString p_sStr)
{
	double v_rTemp;

	swscanf_s(p_sStr.c_str(),L"%lf",&v_rTemp);

	return v_rTemp;
}

bool mmStringUtilities::StringToBool(mmString p_sStr)
{
	if( ( (p_sStr[0] == 't') ||
				(p_sStr[0] == 'T') ) &&
			( (p_sStr[1] == 'r') ||
				(p_sStr[1] == 'R') ) &&
			( (p_sStr[2] == 'u') ||
				(p_sStr[2] == 'U') ) &&
			( (p_sStr[3] == 'e') ||
				(p_sStr[3] == 'E') ) )
	{
		return true;
	};

	return false;
}

/*
void mmStringUtilities::StringToUnsignedCharTable(std::string* p_psStr,unsigned char* p_psBuffer,mmInt* p_piReadCount)
{
	mmInt v_i = 0;
	mmInt v_iActReadByte = 0;
	mmInt v_iStringSize = static_cast<mmInt>(p_psStr->size());
	mmInt v_iLo,v_iHi;

	while(v_i<v_iStringSize)
	{
		if( ((*p_psStr)[v_i] == 32) || ((*p_psStr)[v_i] == 9) )
		{
			v_i++;
		}
		else
		{
			v_i += 2;
			v_iHi = (*p_psStr)[v_i];
			v_i++;
			v_iLo = (*p_psStr)[v_i];
			v_i++;

			if(v_iHi < 58)
			{
				// 0-9
				v_iHi -= 48;
			}
			else
			{
				if(v_iHi < 71)
				{
					// A-F
					v_iHi -= 55;
				}
				else
				{
					// a-f
					v_iHi -= 87;
				};
			};

			if(v_iLo < 58)
			{
				// 0-9
				v_iLo -= 48;
			}
			else
			{
				if(v_iLo < 71)
				{
					// A-F
					v_iLo -= 55;
				}
				else
				{
					// a-f
					v_iLo -= 87;
				};
			};

			p_psBuffer[v_iActReadByte] = v_iHi*16+v_iLo;
			v_iActReadByte++;
		};

		if(v_iActReadByte == *p_piReadCount)
		{
			break;
		};
	};

	if(v_iActReadByte < *p_piReadCount)
	{
		*p_piReadCount = v_iActReadByte;
		throw mmError(mmeInputStringIsTooSmall);
	};
}
*/

mmString mmStringUtilities::GetCurrentDateTimeString(void)
{
	std::tm v_sDateTimeStruc;
	wchar_t v_tcTemp[64];

	std::time_t const v_sCurrentDateTime = std::time(NULL);
	::localtime_s(&v_sDateTimeStruc, &v_sCurrentDateTime);

	swprintf_s(v_tcTemp,L"%04d%02d%02d_%02d%02d%02d",v_sDateTimeStruc.tm_year+1900,
																								 v_sDateTimeStruc.tm_mon+1,
																								 v_sDateTimeStruc.tm_mday,
																								 v_sDateTimeStruc.tm_hour,
																								 v_sDateTimeStruc.tm_min,
																								 v_sDateTimeStruc.tm_sec);

	return mmString(v_tcTemp);
}

mmString mmStringUtilities::MMIntToHHMMSSString(mmInt p_iInput)
{
	mmInt v_iHours = p_iInput/3600;
	mmInt v_iMinutes = (p_iInput - v_iHours*3600)/60;
	mmInt v_iSeconds = p_iInput - v_iHours*3600 - v_iMinutes*60;

	mmString v_sOutString = MMIntToString(v_iHours) +
													mmString(L"h") +
													MMIntToString(v_iMinutes,2) +
													mmString(L"m") +
													MMIntToString(v_iSeconds,2) +
													mmString(L"s");

	return v_sOutString;
}

mmString mmStringUtilities::MMCharStringToMMString(const std::string p_sInputString)
{
	std::wostringstream v_sStream;

	const std::ctype<wchar_t>& v_sCtfacet = std::use_facet< std::ctype<wchar_t> >(v_sStream.getloc());

	mmInt v_iCSize = p_sInputString.size();
	for(mmInt v_iC=0;v_iC<v_iCSize;v_iC++)
	{
		v_sStream << v_sCtfacet.widen(p_sInputString[v_iC]);
	};

	return v_sStream.str();
}

std::string mmStringUtilities::MMStringToCharString(const mmString p_sInputString)
{
	std::ostringstream v_sStream;

	const std::ctype<char>& v_sCtfacet = std::use_facet< std::ctype<char> >(v_sStream.getloc());

	mmInt v_iCSize = p_sInputString.size();
	for(mmInt v_iC=0;v_iC<v_iCSize;v_iC++)
	{
		v_sStream << v_sCtfacet.narrow(static_cast<std::ctype<char>::_Elem>(p_sInputString[v_iC]),0);
	};

	return v_sStream.str();
}

std::vector<mmInt> mmStringUtilities::MMStringToVectorOfInt(const mmString p_sInputString)
{
	std::vector<mmInt> v_vOutVector;

	mmInt v_iInt,v_iStart,v_iStop;
	mmInt v_iLen = p_sInputString.size();

	v_iStart = 0;
	v_iStop = -1;
	while(v_iStop < v_iLen)
	{
		v_iStop++;
		v_iStart = v_iStop;
		while(v_iStop < v_iLen)
		{
			if(p_sInputString[v_iStop] != L' ')
			{
				v_iStop++;
			}
			else
			{
				break;
			};
		};

		if(v_iStop <= v_iLen)
		{
			mmString v_sTempInt = p_sInputString.substr(v_iStart,v_iStop-v_iStart);

			swscanf_s(v_sTempInt.c_str(),L"%d",&v_iInt);
			v_vOutVector.push_back(v_iInt);
		};
	};

	return v_vOutVector;
}

std::vector<mmString> mmStringUtilities::MMStringToVectorOfMMString(const mmString p_sInputString)
{
	std::vector<mmString> v_vOutVector;

	mmInt v_iStart,v_iStop;
	mmInt v_iLen = p_sInputString.size();

	v_iStart = 0;
	v_iStop = -1;
	while(v_iStop < v_iLen)
	{
		v_iStop++;
		v_iStart = v_iStop;
		while(v_iStop < v_iLen)
		{
			if(p_sInputString[v_iStop] != L' ')
			{
				v_iStop++;
			}
			else
			{
				break;
			};
		};

		if(v_iStop <= v_iLen)
		{
			v_vOutVector.push_back(p_sInputString.substr(v_iStart,v_iStop-v_iStart));
		};
	};

	return v_vOutVector;
}

mmString mmStringUtilities::MMStringToUpper(const mmString p_sInputString)
{
	mmString v_sOutString = p_sInputString;

	mmInt v_iCSize = p_sInputString.size();
	for(mmInt v_iC=0;v_iC<v_iCSize;v_iC++)
	{
		v_sOutString[v_iC] = towupper(p_sInputString[v_iC]);
	};

	return v_sOutString;
}

mmString mmStringUtilities::PixelFormatToString(mmImageProcessing::ePixelType p_ePixelType)
{
	mmString v_sResult;

	switch(p_ePixelType) {
		case mmImageProcessing::pixel_R8G8B8:
			v_sResult = L"24bit Red(8 bits)Green(8 bits)Blue(8 bits)";
			break;
		case mmImageProcessing::pixel_B8G8R8:
			v_sResult = L"24bit Blue(8 bits)Green(8 bits)Red(8 bits)";
			break;
		case mmImageProcessing::pixel_R8G8B8A8:
			v_sResult = L"32bit Red(8 bits)Green(8 bits)Blue(8 bits)Alpha(8 bits)";
			break;
		case mmImageProcessing::pixel_B8G8R8A8:
			v_sResult = L"32bit Blue(8 bits)Green(8 bits)Red(8 bits)Alpha(8 bits)";
			break;
		case mmImageProcessing::pixel_Gray8:
			v_sResult = L"8bit Intensity(8 bits)";
			break;
		case mmImageProcessing::pixel_Gray16:
			v_sResult = L"16bit Intensity(16 bits)";
			break;
		case mmImageProcessing::pixel_R16G16B16:
			v_sResult = L"48bit Red(16 bits)Green(16 bits)Blue(16 bits)";
			break;
		default:
			v_sResult = L"Invalid pixel type";
	};
	return v_sResult;
}

mmImageProcessing::ePixelType mmStringUtilities::StringToPixelFormat(mmString p_sStr)
{
	if (p_sStr.compare(L"24bit Red(8 bits)Green(8 bits)Blue(8 bits)") == 0)
		return mmImageProcessing::pixel_R8G8B8;
	else if (p_sStr.compare(L"24bit Blue(8 bits)Green(8 bits)Red(8 bits)") == 0)
		return mmImageProcessing::pixel_B8G8R8;
	else if (p_sStr.compare(L"32bit Red(8 bits)Green(8 bits)Blue(8 bits)Alpha(8 bits)") == 0)
		return mmImageProcessing::pixel_R8G8B8A8;
	else if (p_sStr.compare(L"32bit Blue(8 bits)Green(8 bits)Red(8 bits)Alpha(8 bits)") == 0)
		return mmImageProcessing::pixel_B8G8R8A8;
	else if (p_sStr.compare(L"8bit Intensity(8 bits)") == 0)
		return mmImageProcessing::pixel_Gray8;
	else if (p_sStr.compare(L"16bit Intensity(16 bits)") == 0)
		return mmImageProcessing::pixel_Gray16;
	else if (p_sStr.compare(L"48bit Red(16 bits)Green(16 bits)Blue(16 bits)") == 0)
		return mmImageProcessing::pixel_R16G16B16;
	else return mmImageProcessing::pixel_AnyType;
}
