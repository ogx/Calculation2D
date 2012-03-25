#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// Flip image calculation method. It makes a mirrored image.
	class cmLayerTransport : public mmCalcMethod
	{
		public:			// methods
			cmLayerTransport(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);

			virtual ~cmLayerTransport() {}

		protected: //implementation
			bool mmCalcMethod::Calculate();

		private: // members
			mmImages::mmLayerI::sID m_sInputLayer1;
			mmImages::mmLayerI::sID m_sInputLayer2;
			mmID m_sOutputImage;
			mmImages::mmLayerI::sID m_sOutputLayer;
	};
};
