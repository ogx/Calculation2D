

//#include <StdAfx.h>
//#endif
// uwaga - StdAfx jest specyficzny dla visuala. Po jego za³¹czeniu nie mo¿na daæ #endif zamykaj¹cego #ifndef MADMAC

// uwaga2 - zdefiniowanie MADMAC odblokowuje logowanie i rzucanie wyjatkow
#define MADMAC
#include <xml\mmXMLBB.h>

#ifndef RELEASE_PTR
#define RELEASE_PTR(x)	\
	if( x != NULL )		\
		{					\
		x.Release();	\
		x = NULL;		\
		}
#endif



#ifdef MADMAC
#include <mmStringUtilities.h>
#include <mmInterfaceInitializers.h>
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmXML::mmXMLNodeBB
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int Is_MSXML_Node(MSXML2::IXMLDOMNodePtr pChild)
{     

	MSXML2::tagDOMNodeType v_i = ( pChild->nodeType);
        // I handle only few node type other are ignored and are considered as NODE_ELEMENT
        //
        // Handled Node type by this wrapper:
        //      - NODE_ELEMENT
        //      - NODE_TEXT
        //      - NODE_CDATA_SECTION
        //      - NODE_PROCESSING_INSTRUCTION

  //      if(pChild->nodeType == MSXML2::NODE_TEXT) return MSXML2::NODE_TEXT;
  //      if(pChild->nodeType == MSXML2::NODE_CDATA_SECTION) return MSXML2::NODE_CDATA_SECTION;
  //      if(pChild->nodeType == MSXML2::NODE_PROCESSING_INSTRUCTION ) return MSXML2::NODE_PROCESSING_INSTRUCTION;
		//return HRESULT( pChild->nodeType);
  //      return MSXML2::NODE_ELEMENT;
		return v_i;
}

#ifdef MADMAC
mmXML::mmXMLNodeBB::mmXMLNodeBB(								mmLog::mmLogReceiverI* p_psLogReceiver):
mmLog::mmLogSender(L"mmXML::mmXMLNodeBB",p_psLogReceiver)
{
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));

	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
}
#else
mmXML::mmXMLNodeBB::mmXMLNodeBB( )
{
	
}
#endif

mmXML::mmXMLNodeBB::~mmXMLNodeBB()
{
	//SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));

	RemoveAllChilds();
	
	//SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
}

mmString mmXML::mmXMLNodeBB::GetName(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetName"));

	SendLogMessage(mmLog::debug,mmString(L"End GetName Name=") +
															m_sNodeName);
#endif
	return m_sNodeName;
}

void mmXML::mmXMLNodeBB::SetName(mmString p_sName)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start SetName") +
															m_sNodeName);
#endif
	m_sNodeName = p_sName;
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End SetName"));
#endif
}

bool mmXML::mmXMLNodeBB::IsLeaf(void)
{
	bool v_bIsLeaf = true;
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start IsLeaf"));
#endif
	if(m_sChilds.size() != 0)
	{
		v_bIsLeaf = false;
	};

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End IsLeaf Result=") +
															mmStringUtilities::BoolToString(v_bIsLeaf));
#endif

	return v_bIsLeaf;
}

std::vector<mmXML::sXMLAttribute> mmXML::mmXMLNodeBB::GetAttributes(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetAttributes"));

	SendLogMessage(mmLog::debug,mmString(L"End GetAttributes"));
#endif
	return m_sAttributes;
}

void mmXML::mmXMLNodeBB::AddAttribute(mmString p_sAttrName,
																			mmString p_sAttrValue)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start AddAttribute Name=") +
															p_sAttrName +
															mmString(L" Value=") +
															p_sAttrValue);
#endif
	mmXML::sXMLAttribute v_sAttr;

	v_sAttr.sName = p_sAttrName;
	v_sAttr.sValue = p_sAttrValue;

	m_sAttributes.push_back(v_sAttr);

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End AddAttribute"));
#endif
}

#ifdef MADMAC
void mmXML::mmXMLNodeBB::RemoveAttribute(mmString p_sAttrName)
#else
bool mmXML::mmXMLNodeBB::RemoveAttribute(mmString p_sAttrName)
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start RemoveAttribute Name=") +
															p_sAttrName);
#endif;

	mmInt v_i,v_iAttrCount,v_iRemovedCount;
	std::vector<sXMLAttribute> v_sNewAttributes;

	v_iRemovedCount = 0;
	v_iAttrCount = static_cast<mmInt>(m_sAttributes.size());
	for(v_i=0;v_i<v_iAttrCount;v_i++)
	{
#ifdef MADMAC
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
#else
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
#endif
		{
			v_iRemovedCount++;
		}
		else
		{
			v_sNewAttributes.push_back(m_sAttributes[v_i]);
		};
	};

	m_sAttributes = v_sNewAttributes;

	if(v_iRemovedCount == 0)
	{
		#ifdef MADMAC
			SendLogMessage(mmLog::critical,mmString(L"RemoveAttribute NoSuchAttribute"));

			throw mmError(mmeXMLNoSuchAttribute);
		#else
			return false;
		#endif

	};
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End RemoveAttribute"));
#else
	return true;
#endif
}

void mmXML::mmXMLNodeBB::RemoveAllAttributes(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start RemoveAllAttributes"));
#endif
	m_sAttributes.clear();
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End RemoveAllAttributes"));
#endif
}

bool mmXML::mmXMLNodeBB::IsAttribute(mmString p_sAttrName)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start IsAttribute Name=") +
															p_sAttrName);
#endif

	mmInt v_i,v_iAttrCount;
	bool v_bIsAttr = false;

	v_iAttrCount = static_cast<mmInt>(m_sAttributes.size());
	for(v_i=0;v_i<v_iAttrCount;v_i++)
	{
#ifdef MADMAC
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
#else
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
#endif
		{
			v_bIsAttr = true;
			break;
		};
	};
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End IsAttribute Result=") +
															mmStringUtilities::BoolToString(v_bIsAttr));
#endif
	return v_bIsAttr;
}

#ifdef MADMAC
mmString mmXML::mmXMLNodeBB::GetAttributeValue(mmString p_sAttrName)
#else
mmString mmXML::mmXMLNodeBB::GetAttributeValue(mmString p_sAttrName)
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetAttributeValue Name=") +
															p_sAttrName);
#endif

	mmInt v_i,v_iAttrCount,v_iAttrIndex;

	v_iAttrIndex = -1;
	v_iAttrCount = static_cast<mmInt>(m_sAttributes.size());
	for(v_i=0;v_i<v_iAttrCount;v_i++)
	{
#ifdef MADMAC
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
#else
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
#endif
		{
			v_iAttrIndex = v_i;
			break;
		};
	};

	if(v_iAttrIndex == -1)
	{
#ifdef MADMAC
		SendLogMessage(mmLog::critical,mmString(L"GetAttributeValue NoSuchAttribute"));

		throw mmError(mmeXMLNoSuchAttribute);
#else
		mmString v_sEmptyString;
		v_sEmptyString = "";
		return v_sEmptyString;
#endif
	};

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End GetAttributeValue Value=") +
															m_sAttributes[v_iAttrIndex].sValue);
#endif

	return m_sAttributes[v_iAttrIndex].sValue;
}

void mmXML::mmXMLNodeBB::SetAttributeValue(mmString p_sAttrName,mmString p_sAttrValue)
{

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start SetAttributeValue Name=") +
															p_sAttrName +
															mmString(L" Value=") +
															p_sAttrValue);
#endif
	mmInt v_i,v_iAttrCount,v_iAttrIndex;

	v_iAttrIndex = -1;
	v_iAttrCount = static_cast<mmInt>(m_sAttributes.size());
	for(v_i=0;v_i<v_iAttrCount;v_i++)
	{
#ifdef MADMAC
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
#else
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
#endif
		{
			v_iAttrIndex = v_i;
			break;
		};
	};

	if(v_iAttrIndex == -1)
	{
		mmXML::sXMLAttribute v_sAttr;

		v_sAttr.sName = p_sAttrName;
		v_sAttr.sValue = p_sAttrValue;

		m_sAttributes.push_back(v_sAttr);
	}
	else
	{
		m_sAttributes[v_iAttrIndex].sValue = p_sAttrValue;
	};

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End SetAttributeValue"));
#endif
}

mmString mmXML::mmXMLNodeBB::GetText(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetText"));

	SendLogMessage(mmLog::debug,mmString(L"End GetText"));
#endif
	return m_sNodeValue;
}

void mmXML::mmXMLNodeBB::SetText(mmString p_sText)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start SetText"));
#endif
	m_sNodeValue = p_sText;
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End SetText"));
#endif
}

std::vector<mmXML::mmXMLNodeI*> mmXML::mmXMLNodeBB::GetChilds(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetChilds"));

	SendLogMessage(mmLog::debug,mmString(L"End GetChilds"));
#endif
	return m_sChilds;
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::AddChild(mmString p_sChildName)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start AddChild Name=") +
															p_sChildName);
	
	mmXMLNodeBB* v_psNode = new mmXML::mmXMLNodeBB(GetLogReceiver());
#else
	mmXMLNodeBB* v_psNode = new mmXML::mmXMLNodeBB();
#endif

	v_psNode->m_sNodeName = p_sChildName;
	v_psNode->m_psParent = this;
	m_sChilds.push_back(v_psNode);

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End AddChild"));
#endif
	return v_psNode;
}

void mmXML::mmXMLNodeBB::AddChildWithStructure(mmXMLNodeI* p_psChildNode)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start AddChildWithStructure"));
	mmXMLNodeBB* v_psNode = new mmXML::mmXMLNodeBB(GetLogReceiver());
#else
	mmXMLNodeBB* v_psNode = new mmXML::mmXMLNodeBB();
#endif

	v_psNode->m_psParent = p_psChildNode->GetParent();
	v_psNode->m_sNodeName = p_psChildNode->GetName();
	v_psNode->m_sNodeValue = p_psChildNode->GetText();
	v_psNode->m_sAttributes = p_psChildNode->GetAttributes();

	std::vector<mmXML::mmXMLNodeI*> v_vChilds = p_psChildNode->GetChilds();
	mmInt v_iChildsCount = v_vChilds.size();
	for(mmInt v_iC=0;v_iC<v_iChildsCount;v_iC++)
	{
		v_psNode->AddChildWithStructure(v_vChilds[v_iC]);
	};

	m_sChilds.push_back(v_psNode);
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End AddChildWithStructure"));
#endif
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::FindChild(mmString p_sChildName)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start FindChild Name=")+
															p_sChildName);
#endif

	mmInt v_i,v_iChildCount;
	mmXML::mmXMLNodeI* v_psFoundNode = NULL;

	v_iChildCount = static_cast<mmInt>(m_sChilds.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
#ifdef MADMAC
		if(m_sChilds[v_i]->GetName().compare(p_sChildName) == 0)
#else
		if(m_sChilds[v_i]->GetName().compare(p_sChildName) == 0)
#endif
		{
			v_psFoundNode = m_sChilds[v_i];
			break;
		};
	};
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End FindChild"));
#endif
	return v_psFoundNode;
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::FindNextChild(mmXML::mmXMLNodeI* p_psCurrentChild, mmString p_sChildName)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start FindChild Name=")+
															p_sChildName);
#endif

	mmInt v_i,v_iChildCount;
	mmXML::mmXMLNodeI* v_psFoundNode = NULL;

	v_iChildCount = static_cast<mmInt>(m_sChilds.size());
	bool v_bCurrentNodeFound = false;

	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
#ifdef MADMAC
		if(m_sChilds[v_i]->GetName().compare(p_sChildName) == 0)
#else
		if(m_sChilds[v_i]->GetName().compare(p_sChildName) == 0)
#endif
		{
			if (( v_bCurrentNodeFound == true) )
			{
				v_psFoundNode = m_sChilds[v_i];
				break;
			}
			if ( (p_psCurrentChild == m_sChilds[ v_i ] ) && ( v_bCurrentNodeFound == false) )
				v_bCurrentNodeFound = true;
		
		};
	};
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End FindChild"));
#endif
	return v_psFoundNode;
}

#ifdef MADMAC
mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetChild(mmInt p_iChildIndex)
#else
mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetChild(mmInt p_iChildIndex)
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetChild"));
#endif

	mmInt v_i,v_iChildCount;
	mmXML::mmXMLNodeI* v_psFoundNode = NULL;

	v_iChildCount = static_cast<mmInt>(m_sChilds.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
		if(v_i == p_iChildIndex)
		{
			v_psFoundNode = m_sChilds[v_i];
		};
	};
#ifdef MADMAC
	if(v_psFoundNode == NULL)
	{
		SendLogMessage(mmLog::critical,mmString(L"GetChild NoSuchChild"));

		throw mmError(mmeXMLNoSuchNode);
	};


	SendLogMessage(mmLog::debug,mmString(L"End GetChild"));
#endif

	return v_psFoundNode;
}

#ifdef MADMAC
void mmXML::mmXMLNodeBB::RemoveChild(mmString p_sChildName)
#else
bool mmXML::mmXMLNodeBB::RemoveChild(mmString p_sChildName)
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start RemoveChild Name=")+
															p_sChildName);
#endif
	mmInt v_i,v_iChildCount,v_iRemovedCount;
	std::vector<mmXML::mmXMLNodeI*> v_sNewChilds;

	v_iRemovedCount = 0;
	v_iChildCount = static_cast<mmInt>(m_sChilds.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
#ifdef MADMAC		
		if(m_sChilds[v_i]->GetName().compare(p_sChildName) == 0)
#else
		if(m_sChilds[v_i]->GetName().compare(p_sChildName) == 0)
#endif
		{
			v_iRemovedCount++;

			delete m_sChilds[v_i];
		}
		else
		{
			v_sNewChilds.push_back(m_sChilds[v_i]);
		};
	};

	m_sChilds = v_sNewChilds;

	if(v_iRemovedCount == 0)
	{
#ifdef MADMAC
		SendLogMessage(mmLog::critical,mmString(L"RemoveChild NoSuchChild"));

		throw mmError(mmeXMLNoSuchNode);
#else
		return false;
#endif
	};

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End RemoveChild RemovedCount=") +
															mmStringUtilities::MMIntToString(v_iRemovedCount));
#else
	return true;
#endif
}

#ifdef MADMAC
void mmXML::mmXMLNodeBB::RemoveChild(mmInt p_iChildIndex)
#else
bool mmXML::mmXMLNodeBB::RemoveChild(mmInt p_iChildIndex)
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start RemoveChild"));
#endif
	mmInt v_i,v_iChildCount;
	bool v_bIsRemoved = false;
	std::vector<mmXML::mmXMLNodeI*> v_sNewChilds;

	v_iChildCount = static_cast<mmInt>(m_sChilds.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
		if(v_i == p_iChildIndex)
		{
			v_bIsRemoved = true;
			delete m_sChilds[v_i];
		}
		else
		{
			v_sNewChilds.push_back(m_sChilds[v_i]);
		};
	};

	m_sChilds = v_sNewChilds;

	if(!v_bIsRemoved)
	{
		#ifdef MADMAC
			SendLogMessage(mmLog::critical,mmString(L"RemoveChild NoSuchChild"));

			throw mmError(mmeXMLNoSuchNode);
		#else
			return false;
		#endif
	};

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End RemoveChild"));
#else
	return true;
#endif
}

void mmXML::mmXMLNodeBB::RemoveAllChilds(void)
{

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start RemoveAllChilds"));
#endif

	mmInt v_i,v_iChildCount;

	v_iChildCount = static_cast<mmInt>(m_sChilds.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
		delete m_sChilds[v_i];
	};
	m_sChilds.clear();

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End RemoveAllChilds"));
#endif

}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetParent( void ) {
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetParent"));
	SendLogMessage(mmLog::debug,mmString(L"End GetParent"));
#endif
	return m_psParent;
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetNextSibling( void ) {
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End GetNextSibling"));
#endif
	if( m_psParent == NULL )
    	return NULL;
	std::vector<mmXML::mmXMLNodeI*> v_sChildNodes = m_psParent->GetChilds();
	mmXML::mmXMLNodeI* v_psNode = NULL;
	for( std::vector<mmXML::mmXMLNodeI*>::iterator v_sI = v_sChildNodes.begin(); v_sI != v_sChildNodes.end()-1; ++v_sI ) {
		if( *v_sI == static_cast<mmXML::mmXMLNodeI*>(this) ) {
			v_psNode = *(v_sI+1);
			break;
		}
	}
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End GetNextSibling"));
#endif
	return v_psNode;
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetPrevSibling( void ) {
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End GetPrevSibling"));
#endif	
	if( m_psParent == NULL )
    	return NULL;
	std::vector<mmXML::mmXMLNodeI*> v_sChildNodes = m_psParent->GetChilds();
	mmXML::mmXMLNodeI* v_psNode = NULL;
	for( std::vector<mmXML::mmXMLNodeI*>::iterator v_sI = v_sChildNodes.begin()+1; v_sI != v_sChildNodes.end(); ++v_sI ) {
		if( *v_sI == static_cast<mmXML::mmXMLNodeI*>(this) ) {
			v_psNode = *(v_sI-1);
			break;
		}
	}
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End GetPrevSibling"));
#endif
	return v_psNode;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmXML::mmXMLDocBB
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef MADMAC
mmXML::mmXMLDocBB::mmXMLDocBB(mmLog::mmLogReceiverI *p_psLogReceiver):
mmLog::mmLogSender(L"mmXML::mmXMLBB",p_psLogReceiver)
#else
mmXML::mmXMLDocBB::mmXMLDocBB()
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start Constructor"));
#endif
	//CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	m_bOLEInitialized = false;
	InitializeXML();
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End Constructor"));
#endif
}

mmXML::mmXMLDocBB::~mmXMLDocBB()
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start Destructor"));
#endif
	
	this->Reset_XML_Document();
	m_psXMLBBDoc.Release();
	CoUninitialize();
#ifdef MADMAC
	

	SendLogMessage(mmLog::debug,mmString(L"End Destructor"));
#endif
}

#ifdef MADMAC
void mmXML::mmXMLDocBB::ParseXMLFile(mmString p_sXMLFileName)
#else
bool mmXML::mmXMLDocBB::ParseXMLFile(mmString p_sXMLFileName)
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start ParseXMLFile File=") +
															p_sXMLFileName);
#endif
	// delete XML structure
	m_sRootNode.RemoveAllChilds();
	m_sRootNode.RemoveAllAttributes();

	// load XMLDocument

	if (Load_XML_Document(p_sXMLFileName) == false)
	
#ifdef MADMAC
	{

		SendLogMessage(mmLog::critical,mmString(L"ParseXMLFile Error") );

		throw mmError(mmeXMLParseError);

	};
#else


		return false;
#endif

	// refresh XML structure from XMLDocument
	LoadNodeStructureFromXMLDocument();
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End ParseXMLFile"));
#else
	return true;
#endif
}
#ifdef MADMAC
void mmXML::mmXMLDocBB::ParseXMLBuffer(mmString p_sXMLBuffer)
#else
bool mmXML::mmXMLDocBB::ParseXMLBuffer(mmString p_sXMLBuffer)
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start ParseXMLBuffer"));
#endif
	// delete XML structure
	m_sRootNode.RemoveAllChilds();
	m_sRootNode.RemoveAllAttributes();

	// load XMLDocument
	try
	{
		this->Load_XML_From_Buffer(p_sXMLBuffer);
	}
#ifdef MADMAC
	catch(...)
	{
		SendLogMessage(mmLog::critical,mmString(L"ParseXMLFile Error"));

		throw mmError(mmeXMLParseError);

	}
#else
	catch(...)
	{

		return false;


	};
#endif
	// refresh XML structure from XMLDocument
	LoadNodeStructureFromXMLDocument();
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End ParseXMLBuffer"));
#else
	return true;
#endif
}

#ifdef MADMAC
void mmXML::mmXMLDocBB::SaveToXMLFile(mmString p_sXMLFileName)
#else
bool mmXML::mmXMLDocBB::SaveToXMLFile(mmString p_sXMLFileName)
#endif
{

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start SaveToXMLFile File=") +
															p_sXMLFileName);
#endif

	// save XML structure into XMLDocument
	SaveNodeStructureIntoXMLDocument();

	// using XMLDocument API
	mmString v_sOutString = GetXMLFormattedString();
	
	// opening TXT file
#ifdef MADMAC
	mmFileIO::mmTextFileWriteI* v_psTxtFile = mmInterfaceInitializers::CreateTextFileForWrite(this->GetLogReceiver());
	v_psTxtFile->Open(p_sXMLFileName,mmFileIO::trunc_for_write);
	v_psTxtFile->WriteLine(v_sOutString);
	delete v_psTxtFile;
#else
	m_psXMLBBDoc->save( _bstr_t( p_sXMLFileName.c_str() ) );
#endif
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End SaveToXMLFile"));
#else
	return true;
#endif
}

mmString mmXML::mmXMLDocBB::SaveToXMLBuffer(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start SaveToXMLBuffer"));
#endif
	// save XML structure into XMLDocument
	SaveNodeStructureIntoXMLDocument();

	// using XMLDocument API
	mmString v_sOutString = GetXMLFormattedString();
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End SaveToXMLBuffer"));
#endif
	return v_sOutString;
}

bool mmXML::mmXMLDocBB::IsXMLStructureValid(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start IsXMLStructureValid"));
#endif
	bool v_bIsValid = true;
	// TODO: validate XML structure

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End IsXMLStructureValid"));
#endif
	return v_bIsValid;
}

#ifdef MADMAC
mmXML::mmXMLNodeI* mmXML::mmXMLDocBB::GetXMLRootNode(void)
#else
mmXML::mmXMLNodeI* mmXML::mmXMLDocBB::GetXMLRootNode( void )
#endif
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetXMLRootNode"));

	SendLogMessage(mmLog::debug,mmString(L"End GetXMLRootNode"));
#endif;
	return &m_sRootNode;
}

void mmXML::mmXMLDocBB::CreateXMLRootNode(mmString p_sRootNodeName)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start CreateXMLRootNode NodeName=") +
															p_sRootNodeName);
#endif
	m_sRootNode.RemoveAllChilds();
	m_sRootNode.RemoveAllAttributes();
	m_sRootNode.SetName(p_sRootNodeName);
	m_sRootNode.SetText(mmString(L""));

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End CreateXMLRootNode"));
#endif
}

void mmXML::mmXMLDocBB::CopyXMLRootNodeStructure(mmXMLNodeI* p_psNode)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start CopyXMLRootNodeStructure"));
#endif
	CreateXMLRootNode(p_psNode->GetName());

	std::vector<mmXML::sXMLAttribute> v_vRootAttributes = p_psNode->GetAttributes();
	mmInt v_iAttrCount = v_vRootAttributes.size();
	for(mmInt v_iA=0;v_iA<v_iAttrCount;v_iA++)
	{
		m_sRootNode.AddAttribute(v_vRootAttributes[v_iA].sName,
														 v_vRootAttributes[v_iA].sValue);
	};

	std::vector<mmXML::mmXMLNodeI*> v_vChilds = p_psNode->GetChilds();
	mmInt v_iChildsCount = v_vChilds.size();
	for(mmInt v_iC=0;v_iC<v_iChildsCount;v_iC++)
	{
		m_sRootNode.AddChildWithStructure(v_vChilds[v_iC]);
	};
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End CopyXMLRootNodeStructure"));
#endif
}



void mmXML::mmXMLDocBB::FillXMLNodeIntoXMLDocument(MSXML2::IXMLDOMNodePtr p_sBBNode,mmXMLNodeI* p_psMMNode)
{


#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start FillXMLNodeIntoXMLDocument"));
#endif

	// adding attributes
	mmInt v_i,v_iCount;

	std::vector<mmXML::sXMLAttribute> v_sAttribs = p_psMMNode->GetAttributes();
	v_iCount = v_sAttribs.size();


	for(v_i=0;v_i<v_iCount;v_i++)
	{
		MSXML2::IXMLDOMNamedNodeMap*  pXMLNamedNodeMap;

		HRESULT hr;
		hr = p_sBBNode->get_attributes( &pXMLNamedNodeMap );

		MSXML2::IXMLDOMNodePtr pAtty = 
			m_psXMLBBDoc->createNode( _variant_t( (long)MSXML2::NODE_ATTRIBUTE), _bstr_t(v_sAttribs[ v_i ].sName.c_str()), ""); 

		pAtty->put_text( _bstr_t(v_sAttribs[ v_i ].sValue.c_str()) ); 
		pXMLNamedNodeMap->setNamedItem( pAtty);

	};


	// adding child nodes or text values for leaves
	std::vector<mmXML::mmXMLNodeI*> v_sChilds = p_psMMNode->GetChilds();
	v_iCount = v_sChilds.size();
	if(v_iCount > 0)
	{
		// childs
		for(v_i=0;v_i<v_iCount;v_i++)
		{
			MSXML2::IXMLDOMNodePtr v_sNewNode =  m_psXMLBBDoc->createNode(MSXML2::NODE_ELEMENT, _bstr_t(v_sChilds[v_i]->GetName().c_str()), "");


			v_sNewNode = p_sBBNode->appendChild(v_sNewNode);
			FillXMLNodeIntoXMLDocument(v_sNewNode,v_sChilds[v_i]);
		};
	}
	else
	{
		// text
		p_sBBNode->put_text(_bstr_t(p_psMMNode->GetText().c_str()));
	};
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End FillXMLNodeIntoXMLDocument"));
#endif
}


void mmXML::mmXMLDocBB::SaveNodeStructureIntoXMLDocument(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start SaveNodeStructureIntoXMLDocument"));
#endif
	// delete existing XMLDocument
	m_psXMLBBDoc.Release();

	// create new one
	InitializeXML();

	wchar_t v_pcXML[1024];
	
	swprintf_s(v_pcXML, 1023, L"%s <%s></%s>", L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>", m_sRootNode.GetName().c_str(), m_sRootNode.GetName().c_str());
	
	m_psXMLBBDoc->loadXML(_bstr_t( v_pcXML ));
	m_psDocRoot = m_psXMLBBDoc->documentElement;

	 
	FillXMLNodeIntoXMLDocument(m_psDocRoot,&m_sRootNode);
	m_psXMLBBDoc->documentElement = m_psDocRoot;

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End SaveNodeStructureIntoXMLDocument"));
#endif
}

mmString mmXML::mmXMLDocBB::GetXMLFormattedString(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start GetXMLFormattedString"));
#endif
	mmString v_sOutString(L"");

	// formatowanie

		if( m_psXMLBBDoc == NULL )
		{
			return v_sOutString;
		}

		HRESULT hr = S_OK;

		MSXML2::IMXWriterPtr pMXWriter = NULL;
		MSXML2::ISAXXMLReaderPtr pSAXReader = NULL;
		MSXML2::ISAXContentHandlerPtr pISAXContentHandler = NULL;
		MSXML2::ISAXErrorHandlerPtr pISAXErrorHandler = NULL;
		MSXML2::ISAXDTDHandlerPtr pISAXDTDHandler = NULL;
		try
		{
		
			// create
			hr = pMXWriter.CreateInstance( __uuidof(MSXML2::MXXMLWriter));
			if( FAILED(hr) )
			{ 
				RELEASE_PTR(pISAXDTDHandler);
				RELEASE_PTR(pISAXErrorHandler);
				RELEASE_PTR(pISAXContentHandler);
				RELEASE_PTR(pSAXReader);
				RELEASE_PTR(pMXWriter);
			}
			
			hr = pSAXReader.CreateInstance( __uuidof(MSXML2::SAXXMLReader));
			if( FAILED(hr) )
			{
				RELEASE_PTR(pISAXDTDHandler);
				RELEASE_PTR(pISAXErrorHandler);
				RELEASE_PTR(pISAXContentHandler);
				RELEASE_PTR(pSAXReader);
				RELEASE_PTR(pMXWriter);
			}

			// save in formatted
			pISAXContentHandler = pMXWriter;
			pISAXErrorHandler = pMXWriter;
			pISAXDTDHandler = pMXWriter;

			if (FAILED (pMXWriter->put_omitXMLDeclaration (VARIANT_FALSE)) ||
				FAILED (pMXWriter->put_standalone (VARIANT_FALSE)) ||
				FAILED (pMXWriter->put_indent (VARIANT_TRUE)) ||
				FAILED (pMXWriter->put_encoding(_bstr_t("UTF-8")) )
				)
			{
				
				RELEASE_PTR(pISAXDTDHandler);
				RELEASE_PTR(pISAXErrorHandler);
				RELEASE_PTR(pISAXContentHandler);
				RELEASE_PTR(pSAXReader);
				RELEASE_PTR(pMXWriter);
			}

			if (FAILED(pSAXReader->putContentHandler (pISAXContentHandler)) ||
				FAILED(pSAXReader->putDTDHandler (pISAXDTDHandler)) ||
				FAILED(pSAXReader->putErrorHandler (pISAXErrorHandler)) ||
				FAILED(pSAXReader->putProperty ((unsigned short*)L"http://xml.org/sax/properties/lexical-handler", _variant_t(pMXWriter.GetInterfacePtr()))) ||
				FAILED(pSAXReader->putProperty ((unsigned short*)L"http://xml.org/sax/properties/declaration-handler", _variant_t(pMXWriter.GetInterfacePtr()))))
			{
				RELEASE_PTR(pISAXDTDHandler);
				RELEASE_PTR(pISAXErrorHandler);
				RELEASE_PTR(pISAXContentHandler);
				RELEASE_PTR(pSAXReader);
				RELEASE_PTR(pMXWriter);
			}
						
			IStream * pOutStream = NULL;
			::CreateStreamOnHGlobal( NULL, TRUE, &pOutStream);

			_bstr_t m_sString;
			_variant_t out;
			HGLOBAL hg = NULL;
			hr = pMXWriter->put_output(_variant_t(pOutStream));
			GetHGlobalFromStream(pOutStream,&hg);	
			void * output;
			output = GlobalLock(hg);

			 
			
			if( FAILED(hr) )
			{
				RELEASE_PTR(pISAXDTDHandler);
				RELEASE_PTR(pISAXErrorHandler);
				RELEASE_PTR(pISAXContentHandler);
				RELEASE_PTR(pSAXReader);
				RELEASE_PTR(pMXWriter); 
			}

			hr = pSAXReader->parse(m_psXMLBBDoc.GetInterfacePtr());
			if( FAILED(hr) )
			{ 
				RELEASE_PTR(pISAXDTDHandler);
				RELEASE_PTR(pISAXErrorHandler);
				RELEASE_PTR(pISAXContentHandler);
				RELEASE_PTR(pSAXReader);
				RELEASE_PTR(pMXWriter);
			}
			
	// zapisz do stringa
		HRESULT hr = S_OK;
		LPVOID pOutput = NULL;
		HGLOBAL hGlobal = NULL;

		hr = ::GetHGlobalFromStream( pOutStream, &hGlobal);
		if( FAILED(hr) ) 
		{ 
			return v_sOutString; 
		}
		::GlobalUnlock(hGlobal);
		LARGE_INTEGER llStart = {0, 0};
		ULARGE_INTEGER ullSize = {0, 0};
		pOutStream->Seek( llStart, STREAM_SEEK_CUR, &ullSize);
		pOutput = ::GlobalLock(hGlobal);


		char * v_pcText = new char[ int(ullSize.QuadPart) + 1 ];
		ZeroMemory( v_pcText, int(ullSize.QuadPart) + 1 );
		memcpy( v_pcText, (const char * ) pOutput, int(ullSize.QuadPart) );

		v_sOutString.clear();
		v_sOutString.append( (mmStringUtilities::MMCharStringToMMString(v_pcText)).c_str());
		


		::GlobalUnlock(hGlobal);
		delete [] v_pcText;
		
		}
		catch ( _com_error e )
		{
//			TRACE( _T("CXml::SaveWithFormatted( %s ) failed:%s\n"), lpszFilePath, e.ErrorMessage());
			hr = e.Error();
		}
		RELEASE_PTR(pISAXDTDHandler);
		RELEASE_PTR(pISAXErrorHandler);
		RELEASE_PTR(pISAXContentHandler);
		RELEASE_PTR(pSAXReader);
		RELEASE_PTR(pMXWriter);
	
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End GetXMLFormattedString"));
#endif
	
	return v_sOutString;
}

void mmXML::mmXMLDocBB::FillXMLNodeFromXMLDocument(MSXML2::IXMLDOMNodePtr p_sBBNode, mmXMLNodeI* p_psMMNode)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start FillXMLNodeFromXMLDocument"));
#endif
	// adding attributes
	mmInt v_i,v_iCount;
	sXMLAttribute v_sXMLAttr;

	v_iCount = p_sBBNode->Getattributes()->Getlength();
	for(v_i=0;v_i<v_iCount;v_i++)
	{
		v_sXMLAttr.sName = (mmString)(const wchar_t *)(p_sBBNode->Getattributes()->Getitem( v_i )->GetnodeName());
		v_sXMLAttr.sValue = (mmString)(const wchar_t *) _bstr_t (p_sBBNode->Getattributes()->Getitem( v_i )->GetnodeValue());

		p_psMMNode->AddAttribute(v_sXMLAttr.sName,v_sXMLAttr.sValue);
	};

	// adding child nodes or text values for leafs
	v_iCount = p_sBBNode->GetchildNodes()->Getlength();

	if(v_iCount > 0)
	{
		// bylo 0!
	
		if( (v_iCount == 1) && ( Is_MSXML_Node(p_sBBNode->GetfirstChild()) == MSXML2::NODE_TEXT)  )
		{
			// text
			p_psMMNode->SetText((mmString)(const wchar_t *)(p_sBBNode->Gettext()));
		}
		else
		{
			// childs
			for(v_i=0;v_i<v_iCount;v_i++)
			{
				mmXMLNodeI* v_psChildNode = p_psMMNode->AddChild((mmString)(const wchar_t *)(p_sBBNode->GetchildNodes()->Getitem(v_i)->GetnodeName()));
				 
				FillXMLNodeFromXMLDocument(p_sBBNode->GetchildNodes()->Getitem(v_i),v_psChildNode);
			};
		};
	};
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End FillXMLNodeFromXMLDocument"));
#endif
}

void mmXML::mmXMLDocBB::LoadNodeStructureFromXMLDocument(void)
{
#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"Start LoadNodeStructureFromXMLDocument"));
#endif
	MSXML2::IXMLDOMElementPtr v_sRootNode = m_psXMLBBDoc->documentElement;
	m_sRootNode.SetName((mmStringUtilities::MMCharStringToMMString((const char *) v_sRootNode->GetnodeName())));
//	m_sRootNode.SetName((mmString((const wchar_t *) v_sRootNode->GetnodeName())));
	FillXMLNodeFromXMLDocument(v_sRootNode,&m_sRootNode);

#ifdef MADMAC
	SendLogMessage(mmLog::debug,mmString(L"End LoadNodeStructureFromXMLDocument"));
#endif
}


bool mmXML::mmXMLDocBB::InitializeXML()
{
       m_sLastError = L"ok";

        // -- Init OLE Object Comunication for MSXML (only once time)--
        //
       
        if(!m_bOLEInitialized)
        {       CoInitializeEx(NULL, COINIT_APARTMENTTHREADED );
                m_bOLEInitialized = true;
        }

        // -- Instantiate an XML document --
        //
		HRESULT hr = m_psXMLBBDoc.CreateInstance(MSXML2::CLSID_DOMDocument);
        if (FAILED(hr))
		{
			// coinit not called
			if (hr == -2147221008)
			{
				m_bOLEInitialized = false;
				InitializeXML();
			}
			else
                return false;
        }

        return true;
}
void mmXML::mmXMLDocBB::Reset_XML_Document()
{
        // Init
        //
        this->InitializeXML();
        m_psDocRoot = m_psXMLBBDoc->documentElement;
        

}
bool mmXML::mmXMLDocBB::Load_XML_Document(mmString strFileName)
{       
		m_sLastError = L"ok";

        // Reset Document
        //
        this->Reset_XML_Document();

        // Convert xml file name string to something COM can handle (BSTR)
        //
        //mmString sFileName = strFileName;
        //_bstr_t bstrFileName;
        //bstrFileName = sFileName.AllocSysString();

        // Call the MSXML2::IXMLDOMDocumentPtr's load function to load the XML document
        //
        variant_t vResult;
		// UWAGA - VS 2008 SP1 - load nie dziala, trzeba loadXML
		vResult = m_psXMLBBDoc->load (_bstr_t(strFileName.c_str()));
        if (((bool)vResult) == TRUE) // success!
        {
                // Now that the document is loaded, we need to initialize the root pointer
                //
                m_psDocRoot = m_psXMLBBDoc->documentElement;

                

                return true;
        }
        else
        {       // XML document is not loaded, return error
                //
                m_sLastError = L"XML Document FAILED to load";
                return false;
        }
}

void mmXML::mmXMLDocBB::Load_XML_From_Buffer(mmString source)
{
        // Reset the document
        //
        this->Reset_XML_Document();
		
        // Load from Buffer
        //
		variant_t vResult;
		// UWAGA - VS 2008 SP1 - load nie dziala, trzeba loadXML
		vResult = m_psXMLBBDoc->loadXML(_bstr_t(source.c_str()));
        if (((bool)vResult) == TRUE) // success!
        {
                // Now that the document is loaded, we need to initialize the root pointer
                //
                m_psDocRoot = m_psXMLBBDoc->documentElement;

                

                return ;
        }
        else
        {       // XML document is not loaded, return error
                //
			    MSXML2::IXMLDOMParseErrorPtr v_ptr = 	m_psXMLBBDoc->parseError;
				m_sLastError = v_ptr->reason;
                return ;
        }


}


