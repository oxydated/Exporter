#include "processSkin.h"
#include "processGeometry.h"
#include "xmlSkinExporter.h"

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

bool extractSkinDataFromObj(IDerivedObject* theDerivedObj, INode* theNode, TCHAR* skinNodeName, FILE* expFile){
	if (!isThereASkinModifier(theDerivedObj)) return false;
	
	static int skinID = 0;
	ISkin* theSkinInterface = NULL;
	int skinIndexInModifierStack = 0;

	int numModifiers = theDerivedObj->NumModifiers();
	for (int i = 0; i < numModifiers; i++){

		Modifier* theModifier = theDerivedObj->GetModifier(i);
		DebugPrint("Name of the modifier %i: %s\n", i, theModifier->GetName());
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

	int meshID = extractTargetMesh(theDerivedObj, skinIndexInModifierStack, expFile);

	int thisSkin = skinID++;

	ISkinContextData* theSkinContextData = theSkinInterface->GetContextInterface(theNode);
	int numVertices = theSkinContextData->GetNumPoints();

	IXMLDOMElement* skinNode = insertSkinByID(thisSkin, skinNodeName, meshID, numBones, numVertices);

	Matrix3 theMatrix;
	theSkinInterface->GetSkinInitTM(theNode, theMatrix);
	DebugPrint("	SkinInitTM row 0: (%10f, %10f, %10f)\n", theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z);
	DebugPrint("	SkinInitTM row 1: (%10f, %10f, %10f)\n", theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z);
	DebugPrint("	SkinInitTM row 2: (%10f, %10f, %10f)\n", theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z);
	DebugPrint("	SkinInitTM row 3: (%10f, %10f, %10f)\n\n", theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);

	Matrix3 theObjectTM = theNode->GetObjectTM(0);
	DebugPrint("	ObjectTM row 0: (%10f, %10f, %10f)\n", theObjectTM.GetRow(0).x, theObjectTM.GetRow(0).y, theObjectTM.GetRow(0).z);
	DebugPrint("	ObjectTM row 1: (%10f, %10f, %10f)\n", theObjectTM.GetRow(1).x, theObjectTM.GetRow(1).y, theObjectTM.GetRow(1).z);
	DebugPrint("	ObjectTM row 2: (%10f, %10f, %10f)\n", theObjectTM.GetRow(2).x, theObjectTM.GetRow(2).y, theObjectTM.GetRow(2).z);
	DebugPrint("	ObjectTM row 3: (%10f, %10f, %10f)\n\n", theObjectTM.GetRow(3).x, theObjectTM.GetRow(3).y, theObjectTM.GetRow(3).z);

	insertObjectTMforSkin(skinNode, theObjectTM);

	for (int i = 0; i < numBones; i++){
		INode* theBoneNode = theSkinInterface->GetBone(i);
		DebugPrint("Name of the bone %i: %s\n", i, theBoneNode->GetName());

		theSkinInterface->GetBoneInitTM(theBoneNode, theMatrix);
		DebugPrint("	Bone %i InitTM row 0: (%10f, %10f, %10f)\n", i, theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z);
		DebugPrint("	Bone %i InitTM row 1: (%10f, %10f, %10f)\n", i, theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z);
		DebugPrint("	Bone %i InitTM row 2: (%10f, %10f, %10f)\n", i, theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z);
		DebugPrint("	Bone %i InitTM row 3: (%10f, %10f, %10f)\n\n", i, theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);

		IXMLDOMElement* theNewBoneElement = insertBoneForSkin(skinNode, theBoneNode->GetName(), i);
		if (theNewBoneElement != NULL){
			insertObjectTMforBone(theNewBoneElement, theMatrix);
		}
	}

	//////////////////////////////////////    The skin vertices loop

	for (int i = 0; i < numVertices; i++){
		int numBonesForVertex = theSkinContextData->GetNumAssignedBones(i);
		IXMLDOMElement* theVertexNode = insertVertexForSkin(skinNode, i, numBonesForVertex);

		//////////////////////////////////////    bone weights per skin vertex loop

		for (int j = 0; j < numBonesForVertex; j++){
			int boneIndexInSkin = theSkinContextData->GetAssignedBone(i, j);
			float boneWeightForVertex = theSkinContextData->GetBoneWeight(i, j);
			insertBoneEntryForVertex(theVertexNode, j, boneIndexInSkin, boneWeightForVertex);
		}
	}
	return true;
}

int extractTargetMesh(IDerivedObject* theDerivedObj, int skinIndexInModifierStack, FILE* expFile){
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
	DebugPrint("	ObjectStateTM row 0: (%10f, %10f, %10f)\n", theMatrix_p->GetRow(0).x, theMatrix_p->GetRow(0).y, theMatrix_p->GetRow(0).z);
	DebugPrint("	ObjectStateTM row 1: (%10f, %10f, %10f)\n", theMatrix_p->GetRow(1).x, theMatrix_p->GetRow(1).y, theMatrix_p->GetRow(1).z);
	DebugPrint("	ObjectStateTM row 2: (%10f, %10f, %10f)\n", theMatrix_p->GetRow(2).x, theMatrix_p->GetRow(2).y, theMatrix_p->GetRow(2).z);
	DebugPrint("	ObjectStateTM row 3: (%10f, %10f, %10f)\n\n", theMatrix_p->GetRow(3).x, theMatrix_p->GetRow(3).y, theMatrix_p->GetRow(3).z);

	int meshID = -1;

	bool isGeometry = false;
	Mesh &theMesh = extractMeshFromObjectState(theState, 0, isGeometry);
	if (isGeometry){

		////////////////////	actual mesh extraction process

		meshID = processMesh(theMesh, expFile);
	}
	return meshID;
}