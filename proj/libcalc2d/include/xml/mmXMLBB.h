//******************************************************************************
//******************************************************************************
//
//  MSXML2 XML implementation
//
//  Description: These classes implement functionality of wotking with XML files
//               and data.
//
//******************************************************************************
//******************************************************************************
#ifndef mmXMLBBH
#define mmXMLBBH
#define MADMAC
#include "interfaces\mmIXML.h"
#import <msxml3.dll> named_guids
using namespace MSXML2;

namespace mmXML
{
	////////////////////////////////////////////////////////////////////////////////
	/// Implementation of mmXMLNodeI interface based on Borland objects.
	////////////////////////////////////////////////////////////////////////////////
	class mmXMLNodeBB: public mmXMLNodeI
	{
		private:	// fields
			////////////////////////////////////////////////////////////////////////////////
			/// Node name.
			////////////////////////////////////////////////////////////////////////////////
			mmString m_sNodeName;
			////////////////////////////////////////////////////////////////////////////////
			/// Value of node. Used if vector of Children is empty.
			////////////////////////////////////////////////////////////////////////////////
			mmString m_sNodeValue;
			////////////////////////////////////////////////////////////////////////////////
			/// Vector with child nodes.
			////////////////////////////////////////////////////////////////////////////////
			std::vector<mmXML::mmXMLNodeI*> m_sChildren;
			////////////////////////////////////////////////////////////////////////////////
			/// Vector with attributes of node.
			////////////////////////////////////////////////////////////////////////////////
			std::vector<sXMLAttribute> m_sAttributes;
			mmXML::mmXMLNodeI * m_psParent;
		public:		// methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			/// @param[in] p_psLogReceiver pointer to log object.
			////////////////////////////////////////////////////////////////////////////////
			mmXMLNodeBB();
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmXMLNodeBB();

			mmString GetName(void);
			void SetName(mmString const & p_sName);
			bool IsLeaf(void);
			std::vector<mmXML::sXMLAttribute> GetAttributes(void);

			void AddAttribute(mmString const & p_sAttrName, mmString const & p_sAttrValue);
			bool RemoveAttribute(mmString const & p_sAttrName);
			void RemoveAllAttributes(void);
			bool IsAttribute(mmString const & p_sAttrName);

			mmString GetAttributeValue(mmString const & p_sAttrName);
			void SetAttributeValue(mmString const & p_sAttrName, mmString const & p_sAttrValue);

			mmString GetText(void);
			void SetText(mmString const & p_sText);

			std::vector<mmXML::mmXMLNodeI*> GetChildren(void);
			mmXML::mmXMLNodeI* AddChild(mmString const & p_sChildName);
			void AddChildWithStructure(mmXMLNodeI* p_psChildNode);
			mmXML::mmXMLNodeI* FindChild(mmString const & p_sChildName);
			mmXML::mmXMLNodeI* GetChild(mmInt p_iChildIndex);
			bool RemoveChild(mmString const & p_sChildName);
			bool RemoveChild(mmInt p_iChildIndex);
			void RemoveAllChildren(void);

			mmXML::mmXMLNodeI* GetParent( void );
			mmXML::mmXMLNodeI* GetNextSibling( void );
			mmXML::mmXMLNodeI* GetPrevSibling( void );
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Implementation of mmXMLDocI interface. It uses Borland implementation.
	////////////////////////////////////////////////////////////////////////////////
	class mmXMLDocBB: public mmXMLDocI
	{
		private:	// fields
			////////////////////////////////////////////////////////////////////////////////
			/// XML document implemented by BB Team.
			////////////////////////////////////////////////////////////////////////////////

			//_di_IXMLDocument m_psXMLBBDoc;
			MSXML2::IXMLDOMDocumentPtr m_psXMLBBDoc;
			MSXML2::IXMLDOMElementPtr m_psDocRoot;
			MSXML2::IXMLDOMNodePtr m_sCurrentNode;
			////////////////////////////////////////////////////////////////////////////////
			/// Root node object.
			////////////////////////////////////////////////////////////////////////////////
			mmXMLNodeBB m_sRootNode;
			bool m_bOLEInitialized;
			mmString m_sLastError;

	private:	// methods
			////////////////////////////////////////////////////////////////////////////////
			/// This method deletes XMLDocument and creates new one. Next, it fills
			/// node structure into new XMLDocument.
			////////////////////////////////////////////////////////////////////////////////
			void SaveNodeStructureIntoXMLDocument(void);
			////////////////////////////////////////////////////////////////////////////////
			/// Method adds node with its children from XML structure into XMLDocument.
			///
			/// @param[in] p_sBBNode XMLDocument node interface
			/// @param[in] p_psMMNode XML structure element
			////////////////////////////////////////////////////////////////////////////////
			//void FillXMLNodeIntoXMLDocument(_di_IXMLNode p_sBBNode,mmXMLNodeI* p_psMMNode);
			void FillXMLNodeIntoXMLDocument(MSXML2::IXMLDOMNodePtr p_sBBNode,mmXMLNodeI* p_psMMNode);
			////////////////////////////////////////////////////////////////////////////////
			/// Method returns formatted string representing XML structure.
			///
			/// @return string with XML structure
			////////////////////////////////////////////////////////////////////////////////
			mmString GetXMLFormattedString(void);
			////////////////////////////////////////////////////////////////////////////////
			/// This method fills XML structure from XMLDocument.
			////////////////////////////////////////////////////////////////////////////////
			void LoadNodeStructureFromXMLDocument(void);
			////////////////////////////////////////////////////////////////////////////////
			/// Method fills node with its children from XMLDocument into XML structure.
			///
			/// @param[in] p_sBBNode XMLDocument node interface
			/// @param[in] p_psMMNode XML structure element
			////////////////////////////////////////////////////////////////////////////////
			void FillXMLNodeFromXMLDocument(MSXML2::IXMLDOMNodePtr p_sBBNode,mmXMLNodeI* p_psMMNode);
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			/// @param[in] p_psLogReceiver pointer to log object.
			////////////////////////////////////////////////////////////////////////////////
			mmXMLDocBB( );
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmXMLDocBB();
			bool ParseXMLFile(mmString const & p_sXMLFileName);
			bool ParseXMLBuffer(mmString const & p_sXMLBuffer);
			bool SaveToXMLFile(mmString const & p_sXMLFileName);
			mmXMLNodeI* GetXMLRootNode(void);
			mmString SaveToXMLBuffer(void);
			bool IsXMLStructureValid(void);
			void CreateXMLRootNode(mmString const & p_sRootNodeName);
			void CopyXMLRootNodeStructure(mmXMLNodeI* p_psNode);
			bool InitializeXML( void );
			void Load_XML_From_Buffer(mmString const & source);
			bool Load_XML_Document(mmString const & strFileName);
			void Reset_XML_Document();


	};
};

#endif
