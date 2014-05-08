#pragma once

#include <map>
#include <vector>
#include <memory>

#include <interfaces/mmIImages.h>
#include <interfaces/mmISynchronize.h>
#include <log/mmLogSender.h>
#include <serialization/mmGenericParam.h>
#include <math/mmMath.h>

#undef min
#undef max

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

	template<typename pixel_t>
	void DrawLine(mmImages::mmImageI * p_psImage, mmInt const p_iBeginX, mmInt const p_iBeginY, mmInt const p_iEndX, mmInt const p_iEndY, pixel_t const & p_sColor)
	{
		mmInt const v_iDeltaX = p_iEndX - p_iBeginX, v_iDeltaY = p_iEndY - p_iBeginY;
		mmInt const v_iWidth = p_psImage->GetWidth();
		mmInt const v_iMinX = std::min(p_iBeginX, p_iEndX), v_iMaxX = std::max(p_iBeginX, p_iEndX),
					v_iMinY = std::min(p_iBeginY, p_iEndY), v_iMaxY = std::max(p_iBeginY, p_iEndY);

		if(v_iDeltaX == 0 && v_iDeltaY == 0)
		{
			return;
		}

		pixel_t * v_prRows = new pixel_t[v_iWidth * (v_iMaxY - v_iMinY + 1)];
		p_psImage->GetRows(v_iMinY, v_iMaxY - v_iMinY + 1, v_prRows);
		if(::abs(v_iDeltaX) > ::abs(v_iDeltaY))
		{
			mmReal const v_rA = mmReal(v_iDeltaY) / mmReal(v_iDeltaX), v_rB = mmReal(p_iBeginY) - v_rA * mmReal(p_iBeginX);
			for(mmInt v_iX = v_iMinX; v_iX <= v_iMaxX; ++v_iX)
			{
				mmInt const v_iY(v_rA * mmReal(v_iX) + v_rB);
				v_prRows[(v_iY - v_iMinY) * v_iWidth + v_iX] = p_sColor;
			}
		}
		else
		{
			mmReal const v_rA = mmReal(v_iDeltaX) / mmReal(v_iDeltaY), v_rB = mmReal(p_iBeginX) - v_rA * mmReal(p_iBeginY);
			for(mmInt v_iY = v_iMinY; v_iY <= v_iMaxY; ++v_iY)
			{
				mmInt const v_iX(v_rA * mmReal(v_iY) + v_rB);
				v_prRows[(v_iY - v_iMinY) * v_iWidth + v_iX] = p_sColor;
			}
		}
		p_psImage->SetRows(v_iMinY, v_iMaxY - v_iMinY + 1, v_prRows);
		delete [] v_prRows;
	}

	template<typename pixel_t>
	void DrawDot(mmImages::mmImageI * p_psImage, mmInt const p_iX, mmInt const p_iY, mmInt const p_iRadius, pixel_t const & p_sColor)
	{
		mmInt const v_iMinX = std::max(p_iX - p_iRadius, 0), v_iMaxX = std::min<mmInt>(p_iX + p_iRadius, p_psImage->GetWidth() - 1), 
					v_iMinY = std::max(p_iY - p_iRadius, 0), v_iMaxY = std::min<mmInt>(p_iY + p_iRadius, p_psImage->GetHeight() - 1);
		mmInt const v_iWidth = p_psImage->GetWidth();
		pixel_t * v_prRows = new pixel_t[v_iWidth * (v_iMaxY - v_iMinY + 1)];
		p_psImage->GetRows(v_iMinY, (v_iMaxY - v_iMinY + 1), v_prRows);
		for(mmInt v_iY = v_iMinY; v_iY <= v_iMaxY; ++v_iY)
		{
			for(mmInt v_iX = v_iMinX; v_iX <= v_iMaxX; ++v_iX)
			{
				if((v_iY - p_iY) * (v_iY - p_iY) + (v_iX - p_iX) * (v_iX - p_iX) < (p_iRadius) * (p_iRadius))
				{
					v_prRows[(v_iY - v_iMinY) * v_iWidth + v_iX] = p_sColor;
				}
			}
		}
		p_psImage->SetRows(v_iMinY, (v_iMaxY - v_iMinY + 1), v_prRows);
		delete [] v_prRows;
	}
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
			std::auto_ptr<mmSynchronize::mmExclusiveLockI> m_psThreadSynchEL;

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

			template<class param_t>
			void BindInputParam(mmString const & p_sName, mmGenericParamI::mmType const p_eType, param_t & p_sValue) {
				BindParam(m_sInputParams, p_sName, p_eType, p_sValue);
			}
			template<class param_t>
			void BindOutputParam(mmString const & p_sName, mmGenericParamI::mmType const p_eType, param_t & p_sValue) {
				BindParam(m_sOutputParams, p_sName, p_eType, p_sValue);
			}

			std::vector<mmID> GetImageIDs();

			std::vector<mmString> GetImageNames();

			std::vector<mmString> GetDLNames(mmImageI const * const p_psImage);

		private:

			void SerializeParameters();
			void DeserializeParameters(mmImagesCalculationMethodI::sCalculationAutomationParams const & p_sAutomationParams);

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

		protected:
			mmReal m_rProgress;
		private:		// fields
			//mmInt m_iThreadsCount;
			bool m_bIsExecuting;
			bool m_bStopExecution;
			bool m_bFinishImage;
			std::list<mmGenericParamI*> m_sInputParams;
			std::list<mmGenericParamI*> m_sOutputParams;
			std::map<mmID, mmInt> m_mNextRows;		// next available row for each image in structure which is identified by ID
	};
};
