#pragma once

#include <interfaces/mmICalcMethodExt.h>
#include <map>

namespace mmImages {

	class mmCalcKernelI;

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

	class mmCalcMethod: public mmImagesCalculationMethodI, 
	                    public mmCalcMethodExtI,
	                    public mmLog::mmLogSender
	{
		protected:	// methods
			virtual void ExecBeforeSingleImageCalc(mmInt p_iCurrentImageID) {}
			virtual void ExecAfterSingleImageCalc(mmInt p_iCurrentImageID) {}

			virtual void ForEachImage(mmCalcKernelI* p_psKernel);
			virtual void SetParam(mmString p_sName, mmXML::mmXMLDataType p_eType, void* p_psValue, bool p_bIsOutput = false);
			virtual const void* GetParam(mmString p_sName);
			virtual void GetParam(mmString p_sName, void* p_pValue);
			virtual void UpdateParameters();
			virtual std::vector<mmString> GetImageNames();
			virtual std::vector<mmString> GetDLNames(mmUInt const p_iImage);

		private:		// fields
			mmReal m_rProgress;
			mmInt m_iThreadsCount;
			bool m_bIsExecuting;
			bool m_bStopExecution;
			bool m_bFinishImage;
			std::vector<mmCMParameter> m_vParameters;
			std::map<mmString, mmInt> m_mNextRows;		// next available row for each image in structure which is identified by name

		private:
			mmImages::mmImagesCalculationMethodI::sCalculationMethodParams GetCalculationMethodInfo(void);
			void SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImageStructure, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams = NULL);

			void UserAction(mmString p_sXMLParams);
			void UserAction(wchar_t* p_pcXMLParamsBuffer, mmInt p_iXMLParamsBufferSize);

			bool Execute(void);
			void StopExecution(void);
			bool IsExecuting(void);
			mmReal GetProgress(void);

		public:			// methods
			mmCalcMethod(mmLog::mmLogReceiverI *p_psLogReceiver = NULL,
									 mmString p_sClassName = L"");
			virtual ~mmCalcMethod();
	};
};
