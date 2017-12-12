#include "xmlDocumentScene.h"
namespace oxyde {
	namespace exporter {
		namespace XML {

			//oxyDocumentElementPtr oxyNodeElement::createOxyNodeElement(oxyDocumentElementPtr theParent, std::wstring inputNodeName, int inputNodeObject)
			//{
			//	std::shared_ptr<oxyNodeElement> theNodeElement(new oxyNodeElement(theParent, inputNodeName, inputNodeObject));
			//	theNodeElement->buildListOfAttributes();
			//	return theNodeElement;
			//}

			void oxyNodeElement::buildListOfAttributes()
			{
				//	hr = retElement->setAttribute(SysAllocString(L"nodeName"), _variant_t(nodeName));
				//	hr = retElement->setAttribute(SysAllocString(L"nodeObject"), _variant_t(nodeObject));
				attributeList.push_back(elementAttribute(L"nodeName", _variant_t(nodeName.data())));
				attributeList.push_back(elementAttribute(L"nodeObject", _variant_t(nodeObject)));
			}

			oxyNodeKeeper::oxyNodeKeeper(oxyDocumentPtr theInputDocument, std::wstring theElementName):
				oxyDocumentElement(theInputDocument, theElementName),
				keeperParent(nullptr), nodeCount(-1)
			{
				nodeID = updateKeeper();
			}

			oxyNodeKeeper::oxyNodeKeeper(std::shared_ptr<oxyNodeKeeper> inputKeeperParent, std::wstring theElementName) :
				oxyDocumentElement(inputKeeperParent, theElementName),
				keeperParent(inputKeeperParent), nodeCount(-1)
			{
				nodeID = updateKeeper();
			}

			int oxyNodeKeeper::updateKeeper()
			{
				return  keeperParent ? keeperParent->updateKeeper() : nodeCount++;
			}

			oxyNodeElement::oxyNodeElement(std::shared_ptr<oxyNodeKeeper>  theParent, std::wstring inputNodeName) :
				oxyNodeKeeper(theParent, L"node"),
				nodeName(inputNodeName)
			{
				nodeObject = nodeID;
				buildListOfAttributes();
				setElementAttributes();
			}

			//oxyDocumentElementPtr oxySceneElement::branchNodeElement(std::shared_ptr<oxySceneElement> theParent, std::wstring inputNodeName)
			//{
			//	return oxyNodeElement::createOxyNodeElement(theParent, inputNodeName, 1);
			//}
		}
	}
}
