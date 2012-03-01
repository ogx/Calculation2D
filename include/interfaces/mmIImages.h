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
#include <mmError.h>
#include <mmPixel.h>

#include <interfaces\mmIThread.h>

////////////////////////////////////////////////////////////////////////////////
/// Defines size of input/output XML params definition for calculation
/// automation.
////////////////////////////////////////////////////////////////////////////////
#define INOUT_PARAMS_SIZE	4096

////////////////////////////////////////////////////////////////////////////////
/// Namespace for images storage, calculation and manipulation.
////////////////////////////////////////////////////////////////////////////////
namespace mmImages
{
	////////////////////////////////////////////////////////////////////////////////
	/// Forward declarations of classes connected with image storage
	////////////////////////////////////////////////////////////////////////////////
	class mmLayerI;
	class mmImageI;
	class mmImageStructureI;

	////////////////////////////////////////////////////////////////////////////////
	/// Structure containing statistics for an array of mmReal
	////////////////////////////////////////////////////////////////////////////////
	class mmStats {
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Constructor. All values are initialized to rInvalid. 
		////////////////////////////////////////////////////////////////////////////////
		mmStats(void);
		////////////////////////////////////////////////////////////////////////////////
		/// Sets all fields to rInvalid.
		////////////////////////////////////////////////////////////////////////////////
		void Clear(void);
		////////////////////////////////////////////////////////////////////////////////
		/// Calculates statistics for a given array and stores them in the structure
		///
		/// @param[in] p_prValues array with values for statistics
		/// @param[in] p_iSize size of array, in elements
		////////////////////////////////////////////////////////////////////////////////
		void Update(mmReal const p_prValues[], mmUInt const p_iSize);
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Invalid value.
		////////////////////////////////////////////////////////////////////////////////
		static mmReal const rInvalid;
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Minimum value.
		////////////////////////////////////////////////////////////////////////////////
		mmReal rMin;
		////////////////////////////////////////////////////////////////////////////////
		/// Maximum value.
		////////////////////////////////////////////////////////////////////////////////
		mmReal rMax;
		////////////////////////////////////////////////////////////////////////////////
		/// Average value.
		////////////////////////////////////////////////////////////////////////////////
		mmReal rAverage;
		////////////////////////////////////////////////////////////////////////////////
		/// Standard deviation.
		////////////////////////////////////////////////////////////////////////////////
		mmReal rStdDev;
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Interface of a single image layer.
	////////////////////////////////////////////////////////////////////////////////
	class mmLayerI {
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Layer callback class.
		////////////////////////////////////////////////////////////////////////////////
		class mmCallbackI {
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the layer instance, with which registered, after creation.
			///
			/// @param[in] p_psLayer pointer to layer which ran the callback
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnLayerCreate(mmLayerI * p_psLayer) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the layer instance, with which registered, after destruction.
			///
			/// @param[in] p_psLayer pointer to layer which ran the callback
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnLayerDestroy(mmLayerI * p_psLayer) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the layer instance, with which registered, when any of its properties are changed (width, height, name).
			///
			/// @param[in] p_psLayer pointer to layer which ran the callback
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnLayerPropertiesChange(mmLayerI * p_psLayer) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the layer instance, with which registered, when stored values are changed.
			///
			/// @param[in] p_psLayer pointer to layer which ran the callback
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnLayerValuesChange(mmLayerI * p_psLayer) {}
		protected:
			~mmCallbackI(void) {}
		};
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Invalid value.
		////////////////////////////////////////////////////////////////////////////////
		static mmUInt const iInvalid;
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Returns layer ID. ID is unique for the image it belongs to.  
		///
		/// @return layer ID
		////////////////////////////////////////////////////////////////////////////////
		virtual mmID GetID(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns layer width, in pixels. 
		///
		/// @return layer width, in pixels
		////////////////////////////////////////////////////////////////////////////////
		virtual mmUInt GetWidth(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns layer height, in pixels. 
		///
		/// @return layer height, in pixels
		////////////////////////////////////////////////////////////////////////////////
		virtual mmUInt GetHeight(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns the default value used to fill the layer. 
		///
		/// @return default value
		////////////////////////////////////////////////////////////////////////////////
		virtual mmReal GetDefaultValue(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns up-to-date layer statistics. 
		///
		/// @return layer statistics
		////////////////////////////////////////////////////////////////////////////////
		virtual mmStats GetStats(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns layer name. 
		///
		/// @return layer name
		////////////////////////////////////////////////////////////////////////////////
		virtual mmString GetName(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Changes layer name. 
		///
		/// @param[in] p_sName new name
		////////////////////////////////////////////////////////////////////////////////
		virtual void SetName(mmString const & p_sName) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Fills the given array with layer values from a range of rows. Use this function 
		/// instead of GetPixels to increase efficiency. 
		///
		/// @param[in] p_iStart first row (starting from 0)
		/// @param[in] p_iCount number of rows
		/// @param[out] p_prValues array with values
		/// @return true if the call succeeded, false if the supplied row range is larger
		///			than layer dimensions, in this case no changes to the layer are made
		////////////////////////////////////////////////////////////////////////////////
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmReal p_prValues[]) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Fills a range of rows of the layer with values from the array. Use this 
		/// function instead of SetPixels to increase efficiency. 
		///
		/// @param[in] p_iStart first row (starting from 0)
		/// @param[in] p_iCount number of rows
		/// @param[in] p_prValues array with values
		/// @return true if the call succeeded, false if the supplied row range is larger
		///			than layer's dimensions, in this case no changes to the layer are made
		////////////////////////////////////////////////////////////////////////////////
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmReal const p_prValues[]) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Fills the given array with layer values from a pixel rectangle. Use GetRows 
		/// instead of this function to increase efficiency. 
		///
		/// @param[in] p_sRect region rectangle
		/// @param[out] p_prValues array with values
		/// @return true if the call succeeded, false if the supplied rectangle is larger
		///			than layer dimensions, in this case no changes to the layer are made
		////////////////////////////////////////////////////////////////////////////////
		virtual bool GetPixels(mmRect const & p_sRect, mmReal p_prValues[]) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Fills a pixel rectangle of the layer with values from the array. 
		/// Use SetRows instead this function to increase efficiency. 
		///
		/// @param[in] p_sRect region rectangle
		/// @param[out] p_prValues array with values
		/// @return true if the call succeeded, false if the supplied row range is larger
		///			than layer's dimensions, in this case no changes to the layer are made
		////////////////////////////////////////////////////////////////////////////////
		virtual bool SetPixels(mmRect const & p_sRect, mmReal const p_prValues[]) = 0;

		virtual ~mmLayerI(void) {};
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Interface of an image.
	////////////////////////////////////////////////////////////////////////////////
	class mmImageI {
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Image callback class.
		////////////////////////////////////////////////////////////////////////////////
		class mmCallbackI {
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the image instance, with which registered, after creation.
			///
			/// @param[in] p_psImage pointer to image which ran the callback
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnImageCreate(mmImageI * p_psImage) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the image instance, with which registered, after destruction.
			///
			/// @param[in] p_psImage pointer to image which ran the callback
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnImageDestroy(mmImageI * p_psImage) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the image instance, with which registered, when any of its properties are changed (width, height, name).
			///
			/// @param[in] p_psImage pointer to image which ran the callback
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnImagePropertiesChange(mmImageI * p_psImage) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the image instance, with which registered, when its pixels are changed (not additional layers).
			///
			/// @param[in] p_psImage pointer to image which ran the callback
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnImagePixelsChange(mmImageI * p_psImage) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the image instance, with which registered, after creation of a layer. 
			/// Channels of the image are also treated as layers. 
			///
			/// @param[in] p_psImage pointer to image which ran the callback
			/// @param[in] p_psLayer pointer to affected layer
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnLayerCreate(mmImageI * p_psImage, mmLayerI * p_psLayer) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the image instance, with which registered, after destruction of a layer.
			/// Channels of the image are also treated as layers. 
			///
			/// @param[in] p_psImage pointer to image which ran the callback
			/// @param[in] p_psLayer pointer to affected layer
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnLayerDestroy(mmImageI * p_psImage, mmLayerI * p_psLayer) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the image instance, with which registered, after properties of any of its layers' properties change. 
			/// Channels of the image are also treated as layers. 
			///
			/// @param[in] p_psImage pointer to image which ran the callback
			/// @param[in] p_psLayer pointer to affected layer
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnLayerPropertiesChange(mmImageI * p_psImage, mmLayerI * p_psLayer) {}
			////////////////////////////////////////////////////////////////////////////////
			/// Run by the image instance, with which registered, after any of its layers' values change.
			/// Channels of the image are also treated as layers. 
			///
			/// @param[in] p_psImage pointer to image which ran the callback
			/// @param[in] p_psLayer pointer to affected layer
			////////////////////////////////////////////////////////////////////////////////
			virtual void OnLayerValuesChange(mmImageI * p_psImage, mmLayerI * p_psLayer) {}
		protected:
			~mmCallbackI(void) {}
		};
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Enumeration with possible pixel types: 8 bits (1 channel), 24 bits(3 channels), 
		/// 32 bits (4 channels). The enumeration can also be used the number of channels in the image. 
		////////////////////////////////////////////////////////////////////////////////
		enum mmPixelType {mmP8=1, mmP24=3, mmP32=4};
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Invalid value
		////////////////////////////////////////////////////////////////////////////////
		static mmUInt const iInvalid;
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Returns image width, in pixels. 
		///
		/// @return image width, in pixels
		////////////////////////////////////////////////////////////////////////////////
		virtual mmUInt GetWidth(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns image height, in pixels. 
		///
		/// @return image height, in pixels
		////////////////////////////////////////////////////////////////////////////////
		virtual mmUInt GetHeight(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns image unique ID. 
		///
		/// @return image ID
		////////////////////////////////////////////////////////////////////////////////
		virtual mmID GetID(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Resizes the image. Old values are copied to new canvas where possible. 
		/// If new size is larger, new pixels are filled with (0) and layers with 
		///	their default value. 
		///
		/// @param[in] p_iWidth new width
		/// @param[in] p_iHeight new height
		////////////////////////////////////////////////////////////////////////////////
		virtual void Resize(mmUInt const p_iWidth, mmUInt const p_iHeight) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns image pixel type. Can also be used as number of channels. 
		///
		/// @return image pixel type
		////////////////////////////////////////////////////////////////////////////////
		virtual mmPixelType GetPixelType(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns image name. 
		///
		/// @return image name
		////////////////////////////////////////////////////////////////////////////////
		virtual mmString GetName(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Changes image name. 
		///
		/// @param[in] p_sName new name
		////////////////////////////////////////////////////////////////////////////////
		virtual void SetName(mmString const & p_sName) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns pointer to one of the channels. Channels are treated as layers. 
		/// Access image data in this way to increase efficiency. 
		///
		/// @param[in] p_iIndex channel index, number of channels can be obtained using GetPixelType
		/// @return pointer to channel or NULL if channel does not exist
		////////////////////////////////////////////////////////////////////////////////
		virtual mmLayerI * GetChannel(mmUInt const p_iIndex) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Fills the given array with pixel values from a range of rows. Use this function 
		/// instead of GetPixels to increase efficiency and GetChannel for best efficiency. 
		/// This function converts separate channels into a single array with every call. 
		///
		/// mmPixel8 works only for mmP8 images
		/// mmPixel24 works for mmP24 and mmP32 images
		/// mmPixel32 works for mmP32 images
		///
		/// @param[in] p_iStart first row (starting from 0)
		/// @param[in] p_iCount number of rows
		/// @param[out] p_psValues array with values
		/// @return true if the call succeeded, false if the supplied row range is larger
		///			than image dimensions or pixel type is not supported, in this case no 
		///			changes to the image are made
		////////////////////////////////////////////////////////////////////////////////
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel8 p_psValues[]) const = 0;
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel24 p_psValues[]) const = 0;
		virtual bool GetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel32 p_psValues[]) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Fills a range of rows of the image with values from the array. Use this 
		/// function instead of SetPixels to increase efficiency and GetChannel for best efficiency. 
		/// This function converts separate channels into a single array with every call. 
		///
		/// mmPixel8 works only for mmP8 images
		/// mmPixel24 works for mmP24 and mmP32 images
		/// mmPixel32 works for mmP32 images
		///
		/// @param[in] p_iStart first row (starting from 0)
		/// @param[in] p_iCount number of rows
		/// @param[in] p_prValues array with values
		/// @return true if the call succeeded, false if the supplied row range is larger
		///			than image dimensions or pixel type is not supported, in this case no 
		///			changes to the image are made
		////////////////////////////////////////////////////////////////////////////////
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel8 const p_psValues[]) = 0;
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel24 const p_psValues[]) = 0;
		virtual bool SetRows(mmUInt const p_iStart, mmUInt const p_iCount, mmPixel32 const p_psValues[]) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Fills the given array with layer values from a pixel rectangle. Use GetRows 
		/// instead of this function to increase efficiency and GetChannel for best efficiency. 
		/// This function converts separate channels into a single array with every call. 
		///
		/// mmPixel8 works only for mmP8 images
		/// mmPixel24 works for mmP24 and mmP32 images
		/// mmPixel32 works for mmP32 images
		///
		/// @param[in] p_sRect region rectangle
		/// @param[out] p_prValues array with values
		/// @return true if the call succeeded, false if the supplied rectangle is larger
		///			than image dimensions or pixel type is not supported, in this case no 
		///			changes to the image are made
		////////////////////////////////////////////////////////////////////////////////
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel8 p_psValues[]) const = 0;
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel24 p_psValues[]) const = 0;
		virtual bool GetPixels(mmRect const & p_sRect, mmPixel32 p_psValues[]) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Fills a pixel rectangle of the layer with values from the array. 
		/// Use SetRows instead this function to increase efficiency and GetChannel for best efficiency. 
		/// This function converts separate channels into a single array with every call.  
		///
		/// mmPixel8 works only for mmP8 images
		/// mmPixel24 works for mmP24 and mmP32 images
		/// mmPixel32 works for mmP32 images
		///
		/// @param[in] p_sRect region rectangle
		/// @param[out] p_prValues array with values
		/// @return true if the call succeeded, false if the supplied row range is larger
		///			than image dimensions or pixel type is not supported, in this case no 
		///			changes to the image are made
		////////////////////////////////////////////////////////////////////////////////
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel8 const p_psValues[]) = 0;
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel24 const p_psValues[]) = 0;
		virtual bool SetPixels(mmRect const & p_sRect, mmPixel32 const p_psValues[]) = 0;
		////////////////////////////////////////////////////////////////////////////////
		///	Sets the region of interest for an image. Does not affect any other properties. 
		///
		/// @param[in] p_sRegion region rectangle
		////////////////////////////////////////////////////////////////////////////////
		virtual void SetRegionOfInterest(mmRect const & p_sRegion) = 0;
		////////////////////////////////////////////////////////////////////////////////
		///	Retrieves the region of interest set.
		///
		/// @return region rectangle
		////////////////////////////////////////////////////////////////////////////////
		virtual mmRect GetRegionOfInterest(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Creates a new layer. 
		///
		/// @param[in] p_sName name of the layer
		/// @param[in] p_rDefaultValue default value used to fill the layer
		/// @return pointer to created layer
		////////////////////////////////////////////////////////////////////////////////
		virtual mmLayerI* CreateLayer(mmString const & p_sName, mmReal const p_rDefaultValue) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns number of layers for the current image. 
		///
		/// @return number of layers
		////////////////////////////////////////////////////////////////////////////////
		virtual mmUInt GetLayerCount(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns pointer to a layer. 
		///
		/// @param[in] p_sID layer ID
		/// @return pointer to layer or NULL if layer does not exist
		////////////////////////////////////////////////////////////////////////////////
		virtual mmLayerI * GetLayer(mmID const & p_sID) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns ID of first layer of given name occuring after layer with given ID. 
		/// Can be used to iterate through all layers. 
		///
		/// @param[in] p_sName layer name
		/// @param[in] p_sPreviousLayer only layers after layer with given ID are searched
		/// @return layer ID or mmID::invalid if layer was not found
		////////////////////////////////////////////////////////////////////////////////
		virtual mmLayerI * FindLayer(mmLayerI const * const p_psPreviousLayer=NULL, mmString const & p_sName=L"") const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Deletes a layer. 
		///
		/// @param[in] p_iIndex layer index
		/// @return true if the call succeeded, false if layer does not exist
		////////////////////////////////////////////////////////////////////////////////
		virtual bool DeleteLayer(mmUInt const p_iIndex) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Deletes a layer. 
		///
		/// @param[in] p_sName layer name
		/// @return true if the call succeeded, false if layer does not exist
		////////////////////////////////////////////////////////////////////////////////
		virtual bool DeleteLayer(mmString const & p_sName) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Set image metadata in the form of a string. Format of the metadata is arbitrary. 
		///
		/// @param[in] new metadata string
		////////////////////////////////////////////////////////////////////////////////		
		virtual void SetMetadata(mmString const & p_sMetadata) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Retrieves the metadata stored with the image
		///
		/// @return stored metadata string
		////////////////////////////////////////////////////////////////////////////////
		virtual mmString GetMetadata(void) = 0;

		virtual ~mmImageI(void) {};
	};

	class mmImageStructureI {
	public:
		////////////////////////////////////////////////////////////////////////////////
		/// Creates a new image. 
		///
		/// @param[in] p_sName name of the image
		/// @param[in] p_iWidth width of the image
		/// @param[in] p_iHeight height of the image
		/// @param[in] p_ePixelType pixel type of the image
		/// @return pointer to created image
		////////////////////////////////////////////////////////////////////////////////
		virtual mmImageI* CreateImage(mmString const & p_sName, mmUInt const p_iWidth, mmUInt const p_iHeight, mmImageI::mmPixelType const p_ePixelType) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns number of images in structure. 
		///
		/// @return number of images
		////////////////////////////////////////////////////////////////////////////////
		virtual mmUInt GetImageCount(void) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns pointer to an image. 
		///
		/// @param[in] p_sID image index 
		/// @return pointer to image or NULL if image does not exist
		////////////////////////////////////////////////////////////////////////////////
		virtual mmImageI * GetImage(mmID const & p_sID) const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Returns ID of first image of given name occuring after image with given ID. 
		/// Can be used to iterate through all images. 
		///
		/// @param[in] p_sName layer name
		/// @param[in] p_sPreviousImage only images after image with this ID are searched
		/// @return image ID or mmID::invalid if image was not found
		////////////////////////////////////////////////////////////////////////////////
		virtual mmImageI * FindImage(mmImageI const * const p_psPreviousImage=NULL, mmString const & p_sName=L"") const = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Deletes an image. 
		///
		/// @param[in] p_iIndex image index
		/// @return true if the call succeeded, false if image does not exist
		////////////////////////////////////////////////////////////////////////////////
		virtual bool DeleteImage(mmUInt const p_iIndex) = 0;
		////////////////////////////////////////////////////////////////////////////////
		/// Deletes an image. 
		///
		/// @param[in] p_sName image name
		/// @return true if the call succeeded, false if image does not exist
		////////////////////////////////////////////////////////////////////////////////
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
};

#endif
