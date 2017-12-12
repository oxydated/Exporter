#include "nodeNavigation.h"
#include "processGeometry.h"
#include "processMaterials.h"
#include "processTransforms.h"
#include "processControllers.h"
//#include "xmlExporter.h"
#include "xmlDocumentScene.h"
#include "iskin.h"
#include <modstack.h>
#include <memory>
#include "processSkin.h"
#include "skinDataExtraction.h"

using namespace std;

void visitNodes(INode* rootNode, TimeValue t, oxyde::exporter::XML::oxyDocumentPtr theDocument) {
	nodeStack theStack;
	static int nodeObjectID = 0;

	//getRootElement();
	//getObjectListElement();
	//getGeometryElement();
	//getTransformationsElement();

	oxyde::exporter::skin::skinObjectsList::initializeSkinObjectsList(rootNode);

	oxyde::exporter::XML::oxyNodeKeeperPtr theSceneElement = std::make_shared<oxyde::exporter::XML::oxySceneElement>(theDocument);

	theStack.push(nodePair(theSceneElement, rootNode));

	while(!theStack.empty()){
		nodePair thePair = theStack.top();
		INode* topNode = thePair.second;
		//IXMLDOMElement* newParent = insertNode(thePair.first, (_TCHAR*)topNode->GetName(), nodeObjectID);
		//oxyNodeElement(oxyDocumentElementPtr theParent, std::wstring inputNodeName, int inputNodeObject)

		oxyde::exporter::XML::oxyNodeKeeperPtr newParent = std::make_shared<oxyde::exporter::XML::oxyNodeElement>(thePair.first, topNode->GetName());
		theStack.pop();

		DebugPrint(L"Name of the topNode: %s\n",  topNode->GetName());

		bool boneTest = false, skinTest = false, derivedTest = false;
		if (!topNode->IsRootNode()) {
			
			oxyde::exporter::skin::skinObjectsList::addSkinObjectToList(topNode);
		}

		//getControllerInformation(topNode, expFile, newParent);

		int numChildren = topNode->NumberOfChildren();
		for (int i = 0; i < numChildren; i++) {
			INode* child = topNode->GetChildNode(i);
			theStack.push(nodePair(newParent, child));
		}

		nodeObjectID++;
	}

	oxyde::exporter::skin::skinObjectsList::buildSkinObjectsInList();
}
