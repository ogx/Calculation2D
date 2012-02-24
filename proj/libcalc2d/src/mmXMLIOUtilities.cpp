//---------------------------------------------------------------------------


#pragma hdrstop

#include <mmXMLIOUtilities.h>
#include <log/mmLogSender.h>
//---------------------------------------------------------------------------
mmXML::mmXMLNodePath::mmXMLNodePath( void ) {
	Reset();
}
//---------------------------------------------------------------------------

mmXML::mmXMLNodePath::mmXMLNodePath( mmString const & p_tPath ) {
	AddString( p_tPath );
	Reset();
}
//---------------------------------------------------------------------------

mmXML::mmXMLNodePath& mmXML::mmXMLNodePath::operator += ( mmString const & p_tPath ) {
	if( ToString() == L"" )
	   m_lsPath.clear();
	AddString( p_tPath );
	return *this;
}
//---------------------------------------------------------------------------

mmXML::mmXMLNodePath& mmXML::mmXMLNodePath::operator += ( mmXMLNodeID const & p_sNode ) {
	m_lsPath.push_back( p_sNode );
	return *this;
}
//---------------------------------------------------------------------------

void mmXML::mmXMLNodePath::operator = ( mmString const & p_tPath ) {
	m_lsPath.clear();
	AddString( p_tPath );
	Reset();
}
//---------------------------------------------------------------------------

bool mmXML::mmXMLNodePath::operator == ( mmXMLNodePath const & p_sNodePath ) const {
	return ( ToString() == p_sNodePath.ToString() );
}
//---------------------------------------------------------------------------

mmXML::mmXMLNodePath::operator mmXMLNodeID() const {
	return *m_lsiCurrent++;
}
//---------------------------------------------------------------------------

void mmXML::mmXMLNodePath::Reset( void ) const {
	m_lsiCurrent = m_lsPath.begin();
}
//---------------------------------------------------------------------------

bool mmXML::mmXMLNodePath::End( void ) const {
	return ( m_lsiCurrent == m_lsPath.end() );
}
//---------------------------------------------------------------------------

mmString mmXML::mmXMLNodePath::ToString( void ) const {
	std::wstringstream v_sOutput;
	std::list<mmXMLNodeID>::const_iterator v_lsiLast = m_lsPath.end();
	v_lsiLast--;
	for( std::list<mmXMLNodeID>::const_iterator v_lsiI = m_lsPath.begin(); v_lsiI != m_lsPath.end(); ++v_lsiI ) {
		v_sOutput << v_lsiI->tName;
		if( v_lsiI->iIndex )
			v_sOutput << L"," << v_lsiI->iIndex;
		if( v_lsiI != v_lsiLast )
			v_sOutput << L"/";
	}
	return v_sOutput.str();
}
//---------------------------------------------------------------------------

void mmXML::mmXMLNodePath::Reverse( void ) {
	m_lsPath.reverse();
	Reset();
}
//---------------------------------------------------------------------------

void mmXML::mmXMLNodePath::AddString( mmString const & p_usPath ) {
	int v_iPosI;
	mmXMLNodeID v_sNode;

	std::vector<mmString> v_vcNodeNames;
	Tokenize( p_usPath, L'/', v_vcNodeNames );
	for( std::vector<mmString>::iterator v_vciI = v_vcNodeNames.begin(); v_vciI != v_vcNodeNames.end(); ++v_vciI ) {
		if( ( v_iPosI = v_vciI->find_first_of( L"," ) ) > 0 ) {
			v_sNode.iIndex = StringToMMInt( v_vciI->substr( v_iPosI+1 ) );
			v_sNode.tName = v_vciI->substr( 0, v_iPosI );
		} else {
			v_sNode.iIndex = 0;
			v_sNode.tName = *v_vciI;
		}
		m_lsPath.push_back( v_sNode );
	}
}
//---------------------------------------------------------------------------

void mmXML::mmXMLNodePath::Norm( void ) {
	std::for_each( m_lsPath.begin(), m_lsPath.end(), MakeZero() );
}
//---------------------------------------------------------------------------

bool mmXML::IsValidNodePosition( mmXMLNodePosition p_xNodePosition ) {
	return ( p_xNodePosition.iGroupIndex >= 0 && p_xNodePosition.iLeafIndex >= 0 );
}
//---------------------------------------------------------------------------

mmXML::mmXMLPositionedNode mmXML::CreateAutomation( mmXMLDocI * _p_xDoc, mmXMLAutoType p_eXMLAutoType ) {
	mmXML::mmXMLNodeI * _v_xRootNode;
	mmXML::mmXMLPositionedNode v_xPositionedRootNode;

	if( p_eXMLAutoType == mmXML::g_eXMLAutoInput )
		_p_xDoc->CreateXMLRootNode( mmImages::g_pAutoCalcXML_INParams_Node );
	else if( p_eXMLAutoType == mmXML::g_eXMLAutoOutput )
		_p_xDoc->CreateXMLRootNode( mmImages::g_pAutoCalcXML_OUTParams_Node );
	_v_xRootNode = _p_xDoc->GetXMLRootNode();
	v_xPositionedRootNode._v_xNode = _v_xRootNode;
	v_xPositionedRootNode.sPosition.iGroupIndex = 0;
	v_xPositionedRootNode.sPosition.iLeafIndex = 1;

	return v_xPositionedRootNode;
}
//----------------------------------------------------------------------------------

mmXML::mmXMLPositionedNode mmXML::CreateAutomationInput( mmXMLDocI * _p_xDoc ) {
	return CreateAutomation( _p_xDoc, mmXML::g_eXMLAutoInput );
}
//----------------------------------------------------------------------------------

mmXML::mmXMLPositionedNode mmXML::CreateAutomationOutput( mmXMLDocI * _p_xDoc ) {
	return CreateAutomation( _p_xDoc, mmXML::g_eXMLAutoOutput );
}
//----------------------------------------------------------------------------------

mmXML::mmXMLNodePosition mmXML::AddParam( mmXMLPositionedNode * _p_xPositionedRootNode, mmString p_tName, mmXMLDataType p_eType, void * _p_DefaultValue ) {
	mmXML::mmXMLNodePosition v_xOutputP;

	v_xOutputP = AddParam( _p_xPositionedRootNode, p_tName, p_eType, mmXML::ConvertToString( p_eType, _p_DefaultValue ) );

	return v_xOutputP;
}
//----------------------------------------------------------------------------------

mmXML::mmXMLNodePosition mmXML::AddParam( mmXML::mmXMLPositionedNode * _p_xPositionedRootNode, mmString p_tName, mmXML::mmXMLDataType p_eType, mmString p_tDefaultValue ) {
	mmXML::mmXMLNodeI * _v_xParamNode, * _v_xNode, * _v_xRootNode;
	mmXML::mmXMLNodePosition v_xParamPosition;
	mmString v_tType;

	if( ( v_tType = GetTypeTransition( p_eType ) ) == L"" ) {
		v_xParamPosition.iLeafIndex = -1;
		v_xParamPosition.iGroupIndex = -1;
		return v_xParamPosition;
	}

	_v_xRootNode = _p_xPositionedRootNode->_v_xNode;
	_v_xParamNode = _v_xRootNode->AddChild( mmImages::g_pAutoCalcXML_Params_Param_Node );
		_v_xNode = _v_xParamNode->AddChild( mmImages::g_pAutoCalcXML_Params_ParamName_Node );
		_v_xNode->SetText( p_tName );
		_v_xNode = _v_xParamNode->AddChild( mmImages::g_pAutoCalcXML_Params_ParamType_Node );
		_v_xNode->SetText( v_tType );
		_v_xNode = _v_xParamNode->AddChild( mmImages::g_pAutoCalcXML_Params_ParamValue_Node );
		_v_xNode->SetText( p_tDefaultValue );

	v_xParamPosition.iLeafIndex = _p_xPositionedRootNode->sPosition.iGroupIndex++;
	v_xParamPosition.iGroupIndex = 0;

	return v_xParamPosition;
}
//----------------------------------------------------------------------------------

void mmXML::UpdateOutputParam( mmXML::mmXMLDocI * _p_xDoc, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams * _p_sAutomationParams, mmXML::mmXMLNodePosition p_xNodePosition, void * _p_OutputValue ) {
	mmXML::mmXMLNodeI * _v_xRootNode;
	mmString v_tOutputValue;

	_p_xDoc->ParseXMLBuffer( _p_sAutomationParams->sOutParams );
	_v_xRootNode = _p_xDoc->GetXMLRootNode();
	v_tOutputValue = mmXML::ConvertToString( mmXML::GetTypeTransition( _v_xRootNode->GetChild( p_xNodePosition.iLeafIndex )->FindChild( mmImages::g_pAutoCalcXML_Params_ParamType_Node )->GetText() ), _p_OutputValue );
	_v_xRootNode->GetChild( p_xNodePosition.iLeafIndex )->FindChild( mmImages::g_pAutoCalcXML_Params_ParamValue_Node )->SetText( v_tOutputValue );
	CopyOutputParams( _p_xDoc, _p_sAutomationParams );
}
//----------------------------------------------------------------------------------

void mmXML::UpdateOutputParam( mmXML::mmXMLDocI * _p_xDoc, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams * _p_sAutomationParams, mmXML::mmXMLNodePosition p_xNodePosition, mmString p_tOutputValue ) {
	mmXML::mmXMLNodeI * _v_xRootNode;

	_p_xDoc->ParseXMLBuffer( _p_sAutomationParams->sOutParams );
	_v_xRootNode = _p_xDoc->GetXMLRootNode();
	_v_xRootNode->GetChild( p_xNodePosition.iLeafIndex )->FindChild( mmImages::g_pAutoCalcXML_Params_ParamValue_Node )->SetText( p_tOutputValue );
	CopyOutputParams( _p_xDoc, _p_sAutomationParams );
}
//----------------------------------------------------------------------------------

void mmXML::CopyInputParams( mmXML::mmXMLDocI * _p_xDoc, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams * _p_sAutomationParams ) {
	mmString v_tInParams = _p_xDoc->SaveToXMLBuffer();
	mmInt v_iStrSize = v_tInParams._Copy_s( _p_sAutomationParams->sInParams, INOUT_PARAMS_SIZE, INOUT_PARAMS_SIZE, 0 );
	_p_sAutomationParams->sInParams[ v_iStrSize ] = 0;
}
//----------------------------------------------------------------------------------

void mmXML::CopyOutputParams( mmXML::mmXMLDocI * _p_xDoc, mmImages::mmImagesCalculationMethodI::sCalculationAutomationParams * _p_sAutomationParams ) {
	mmString v_tOutParams = _p_xDoc->SaveToXMLBuffer();
	mmInt v_iStrSize = v_tOutParams._Copy_s( _p_sAutomationParams->sOutParams, INOUT_PARAMS_SIZE, INOUT_PARAMS_SIZE, 0 );
	_p_sAutomationParams->sOutParams[ v_iStrSize ] = 0;
}
//----------------------------------------------------------------------------------

bool mmXML::SetValueByName( mmXML::mmXMLDocI * _p_xDoc, mmString p_tParamName, void * _p_InputPointer ) {
	mmXML::mmXMLNodeI * _v_xRootNode, * _v_xGroupNode, * _v_sValueNode;
	mmString v_tType, v_tValue, v_tTypeNodeName, v_tValueNodeName, v_tMainNodeName, v_tNameNodeName;

	_v_xRootNode = _p_xDoc->GetXMLRootNode();

	_v_xGroupNode = _v_xRootNode;
	v_tMainNodeName = mmImages::g_pAutoCalcXML_Params_Param_Node;
	v_tTypeNodeName = mmImages::g_pAutoCalcXML_Params_ParamType_Node;
	v_tValueNodeName = mmImages::g_pAutoCalcXML_Params_ParamValue_Node;
	v_tNameNodeName = mmImages::g_pAutoCalcXML_Params_ParamName_Node;

	std::vector<mmXML::mmXMLNodeI*> v_vcChildNodes = _v_xGroupNode->GetChildren();
	for( std::vector<mmXML::mmXMLNodeI*>::iterator v_vciI = v_vcChildNodes.begin(); v_vciI != v_vcChildNodes.end(); ++v_vciI ) {
		if( (*v_vciI)->FindChild( v_tNameNodeName )->GetText() == p_tParamName ) {
			_v_sValueNode = (*v_vciI)->FindChild( v_tValueNodeName );
			v_tType = (*v_vciI)->FindChild( v_tTypeNodeName )->GetText();
			break;
		}
	}
	if( _v_sValueNode == NULL )
		return false;

	_v_sValueNode->SetText( ConvertToString( GetTypeTransition( v_tType ), _p_InputPointer ) );

	return true;
}
//----------------------------------------------------------------------------------

bool mmXML::SetValue( mmXML::mmXMLDocI * _p_xDoc, mmXML::mmXMLNodePosition p_xNodePosition, void * _p_InputPointer ) {
	mmXML::mmXMLNodeI * _v_xRootNode, * _v_sNode;
	mmString v_tType, v_tValue;

	if( ! IsValidNodePosition( p_xNodePosition ) )
		return false;

	_v_xRootNode = _p_xDoc->GetXMLRootNode();

	_v_sNode = _v_xRootNode->GetChild( p_xNodePosition.iLeafIndex )->FindChild( mmImages::g_pAutoCalcXML_Params_ParamValue_Node );
	v_tType = _v_xRootNode->GetChild( p_xNodePosition.iLeafIndex )->FindChild( mmImages::g_pAutoCalcXML_Params_ParamType_Node )->GetText();

	_v_sNode->SetText( ConvertToString( GetTypeTransition( v_tType ), _p_InputPointer ) );

	return true;
}
//----------------------------------------------------------------------------------

bool mmXML::GetValueByName( mmXML::mmXMLDocI * _p_xDoc, mmString p_tParamName, void * _p_OutputPointer ) {
	mmXML::mmXMLNodeI * _v_xRootNode=NULL, * _v_xGroupNode=NULL, * _v_sNode=NULL, * _v_sValueNode=NULL;
	mmString v_tType, v_tValue, v_tTypeNodeName, v_tValueNodeName, v_tMainNodeName, v_tNameNodeName;
	mmInt * _v_iOutput;
	mmReal * _v_rOutput;
	bool * _v_bOutput;
	mmString * _v_tOutput;

	_v_xRootNode = _p_xDoc->GetXMLRootNode();
	_v_xGroupNode = _v_xRootNode;

	v_tMainNodeName = mmImages::g_pAutoCalcXML_Params_Param_Node;
	v_tTypeNodeName = mmImages::g_pAutoCalcXML_Params_ParamType_Node;
	v_tValueNodeName = mmImages::g_pAutoCalcXML_Params_ParamValue_Node;
	v_tNameNodeName = mmImages::g_pAutoCalcXML_Params_ParamName_Node;

	std::vector<mmXML::mmXMLNodeI*> v_vcChildNodes = _v_xGroupNode->GetChildren();
	for( std::vector<mmXML::mmXMLNodeI*>::iterator v_vciI = v_vcChildNodes.begin(); v_vciI != v_vcChildNodes.end(); ++v_vciI ) {
		if( ( _v_sNode = (*v_vciI)->FindChild( v_tNameNodeName ) ) != NULL && _v_sNode->GetText() == p_tParamName ) {
			v_tValue = (*v_vciI)->FindChild( v_tValueNodeName )->GetText();
			if( (*v_vciI)->FindChild( L"ProperType" ) != NULL )
				v_tType = (*v_vciI)->FindChild( L"ProperType" )->GetText();
			else
				v_tType = (*v_vciI)->FindChild( v_tTypeNodeName )->GetText();
			break;
		}
	}

	switch( GetTypeTransition( v_tType ) ) {
		case g_eXMLReal:
			_v_rOutput = static_cast<mmReal*>( _p_OutputPointer );
			*_v_rOutput = mmStringUtilities::StringToMMReal( v_tValue );
		break;
		case g_eXMLInt:
			_v_iOutput = static_cast<mmInt*>( _p_OutputPointer );
			*_v_iOutput = mmStringUtilities::StringToMMInt( v_tValue );
		break;
		case g_eXMLString:
		case g_eXMLImageName:
		case g_eXMLDataLayerName:
			_v_tOutput = static_cast<mmString*>( _p_OutputPointer );
			*_v_tOutput = v_tValue;
		break;
		case g_eXMLBool:
			_v_bOutput = static_cast<bool*>( _p_OutputPointer );
			*_v_bOutput = mmStringUtilities::StringToBool( v_tValue );
		break;
		default:
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------------

bool mmXML::GetValue( mmXML::mmXMLDocI * _p_xDoc, mmXML::mmXMLNodePosition p_xNodePosition, void * _p_OutputPointer ) {
	mmXML::mmXMLNodeI * _v_xRootNode;
	mmString v_tType, v_tValue;
	mmInt * _v_iOutput;
	mmReal * _v_rOutput;
	bool * _v_bOutput;
	mmString * _v_tOutput;

	if( ! IsValidNodePosition( p_xNodePosition ) )
		return false;

	_v_xRootNode = _p_xDoc->GetXMLRootNode();

	v_tValue = _v_xRootNode->GetChild( p_xNodePosition.iLeafIndex )->FindChild( mmImages::g_pAutoCalcXML_Params_ParamValue_Node )->GetText();
	v_tType = _v_xRootNode->GetChild( p_xNodePosition.iLeafIndex )->FindChild( mmImages::g_pAutoCalcXML_Params_ParamType_Node )->GetText();

	switch( GetTypeTransition( v_tType ) ) {
		case g_eXMLReal:
			_v_rOutput = static_cast<mmReal*>( _p_OutputPointer );
			*_v_rOutput = mmStringUtilities::StringToMMReal( v_tValue );
		break;
		case g_eXMLInt:
			_v_iOutput = static_cast<mmInt*>( _p_OutputPointer );
			*_v_iOutput = mmStringUtilities::StringToMMInt( v_tValue );
		break;
		case g_eXMLString:
		case g_eXMLImageName:
		case g_eXMLDataLayerName:
			_v_tOutput = static_cast<mmString*>( _p_OutputPointer );
			*_v_tOutput = v_tValue;
		break;
		case g_eXMLBool:
			_v_bOutput = static_cast<bool*>( _p_OutputPointer );
			*_v_bOutput = mmStringUtilities::StringToBool( v_tValue );
		break;
		default:
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------------

mmXML::mmXMLDataType mmXML::GetTypeTransition( mmString p_tType ) {
	if( p_tType.compare( mmImages::g_pAutoCalcXML_Params_ParamType_IntegerValue ) == 0 )
		return g_eXMLInt;
	else if( p_tType.compare( mmImages::g_pAutoCalcXML_Params_ParamType_RealValue ) == 0 )
		return g_eXMLReal;
	else if( p_tType.compare( mmImages::g_pAutoCalcXML_Params_ParamType_ImageName ) == 0 )
		return g_eXMLImageName;
	else if( p_tType.compare( mmImages::g_pAutoCalcXML_Params_ParamType_DataLayerName ) == 0 )
		return g_eXMLDataLayerName;
	else if( p_tType.compare( mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue ) == 0 )
		return g_eXMLBool;
	else if( p_tType.compare( mmImages::g_pAutoCalcXML_Params_ParamType_String ) == 0 )
		return g_eXMLString;
	return g_eXMLUnknownDataType;
}
//----------------------------------------------------------------------------------

mmString mmXML::GetTypeTransition( mmXML::mmXMLDataType p_eDataType ) {
	switch( p_eDataType ) {
		case mmXML::g_eXMLReal:
			return mmImages::g_pAutoCalcXML_Params_ParamType_RealValue;
		case mmXML::g_eXMLInt:
			return mmImages::g_pAutoCalcXML_Params_ParamType_IntegerValue;
		case mmXML::g_eXMLImageName:
			return mmImages::g_pAutoCalcXML_Params_ParamType_ImageName;
		case mmXML::g_eXMLDataLayerName:
			return mmImages::g_pAutoCalcXML_Params_ParamType_DataLayerName;
		case mmXML::g_eXMLBool:
			return mmImages::g_pAutoCalcXML_Params_ParamType_BoolValue;
		case mmXML::g_eXMLString:
			return mmImages::g_pAutoCalcXML_Params_ParamType_String;
		default:
			return L"";
    }
}
//----------------------------------------------------------------------------------

mmString mmXML::ConvertToString( mmXML::mmXMLDataType p_eType, void * _p_Data ) {
	if( _p_Data != NULL )
		switch( p_eType ) {
			case g_eXMLInt:
				return mmStringUtilities::MMIntToString( *static_cast<mmInt*>( _p_Data ) );
			case g_eXMLReal:
				return mmStringUtilities::MMRealToString( *static_cast<mmReal*>( _p_Data ) );
			case g_eXMLString:
			case g_eXMLImageName:
			case g_eXMLDataLayerName:
				return *static_cast<mmString*>( _p_Data );
			case g_eXMLBool:
				return mmStringUtilities::BoolToString( *static_cast<bool*>( _p_Data ) );
			default:
				return L"";
		}
	else
		return L"";
}
//----------------------------------------------------------------------------------

mmXML::mmXMLNodeI* mmXML::GetNodeAt( mmXMLNodeI* _p_sRootNode, mmXMLNodePath const & p_sNodePath, bool p_bCreateNode ) {
	mmXMLNodeI* _v_sXMLNode = _p_sRootNode, *_v_sTmpNode;
	mmXMLNodePath::mmXMLNodeID v_sNode;
	if( p_sNodePath.ToString() == L"" )
		return _p_sRootNode;
	p_sNodePath.Reset();
	while( ! p_sNodePath.End() ) {
		v_sNode = p_sNodePath;
		if( ( _v_sTmpNode = _v_sXMLNode->FindChild( v_sNode.tName ) ) != NULL )
			_v_sXMLNode = _v_sTmpNode;
		else
			if( p_bCreateNode )
				_v_sXMLNode = _v_sXMLNode->AddChild( v_sNode.tName );
			else
				return NULL;

		while( v_sNode.iIndex )
			if( ( _v_sTmpNode = _v_sXMLNode->GetNextSibling() ) != NULL )
				if( _v_sTmpNode->GetName() == v_sNode.tName ) {
					_v_sXMLNode = _v_sTmpNode;
					--v_sNode.iIndex;
				}
			else
				if( p_bCreateNode )
					_v_sXMLNode = _v_sXMLNode->GetParent()->AddChild( v_sNode.tName );
				else
					return NULL;
	}
	return _v_sXMLNode;
}
//---------------------------------------------------------------------------

