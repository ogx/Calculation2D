#pragma once

#include <mmGlobalDefs.h>
#include <interfaces/mmIImages.h>
#include <mmXMLIOUtilities.h>

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

	class mmCalcKernelI;

	////////////////////////////////////////////////////////////////////////////////
	/// Interface representing calculation method
	////////////////////////////////////////////////////////////////////////////////
	class mmCalcMethodI
	{
		public:

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
			/// @param[in] p_psCurrentImage pointer to currently processed image
			////////////////////////////////////////////////////////////////////////////////
			virtual void ExecBeforeSingleImageCalc(mmImageI* p_psCurrentImage) = 0;
			
			////////////////////////////////////////////////////////////////////////////////
			/// Method executes once for every image in the structure after the main loop
			/// iterating blocks of pixels when using ForEachImage function. Do not use 
			/// m_psThreadSynchEL for accessing class members in multithread configuration
			/// because this function runs always in a single thread.
			/// (check mmICalcMethodExt.h for documentation)
			///
			/// @param[in] p_psCurrentImage pointer to currently processed image
			////////////////////////////////////////////////////////////////////////////////
			virtual void ExecAfterSingleImageCalc(mmImageI* p_psCurrentImage) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method executes once before the Calculate method which allows for setting
			/// new parameters values after their modification by the user. Place initialization
			/// code here.
			////////////////////////////////////////////////////////////////////////////////
			virtual void RetrieveParameters() = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method iterates through data structure in order to perform specified operation
			/// on every data block. It works in multithread environment and may be executed
			/// by multiple threads to manipulate on the same images structure. It should be
			///	used inside Calculate function (see mmICalcMethod.h for documentation)
			///
			/// @param[in] p_psKernel pointer to function object which implements operation
			///												executed for every data block in every image
			////////////////////////////////////////////////////////////////////////////////
			virtual void ForEachImage(mmCalcKernelI* p_psKernel) = 0;
			
			////////////////////////////////////////////////////////////////////////////////
			/// Method adds new user interface parameter for calculation method 
			///
			/// @param[in] p_sName parameter name which appears as a label in the parameters
			///										 dialog box
			/// @param[in] p_eType parameter type (see mmXMLIOUtilities.h for documentation)
			/// @param[in] p_psValue pointer to variable containing default parameter value
			/// @param[in] p_bIsOutput flag indicating if the new parameter is input or
			///												 output for the calculation method
			////////////////////////////////////////////////////////////////////////////////
			virtual void SetParam(mmString p_sName, mmXML::mmXMLDataType p_eType, void* p_psValue, bool p_bIsOutput = false) = 0;
			
			////////////////////////////////////////////////////////////////////////////////
			/// Method retrieves information about parameter value.
			///
			/// @param[in] p_sName parameter name which appears as a label in the parameters
			///										 dialog box
			///	@return pointer to parameter value. It should be assigned to a pointer of 
			///					the specific type
			////////////////////////////////////////////////////////////////////////////////
			virtual const void* GetParam(mmString p_sName) = 0;
			
			////////////////////////////////////////////////////////////////////////////////
			/// Method retrieves information about parameter value.
			///
			/// @param[in] p_sName parameter name which appears as a label in the parameters
			///										 dialog box
			/// @param[out] p_pValue pointer to a variable where new parameter value should
			///											 be stored. It should have proper type.
			////////////////////////////////////////////////////////////////////////////////
			virtual void GetParam(mmString p_sName, void* p_pValue) = 0;
			
			////////////////////////////////////////////////////////////////////////////////
			/// Method creates internal parameters structure. It should be called after all
			///	SetParam invocations
			////////////////////////////////////////////////////////////////////////////////
			virtual void UpdateParameters() = 0;
			
			////////////////////////////////////////////////////////////////////////////////
			/// Method returns names of all images in the images structure
			///
			///	@return vector of image names
			////////////////////////////////////////////////////////////////////////////////
			virtual std::vector<mmString> GetImageNames() = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method returns names of all data layers of specified image
			///
			/// @param [in] p_iImage image ID
			///	@return vector of image names
			////////////////////////////////////////////////////////////////////////////////
			virtual std::vector<mmString> GetDLNames(mmUInt const p_iImage) = 0;
	};

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
		/// @param[in] p_psCurrentImage pointer to currently processed image,
		/// @param[in] p_iFirstRow index of the first row of pixels scheduled for calculation
		/// @param[in] p_iRowsCount number of rows scheduled for calculation
		////////////////////////////////////////////////////////////////////////////////
		virtual void operator()(mmImageI* p_psCurrentImage, mmInt p_iFirstRow, mmInt p_iRowsCount) = 0;
	};

};
