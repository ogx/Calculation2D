#include <interfaces/mmIHardware.h>

mmInt mmImageProcessing::GetPixelSize(mmImageProcessing::ePixelType p_ePixelType)
{
	mmInt v_iResult;

	switch(p_ePixelType)
	{
		case mmImageProcessing::pixel_R8G8B8:
		{
			v_iResult = 3;
		};
		break;
		case mmImageProcessing::pixel_B8G8R8:
		{
			v_iResult = 3;
		};
		break;
		case mmImageProcessing::pixel_Gray8:
		{
			v_iResult = 1;
		};
		break;
		case mmImageProcessing::pixel_Gray16:
		{
			v_iResult = 2;
		};
		break;
		case mmImageProcessing::pixel_B8G8R8A8:
		{
			v_iResult = 4;
		};
		break;
		case mmImageProcessing::pixel_R8G8B8A8:
		{
			v_iResult = 4;
		};
		break;
		case mmImageProcessing::pixel_R16G16B16:
		{
			v_iResult = 6;
		};
		break;
		default:
		{
			throw mmError(mmeImageProcessingUnknownPixelType);
		};
	};

	return v_iResult;
}
