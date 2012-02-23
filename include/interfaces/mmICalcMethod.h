#pragma once

#include <mmCalcMethod.h>

//******************************************************************************
//******************************************************************************
//
//  Simple interface for calculation method
//
//  Description: This header defines class mmCalcKernelI for function objects
//	which implement operation on a single data block within the image and class
//	mmCalcMethodI which offers simple interface to calculation method. It is a
//	wrapper for mmImagesCalculationMethodI interface and is meant to be overloaded
//	by developer
//
//******************************************************************************
//******************************************************************************

namespace mmImages {
	////////////////////////////////////////////////////////////////////////////////
	/// Interface representing function object for operation on single data block
	////////////////////////////////////////////////////////////////////////////////
	class mmCalcKernelI
	{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// Virtual destructor
			////////////////////////////////////////////////////////////////////////////////
			virtual ~mmCalcKernelI() { }

			////////////////////////////////////////////////////////////////////////////////
			/// Method executes for every data block in the structure. Internal loop over
			/// pixels should be placed here.
			///
			/// @param[in] p_psImageStructure pointer to images structure,
			/// @param[in] p_iCurrentImageID ID number of the current processed image
			/// @param[in] p_iFirstRow index of the first row of pixels scheduled for calculation
			/// @param[in] p_iRowsCount number of rows scheduled for calculation
			////////////////////////////////////////////////////////////////////////////////
			virtual void operator()(mmImageI* p_psCurrentImage,
															mmInt p_iFirstRow,
															mmInt p_iRowsCount) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Interface representing calculation method
	////////////////////////////////////////////////////////////////////////////////
	class mmCalcMethodI: public mmCalcMethod
	{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor. It invokes the mmCalcMethod class constructor which hides 
			/// implementation details
			////////////////////////////////////////////////////////////////////////////////
			mmCalcMethodI(mmLog::mmLogReceiverI *p_psLogReceiver = NULL,
										mmString p_sClassName = L"")
										:mmCalcMethod(p_psLogReceiver, 
																	p_sClassName) { }

			////////////////////////////////////////////////////////////////////////////////
			/// Virtual destructor
			////////////////////////////////////////////////////////////////////////////////
			virtual ~mmCalcMethodI() { }

		protected:
			////////////////////////////////////////////////////////////////////////////////
			/// Method executes for every working thread. Put pixels manipulations code here
			////////////////////////////////////////////////////////////////////////////////
			virtual bool Calculate() = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method executes once for every image in the structure before the main loop
			/// iterating blocks of pixels when using ForEachImage function. Do not use 
			/// m_psThreadSynchEL for accessing class members in multithread configuration
			/// because this function runs always in a single thread.
			/// (check mmICalcMethodExt.h for documentation)
			///
			/// @param[in] p_iCurrentImageID ID number of the current processed image
			////////////////////////////////////////////////////////////////////////////////
			virtual void ExecBeforeSingleImageCalc(mmInt p_iCurrentImageID) = 0;
			
			////////////////////////////////////////////////////////////////////////////////
			/// Method executes once for every image in the structure after the main loop
			/// iterating blocks of pixels when using ForEachImage function. Do not use 
			/// m_psThreadSynchEL for accessing class members in multithread configuration
			/// because this function runs always in a single thread.
			/// (check mmICalcMethodExt.h for documentation)
			///
			/// @param[in] p_iCurrentImageID ID number of the current processed image
			////////////////////////////////////////////////////////////////////////////////
			virtual void ExecAfterSingleImageCalc(mmInt p_iCurrentImageID) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method executes once before the Calculate method which allows for setting
			/// new parameters values after their modification by the user. Place initialization
			/// code here.
			////////////////////////////////////////////////////////////////////////////////
			virtual void RetrieveParameters() = 0;
	};
};
