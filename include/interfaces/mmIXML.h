//******************************************************************************
//******************************************************************************
//
//  XML Interface
//
//
//  Description: This header defines namespace and interfaces for XML data/file manipulation.
//
//******************************************************************************
//******************************************************************************
#ifndef mmIXMLH
#define mmIXMLH

#include <vector>

#include <mmGlobalDefs.h>
#include <mmError.h>

////////////////////////////////////////////////////////////////////////////////
/// Namenpace for XML data manipulation. The idea behind is that any XML
/// engine used parses whole XML document into mmXMLDocI and mmXMLNodeI
/// elements structure and all operation are performed in this structure.
/// Further user can save or export resultant data.
////////////////////////////////////////////////////////////////////////////////
namespace mmXML
{
	////////////////////////////////////////////////////////////////////////////////
  /// XML attribute definition.
  ////////////////////////////////////////////////////////////////////////////////
  typedef struct
  {
    ////////////////////////////////////////////////////////////////////////////////
		/// Attribute name.
		////////////////////////////////////////////////////////////////////////////////
		mmString sName;
		////////////////////////////////////////////////////////////////////////////////
		/// Attribute value.
		////////////////////////////////////////////////////////////////////////////////
		mmString sValue;
	} sXMLAttribute;

	////////////////////////////////////////////////////////////////////////////////
	/// Interface for XML node element.
	////////////////////////////////////////////////////////////////////////////////
	class mmXMLNodeI
	{
		public:		// methods
			////////////////////////////////////////////////////////////////////////////////
			/// Virtual destructor.
			////////////////////////////////////////////////////////////////////////////////
			virtual ~mmXMLNodeI() {};

			////////////////////////////////////////////////////////////////////////////////
			/// This method returns name of XML node element.
			///
			/// @return name of XML node
			////////////////////////////////////////////////////////////////////////////////
			virtual mmString GetName(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method sets new name for XML node element.
			///
			/// @param[in] p_sName new name of XML node.
			////////////////////////////////////////////////////////////////////////////////
			virtual void SetName(mmString const & p_sName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method returns information is this node is leaf of not.
			///
			/// @return TRUE if it is leaf, FALSE otherwise.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool IsLeaf(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method returns attributes of node.
			///
			/// @return vector with node attributes.
			////////////////////////////////////////////////////////////////////////////////
			virtual std::vector<mmXML::sXMLAttribute> GetAttributes(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// This method adds attribute to node.
			///
			/// @param[in] p_sAttrName name of attribute
			/// @param[in] p_sAttrValue value of attribute.
			////////////////////////////////////////////////////////////////////////////////
			virtual void AddAttribute(mmString const & p_sAttrName, mmString const & p_sAttrValue) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method removes attribute(s) with given name. In case of lack attribute
			/// with given name it throws mmError(mmeXMLNoSuchAttribute).
			///
			/// @param[in] p_sAttrName name of attribute.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool RemoveAttribute(mmString const & p_sAttrName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method removes all attributes.
			////////////////////////////////////////////////////////////////////////////////
			virtual void RemoveAllAttributes(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method checks if attribute with given name exists.
			///
			/// @param[in] p_sAttrName name of attribute.
			/// @return TRUE if it exists, FALSE otherwise.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool IsAttribute(mmString const & p_sAttrName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Methods returns attribute value. In case of lack attribute
			/// with given name it throws mmError(mmeXMLNoSuchAttribute).
			///
			/// @param[in] p_sAttrName name of attribute
			/// @return value of attribute.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmString GetAttributeValue(mmString const & p_sAttrName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Methods sets attribute value. In case of lack attribute
			/// with given name it adds it.
			///
			/// @param[in] p_sAttrName name of attribute
			/// @param[in] p_sAttrValue new value of attribute.
			////////////////////////////////////////////////////////////////////////////////
			virtual void SetAttributeValue(mmString const & p_sAttrName, mmString const & p_sAttrValue) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method returns text of node.
			///
			/// @return text of node.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmString GetText(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method sets text of node.
			///
			/// @param[in] p_sText text of node.
			////////////////////////////////////////////////////////////////////////////////
			virtual void SetText(mmString const & p_sText) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method returns vector with child nodes.
			///
			/// @return vector with pointers to elements.
			////////////////////////////////////////////////////////////////////////////////
			virtual std::vector<mmXML::mmXMLNodeI*> GetChildren(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method adds child node. If node has not empty text element it clears it.
			///
			/// @param[in] p_sChildName name of new child element
			/// @return pointer to created node
			////////////////////////////////////////////////////////////////////////////////
			virtual mmXML::mmXMLNodeI* AddChild(mmString const & p_sChildName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method copies child node and its structure.
			///
			/// @param[in] p_psChildNode pointer to node which will be copied
			////////////////////////////////////////////////////////////////////////////////
			virtual void AddChildWithStructure(mmXMLNodeI* p_psChildNode) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method finds child node. If child not exist then it returns NULL;
			///
			/// @param[in] p_sChildName name of new child element
			/// @return pointer to found node
			////////////////////////////////////////////////////////////////////////////////
			virtual mmXML::mmXMLNodeI* FindChild(mmString const & p_sChildName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method returns pointer to child node. In case of lack node with given index
			/// it throws mmError(mmeXMLNoSuchNode).
			///
			/// @param[in] p_iChildIndex index of selected child node.
			/// @return pointer to selected node
			////////////////////////////////////////////////////////////////////////////////
			virtual mmXML::mmXMLNodeI* GetChild(mmInt p_iChildIndex) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method removes child node. In case of lack node with given name it throws
			/// mmError(mmeXMLNoSuchNode).
			///
			/// @param[in] p_sChildName child node name
			////////////////////////////////////////////////////////////////////////////////
			virtual bool RemoveChild(mmString const & p_sChildName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method removes child node. In case of lack node with given name it throws
			/// mmError(mmeXMLNoSuchNode).
			///
			/// @param[in] p_iChildIndex index of selected child node.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool RemoveChild(mmInt p_iChildIndex) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method removes all child nodes.
			////////////////////////////////////////////////////////////////////////////////
			virtual void RemoveAllChildren(void) = 0;
			////////////////////////////////////////////////////////////////////////////////
			/// Method finds parent node of current node.
			///
			/// @return parent node.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmXMLNodeI* GetParent( void )=0;
			////////////////////////////////////////////////////////////////////////////////
			/// Method finds next sibling of current node.
			///
			/// @return next sibling.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmXMLNodeI* GetNextSibling( void )=0;
			////////////////////////////////////////////////////////////////////////////////
			/// Method finds previous sibling of current node.
			///
			/// @return previous sibling.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmXMLNodeI* GetPrevSibling( void )=0;
	};

	////////////////////////////////////////////////////////////////////////////////
	/// Interface for XML document.
	////////////////////////////////////////////////////////////////////////////////
	class mmXMLDocI
	{
		public:   // methods
			////////////////////////////////////////////////////////////////////////////////
			/// Virtual destructor.
			////////////////////////////////////////////////////////////////////////////////
			virtual ~mmXMLDocI() {};

			////////////////////////////////////////////////////////////////////////////////
			/// Method parses XML file. In case of error it throws mmError(mmeXMLParseError).
			///
			/// @param[in] p_sXMLFileName name of file to parse.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool ParseXMLFile(mmString const & p_sXMLFileName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method parses text buffer with XML document. In case of error it throws
			/// mmError(mmeXMLParseError).
			///
			/// @param[in] p_sXMLBuffer text buffer to parse.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool ParseXMLBuffer(mmString const & p_sXMLBuffer) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method saves mmXMLDocI structure as XML file. In case of error it throws
			/// mmError(mmeXMLBadXMLStructure).
			///
			/// @param[in] p_sXMLFileName file name to store XML structure.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool SaveToXMLFile(mmString const & p_sXMLFileName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method saves mmXMLDocI structure as XML buffer. In case of error it throws
			/// mmError(mmeXMLBadXMLStructure).
			///
			/// @return string storing XML structure.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmString SaveToXMLBuffer(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method validates mmXMLDocI structure. In case of error it throws
			/// mmError(mmeXMLBadXMLStructure).
			///
			/// @return TRUE if structure is valid, FALSE otherwise.
			////////////////////////////////////////////////////////////////////////////////
			virtual bool IsXMLStructureValid(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method returns root node of mmXMLDocI document. In case of lack node with
			/// given name it throws mmError(mmeXMLNoSuchNode).
			///
			/// @return pointer to XML node object.
			////////////////////////////////////////////////////////////////////////////////
			virtual mmXMLNodeI* GetXMLRootNode(void) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method creates new root node. It deletes whole existing XML structure.
			///
			/// @param[in] p_sRootNodeName new root node name.
			////////////////////////////////////////////////////////////////////////////////
			virtual void CreateXMLRootNode(mmString const & p_sRootNodeName) = 0;

			////////////////////////////////////////////////////////////////////////////////
			/// Method creates new root node. It deletes whole existing XML structure. New
			/// node is copied from v_psNode with its whole structure.
			///
			/// @param[in] p_psNode pointer to new root node source node.
			////////////////////////////////////////////////////////////////////////////////
			virtual void CopyXMLRootNodeStructure(mmXMLNodeI* p_psNode) = 0;
	};
};

#endif
