#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// Flip image calculation method. It makes a mirrored image.
	class cmAddLayer : public mmCalcMethod
	{
		public:			// methods
			cmAddLayer(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);

			virtual ~cmAddLayer() {}

		protected:	// mmCalcMethodI implementation
			bool mmCalcMethod::Calculate();
			//void mmCalcMethod::RetrieveParameters();

		private:

			// members

			mmString m_sImageName;
			mmString m_sNewLayerName;
			mmReal m_rDefaultValue;

	};
};
