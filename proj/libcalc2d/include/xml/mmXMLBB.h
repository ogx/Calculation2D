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



#ifdef MADMAC
#include <log\mmLogSender.h>
//#include <XMLDoc.hpp>
#endif

namespace mmXML
{
	////////////////////////////////////////////////////////////////////////////////
	/// Implementation of mmXMLNodeI interface based on Borland objects.
	////////////////////////////////////////////////////////////////////////////////
	class mmXMLNodeBB: public mmXMLNodeI
#ifdef MADMAC
		, mmLog::mmLogSender
#endif
	{
		private:	// fields
			////////////////////////////////////////////////////////////////////////////////
			/// Node name.
			////////////////////////////////////////////////////////////////////////////////
			mmString m_sNodeName;
			////////////////////////////////////////////////////////////////////////////////
			/// Value of node. Used if vector of childs is empty.
			////////////////////////////////////////////////////////////////////////////////
			mmString m_sNodeValue;
			////////////////////////////////////////////////////////////////////////////////
			/// Vector with child nodes.
			////////////////////////////////////////////////////////////////////////////////
			std::vector<mmXML::mmXMLNodeI*> m_sChilds;
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
#ifdef MADMAC
			mmXMLNodeBB(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
#else
			mmXMLNodeBB();
#endif
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmXMLNodeBB();

			mmString GetName(void);
			void SetName(mmString p_sName);
			bool IsLeaf(void);
			std::vector<mmXML::sXMLAttribute> GetAttributes(void);
			void AddAttribute(mmString p_sAttrName,mmString p_sAttrValue);
#ifdef MADMAC
			void RemoveAttribute(mmString p_sAttrName);
#else
			// bez wyjatkow, zwraca false jezeli wystapil blad
			bool RemoveAttribute(mmString p_sAttrName);
#endif
			void RemoveAllAttributes(void);
			bool IsAttribute(mmString p_sAttrName);
#ifdef MADMAC
			mmString GetAttributeValue(mmString p_sAttrName);
#else
			// bez wyjatkow, zwraca pustego stringa w przypadku bledu
			mmString GetAttributeValue(mmString p_sAttrName);
#endif
			void SetAttributeValue(mmString p_sAttrName,mmString p_sAttrValue);
			mmString GetText(void);
			void SetText(mmString p_sText);
			std::vector<mmXML::mmXMLNodeI*> GetChilds(void);
			mmXML::mmXMLNodeI* AddChild(mmString p_sChildName);
			void AddChildWithStructure(mmXMLNodeI* p_psChildNode);
			mmXML::mmXMLNodeI* FindChild(mmString p_sChildName);
			mmXML::mmXMLNodeI* FindNextChild(mmXML::mmXMLNodeI* p_psCurrentChild, mmString p_sChildName);
#ifdef MADMAC
			mmXML::mmXMLNodeI* GetChild(mmInt p_iChildIndex);
			void RemoveChild(mmString p_sChildName);
			void RemoveChild(mmInt p_iChildIndex);

#else
			// bez wyjatkow, zwraca NULL jezeli wystapil blad
			mmXML::mmXMLNodeI* GetChild(mmInt p_iChildIndex);
			// bez wyjatkow, zwraca false jezeli wystapil blad
			bool RemoveChild(mmString p_sChildName);
			// bez wyjatkow, zwraca false jezeli wystapil blad
			bool RemoveChild(mmInt p_iChildIndex);
#endif

			void RemoveAllChilds(void);
			mmXML::mmXMLNodeI* GetParent( void );
			mmXML::mmXMLNodeI* GetNextSibling( void );
			mmXML::mmXMLNodeI* GetPrevSibling( void );
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Implementation of mmXMLDocI interface. It uses Borland implementation.
	////////////////////////////////////////////////////////////////////////////////
	class mmXMLDocBB: public mmXMLDocI
#ifdef MADMAC
		,mmLog::mmLogSender
#endif
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
//			void FillXMLNodeFromXMLDocument(_di_IXMLNode p_sBBNode,mmXMLNodeI* p_psMMNode);
			void FillXMLNodeFromXMLDocument(MSXML2::IXMLDOMNodePtr p_sBBNode,mmXMLNodeI* p_psMMNode);
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Constructor.
			///
			/// @param[in] p_psLogReceiver pointer to log object.
			////////////////////////////////////////////////////////////////////////////////
#ifdef MADMAC
			mmXMLDocBB(mmLog::mmLogReceiverI *p_psLogReceiver = NULL);
#else
			mmXMLDocBB( );
#endif
			////////////////////////////////////////////////////////////////////////////////
			/// Destructor.
			////////////////////////////////////////////////////////////////////////////////
			~mmXMLDocBB();

#ifdef MADMAC
			void ParseXMLFile(mmString p_sXMLFileName);
			void ParseXMLBuffer(mmString p_sXMLBuffer);
			void SaveToXMLFile(mmString p_sXMLFileName);
			mmXMLNodeI* GetXMLRootNode(void);
#else
			bool ParseXMLFile(mmString p_sXMLFileName);
			bool ParseXMLBuffer(mmString p_sXMLBuffer);
			bool SaveToXMLFile(mmString p_sXMLFileName);
			mmXMLNodeI* GetXMLRootNode(void);

#endif

			mmString SaveToXMLBuffer(void);
			bool IsXMLStructureValid(void);
			
			void CreateXMLRootNode(mmString p_sRootNodeName);
			void CopyXMLRootNodeStructure(mmXMLNodeI* p_psNode);


			bool InitializeXML( void );
			void Load_XML_From_Buffer(mmString source);
			bool Load_XML_Document(mmString strFileName);
			void Reset_XML_Document();


	};
};

#endif
