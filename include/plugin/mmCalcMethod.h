#pragma once

#include <log/mmLogSender.h>
#include <mmXMLIOUtilities.h>
#include <map>

//******************************************************************************
//******************************************************************************
//
//  Implementation of simple interface for calculation method
//
//  Description: This header defines class mmCalcMethod which implements simple
//	interface for calculation method, alternative from mmImagesCalculationMethodI.
//	The calculation method which uses mmCalcMethodI interface should be derived
//	from the mmCalcMethod class for additional functionality.
//
//******************************************************************************
//******************************************************************************

namespace mmImages {

	// forward declarations
	class mmCalcKernelI;
	class mmCMParameter;

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


	////////////////////////////////////////////////////////////////////////////////
	/// Implementation of calculation method interface
	////////////////////////////////////////////////////////////////////////////////
	class mmCalcMethod: public mmImagesCalculationMethodI, 
	                    public mmLog::mmLogSender
	{
		public:			// methods
			////////////////////////////////////////////////////////////////////////////////
			/// Default constructor
			////////////////////////////////////////////////////////////////////////////////
			mmCalcMethod(mmLog::mmLogReceiverI *p_psLogReceiver = NULL,
									 mmString p_sClassName = L"");

			////////////////////////////////////////////////////////////////////////////////
			/// Destructor
			////////////////////////////////////////////////////////////////////////////////
			virtual ~mmCalcMethod();

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
			/// multi-threaded implementation. 
			////////////////////////////////////////////////////////////////////////////////
			std::tr1::shared_ptr<mmSynchronize::mmExclusiveLockI> m_psThreadSynchEL;

			////////////////////////////////////////////////////////////////////////////////
			/// Number of rows in single data block. Modify to control amount of data processed
			/// by a single thread. The last block may be smaller, depending on image size,
			/// because it is a reminder of integer division of image height.
			////////////////////////////////////////////////////////////////////////////////
			mmInt m_iRowsCountInBlock;

		protected:
			////////////////////////////////////////////////////////////////////////////////
			/// Method executes for every working thread. Put pixels manipulations code here
			////////////////////////////////////////////////////////////////////////////////
			virtual bool Calculate() = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Event fired before every single image calculation in ForEachImage calculation. 
			/// Executed as single-threaded - DO NOT use m_psThreadSynchEL for accessing class 
			/// members in a multi-threaded configuration.
			///
			/// @param[in] p_psCurrentImage pointer to currently processed image
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnBeforeEachImage(mmImageI* p_psCurrentImage) {}
			
			////////////////////////////////////////////////////////////////////////////////
			/// Event fired after every single image calculation in ForEachImage calculation. 
			/// Executed as single-threaded - DO NOT use m_psThreadSynchEL for accessing class 
			/// members in a multi-threaded configuration.
			///
			/// @param[in] p_psCurrentImage pointer to currently processed image
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnAfterEachImage(mmImageI* p_psCurrentImage) {}

			////////////////////////////////////////////////////////////////////////////////
			/// Method iterates through data structure in order to perform specified operation
			/// on every data block. It works in multi-threaded environment and may be executed
			/// by multiple threads to manipulate on the same images structure. It should be
			///	used inside Calculate function.
			///
			/// @param[in] p_psKernel pointer to function object which implements operation
			///												executed for every data block in every image
			////////////////////////////////////////////////////////////////////////////////
			void ForEachImage(mmCalcKernelI* p_psKernel);

			////////////////////////////////////////////////////////////////////////////////
			/// Method registers a derived class member as a parameter to be retrieved from the 
			/// user prior to execution.
			///
			/// @param[in] p_sName unique name for the parameter
			/// @param[in] p_eType type name defined by mmXMLDataType (int, real, bool, etc.)
			/// @param[in] p_psValue pointer to the appropriate member field
			/// @param[in] p_bIsOutput optional, default is a user input parameter
			////////////////////////////////////////////////////////////////////////////////
			void SetParam(mmString p_sName, mmXML::mmXMLDataType p_eType, void* p_psValue, bool p_bIsOutput = false);			

			std::vector<mmID> GetImageIDs();

			std::vector<mmString> GetImageNames();

			std::vector<mmString> GetDLNames(mmImageI const * const p_psImage);

		private:

			void UpdateParameters();

		private:
			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			mmImages::mmImagesCalculationMethodI::sCalculationMethodParams mmImagesCalculationMethodI::GetCalculationMethodInfo(void);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			void mmImagesCalculationMethodI::SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImageStructure, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams = NULL);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			bool mmThreadExecutionI::Execute(void);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			void mmThreadExecutionI::StopExecution(void);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			bool mmThreadExecutionI::IsExecuting(void);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			mmReal mmThreadExecutionI::GetProgress(void);

		private:		// fields
			mmReal m_rProgress;
			//mmInt m_iThreadsCount;
			bool m_bIsExecuting;
			bool m_bStopExecution;
			bool m_bFinishImage;
			std::vector<mmCMParameter> m_vParameters;
			std::map<mmID, mmInt> m_mNextRows;		// next available row for each image in structure which is identified by ID
	};
};
