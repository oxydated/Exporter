#include "xmlDocument.h"
#include "resource.h"
#include "dllFunc.h"

namespace oxyde {
	namespace exporter {
		namespace XML {

			oxyDocumentPtr oxyDocument::theInstance = nullptr;

			oxyDocument::oxyDocument() {
				HRESULT hr = xmlDocumentPointer.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
				if (hr == S_OK) {
					xmlDocumentPointer->async = VARIANT_FALSE;
					xmlDocumentPointer->validateOnParse = VARIANT_FALSE;
					xmlDocumentPointer->resolveExternals = VARIANT_FALSE;
					xmlDocumentPointer->preserveWhiteSpace = VARIANT_FALSE;

					MSXML2::IXMLDOMProcessingInstructionPtr pi =  xmlDocumentPointer->createProcessingInstruction(L"xml", L"version='1.0'");
					xmlDocumentPointer->appendChild(pi);

					xmlRootPointer =  xmlDocumentPointer->createElement(L"document");
					xmlDocumentPointer->appendChild(xmlRootPointer);
				}
			}

			oxyDocumentPtr oxyDocument::getTheInstance()
			{
				return theInstance;
			}

			void oxyDocument::createDocument()
			{
				//theInstance = std::make_shared<oxyDocument>();
				CoInitialize(NULL);
				theInstance = std::shared_ptr<oxyDocument>(new oxyDocument());
			}

			void oxyDocument::releaseDocument()
			{
				theInstance.reset();
				CoUninitialize();
			}

			void oxyDocument::save(std::wstring name)
			{
				MSXML2::IXMLDOMDocument3Ptr xslDoc = nullptr;
				HRESULT hr = xslDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
				if (hr == S_OK) {
					xslDoc->async = VARIANT_FALSE;
					xslDoc->validateOnParse = VARIANT_FALSE;
					xslDoc->resolveExternals = VARIANT_FALSE;
					xslDoc->preserveWhiteSpace = VARIANT_FALSE;

					LPTSTR theStyle = new _TCHAR[800];
					LoadString(getThisDLLModule(), IDS_STYLE_INDENT, theStyle, 800);

					//BSTR theInputStyle = _bstr_t(theStyle);

					//VARIANT loaded = _variant_t(false).Detach();

					//hr = xslDoc->loadXML(theInputStyle, &loaded.boolVal);
					//bool itWorked = (loaded.boolVal == VARIANT_TRUE);
					//VariantClear(&loaded);

					VARIANT_BOOL isLoaded = xslDoc->loadXML(_bstr_t(theStyle));


					MSXML2::IXMLDOMDocument3Ptr outDoc = nullptr;

					hr = outDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
					if (hr == S_OK) {
						outDoc->async = VARIANT_FALSE;
						outDoc->validateOnParse = VARIANT_FALSE;
						outDoc->resolveExternals = VARIANT_FALSE;
						outDoc->preserveWhiteSpace = VARIANT_FALSE;

						hr = xmlDocumentPointer->transformNodeToObject(xslDoc, outDoc.GetInterfacePtr());
						if (hr == S_OK) {
							outDoc->save(_variant_t(name.data()));
						}
					}

				}

				//IXMLDOMDocument3* outDoc = NULL;

				//IDispatch* dispOutDoc = NULL;
				//hr = outDoc->QueryInterface(__uuidof(IDispatch), (void**)&dispOutDoc);
				//_variant_t vOutDoc(dispOutDoc);

				//hr = theDocument->transformNodeToObject(xslDoc, vOutDoc);

				//hr = theDocument->save(_variant_t(L"myExportTestOriginal.xml"));
				//hr = outDoc->save(_variant_t("myExportTestXSL.xml"));

				//hr = outDoc->save(_variant_t(name));

				//if (theRootElement != NULL) {
				//	theRootElement->Release();
				//	theRootElement = NULL;
				//}
				//if (theSceneElement != NULL) {
				//	theSceneElement->Release();
				//	theSceneElement = NULL;
				//}
				//if (theObjectListElement != NULL) {
				//	theObjectListElement->Release();
				//	theObjectListElement = NULL;
				//}
				//if (theGeometryElement != NULL) {
				//	theGeometryElement->Release();
				//	theGeometryElement = NULL;
				//}
				//if (theTransformationsElement != NULL) {
				//	theTransformationsElement->Release();
				//	theTransformationsElement = NULL;
				//}

				//xslDoc->Release();
				//outDoc->Release();
				//theDocument->Release();

				//xslDoc = NULL;
				//outDoc = NULL;
				//theDocument = NULL;
			}

			MSXML2::IXMLDOMElementPtr oxyDocumentElement::addElement(std::wstring elementName, oxyDocumentElementPtr theParent)
			{
				MSXML2::IXMLDOMElementPtr newElement = theParent->getDocument()->getXMLDocument()->createElement(_bstr_t(elementName.data()));
				theParent->getXMLDOMElement()->appendChild(newElement);
				return newElement;
			}

			MSXML2::IXMLDOMElementPtr oxyDocumentElement::addElement(std::wstring elementName, oxyDocumentPtr theDocument)
			{
				MSXML2::IXMLDOMElementPtr newElement = theDocument->getXMLDocument()->createElement(_bstr_t(elementName.data()));
				theDocument->getRoot()->appendChild(newElement);
				return newElement;
			}

			void oxyDocumentElement::setElementAttributes()
			{
				for (auto && item : attributeList) {
					xmlElementPointer->setAttribute(item.first.data(), item.second);
				}
			}

			oxyDocumentElement::oxyDocumentElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) :elementName(theElementName), theDocument(theInputDocument)
			{
				xmlElementPointer = addElement(elementName, theDocument);
				theDocument = theInputDocument; 
				buildListOfAttributes();
				setElementAttributes();
			}

			oxyDocumentElement::oxyDocumentElement(oxyDocumentElementPtr theParent, std::wstring theElementName) :elementName(theElementName)
			{
				xmlElementPointer = addElement(elementName, theParent);
				theDocument = theParent->getDocument();
				buildListOfAttributes();
				setElementAttributes();
			}
			namespace {
			}

			_variant_t variantFromFloat(float value)
			{
				std::wstring outStr;
				std::wstring resultString;
				std::wostringstream outStream(outStr);

				outStream << std::fixed << value;
				resultString = outStream.str();
				return _variant_t(resultString.c_str());
			}

		}
	}
}
