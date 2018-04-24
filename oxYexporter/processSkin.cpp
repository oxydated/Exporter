#include <stdmat.h>
#include <Path.h>
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

bool extractSkinDataFromObj(IDerivedObject* theDerivedObj, INode* theNode, _TCHAR* skinNodeName,
	oxyde::exporter::XML::oxyObjectListPtr theObjectList, oxyde::exporter::XML::oxyGeometryElementPtr theGeometrySection) {

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

	//** create mesh node in geometry section here

	int meshID = extractTargetMesh(theDerivedObj, theNode, skinIndexInModifierStack, theGeometrySection);

	int thisSkin = skinID++;

	ISkinContextData* theSkinContextData = theSkinInterface->GetContextInterface(theNode);
	int numVertices = theSkinContextData->GetNumPoints();

	//** create skin node here

	//theSkinElement->buildSkinElement();
	//oxyde::exporter::XML::oxySkinElementPtr dumbTest = theSkinElement->getPtr();

	//IXMLDOMElement* skinNode = insertSkinByID(thisSkin, skinNodeName, meshID, numBones, numVertices);

	Matrix3 theMatrix;
	theSkinInterface->GetSkinInitTM(theNode, theMatrix);

	Matrix3 theObjectTM = theNode->GetObjectTM(0);

	//insertObjectTMforSkin(skinNode, theObjectTM);	

	oxyde::exporter::XML::oxySkinElementPtr theSkinElement = oxyde::exporter::XML::oxySkinElement::createSkinElement(theObjectList, skinNodeName, meshID,
		numBones,
		numVertices,
		theObjectTM.GetRow(0).x, theObjectTM.GetRow(0).y, theObjectTM.GetRow(0).z,
		theObjectTM.GetRow(1).x, theObjectTM.GetRow(1).y, theObjectTM.GetRow(1).z,
		theObjectTM.GetRow(2).x, theObjectTM.GetRow(2).y, theObjectTM.GetRow(2).z,
		theObjectTM.GetRow(3).x, theObjectTM.GetRow(3).y, theObjectTM.GetRow(3).z);

	oxyde::exporter::skin::skinPoseCorrector theSkinPoseCorrector(theSkinInterface);

	for (int i = 0; i < numBones; i++) {
		INode* theBoneNode = theSkinInterface->GetBone(i);

		theSkinInterface->GetBoneInitTM(theBoneNode, theMatrix);

		//IXMLDOMElement* theNewBoneElement = insertBoneForSkin(skinNode, (_TCHAR*)theBoneNode->GetName(), i);
		/*if (theNewBoneElement != NULL)*/ {
			//insertObjectTMforBone(theNewBoneElement, theMatrix);

			dualQuat q = theSkinPoseCorrector.getSkinPosedualQuatForNode(theBoneNode);

			oxyde::log::printLine();

			oxyde::log::printDualQuat(L"DualQuatforBoneQ", DUALQUAARRAY(q));

			theSkinElement->addBone((_TCHAR*)theBoneNode->GetName(), i,
				theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z,
				theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z,
				theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z,
				theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z,
				DUALQUAARRAY(q)
			);
			//insertDualQuatForBone(theNewBoneElement, DUALQUAARRAY(q));

		}
	}

	//////////////////////////////////////    The skin vertices loop

	for (int i = 0; i < numVertices; i++){
		int numBonesForVertex = theSkinContextData->GetNumAssignedBones(i);

		//auto theSkinVertex =  theSkinElement->addVertex(i);
		oxyde::exporter::XML::oxySkinElement::oxySkinVertexPtr theSkinVertex  = theSkinElement->addVertex(i, numBonesForVertex);
		//IXMLDOMElement* theVertexNode = insertVertexForSkin(skinNode, i, numBonesForVertex);

		//////////////////////////////////////    bone weights per skin vertex loop

		for (int j = 0; j < numBonesForVertex; j++){
			int boneIndexInSkin = theSkinContextData->GetAssignedBone(i, j);
			float boneWeightForVertex = theSkinContextData->GetBoneWeight(i, j);
			theSkinVertex->addBoneEntry(j, boneIndexInSkin, boneWeightForVertex);
			//insertBoneEntryForVertex(theVertexNode, j, boneIndexInSkin, boneWeightForVertex);
		}
	}
	return true;
}

int extractTargetMesh(IDerivedObject* theDerivedObj, INode* theNode, int skinIndexInModifierStack, oxyde::exporter::XML::oxyGeometryElementPtr theGeometrySection) {
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
		//** pass mesh node to this function
		std::wstring textureFileName = L"";
		Mtl* theMaterial = theNode->GetMtl();
		if (theMaterial != nullptr || theMaterial->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
			StdMat *theStdMaterial = (StdMat*)(theMaterial);
			Texmap* theTexMap = theStdMaterial->GetSubTexmap(ID_DI);
			if (theTexMap != nullptr || theTexMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0)) {
				BitmapTex* theBitmap = (BitmapTex*)(theTexMap);
				textureFileName = std::wstring(MaxSDK::Util::Path(theBitmap->GetMapName()).StripToLeaf().GetString());
			}

		}
		meshID = processMesh(theMesh, theGeometrySection, textureFileName);
	}
	return meshID;
}