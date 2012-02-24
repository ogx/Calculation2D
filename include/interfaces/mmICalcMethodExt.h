#pragma once

#include <interfaces/mmICalcMethod.h>
#include <interfaces/mmIImages.h>
#include <mmXMLIOUtilities.h>

//******************************************************************************
//******************************************************************************
//
//  Extension for calculation method interface
//
//  Description: This header defines class mmCalcMethodExtI which extends
//	calculation method interface in order to simplify usage. It contains a set 
//	of tool functions for parameters manipulation and iteration through data
//	structure in multithread environment. These functions are meant to be used 
//	by developer rather than overload.
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
	/// Interface representing interface extension for calculation method
	////////////////////////////////////////////////////////////////////////////////
	class mmCalcMethodExtI: public mmCalcMethodI
	{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// Virtual destructor
			////////////////////////////////////////////////////////////////////////////////
			~mmCalcMethodExtI() { }

		protected:	// methods
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

		protected:	// fields
			////////////////////////////////////////////////////////////////////////////////
			/// Calculation method parameters. Developer should assign the method name, ID as
			///	a GUID and indicate if the method should be executed by multiple threads (default)
			////////////////////////////////////////////////////////////////////////////////
			mmImages::mmImagesCalculationMethodI::sCalculationMethodParams m_sCMParams;
			
			////////////////////////////////////////////////////////////////////////////////
			/// Pointer to images structure. Use to access all loaded data.
			////////////////////////////////////////////////////////////////////////////////
			mmImages::mmImageStructureI* m_psImageStructure;

			////////////////////////////////////////////////////////////////////////////////
			/// Pointer exclusive lock object. Use to control access to class members in
			/// multithread implementation. 
			////////////////////////////////////////////////////////////////////////////////
			std::tr1::shared_ptr<mmSynchronize::mmExclusiveLockI> m_psThreadSynchEL;

			////////////////////////////////////////////////////////////////////////////////
			/// Number of rows in single data block. Modify to control amount of data processed
			/// by a single thread. The last block may be smaller, depending on image size,
			/// because it is a reminder of integer division of image height.
			////////////////////////////////////////////////////////////////////////////////
			mmInt m_iRowsCountInBlock;
	};
};
