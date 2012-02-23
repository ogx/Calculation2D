//******************************************************************************
//******************************************************************************
//
//  Images interfaces
//
//  Description: This header defines mmImages namespace and its main interfaces.
//
//******************************************************************************
//******************************************************************************
#ifndef mmIImagesInterfacesH
#define mmIImagesInterfacesH

#include <mmGlobalDefs.h>
#include <math\mmMath.h>
#include <mmError.h>
#include <mmPixel.h>

#include <interfaces\mmIThread.h>

////////////////////////////////////////////////////////////////////////////////
/// It defines size of input/output XML params definition for calculation
/// automation.
////////////////////////////////////////////////////////////////////////////////
#define INOUT_PARAMS_SIZE	4096

////////////////////////////////////////////////////////////////////////////////
/// Namespace for images storage, calculation and manipulation.
////////////////////////////////////////////////////////////////////////////////
namespace mmImages
{
	class mmLayerI;
	class mmImageI;
	class mmImageStructureI;

	class mmStats {
	public:
		mmStats(void);
		void Clear(void);
		void Update(mmReal const p_prValues[], mmUInt const p_iSize);
	public:
		static mmReal const rInvalid;
	public:
		mmReal rMin;
		mmReal rMax;
		mmReal rAverage;
		mmReal rStdDev;
	};

	class mmLayerI {
	public:
		class mmCallbackI {
		public:
			virtual void OnLayerCreate(mmLayerI * p_psLayer) = 0;
			virtual void OnLayerDestroy(mmLayerI * p_psLayer) = 0;
			virtual void OnLayerPropertiesChange(mmLayerI * p_psLayer) = 0;
			virtual void OnLayerValuesChange(mmLayerI * p_psLayer) = 0;
		};
	public:
		static mmUInt const iInvalid;
	public:
		virtual mmUInt GetWidth(void) const = 0;
		virtual mmUInt GetHeight(void) const = 0;
		virtual mmReal GetDefaultValue(void) const = 0;

		virtual mmStats GetStats(void) const = 0;

		virtual mmString GetName(void) const = 0;
		virtual void SetName(mmString const & p_sName) = 0;
		
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmReal p_prValues[]) const = 0;
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmReal const p_prValues[]) = 0;

		virtual bool GetPixels(mmRect const & p_sRect, mmReal p_prValues[]) const = 0;
		virtual bool SetPixels(mmRect const & p_sRect, mmReal const p_prValues[]) = 0;

		virtual ~mmLayerI(void) {};
	};

	class mmImageI {
	public:
		class mmCallbackI {
		public:
			virtual void OnImageCreate(mmImageI * p_psImage) = 0;
			virtual void OnImageDestroy(mmImageI * p_psImage) = 0;
			virtual void OnImagePropertiesChange(mmImageI * p_psImage) = 0;
			virtual void OnImagePixelsChange(mmImageI * p_psImage) = 0;
			virtual void OnLayerCreate(mmImageI * p_psImage, mmLayerI * p_psLayer) = 0;
			virtual void OnLayerDestroy(mmImageI * p_psImage, mmLayerI * p_psLayer) = 0;
			virtual void OnLayerPropertiesChange(mmImageI * p_psImage, mmLayerI * p_psLayer) = 0;
			virtual void OnLayerValuesChange(mmImageI * p_psImage, mmLayerI * p_psLayer) = 0;
		};
	public:
		enum mmPixelType {mmP8=1, mmP24=3, mmP32=4};
	public:
		static mmUInt const iInvalid;
	public:
		virtual mmUInt GetWidth(void) const = 0;
		virtual mmUInt GetHeight(void) const = 0;
		virtual void Resize(mmUInt const p_iWidth, mmUInt const p_iHeight) = 0;

		virtual mmPixelType GetPixelType(void) const = 0;

		virtual mmString GetName(void) const = 0;
		virtual void SetName(mmString const & p_sName) = 0;
		
		virtual mmLayerI * GetChannel(mmUInt const p_iIndex) const = 0;
		
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel8 p_psValues[]) const = 0;
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel24 p_psValues[]) const = 0;
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel32 p_psValues[]) const = 0;
		
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel8 const p_psValues[]) = 0;
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel24 const p_psValues[]) = 0;
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel32 const p_psValues[]) = 0;
		
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel8 p_psValues[]) const = 0;
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel24 p_psValues[]) const = 0;
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel32 p_psValues[]) const = 0;
		
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel8 const p_psValues[]) = 0;
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel24 const p_psValues[]) = 0;
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel32 const p_psValues[]) = 0;
				
		virtual void SetRegionOfInterest(mmRect const & p_sRegion) = 0;
		virtual mmRect GetRegionOfInterest(void) const = 0;
		
		virtual mmUInt CreateLayer(mmString const & p_sName, mmReal const p_rDefaultValue) = 0;
		virtual mmUInt GetLayerCount(void) const = 0;
		virtual mmLayerI * GetLayer(mmUInt const p_iIndex) const = 0;
		virtual mmLayerI * GetLayer(mmString const & p_sName) const = 0;
		virtual bool DeleteLayer(mmUInt const p_iIndex) = 0;
		virtual bool DeleteLayer(mmString const & p_sName) = 0;
		
		virtual void SetMetadata(mmString const & p_sMetadata) = 0;
		virtual mmString GetMetadata(void) = 0;

		virtual ~mmImageI(void) {};
	};

	class mmImageStructureI {
	public:
		virtual mmUInt CreateImage(mmString const & p_sName, mmUInt const p_iWidth, mmUInt const p_iHeight, mmImageI::mmPixelType const p_ePixelType) = 0;
		virtual mmUInt GetImageCount(void) const = 0;
		virtual mmImageI * GetImage(mmUInt const p_iIndex) const = 0;
		virtual mmImageI * GetImage(mmString const & p_sName) const = 0;
		virtual bool DeleteImage(mmUInt const p_iIndex) = 0;
		virtual bool DeleteImage(mmString const & p_sName) = 0;
		
		virtual ~mmImageStructureI(void) {};
	};

	extern const wchar_t* g_pAutoCalcXML_Params_ParamType_ImageName;
	extern const wchar_t* g_pAutoCalcXML_Params_ParamType_ImagesNames;
        extern const wchar_t* g_pAutoCalcXML_Params_ParamType_ImageCount_All;
	extern const wchar_t* g_pAutoCalcXML_Params_ParamType_DataLayerName;
	extern const wchar_t* g_pAutoCalcXML_Params_ParamType_RealValue;
	extern const wchar_t* g_pAutoCalcXML_Params_ParamType_IntegerValue;
	extern const wchar_t* g_pAutoCalcXML_Params_ParamType_BoolValue;
        extern const wchar_t* g_pAutoCalcXML_Params_ParamType_BoolValue_YES;
        extern const wchar_t* g_pAutoCalcXML_Params_ParamType_BoolValue_NO;
	extern const wchar_t* g_pAutoCalcXML_Params_ParamType_String;

	extern const wchar_t* g_pAutoCalcXML_INParams_Node;
	extern const wchar_t* g_pAutoCalcXML_OUTParams_Node;
        extern const wchar_t* g_pAutoCalcXML_Params_Param_Node;
        extern const wchar_t* g_pAutoCalcXML_Params_ParamName_Node;
        extern const wchar_t* g_pAutoCalcXML_Params_ParamType_Node;
        extern const wchar_t* g_pAutoCalcXML_Params_ParamValue_Node;

	////////////////////////////////////////////////////////////////////////////////
	/// Interface representing image calculation method class. It should be
	/// derived and implemented by developers designing calculation method classes for
  	/// images processing.
	////////////////////////////////////////////////////////////////////////////////
	class mmImagesCalculationMethodI: public mmThread::mmThreadExecutionI
        {
		public:	// definitions
			////////////////////////////////////////////////////////////////////////////////
			/// This structure defines calculation method params for automated processing
			/// by using calculation patterns.
			////////////////////////////////////////////////////////////////////////////////
			typedef struct
			{
				////////////////////////////////////////////////////////////////////////////////
				/// Definition of calculation method input params in XML.
				////////////////////////////////////////////////////////////////////////////////
				wchar_t sInParams[INOUT_PARAMS_SIZE];
				////////////////////////////////////////////////////////////////////////////////
				/// Definition of calculation method output params in XML.
				////////////////////////////////////////////////////////////////////////////////
				wchar_t sOutParams[INOUT_PARAMS_SIZE];
			} sCalculationAutomationParams;

			////////////////////////////////////////////////////////////////////////////////
			/// This structure defines calculation method params defining its functionality and
			/// description.
			////////////////////////////////////////////////////////////////////////////////
			typedef struct
			{
				////////////////////////////////////////////////////////////////////////////////
				/// Short name for calculation method.
				////////////////////////////////////////////////////////////////////////////////
				wchar_t sShortName[128];
				////////////////////////////////////////////////////////////////////////////////
				/// Unique name for calculation method.
				////////////////////////////////////////////////////////////////////////////////
				wchar_t sIDName[128];
				////////////////////////////////////////////////////////////////////////////////
				/// Description of calculation method functionality.
				////////////////////////////////////////////////////////////////////////////////
				wchar_t sDescription[INOUT_PARAMS_SIZE];
				////////////////////////////////////////////////////////////////////////////////
				/// It defines if this calculation method can be executed in multithreaded
				/// environment. TRUE - it can be multithreaded, FALSE - only one thread for
				/// this calculation method is allowed.
				////////////////////////////////////////////////////////////////////////////////
				bool bIsMultithreaded;
				////////////////////////////////////////////////////////////////////////////////
				/// Automation params definition.
				/// See mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams
				/// structure description.
				////////////////////////////////////////////////////////////////////////////////
				sCalculationAutomationParams sAutoParams;
			} sCalculationMethodParams;
		public:		// methods
			////////////////////////////////////////////////////////////////////////////////
			/// Virtual destructor.
			////////////////////////////////////////////////////////////////////////////////
			virtual ~mmImagesCalculationMethodI() {};

			////////////////////////////////////////////////////////////////////////////////
			/// Returns list with available image of points calculation methods.
			///
			/// @return vector with mmImages::mmImagesStructureI::sCalculationMethodParams
			///				  structures defining available calculation methods and their functionality.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmImages::mmImagesCalculationMethodI::sCalculationMethodParams GetCalculationMethodInfo(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method initializes parameters for calculation method. It also invokes user interface
			/// dialog for input parameters specification or parses automation parameters from XML.
			///
			/// @param[in] p_psImagesStructure pointer to mmImages::mmImagesStructureI
			///						 representing images structure to calculate,
			/// @param[in] p_psAutomationParams pointer to mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams
			///						 structure representing automation params. If NULL than user interface asks
			///						 user about input parametrs specification, otherwise this structure is
			///						 parsed and all parameters are set automatically.
			////////////////////////////////////////////////////////////////////////////////
			virtual void SetCalculationMethodParameters(mmImages::mmImageStructureI* p_psImagesStructure,
                        	mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams* p_psAutomationParams = NULL) = 0;
	};

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
			/// Initializes selected images calculation method and returns pointer
			/// into its interface. In case of error it throws
			/// mmError(mmeImagesStructureUnknownImagesCalculationMethod).
			///
			/// @param[in] p_sCalculationMethodName name of images calculation method to initialize,
			/// @return pointer to initialized mmImages::mmImagesCalculationMethodI object.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmImages::mmImagesCalculationMethodI* InitializeImagesCalculationMethod(mmString p_sCalculationMethodName) = 0;
	};
};

#endif
