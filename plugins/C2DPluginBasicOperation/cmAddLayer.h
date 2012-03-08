#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// Adds a new layer when provided with image, name and default value.
	class cmAddLayer : public mmCalcMethod
	{
		public:			// methods
			cmAddLayer(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);

			virtual ~cmAddLayer() {}

		protected:	// implementation
			bool mmCalcMethod::Calculate();

		private:    // members
			mmString m_sImageName;
			mmString m_sNewLayerName;
			mmReal m_rDefaultValue;
	};
};
