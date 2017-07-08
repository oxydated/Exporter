#include "processSkin.h"
#include "processGeometry.h"
#include "xmlSkinExporter.h"
#include "matrixUtility.h"

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

bool extractSkinDataFromObj(IDerivedObject* theDerivedObj, INode* theNode, _TCHAR* skinNodeName, FILE* expFile){
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

	int meshID = extractTargetMesh(theDerivedObj, skinIndexInModifierStack, expFile);

	int thisSkin = skinID++;

	ISkinContextData* theSkinContextData = theSkinInterface->GetContextInterface(theNode);
	int numVertices = theSkinContextData->GetNumPoints();

	IXMLDOMElement* skinNode = insertSkinByID(thisSkin, skinNodeName, meshID, numBones, numVertices);

	Matrix3 theMatrix;
	theSkinInterface->GetSkinInitTM(theNode, theMatrix);
	DebugPrint(L"	SkinInitTM row 0: (%10f, %10f, %10f)\n", theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z);
	DebugPrint(L"	SkinInitTM row 1: (%10f, %10f, %10f)\n", theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z);
	DebugPrint(L"	SkinInitTM row 2: (%10f, %10f, %10f)\n", theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z);
	DebugPrint(L"	SkinInitTM row 3: (%10f, %10f, %10f)\n\n", theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);

	Matrix3 theObjectTM = theNode->GetObjectTM(0);
	DebugPrint(L"	ObjectTM row 0: (%10f, %10f, %10f)\n", theObjectTM.GetRow(0).x, theObjectTM.GetRow(0).y, theObjectTM.GetRow(0).z);
	DebugPrint(L"	ObjectTM row 1: (%10f, %10f, %10f)\n", theObjectTM.GetRow(1).x, theObjectTM.GetRow(1).y, theObjectTM.GetRow(1).z);
	DebugPrint(L"	ObjectTM row 2: (%10f, %10f, %10f)\n", theObjectTM.GetRow(2).x, theObjectTM.GetRow(2).y, theObjectTM.GetRow(2).z);
	DebugPrint(L"	ObjectTM row 3: (%10f, %10f, %10f)\n\n", theObjectTM.GetRow(3).x, theObjectTM.GetRow(3).y, theObjectTM.GetRow(3).z);

	insertObjectTMforSkin(skinNode, theObjectTM);

	for (int i = 0; i < numBones; i++){
		INode* theBoneNode = theSkinInterface->GetBone(i);
		DebugPrint(L"Name of the bone %i: %s\n", i, theBoneNode->GetName());

		theSkinInterface->GetBoneInitTM(theBoneNode, theMatrix);
		DebugPrint(L"	Bone %i InitTM row 0: (%10f, %10f, %10f)\n", i, theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z);
		DebugPrint(L"	Bone %i InitTM row 1: (%10f, %10f, %10f)\n", i, theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z);
		DebugPrint(L"	Bone %i InitTM row 2: (%10f, %10f, %10f)\n", i, theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z);
		DebugPrint(L"	Bone %i InitTM row 3: (%10f, %10f, %10f)\n\n", i, theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);

		IXMLDOMElement* theNewBoneElement = insertBoneForSkin(skinNode, (_TCHAR*)theBoneNode->GetName(), i);
		if (theNewBoneElement != NULL){
			insertObjectTMforBone(theNewBoneElement, theMatrix);
			
			double m[16];
			m[0] = theMatrix.GetRow(0).x;		m[1] = theMatrix.GetRow(0).y;		m[2] = theMatrix.GetRow(0).z;		m[3] = 0.;
			m[4] = theMatrix.GetRow(1).x;		m[5] = theMatrix.GetRow(1).y;		m[6] = theMatrix.GetRow(1).z;		m[7] = 0.;
			m[8] = theMatrix.GetRow(2).x;		m[9] = theMatrix.GetRow(2).y;		m[10] = theMatrix.GetRow(2).z;		m[11] = 0.;
			m[12] = theMatrix.GetRow(3).x;		m[13] = theMatrix.GetRow(3).y;		m[14] = theMatrix.GetRow(3).z;		m[15] = 1.;
			float qs, qx, qy, qz, dqs, dqx, dqy, dqz;
			oxyde::math::getDualQuaternionFromMatrix(m, qs, qx, qy, qz, dqs, dqx, dqy, dqz);
			insertDualQuatForBone(theNewBoneElement, qs, qx, qy, qz, dqs, dqx, dqy, dqz);

			std::wstring outStr;
			std::wostringstream outStream(outStr);
			outStream << std::endl << "______________________________________________________" << std::endl << std::endl;

			outStream << "ObjectTMforBone = Transpose[{";
			outStream << "{" << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << "}" << ",";
			outStream << "{" << m[4] << ", " << m[5] << ", " << m[6] << ", " << m[7] << "}" << ",";
			outStream << "{" << m[8] << ", " << m[9] << ", " << m[10] << ", " << m[11] << "}" << ",";
			outStream << "{" << m[12] << ", " << m[13] << ", " << m[14] << ", " << m[15] << "}" << "}]";
			outStream << std::endl << std::endl;

			outStream << "DualQuatForBone = {";
			outStream << qs << "," << qx << "," << qy << "," << qz << ",";
			outStream << dqs << "," << dqx << "," << dqy << "," << dqz;
			outStream << "}.dualQuatUnit";
			outStream << std::endl;
			outStream << "rotationMatrixFromDualQuat[dualQuatUnit]" << std::endl;

			outStream << std::endl << "______________________________________________________" << std::endl << std::endl;

			std::wstring resultString(outStream.str());
			DebugOutputString(resultString.c_str());
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
	DebugPrint(L"	ObjectStateTM row 0: (%10f, %10f, %10f)\n", theMatrix_p->GetRow(0).x, theMatrix_p->GetRow(0).y, theMatrix_p->GetRow(0).z);
	DebugPrint(L"	ObjectStateTM row 1: (%10f, %10f, %10f)\n", theMatrix_p->GetRow(1).x, theMatrix_p->GetRow(1).y, theMatrix_p->GetRow(1).z);
	DebugPrint(L"	ObjectStateTM row 2: (%10f, %10f, %10f)\n", theMatrix_p->GetRow(2).x, theMatrix_p->GetRow(2).y, theMatrix_p->GetRow(2).z);
	DebugPrint(L"	ObjectStateTM row 3: (%10f, %10f, %10f)\n\n", theMatrix_p->GetRow(3).x, theMatrix_p->GetRow(3).y, theMatrix_p->GetRow(3).z);

	int meshID = -1;

	bool isGeometry = false;
	Mesh &theMesh = extractMeshFromObjectState(theState, 0, isGeometry);
	if (isGeometry){

		////////////////////	actual mesh extraction process

		meshID = processMesh(theMesh, expFile);
	}
	return meshID;
}