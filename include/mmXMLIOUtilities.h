//---------------------------------------------------------------------------

#ifndef mmXMLIOUtilitiesH
#define mmXMLIOUtilitiesH
//---------------------------------------------------------------------------
#include <mmStringUtilities.h>
#include <interfaces/mmIImages.h>
#include <mmOperatingSystemCalls.h>
#include <list>
#include <algorithm>

using namespace mmStringUtilities;

// DEPRECATED
// use mmGenericParam instead
namespace mmXML {
	typedef std::size_t mmUInt;
	typedef struct { mmInt iGroupIndex; mmInt iLeafIndex; } mmXMLNodePosition;
	typedef struct { mmXMLNodeI * _v_xNode; mmXMLNodePosition sPosition; } mmXMLPositionedNode;
	enum mmXMLDataType { g_eXMLReal = 0x00, g_eXMLInt=0x01, g_eXMLString=0x02, g_eXMLBool=0x03, g_eXMLImageName=0x04, g_eXMLDataLayerName=0x05, g_eXMLUnknownDataType=0x06 };
	enum mmXMLAutoType { g_eXMLAutoInput=0x00, g_eXMLAutoOutput=0x01 };

	class mmXMLNodePath {
		public:
			typedef struct { mmString tName; mmInt iIndex; } mmXMLNodeID;
			mmXMLNodePath( void );
			mmXMLNodePath( mmString const & p_usPath );
			mmXMLNodePath& operator += ( mmString const & p_tPath );
			mmXMLNodePath& operator += ( mmXMLNodeID const & p_sNode );
			void operator = ( mmString const & p_tPath );
			bool operator == ( mmXMLNodePath const & p_sNodePath ) const;
			operator mmXMLNodeID() const;
			void Reset( void ) const;
			bool End( void ) const;
			mmString ToString( void ) const;
			void Reverse( void );
			void Norm( void );
		private:
			struct MakeZero { void operator () ( mmXMLNodeID & p_sNode ) const { p_sNode.iIndex = 0; } };
			std::list<mmXMLNodeID> m_lsPath;
			mutable std::list<mmXMLNodeID>::const_iterator m_lsiCurrent;
			void AddString( mmString const & p_tPath );
	};

	bool IsValidNodePosition( mmXMLNodePosition p_xNodePosition );
	mmXMLPositionedNode CreateAutomation( mmXMLDocI * _p_xDoc, mmXMLAutoType p_eXMLAutoType );
	mmXMLPositionedNode CreateAutomationInput( mmXMLDocI * _p_xDoc );
	mmXMLPositionedNode CreateAutomationOutput( mmXMLDocI * _p_xDoc );
	mmXMLNodePosition AddParam( mmXMLPositionedNode * _p_xPositionedRootNode, mmString p_tName, mmXMLDataType p_eType, void * _p_DefaultValue = NULL );
	mmXMLNodePosition AddParam( mmXMLPositionedNode * _p_xPositionedRootNode, mmString p_tName, mmXMLDataType p_eType, mmString p_tDefaultValue );
	void CopyInputParams( mmXMLDocI * _p_xDoc, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams * _p_sAutomationParams );
	void CopyOutputParams( mmXMLDocI * _p_xDoc, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams * _p_sAutomationParams );
	void UpdateOutputParam( mmXML::mmXMLDocI * _p_xDoc, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams * _p_sAutomationParams, mmXMLNodePosition p_xNodePosition, void * _p_OutputValue );
	void UpdateOutputParam( mmXML::mmXMLDocI * _p_xDoc, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams * _p_sAutomationParams, mmXMLNodePosition p_xNodePosition, mmString p_tOutputValue );
	bool GetValueByName( mmXML::mmXMLDocI * _p_xDoc, mmString p_tParamName, void * _p_OutputPointer );
	bool GetValue( mmXMLDocI * _p_xDoc, mmXMLNodePosition p_xNodePosition, void * _p_OutputPointer );
	bool SetValueByName( mmXML::mmXMLDocI * _p_xDoc, mmString p_tParamName, void * _p_InputPointer );
	bool SetValue( mmXMLDocI * _p_xDoc, mmXMLNodePosition p_xNodePosition, void * _p_InputPointer );
	mmString GetTypeTransition( mmXMLDataType p_eDataType );
	mmXMLDataType GetTypeTransition( mmString p_tType );
	mmString ConvertToString( mmXMLDataType p_eType, void * _p_Data );
	mmXMLNodeI* GetNodeAt( mmXMLNodeI* _p_sRootNode, mmXMLNodePath const & p_sNodePath, bool p_bCreateNode );
}

#endif
