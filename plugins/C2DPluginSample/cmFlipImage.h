#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// Flip image calculation method. It makes a mirrored image.
	class cmFlipImage : public mmCalcMethod
	{
		public:			// methods
			cmFlipImage(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);

			virtual ~cmFlipImage() {}

		protected: //implementation
			bool mmCalcMethod::Calculate();

		private: // members
			mmString m_sImageName;
			mmString m_sNewImageName;
			mmString m_sNewLayerName;
			bool m_bHorizontal;
			bool m_bVertical;
			bool m_bNewImage;
	};
};
