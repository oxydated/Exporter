#pragma once
#include "xmlDocument.h"
namespace oxyde {
	namespace exporter {
		namespace XML {

		//oxyNodeKeeper:
		//	oxyNodeKeeperPtr keeperParent
		//		nodeCount = -1
		//		int updateKeeper();

		//oxySceneElement: oxyNodeKeeper
		//	:keeperParent(nullptr_t)
		//	nodeID 0 = updateKeeper() { return nodeCount++; }

		//			 oxyNodeElement: oxyNodeKeeper
		//				 :keeperParent(parentElement)
		//				 nodeID = updateKeeper{ return keeperParent->updateKeeper(); }

			class oxyNodeKeeper : public oxyDocumentElement {
			public:
				oxyNodeKeeper(oxyDocumentPtr theInputDocument, std::wstring theElementName);
				oxyNodeKeeper(std::shared_ptr<oxyNodeKeeper> inputKeeperParent, std::wstring theElementName);
				int updateKeeper();

			protected:
				int nodeCount;
				int nodeID;
				std::shared_ptr<oxyNodeKeeper> keeperParent;
			};

			using oxyNodeKeeperPtr = std::shared_ptr<oxyNodeKeeper>;

			class oxySceneElement : public oxyNodeKeeper {
			public:
				oxySceneElement(oxyDocumentElementPtr theParent, std::wstring theElementName) = delete;
				oxySceneElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxySceneElement(oxyDocumentPtr theInputDocument) :oxyNodeKeeper(theInputDocument, L"scene") {}

				//static oxyDocumentElementPtr branchNodeElement(std::shared_ptr<oxySceneElement> theParent, std::wstring inputNodeName);
			};

			//IXMLDOMElement* insertNode(IXMLDOMElement* parent, _TCHAR* nodeName, int nodeObject) {
			//	IXMLDOMElement* retElement = NULL;
			//	HRESULT hr = S_OK;
			//	hr = theDocument->createElement(SysAllocString(L"node"), &retElement);
			//	hr = retElement->setAttribute(SysAllocString(L"nodeName"), _variant_t(nodeName));
			//	hr = retElement->setAttribute(SysAllocString(L"nodeObject"), _variant_t(nodeObject));
			//	hr = parent->appendChild(retElement, NULL);
			//	return retElement;
			//}

			class oxyNodeElement : public oxyNodeKeeper {
			public:
				oxyNodeElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyNodeElement(std::shared_ptr<oxyNodeKeeper>  theParent, std::wstring inputNodeName);
					//:oxyDocumentElement(theParent, L"node"),
					//nodeName(inputNodeName),
					//nodeObject(inputNodeObject) {}

			protected:
				//friend oxyDocumentElementPtr oxySceneElement::branchNodeElement(std::shared_ptr<oxySceneElement> theParent, std::wstring inputNodeName);


				//static oxyDocumentElementPtr createOxyNodeElement(oxyDocumentElementPtr theParent, std::wstring inputNodeName, int inputNodeObject);

				void buildListOfAttributes() override;
				std::wstring nodeName;
				int nodeObject;
			};
		}
	}
}
