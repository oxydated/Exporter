#include "nodeNavigation.h"
#include "processGeometry.h"
#include "processMaterials.h"
#include "processTransforms.h"
#include "processControllers.h"
#include "xmlExporter.h"
#include "iskin.h"
#include <modstack.h>
#include "processSkin.h"

using namespace std;

void visitNodes(INode* rootNode, TimeValue t, FILE* expFile){
	nodeStack theStack;
	static int nodeObjectID = 0;

	getRootElement();
	getObjectListElement();
	getGeometryElement();
	getTransformationsElement();

	theStack.push(nodePair(getSceneElement(), rootNode));

	while(!theStack.empty()){
		nodePair thePair = theStack.top();
		INode* topNode = thePair.second;
		IXMLDOMElement* newParent = insertNode(thePair.first, (_TCHAR*)topNode->GetName(), nodeObjectID);
		theStack.pop();

		DebugPrint(L"Name of the topNode: %s\n",  topNode->GetName());

		bool boneTest = false, skinTest = false, derivedTest = false;
		if (!topNode->IsRootNode()){
			boneTest = false;
			skinTest = false;
			derivedTest = false;
			Object* theTopNodeObject = topNode->GetObjectRef();
			Class_ID theTopNodeObjectClassID = theTopNodeObject->ClassID();
			SClass_ID theTopNodeObjectSuperClassID = theTopNodeObject->SuperClassID();
			boneTest = (BONE_OBJ_CLASSID == theTopNodeObjectClassID);
			derivedTest = (GEN_DERIVOB_CLASS_ID == theTopNodeObjectSuperClassID );
			if (derivedTest){
				IDerivedObject* theDerivedObj = (IDerivedObject*)theTopNodeObject;

				if (isThereASkinModifier(theDerivedObj)){
					extractSkinDataFromObj(theDerivedObj, topNode, (_TCHAR*)topNode->GetName(), expFile);
				}
			}
			bool toSee = boneTest;
		}
		
		fprintf(expFile, "node name: %s\n", topNode->GetName());
		
		getNodeTransformations( topNode, t, expFile);
		getControllerInformation(topNode, expFile, newParent);

		int numChildren = topNode->NumberOfChildren();
		for( int i = 0; i < numChildren; i++){
			INode* child = topNode->GetChildNode(i);
			theStack.push(nodePair(newParent, child));
		}

		nodeObjectID++;
	}
	//closeFile("teste");
}
