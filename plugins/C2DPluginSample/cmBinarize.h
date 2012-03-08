#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// Flip image calculation method. It makes a mirrored image.
	class cmBinarize : public mmCalcMethod
	{
		public:			// methods
			cmBinarize(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);

			virtual ~cmBinarize() {}

		protected: //implementation
			bool mmCalcMethod::Calculate();

		private: // members
			mmString m_sLayerName;
			mmString m_sImageName;
			mmReal m_rThreshold;
	};
};
