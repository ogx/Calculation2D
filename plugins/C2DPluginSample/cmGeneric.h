#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// Generic calculation method. It does nothing and serves as a sample only.
	class cmGeneric : public mmCalcMethod
	{
		public:			// methods
			cmGeneric(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			virtual ~cmGeneric() {}

		protected:	// mmCalcMethodI implementation
			bool mmCalcMethod::Calculate();
			//void mmCalcMethodI::RetrieveParameters();

		private:

		// TODO: add members

	};
};
