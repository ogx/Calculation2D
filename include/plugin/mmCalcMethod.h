#pragma once

#include <interfaces/mmICalcMethod.h>
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
	/// Implementation of calculation method interface
	////////////////////////////////////////////////////////////////////////////////
	class mmCalcMethod: public mmImagesCalculationMethodI, 
	                    public mmCalcMethodI,
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
			/// multithread implementation. 
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
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual void ExecBeforeSingleImageCalc(mmImageI* p_psCurrentImage) {}
			
			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual void ExecAfterSingleImageCalc(mmImageI* p_psCurrentImage) {}

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual void ForEachImage(mmCalcKernelI* p_psKernel);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual void SetParam(mmString p_sName, mmXML::mmXMLDataType p_eType, void* p_psValue, bool p_bIsOutput = false);
			
			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual const void* GetParam(mmString p_sName);
			
			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual void GetParam(mmString p_sName, void* p_pValue);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual void UpdateParameters();

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual std::vector<mmString> GetImageNames();

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmCalcMethodI interface (see mmICalcMethod.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			virtual std::vector<mmString> GetDLNames(mmUInt const p_iImage);

		private:
			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			mmImages::mmImagesCalculationMethodI::sCalculationMethodParams GetCalculationMethodInfo(void);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			void SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImageStructure, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams = NULL);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			void UserAction(mmString p_sXMLParams);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			void UserAction(wchar_t* p_pcXMLParamsBuffer, mmInt p_iXMLParamsBufferSize);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			bool Execute(void);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			void StopExecution(void);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			bool IsExecuting(void);

			////////////////////////////////////////////////////////////////////////////////
			/// method inherited from mmImagesCalculationMethodI interface (see mmIImages.h for documentation)
			////////////////////////////////////////////////////////////////////////////////
			mmReal GetProgress(void);

		private:		// fields
			mmReal m_rProgress;
			//mmInt m_iThreadsCount;
			bool m_bIsExecuting;
			bool m_bStopExecution;
			bool m_bFinishImage;
			std::vector<mmCMParameter> m_vParameters;
			std::map<mmString, mmInt> m_mNextRows;		// next available row for each image in structure which is identified by name
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Additional tool class simplifying edition of method's parameters 
	////////////////////////////////////////////////////////////////////////////////
	class mmCMParameter
	{
		public:
			mmCMParameter(): m_sName(L""), 
											 m_eType(mmXML::g_eXMLUnknownDataType),
											 m_bIsOutput(false) { }
			mmCMParameter(mmString p_sName, 
										mmXML::mmXMLDataType p_eType, 
										void* p_psValue,
										bool p_bIsOutput = false)
			{
				m_sName = p_sName;
				m_eType = p_eType;
				m_bIsOutput = p_bIsOutput;
				
				switch (m_eType) {
					case mmXML::g_eXMLReal:
						m_pValue.reset(new mmReal(*reinterpret_cast<mmReal*>(p_psValue)));
						break;
					case mmXML::g_eXMLInt:
						m_pValue.reset(new mmInt(*reinterpret_cast<mmInt*>(p_psValue)));
						break;
					case mmXML::g_eXMLString:
					case mmXML::g_eXMLImageName:
					case mmXML::g_eXMLDataLayerName:
						m_pValue.reset(new mmString(*reinterpret_cast<mmString*>(p_psValue)));
						break;
					case mmXML::g_eXMLBool:
						m_pValue.reset(new bool(*reinterpret_cast<bool*>(p_psValue)));
						break;
				}
			}
			~mmCMParameter() { }
		
			mmString m_sName;
			mmXML::mmXMLDataType m_eType;
			mmXML::mmXMLNodePosition m_sPosition;
			std::tr1::shared_ptr<void> m_pValue;
			bool m_bIsOutput;
	};
};
