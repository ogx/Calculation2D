

#include <xml\mmXMLBB.h>

#include <mmStringUtilities.h>

#ifndef RELEASE_PTR
#define RELEASE_PTR(x)	\
	if( x != NULL )		\
		{					\
		x.Release();	\
		x = NULL;		\
		}
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

mmXML::mmXMLNodeBB::mmXMLNodeBB( )
{
	
}
mmXML::mmXMLNodeBB::~mmXMLNodeBB()
{
	RemoveAllChildren();
}

mmString mmXML::mmXMLNodeBB::GetName(void)
{
	return m_sNodeName;
}

void mmXML::mmXMLNodeBB::SetName(mmString const & p_sName)
{
	m_sNodeName = p_sName;
}

bool mmXML::mmXMLNodeBB::IsLeaf(void)
{
	bool v_bIsLeaf = true;
	if(m_sChildren.size() != 0)
	{
		v_bIsLeaf = false;
	};

	return v_bIsLeaf;
}

std::vector<mmXML::sXMLAttribute> mmXML::mmXMLNodeBB::GetAttributes(void)
{
	return m_sAttributes;
}

void mmXML::mmXMLNodeBB::AddAttribute(mmString const & p_sAttrName, mmString const & p_sAttrValue)
{
	mmXML::sXMLAttribute v_sAttr;

	v_sAttr.sName = p_sAttrName;
	v_sAttr.sValue = p_sAttrValue;

	m_sAttributes.push_back(v_sAttr);
}

bool mmXML::mmXMLNodeBB::RemoveAttribute(mmString const & p_sAttrName)
{
	mmInt v_i,v_iAttrCount,v_iRemovedCount;
	std::vector<sXMLAttribute> v_sNewAttributes;

	v_iRemovedCount = 0;
	v_iAttrCount = static_cast<mmInt>(m_sAttributes.size());
	for(v_i=0;v_i<v_iAttrCount;v_i++)
	{
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
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
		return false;
	};

	return true;
}

void mmXML::mmXMLNodeBB::RemoveAllAttributes(void)
{
	m_sAttributes.clear();
}

bool mmXML::mmXMLNodeBB::IsAttribute(mmString const & p_sAttrName)
{
	mmInt v_i,v_iAttrCount;
	bool v_bIsAttr = false;

	v_iAttrCount = static_cast<mmInt>(m_sAttributes.size());
	for(v_i=0;v_i<v_iAttrCount;v_i++)
	{
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
		{
			v_bIsAttr = true;
			break;
		};
	};

	return v_bIsAttr;
}

mmString mmXML::mmXMLNodeBB::GetAttributeValue(mmString const & p_sAttrName)
{
	mmInt v_i,v_iAttrCount,v_iAttrIndex;

	v_iAttrIndex = -1;
	v_iAttrCount = static_cast<mmInt>(m_sAttributes.size());
	for(v_i=0;v_i<v_iAttrCount;v_i++)
	{
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
		{
			v_iAttrIndex = v_i;
			break;
		};
	};

	if(v_iAttrIndex == -1)
	{
		mmString v_sEmptyString;
		v_sEmptyString = L"";
		return v_sEmptyString;
	};

	return m_sAttributes[v_iAttrIndex].sValue;
}

void mmXML::mmXMLNodeBB::SetAttributeValue(mmString const & p_sAttrName, mmString const & p_sAttrValue)
{
	mmInt v_i,v_iAttrCount,v_iAttrIndex;

	v_iAttrIndex = -1;
	v_iAttrCount = static_cast<mmInt>(m_sAttributes.size());
	for(v_i=0;v_i<v_iAttrCount;v_i++)
	{
		if(m_sAttributes[v_i].sName.compare(p_sAttrName) == 0)
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
}

mmString mmXML::mmXMLNodeBB::GetText(void)
{
	return m_sNodeValue;
}

void mmXML::mmXMLNodeBB::SetText(mmString const & p_sText)
{
	m_sNodeValue = p_sText;
}

std::vector<mmXML::mmXMLNodeI*> mmXML::mmXMLNodeBB::GetChildren(void)
{
	return m_sChildren;
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::AddChild(mmString const & p_sChildName)
{
	mmXMLNodeBB* v_psNode = new mmXML::mmXMLNodeBB();

	v_psNode->m_sNodeName = p_sChildName;
	v_psNode->m_psParent = this;
	m_sChildren.push_back(v_psNode);

	return v_psNode;
}

void mmXML::mmXMLNodeBB::AddChildWithStructure(mmXMLNodeI* p_psChildNode)
{
	mmXMLNodeBB* v_psNode = new mmXML::mmXMLNodeBB();

	v_psNode->m_psParent = p_psChildNode->GetParent();
	v_psNode->m_sNodeName = p_psChildNode->GetName();
	v_psNode->m_sNodeValue = p_psChildNode->GetText();
	v_psNode->m_sAttributes = p_psChildNode->GetAttributes();

	std::vector<mmXML::mmXMLNodeI*> v_vChildren = p_psChildNode->GetChildren();
	std::size_t v_iChildrenCount = v_vChildren.size();
	for(std::size_t v_iC=0;v_iC<v_iChildrenCount;v_iC++)
	{
		v_psNode->AddChildWithStructure(v_vChildren[v_iC]);
	};

	m_sChildren.push_back(v_psNode);
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::FindChild(mmString const & p_sChildName)
{
	mmInt v_i,v_iChildCount;
	mmXML::mmXMLNodeI* v_psFoundNode = NULL;

	v_iChildCount = static_cast<mmInt>(m_sChildren.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
		if(m_sChildren[v_i]->GetName().compare(p_sChildName) == 0)
		{
			v_psFoundNode = m_sChildren[v_i];
			break;
		};
	};
	return v_psFoundNode;
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetChild(mmInt p_iChildIndex)
{
	mmInt v_i,v_iChildCount;
	mmXML::mmXMLNodeI* v_psFoundNode = NULL;

	v_iChildCount = static_cast<mmInt>(m_sChildren.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
		if(v_i == p_iChildIndex)
		{
			v_psFoundNode = m_sChildren[v_i];
		};
	};

	return v_psFoundNode;
}

bool mmXML::mmXMLNodeBB::RemoveChild(mmString const & p_sChildName)
{
	mmInt v_i,v_iChildCount,v_iRemovedCount;
	std::vector<mmXML::mmXMLNodeI*> v_sNewChildren;

	v_iRemovedCount = 0;
	v_iChildCount = static_cast<mmInt>(m_sChildren.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
		if(m_sChildren[v_i]->GetName().compare(p_sChildName) == 0)
		{
			v_iRemovedCount++;

			delete m_sChildren[v_i];
		}
		else
		{
			v_sNewChildren.push_back(m_sChildren[v_i]);
		};
	};

	m_sChildren = v_sNewChildren;

	if(v_iRemovedCount == 0)
	{
		return false;
	};

	return true;
}

bool mmXML::mmXMLNodeBB::RemoveChild(mmInt p_iChildIndex)
{
	mmInt v_i,v_iChildCount;
	bool v_bIsRemoved = false;
	std::vector<mmXML::mmXMLNodeI*> v_sNewChildren;

	v_iChildCount = static_cast<mmInt>(m_sChildren.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
		if(v_i == p_iChildIndex)
		{
			v_bIsRemoved = true;
			delete m_sChildren[v_i];
		}
		else
		{
			v_sNewChildren.push_back(m_sChildren[v_i]);
		};
	};

	m_sChildren = v_sNewChildren;

	if(!v_bIsRemoved)
	{
		return false;
	};

	return true;
}

void mmXML::mmXMLNodeBB::RemoveAllChildren(void)
{
	mmInt v_i,v_iChildCount;

	v_iChildCount = static_cast<mmInt>(m_sChildren.size());
	for(v_i=0;v_i<v_iChildCount;v_i++)
	{
		delete m_sChildren[v_i];
	};
	m_sChildren.clear();
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetParent( void ) {
	return m_psParent;
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetNextSibling( void ) {
	if( m_psParent == NULL )
    	return NULL;
	std::vector<mmXML::mmXMLNodeI*> v_sChildNodes = m_psParent->GetChildren();
	mmXML::mmXMLNodeI* v_psNode = NULL;
	for( std::vector<mmXML::mmXMLNodeI*>::iterator v_sI = v_sChildNodes.begin(); v_sI != v_sChildNodes.end()-1; ++v_sI ) {
		if( *v_sI == static_cast<mmXML::mmXMLNodeI*>(this) ) {
			v_psNode = *(v_sI+1);
			break;
		}
	}
	return v_psNode;
}

mmXML::mmXMLNodeI* mmXML::mmXMLNodeBB::GetPrevSibling( void ) {
	if( m_psParent == NULL )
    	return NULL;
	std::vector<mmXML::mmXMLNodeI*> v_sChildNodes = m_psParent->GetChildren();
	mmXML::mmXMLNodeI* v_psNode = NULL;
	for( std::vector<mmXML::mmXMLNodeI*>::iterator v_sI = v_sChildNodes.begin()+1; v_sI != v_sChildNodes.end(); ++v_sI ) {
		if( *v_sI == static_cast<mmXML::mmXMLNodeI*>(this) ) {
			v_psNode = *(v_sI-1);
			break;
		}
	}
	return v_psNode;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  mmXML::mmXMLDocBB
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
mmXML::mmXMLDocBB::mmXMLDocBB()
{
	//CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	m_bOLEInitialized = false;
	InitializeXML();
}

mmXML::mmXMLDocBB::~mmXMLDocBB()
{
	this->Reset_XML_Document();
	m_psXMLBBDoc.Release();
	CoUninitialize();
}

bool mmXML::mmXMLDocBB::ParseXMLFile(mmString const & p_sXMLFileName)
{
	// delete XML structure
	m_sRootNode.RemoveAllChildren();
	m_sRootNode.RemoveAllAttributes();

	// load XMLDocument

	if (Load_XML_Document(p_sXMLFileName) == false)
		return false;

	// refresh XML structure from XMLDocument
	LoadNodeStructureFromXMLDocument();

	return true;
}
bool mmXML::mmXMLDocBB::ParseXMLBuffer(mmString const & p_sXMLBuffer)
{
	// delete XML structure
	m_sRootNode.RemoveAllChildren();
	m_sRootNode.RemoveAllAttributes();

	// load XMLDocument
	try
	{
		this->Load_XML_From_Buffer(p_sXMLBuffer);
	}
	catch(...)
	{

		return false;


	};
	// refresh XML structure from XMLDocument
	LoadNodeStructureFromXMLDocument();

	return true;
}

bool mmXML::mmXMLDocBB::SaveToXMLFile(mmString const & p_sXMLFileName)
{
	// save XML structure into XMLDocument
	SaveNodeStructureIntoXMLDocument();

	// using XMLDocument API
	mmString v_sOutString = GetXMLFormattedString();
	
	// opening TXT file
	m_psXMLBBDoc->save( _bstr_t( p_sXMLFileName.c_str() ) );

	return true;
}

mmString mmXML::mmXMLDocBB::SaveToXMLBuffer(void)
{
	// save XML structure into XMLDocument
	SaveNodeStructureIntoXMLDocument();

	// using XMLDocument API
	mmString v_sOutString = GetXMLFormattedString();

	return v_sOutString;
}

bool mmXML::mmXMLDocBB::IsXMLStructureValid(void)
{
	bool v_bIsValid = true;
	// TODO: validate XML structure

	return v_bIsValid;
}

mmXML::mmXMLNodeI* mmXML::mmXMLDocBB::GetXMLRootNode( void )
{
	return &m_sRootNode;
}

void mmXML::mmXMLDocBB::CreateXMLRootNode(mmString const & p_sRootNodeName)
{
	m_sRootNode.RemoveAllChildren();
	m_sRootNode.RemoveAllAttributes();
	m_sRootNode.SetName(p_sRootNodeName);
	m_sRootNode.SetText(mmString(L""));
}

void mmXML::mmXMLDocBB::CopyXMLRootNodeStructure(mmXMLNodeI* p_psNode)
{
	CreateXMLRootNode(p_psNode->GetName());

	std::vector<mmXML::sXMLAttribute> v_vRootAttributes = p_psNode->GetAttributes();
	std::size_t v_iAttrCount = v_vRootAttributes.size();
	for(std::size_t v_iA=0;v_iA<v_iAttrCount;v_iA++)
	{
		m_sRootNode.AddAttribute(v_vRootAttributes[v_iA].sName,
														 v_vRootAttributes[v_iA].sValue);
	};

	std::vector<mmXML::mmXMLNodeI*> v_vChildren = p_psNode->GetChildren();
	std::size_t v_iChildrenCount = v_vChildren.size();
	for(std::size_t v_iC=0;v_iC<v_iChildrenCount;v_iC++)
	{
		m_sRootNode.AddChildWithStructure(v_vChildren[v_iC]);
	};
}



void mmXML::mmXMLDocBB::FillXMLNodeIntoXMLDocument(MSXML2::IXMLDOMNodePtr p_sBBNode,mmXMLNodeI* p_psMMNode)
{
	// adding attributes
	std::size_t v_i,v_iCount;

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
	std::vector<mmXML::mmXMLNodeI*> v_sChildren = p_psMMNode->GetChildren();
	v_iCount = v_sChildren.size();
	if(v_iCount > 0)
	{
		// Children
		for(v_i=0;v_i<v_iCount;v_i++)
		{
			MSXML2::IXMLDOMNodePtr v_sNewNode =  m_psXMLBBDoc->createNode(MSXML2::NODE_ELEMENT, _bstr_t(v_sChildren[v_i]->GetName().c_str()), "");


			v_sNewNode = p_sBBNode->appendChild(v_sNewNode);
			FillXMLNodeIntoXMLDocument(v_sNewNode,v_sChildren[v_i]);
		};
	}
	else
	{
		// text
		p_sBBNode->put_text(_bstr_t(p_psMMNode->GetText().c_str()));
	};
}


void mmXML::mmXMLDocBB::SaveNodeStructureIntoXMLDocument(void)
{
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
}

mmString mmXML::mmXMLDocBB::GetXMLFormattedString(void)
{
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
		hr = e.Error();
	}
	RELEASE_PTR(pISAXDTDHandler);
	RELEASE_PTR(pISAXErrorHandler);
	RELEASE_PTR(pISAXContentHandler);
	RELEASE_PTR(pSAXReader);
	RELEASE_PTR(pMXWriter);
	
	return v_sOutString;
}

void mmXML::mmXMLDocBB::FillXMLNodeFromXMLDocument(MSXML2::IXMLDOMNodePtr p_sBBNode, mmXMLNodeI* p_psMMNode)
{
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
			// Children
			for(v_i=0;v_i<v_iCount;v_i++)
			{
				mmXMLNodeI* v_psChildNode = p_psMMNode->AddChild((mmString)(const wchar_t *)(p_sBBNode->GetchildNodes()->Getitem(v_i)->GetnodeName()));
				 
				FillXMLNodeFromXMLDocument(p_sBBNode->GetchildNodes()->Getitem(v_i),v_psChildNode);
			};
		};
	};
}

void mmXML::mmXMLDocBB::LoadNodeStructureFromXMLDocument(void)
{
	MSXML2::IXMLDOMElementPtr v_sRootNode = m_psXMLBBDoc->documentElement;
	m_sRootNode.SetName((mmStringUtilities::MMCharStringToMMString((const char *) v_sRootNode->GetnodeName())));
//	m_sRootNode.SetName((mmString((const wchar_t *) v_sRootNode->GetnodeName())));
	FillXMLNodeFromXMLDocument(v_sRootNode,&m_sRootNode);
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
bool mmXML::mmXMLDocBB::Load_XML_Document(mmString const & strFileName)
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

void mmXML::mmXMLDocBB::Load_XML_From_Buffer(mmString const & source)
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


