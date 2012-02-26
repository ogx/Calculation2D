#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// [!output CALCMETHOD_DESCRIPTION]
	class [!output CALCMETHOD_SAFE_NAME] : public mmCalcMethod
	{
		public:			// methods
			[!output CALCMETHOD_SAFE_NAME](mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			virtual ~[!output CALCMETHOD_SAFE_NAME]() {}

		protected:	// mmCalcMethodI implementation
			bool mmCalcMethodI::Calculate();
			void mmCalcMethodI::RetrieveParameters();

		private:

		// TODO: add members

	};
};
