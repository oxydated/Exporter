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
#include "processCamera.h"
#include "debugLog.h"

using namespace std;

void visitNodes(INode* rootNode, TimeValue t, oxyde::exporter::XML::oxyDocumentPtr theDocument) {
	nodeStack theStack;
	static int nodeObjectID = 0;
	std::vector<INodePtr> cameraNodes;

	//getRootElement();
	//getObjectListElement();
	//getGeometryElement();
	//getTransformationsElement();

	oxyde::exporter::skin::skinObjectsList::initializeSkinObjectsList(rootNode);

	oxyde::exporter::XML::oxyNodeKeeperPtr theSceneElement = std::make_shared<oxyde::exporter::XML::oxySceneElement>(theDocument);

	//oxyde::exporter::XML::oxyCamerasInSceneElementPtr camerasInSceneElement = std::make_shared<oxyde::exporter::XML::oxyCamerasInSceneElement>(theDocument);

	theStack.push(nodePair(theSceneElement, rootNode));

	while(!theStack.empty()){
		nodePair thePair = theStack.top();
		INode* topNode = thePair.second;
		//IXMLDOMElement* newParent = insertNode(thePair.first, (_TCHAR*)topNode->GetName(), nodeObjectID);
		//oxyNodeElement(oxyDocumentElementPtr theParent, std::wstring inputNodeName, int inputNodeObject)

		oxyde::exporter::XML::oxyNodeElementPtr newParent = std::make_shared<oxyde::exporter::XML::oxyNodeElement>(thePair.first, topNode->GetName());
		theStack.pop();

		DebugPrint(L"Name of the topNode: %s\n",  topNode->GetName());

		bool boneTest = false, skinTest = false, derivedTest = false;
		if (!topNode->IsRootNode()) {

			oxyde::exporter::skin::skinObjectsList::addSkinObjectToList(topNode);

			Object* theTopNodeObject = topNode->GetObjectRef();
			Class_ID theTopNodeObjectClassID = theTopNodeObject->ClassID();
			SClass_ID theTopNodeObjectSuperClassID = theTopNodeObject->SuperClassID();
			MSTR objectClassName;
			theTopNodeObject->GetClassName(objectClassName);

			oxyde::log::printText(std::wstring(L"Class Name: ") + std::wstring(objectClassName));

			if (theTopNodeObject->CanConvertToType(Class_ID(LOOKAT_CAM_CLASS_ID, 0))) {
				GenCamera* theLookAtCamera = reinterpret_cast<GenCamera*>(theTopNodeObject->ConvertToType(0, Class_ID(LOOKAT_CAM_CLASS_ID, 0)));
				cameraNodes.push_back(topNode);
				//oxyde::log::printText(std::wstring(L"Class Name: ") + std::wstring(objectClassName));
			}
		}

		getControllerInformation(topNode, newParent);

		int numChildren = topNode->NumberOfChildren();
		for (int i = 0; i < numChildren; i++) {
			INode* child = topNode->GetChildNode(i);
			theStack.push(nodePair(newParent, child));
		}

		nodeObjectID++;
	}

	oxyde::exporter::camera::exportAllCameras(theDocument, cameraNodes);

	oxyde::exporter::skin::skinObjectsList::buildSkinObjectsInList(theDocument);
}
