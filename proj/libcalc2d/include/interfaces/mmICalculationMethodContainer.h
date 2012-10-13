#ifndef mmICalculationMethodContainerH
#define mmICalculationMethodContainerH

#include <vector>
#include <map>

#include <interfaces\mmIImages.h>

namespace mmImages {
	////////////////////////////////////////////////////////////////////////////////
	/// Images calculation method container interface. It is responsible for
	/// searching all available calculation methods, querying them. It also initializes
	/// calculation method objects and return pointers to them.
	////////////////////////////////////////////////////////////////////////////////
	class mmImagesCalculationMethodContainerI
	{
		public:			// methods
			////////////////////////////////////////////////////////////////////////////////
			/// Virtual destructor.
			////////////////////////////////////////////////////////////////////////////////
			virtual ~mmImagesCalculationMethodContainerI() {};

			////////////////////////////////////////////////////////////////////////////////
			/// Returns list with available images calculation methods.
			///
			/// @return vector with mmImages::mmImagesCalculationMethodI::sCalculationMethodParams
			///				  structures defining available exporters and their functionality.
			////////////////////////////////////////////////////////////////////////////////
			virtual std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> GetAvailableImagesCalculationMethods(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Returns mapping MethodID -> DLL file name.
			///
			/// @return mapping MethodID -> DLL file name
			////////////////////////////////////////////////////////////////////////////////
			virtual std::map<mmString, mmString> GetMethodFileMapping(void) const = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// Initializes selected images calculation method and returns pointer
			/// into its interface. In case of error it throws
			/// mmError(mmeImagesStructureUnknownImagesCalculationMethod).
			///
			/// @param[in] p_sCalculationMethodName name of images calculation method to initialize,
			/// @return pointer to initialized mmImages::mmImagesCalculationMethodI object.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmImages::mmImagesCalculationMethodI* InitializeImagesCalculationMethod(mmString const & p_sCalculationMethodName) = 0;
	};
};

#endif