#pragma once

#include <plugin/mmCalcMethod.h>

namespace mmImages {

	// [!output CALCMETHOD_DESCRIPTION]
	class [!output CALCMETHOD_SAFE_NAME] : public mmCalcMethod
		[!if CALCMETHOD_EXTEND]
		, private mmCalcKernelI
		[!endif]
	{
		public:			// methods
			[!output CALCMETHOD_SAFE_NAME](mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
			virtual ~[!output CALCMETHOD_SAFE_NAME]() {}

		protected:
			// mmCalcMethod implementation
			bool mmCalcMethod::Calculate();

			[!if CALCMETHOD_EXTEND]
			void OnBeforeEachImage(mmImageI* p_psCurrentImage);
			void OnAfterEachImage(mmImageI* p_psCurrentImage);

			// mmCalcKernelI implementation
			void mmCalcKernelI::operator()(mmImageI* p_psCurrentImage, mmInt p_iFirstRow, mmInt p_iRowsCount);
			[!endif]

		private:

		// TODO: add members

	};
};
