#include "processSkin.h"
#include "processGeometry.h"
//#include "xmlSkinExporter.h"
#include "matrixUtility.h"
#include "debugLog.h"
#include "dualQuaternionMath.h"
#include "skinDataExtraction.h"

bool isThereASkinModifier(IDerivedObject* theDerivedObj){
	bool thereIsASkinModifier = false;
	int numModifiers = theDerivedObj->NumModifiers();
	for (int i = 0; i < numModifiers; i++){
		Modifier* theModifier = theDerivedObj->GetModifier(i);
		Class_ID modifierClassID = theModifier->ClassID();
		bool isSkinModifier = (SKIN_CLASSID == modifierClassID);
		if (isSkinModifier){
			thereIsASkinModifier = true;
		}
	}
	return thereIsASkinModifier;
}

bool extractSkinDataFromObj(IDerivedObject* theDerivedObj, INode* theNode, _TCHAR* skinNodeName) {

	using dualQuat = std::array<double, 8>;
	using matrix4x4 = std::array<double, 16>;

	if (!isThereASkinModifier(theDerivedObj)) return false;
	
	static int skinID = 0;
	ISkin* theSkinInterface = NULL;
	int skinIndexInModifierStack = 0;

	int numModifiers = theDerivedObj->NumModifiers();
	for (int i = 0; i < numModifiers; i++){

		Modifier* theModifier = theDerivedObj->GetModifier(i);
		DebugPrint(L"Name of the modifier %i: %s\n", i, theModifier->GetName());
		Class_ID modifierClassID = theModifier->ClassID();
		bool isSkinModifier = (SKIN_CLASSID == modifierClassID);
		if (isSkinModifier){
			skinIndexInModifierStack = i;
			theSkinInterface = static_cast<ISkin*>(theModifier->GetInterface(I_SKIN));
		}
	}
	if (theSkinInterface == NULL) return false;

	int numBones = theSkinInterface->GetNumBones();

	///////////////// extracting the mesh data for the skin object

	int meshID = extractTargetMesh(theDerivedObj, skinIndexInModifierStack);

	int thisSkin = skinID++;

	ISkinContextData* theSkinContextData = theSkinInterface->GetContextInterface(theNode);
	int numVertices = theSkinContextData->GetNumPoints();

	//IXMLDOMElement* skinNode = insertSkinByID(thisSkin, skinNodeName, meshID, numBones, numVertices);

	Matrix3 theMatrix;
	theSkinInterface->GetSkinInitTM(theNode, theMatrix);

	Matrix3 theObjectTM = theNode->GetObjectTM(0);

	//insertObjectTMforSkin(skinNode, theObjectTM);

	oxyde::exporter::skin::skinPoseCorrector theSkinPoseCorrector(theSkinInterface);

	for (int i = 0; i < numBones; i++){
		INode* theBoneNode = theSkinInterface->GetBone(i);

		theSkinInterface->GetBoneInitTM(theBoneNode, theMatrix);

		//IXMLDOMElement* theNewBoneElement = insertBoneForSkin(skinNode, (_TCHAR*)theBoneNode->GetName(), i);
		/*if (theNewBoneElement != NULL)*/{
			//insertObjectTMforBone(theNewBoneElement, theMatrix);
			
			dualQuat q = theSkinPoseCorrector.getSkinPosedualQuatForNode(theBoneNode);

			oxyde::log::printLine();

			oxyde::log::printDualQuat(L"DualQuatforBoneQ", DUALQUAARRAY(q));
			//insertDualQuatForBone(theNewBoneElement, DUALQUAARRAY(q));

		}
	}

	//////////////////////////////////////    The skin vertices loop

	for (int i = 0; i < numVertices; i++){
		int numBonesForVertex = theSkinContextData->GetNumAssignedBones(i);
		//IXMLDOMElement* theVertexNode = insertVertexForSkin(skinNode, i, numBonesForVertex);

		//////////////////////////////////////    bone weights per skin vertex loop

		for (int j = 0; j < numBonesForVertex; j++){
			int boneIndexInSkin = theSkinContextData->GetAssignedBone(i, j);
			float boneWeightForVertex = theSkinContextData->GetBoneWeight(i, j);
			//insertBoneEntryForVertex(theVertexNode, j, boneIndexInSkin, boneWeightForVertex);
		}
	}
	return true;
}

int extractTargetMesh(IDerivedObject* theDerivedObj, int skinIndexInModifierStack) {
	ObjectState theState;
	int nextModifier = skinIndexInModifierStack + 1;
	if (nextModifier == (theDerivedObj->NumModifiers())){
		theState = theDerivedObj->GetObjRef()->Eval(0);
	}
	else {
		theState = theDerivedObj->Eval(0, nextModifier);
	}

	Matrix3* theMatrix_p = NULL;
	theMatrix_p = theState.GetTM();

	int meshID = -1;

	bool isGeometry = false;
	Mesh &theMesh = extractMeshFromObjectState(theState, 0, isGeometry);
	if (isGeometry){

		////////////////////	actual mesh extraction process

		meshID = processMesh(theMesh);
	}
	return meshID;
}