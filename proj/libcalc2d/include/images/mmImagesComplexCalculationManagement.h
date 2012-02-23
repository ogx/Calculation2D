//******************************************************************************
//******************************************************************************
//
//  Images complex calculation management class
//
//  Description: This header defines mmImages complex calculation management class
//
//******************************************************************************
//******************************************************************************
#ifndef mmImagesComplexCalculationManagementH
#define mmImagesComplexCalculationManagementH

#include <deque>
#include <stack>

#include <mmGlobalDefs.h>
#include <mmError.h>
#include <log\mmLogSender.h>
#include <mmInterfaceInitializers.h>
#include <images\mmImagesCalculationManagement.h>

namespace mmImages
{
	extern const wchar_t* g_pComplexCalcXML_PatternContainer_Node;

	extern const wchar_t* g_pComplexCalcXML_Pattern_Node;
		extern const wchar_t* g_pComplexCalcXML_PatternName_Node;
		extern const wchar_t* g_pComplexCalcXML_PatternIDName_Node;
		extern const wchar_t* g_pComplexCalcXML_PatternDescription_Node;
		extern const wchar_t* g_pComplexCalcXML_PatternInputParams_Node;
			extern const wchar_t* g_pComplexCalcXML_PatternInputParam_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternInputParam_Constant_Attr;
				extern const wchar_t* g_pComplexCalcXML_PatternInputParamName_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternInputParamType_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternInputParamValue_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternInputParamValueModifiedNodeName_Node;
		extern const wchar_t* g_pComplexCalcXML_PatternOutputParams_Node;
			extern const wchar_t* g_pComplexCalcXML_PatternOutputParam_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternOutputParamName_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternOutputParamType_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternOutputParamValue_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternOutputParamValueModifiedNodeName_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternOutputParamValueModifiedSecondNodeName_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternOutputParamOperation_Node;
		extern const wchar_t* g_pComplexCalcXML_PatternDefinition_Node;
			extern const wchar_t* g_pComplexCalcXML_PatternCalcMethod_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternCalcMethodIDName_Node;
			extern const wchar_t* g_pComplexCalcXML_PatternInternalAction_Node;
				extern const wchar_t* g_pComplexCalcXML_PatternInternalActionName_Node;

// PB //
//	extern const wchar_t* g_pComplexCalcXML_Action_AddSimpleCalculationMethod;
//	extern const wchar_t* g_pComplexCalcXML_Action_AddOutputValueFromExisting;
//	extern const wchar_t* g_pComplexCalcXML_Action_MixOutputValuesFromExisting;
//	extern const wchar_t* g_pComplexCalcXML_Action_AddInputValueFromExistingOrConst;
//	extern const wchar_t* g_pComplexCalcXML_Action_AddInputValueConstDecision;
//	extern const wchar_t* g_pComplexCalcXML_Action_CreateSimplifiedCloudStructure;
//	extern const wchar_t* g_pComplexCalcXML_Action_DeleteSimplifiedCloudStructure;

// PB //
//	extern const wchar_t* g_pComplexCalcXML_Action_AddWhileLoop;
//	extern const wchar_t* g_pComplexCalcXML_Action_AddEndWhile;
//	extern const wchar_t* g_pComplexCalcXML_Action_CopyBinaryOutputToInputValue;
//	extern const wchar_t* g_pComplexCalcXML_Action_CopyIntOutputToInputValue;
//	extern const wchar_t* g_pComplexCalcXML_Action_CopyRealOutputToInputValue;

// PB //
//	extern const wchar_t* g_pComplexCalcXML_PatternInputParamUserSpecified;
//	extern const wchar_t* g_pComplexCalcXML_PatternInputParamConstant_Real;
//	extern const wchar_t* g_pComplexCalcXML_PatternInputParamConstant_Int;


	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Addition;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Subtraction;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Multiplication;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Division;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Power;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Minimum;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Maximum;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Equal;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Less;
	extern const wchar_t* g_pComplexCalcXML_PatternOutputFormula_Greater;

	////////////////////////////////////////////////////////////////////////////////
	/// Interface for images complex calculation management change receiver.
	////////////////////////////////////////////////////////////////////////////////
	class mmImagesComplexCalculationManagementStateChangeReceiverI
	{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// Virtual destructor.
			////////////////////////////////////////////////////////////////////////////////
			virtual ~mmImagesComplexCalculationManagementStateChangeReceiverI() {};

			virtual void StartOfComplexCalculation(void) = 0;
			virtual void StopOfComplexCalculation(void) = 0;

			virtual void ListOfComplexCalculationPatternsUpdated(void) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Class for images complex calculation management.
	////////////////////////////////////////////////////////////////////////////////
	class mmImagesComplexCalculationManagement: public mmLog::mmLogSender,
												public mmThread::mmThreadExecutionI
	{
		public:		// defs
			typedef struct
			{
				mmString sName;
				mmString sIDName;
				mmString sDescription;
			} sComplexCalculationInfo;
		private:	// defs
			typedef struct
			{
				mmString sIDName;
				mmImages::mmImageStructureI* psImageStructure;
			} sComplexCalculationDefinition;
		private:
			bool m_bIsActiveCalculation;
			mmImages::mmImagesCalculationMethodContainerI* m_psImageCalculationMethodContainer;
			mmImages::mmImageStructureI* m_psImageStructure;
			mmImages::mmImagesCalculationManagement* m_psImageCalculationManagement;

			// control thread
			bool m_bTerminateControlThreadExecution;
			mmThread::mmThreadI* m_psControlThread;
			mmInt m_iThreadExecutionSleepTime;

			// structures with information about simple calculation methods
			std::vector<mmImages::mmImagesCalculationMethodI::sCalculationMethodParams> m_vSimpleCalculationMethodsInfo;

			// XML with new pattern definition
			mmXML::mmXMLDocI* m_psXMLCurrentPatternDefinition;
			bool m_bIsNew;

			// XML with complex patterns container
			mmXML::mmXMLDocI* m_psXMLComplexPatternContainer;
			mmString m_sComplexPatternContainerXMLFile;

			// state change receiver
			mmImagesComplexCalculationManagementStateChangeReceiverI* m_psStateChangeReceiver;

			// execution queue
			std::deque<sComplexCalculationDefinition> m_vCalculationSheme;
			mmSynchronize::mmExclusiveLockI* m_psThreadSynchronizationEL;

			// simplified clouds stack
			std::stack<mmImages::mmImageStructureI*> m_vSimplifiedImage;

			// XML with currently calculated pattern definition
			mmXML::mmXMLNodeI* m_psXMLCalculatedPatternDefinition;
			sComplexCalculationDefinition m_sCurrentComplexCalculation;
			mmImages::mmImagesCalculationMethodI* m_psCurrentCalculationMethod;
			mmInt m_iCurrentComplexCalculationItem;
		private:
// PB //
//			mmString CreateComplexCalculationPatternUI(	mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition,
//														bool p_bIsReadOnly);
//			mmString GetFromUIDefinitionForSingleCalculationMethod(	mmString p_sCalculationMethodIDName,
//																	bool* p_pbNoParams);

// PB //
//			mmImages::mmImagesCalculationMethodI::sCalculationMethodParams* GetCalculationMethodInfo(mmString p_sCalculationMethodIDName);
//			mmString GetCalculationMethodName(mmString p_sCalculationMethodIDName);

// PB //
//			mmString UpdateInputOutputParameters(mmString p_psInOutParams);

// PB //
//			mmString GetFromUIReadableInputParameterName(	mmString p_sTitle,
//															mmString p_sDescription,
//															mmString p_sParamName);

// PB //
//			mmString CreateNewOutputValueFromExistingUI(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition);
//			mmString CreateNewOutputValueFromMixOfTwoExistingUI(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition);
//			mmString CreateNewConstantInputValueUI(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition);
//			mmString CreateNewConstantInputDecisionUI(void);

// PB //
//			void AddSingleCalculationMethodIntoPattern(	mmString p_sCalculationMethodParams,
//														mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition);

			void LoadComplexPatternsFromFile(mmString p_sCPFile);
// PB //
//			void SaveComplexPatternsIntoFile(mmString p_sCPFile);
//			void AddComplexPatternIntoContainer(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition);
//			void UpdateComplexPatternInContainer(mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition);

			mmXML::mmXMLNodeI* GetComplexCalculationPatternDefinition(mmString p_sCCPIDName);

			mmXML::mmXMLNodeI* GetInputParamsOfCCPattern(mmString p_sCCPIDName);
			mmXML::mmXMLNodeI* GetOutputParamsOfCCPattern(mmString p_sCCPIDName);

// PB //
//			void AddInternalWHILELoop(void);
//			void AddInternalENDWHILELoop(void);
//			void AddInternalCopyBinaryOutToInVariable(void);
//			void AddInternalCopyIntOutToInVariable(void);
//			void AddInternalCopyRealOutToInVariable(void);
//			mmString GetUIDefinitionForInternalAction(	mmString p_sInternalActionName,
//														mmString p_sIAInParams,
//														mmString p_sIAOutParams,
//														bool* p_pbNoParams);
//			mmString GetUIDefinitionForMultiSelectValues(mmString p_sUIData);
//			void AddInternalMethodIntoPattern(	mmString p_sInOutParams,
//												mmXML::mmXMLDocI* p_psXMLComplexCalculationPatternDefinition);

// PB //
//			void InitializeCurrentComplexCalculationPatternExecution(void);

			bool ManageCurrentComplexCalculationPatternExecution(void);
// PB //
//			void GetComplexCalculationPatternInputParamsFromUI(mmXML::mmXMLNodeI* p_psComplexCalculationDefinition);

			mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams InitializeParamsOfSimpleCalculationMethod(	mmXML::mmXMLNodeI* p_psXMLCalcPatternDefinition,
																															mmInt p_iCalculationMethodToInitializeID);
// PB //
			void UpdateOutputParamsOfSimpleCalculationMethod(	mmXML::mmXMLNodeI* p_psXMLCalcPatternDefinition,
																mmInt p_iCalculationMethodToUpdateID);

			mmString GetValueOfParameter(mmXML::mmXMLNodeI* p_psXMLCalcPatternDefinition,
																	 mmString p_sParamName);
			void SetValueOfParameter(mmXML::mmXMLNodeI* p_psXMLCalcPatternDefinition,
															 mmString p_sParamName,
															 mmString p_sParamValue);

		public:
			mmImagesComplexCalculationManagement(	mmImages::mmImagesCalculationMethodContainerI* p_psImageCalculationMethodContainer,
													mmImages::mmImagesCalculationManagement* p_psImageCalculationManagement,
													mmLog::mmLogReceiverI *p_psLogReceiver);
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmImagesComplexCalculationManagement();

			mmInt GetCountOfComplexCalculationPatterns(void);
			mmImages::mmImagesComplexCalculationManagement::sComplexCalculationInfo GetComplexCalculationPatternInfo(mmInt p_iPatternID);

			void ExecuteComplexCalculationPattern(	mmString p_sPatternIDName,
													mmImages::mmImageStructureI* p_psImageStructure);

			void RegisterStateChangeReceiver(mmImagesComplexCalculationManagementStateChangeReceiverI* p_psStateChangeReceiver);
			void UnregisterStateChangeReceiver(mmImagesComplexCalculationManagementStateChangeReceiverI* p_psStateChangeReceiver);

			bool IsCalculating(void);

			bool Execute(void);
			void StopExecution(void);
			bool IsExecuting(void);
			mmReal GetProgress(void);

			mmString GetInputParamsOfComplexCalculationPattern(mmString p_sCCPIDName);
			mmString GetOutputParamsOfComplexCalculationPattern(mmString p_sCCPIDName);
			mmString SetInputParamsOfComplexCalculationPattern(mmString p_sCCPIDName, mmString p_sNewParams);
			mmString SetOutputParamsOfComplexCalculationPattern(mmString p_sCCPIDName, mmString p_sNewParams);
	};
};

#endif
