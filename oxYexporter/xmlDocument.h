#pragma once

#include <Windows.h>
#include <stdio.h>
#import<msxml6.dll>
#include <comutil.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <memory>
#include <utility>
#include <vector>

namespace oxyde {
	namespace exporter {
		namespace XML {
			class oxyDocument;
			using oxyDocumentPtr = std::shared_ptr<oxyDocument>;

			class oxyDocumentElement;
			using oxyDocumentElementPtr = std::shared_ptr<oxyDocumentElement>;

			using elementAttribute = std::pair<std::wstring, _variant_t>;

			//oxyDocumentElement
			class oxyDocumentElement{
				//	keeps track of oxyDocument
			protected:
				oxyDocumentPtr theDocument;

				std::vector<elementAttribute> attributeList;

				std::wstring elementName;

				//Keep this Pointer:
				//IXMLDOMElementPtr addElement(Name) {
				//	return IXMLDOMElementPtr(getXMLDOMElement(), getDocument()->getXMLDOMDocument())
				//}

				oxyDocumentElementPtr parentElement;
				MSXML2::IXMLDOMElementPtr xmlElementPointer;

				static MSXML2::IXMLDOMElementPtr addElement(std::wstring elementName, oxyDocumentElementPtr theParent);
				static MSXML2::IXMLDOMElementPtr addElement(std::wstring elementName, oxyDocumentPtr theDocument);

				//	virtual buildListOfAttributes()
				virtual void buildListOfAttributes() {};

				//	setElementAttributes()
				void setElementAttributes();

			public:
				oxyDocumentElement(oxyDocumentPtr theInputDocument, std::wstring theElementName);
				oxyDocumentElement(oxyDocumentElementPtr theParent, std::wstring theElementName);

				//	oxyDocumentElement(oxyDocument) {
				//	oxyDocument->getXMLDOMDocument()
				//		oxyDocument->getRoot()
				//}

				//oxyDocumentElement(oxyDocumentElementPtr) {
				//	oxyDocumentElementPtr->getDocument()
				//		oxyDocumentElementPtr->getXMLDOMElement()
				//}

				//oxyDocumentPtr getDocument()
				oxyDocumentPtr getDocument() {
					return theDocument;
				}

				//	IXMLDOMElementPtr getXMLDOMElement()
				MSXML2::IXMLDOMElementPtr getXMLDOMElement() {
					return xmlElementPointer;
				}

			};

			class oxyDocument {
			private:
				static oxyDocumentPtr theInstance;
				MSXML2::IXMLDOMDocument3Ptr xmlDocumentPointer;
				MSXML2::IXMLDOMElementPtr xmlRootPointer;

				oxyDocument();

			public:

				static oxyDocumentPtr getTheInstance();
				static void createDocument();
				static void releaseDocument();

				MSXML2::IXMLDOMElementPtr getRoot() {
					return xmlRootPointer;
				}

				MSXML2::IXMLDOMDocument3Ptr getXMLDocument() {
					return xmlDocumentPointer;
				}

				void save(std::wstring name);
			};

			_variant_t variantFromFloat(float value);
		}
	}
}
