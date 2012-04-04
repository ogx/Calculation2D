#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// Flip image calculation method. It makes a mirrored image.
	class cmExceptionHandling : public mmCalcMethod
	{
		public:			// methods
			cmExceptionHandling(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);

			virtual ~cmExceptionHandling() {}

		protected: //implementation
			bool mmCalcMethod::Calculate();

		private: // members
	};
};
