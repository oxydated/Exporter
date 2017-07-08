#include "xmlSkinExporter.h"

IXMLDOMElement* insertSkinByID(int ID, TCHAR* skinNodeName, int meshID, int numBones, int numVertices){
	//		<skin: ID, skinNodeName: skinNodeName, mesh: meshID>

	IXMLDOMDocument3* theDocument = NULL;
	IXMLDOMElement* theObjectListElement = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	theObjectListElement = getObjectListElement();

	IXMLDOMElement* theSkinElement = NULL;
	hr = theDocument->createElement(L"skin", &theSkinElement);
	hr = theSkinElement->setAttribute(SysAllocString(L"ID"), _variant_t(ID));
	hr = theSkinElement->setAttribute(SysAllocString(L"skinNodeName"), _variant_t(skinNodeName));
	hr = theSkinElement->setAttribute(SysAllocString(L"mesh"), _variant_t(meshID));
	hr = theObjectListElement->appendChild(theSkinElement, NULL);

	//		<bones: numBones>
	IXMLDOMElement* theBonesElement = NULL;
	hr = theDocument->createElement(L"bones", &theBonesElement);
	hr = theBonesElement->setAttribute(SysAllocString(L"numBones"), _variant_t(numBones));
	hr = theSkinElement->appendChild(theBonesElement, NULL);

	////			<vertices numVertices: >
	IXMLDOMElement* theVerticesElement = NULL;
	hr = theDocument->createElement(L"vertices", &theVerticesElement);
	hr = theVerticesElement->setAttribute(SysAllocString(L"numVertices"), _variant_t(numVertices));
	hr = theSkinElement->appendChild(theVerticesElement, NULL);

	return theSkinElement;
}

IXMLDOMElement* insertObjectTMforSkin(IXMLDOMElement* skinNode, Matrix3 theObjectTM){
	// <objectMatrix r0c0 = "0,3" r0c1 = "0,3" r0c2 = "0,3" r1c0 = "0,3" r1c1 = "0,3" r1c2 = "0,3" r2c0 = "0,3" r2c1 = "0,3" r2c2 = "0,3" r3c0 = "0,3" r3c1 = "0,3" r3c2 = "0,3" / >
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theObjectTMElement = NULL;
	hr = theDocument->createElement(L"objectMatrix", &theObjectTMElement);
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r0c0"), _variant_t(theObjectTM.GetRow(0).x));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r0c1"), _variant_t(theObjectTM.GetRow(0).y));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r0c2"), _variant_t(theObjectTM.GetRow(0).z));

	hr = theObjectTMElement->setAttribute(SysAllocString(L"r1c0"), _variant_t(theObjectTM.GetRow(1).x));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r1c1"), _variant_t(theObjectTM.GetRow(1).y));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r1c2"), _variant_t(theObjectTM.GetRow(1).z));

	hr = theObjectTMElement->setAttribute(SysAllocString(L"r2c0"), _variant_t(theObjectTM.GetRow(2).x));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r2c1"), _variant_t(theObjectTM.GetRow(2).y));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r2c2"), _variant_t(theObjectTM.GetRow(2).z));

	hr = theObjectTMElement->setAttribute(SysAllocString(L"r3c0"), _variant_t(theObjectTM.GetRow(3).x));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r3c1"), _variant_t(theObjectTM.GetRow(3).y));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r3c2"), _variant_t(theObjectTM.GetRow(3).z));

	hr = skinNode->appendChild(theObjectTMElement, NULL);
	return theObjectTMElement;
}

IXMLDOMElement* insertBoneForSkin(IXMLDOMElement* skinNode, _TCHAR* boneName, int boneIndex){
	IXMLDOMDocument3* theDocument = NULL;
	IXMLDOMNode* theBonesNode = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();

	hr = skinNode->selectSingleNode(SysAllocString(L"./bones"), &theBonesNode);

	if (hr == S_FALSE) return NULL;

	IXMLDOMElement* theBonesElement = NULL;
	hr = theBonesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theBonesElement);

	IXMLDOMElement* theNewBoneElement = NULL;
	hr = theDocument->createElement(L"bone", &theNewBoneElement);
	hr = theNewBoneElement->setAttribute(SysAllocString(L"boneName"), _variant_t(boneName));
	hr = theNewBoneElement->setAttribute(SysAllocString(L"boneIndex"), _variant_t(boneIndex));

	hr = theBonesElement->appendChild(theNewBoneElement, NULL);

	return theNewBoneElement;
}

IXMLDOMElement* insertObjectTMforBone(IXMLDOMElement* boneNode, Matrix3 theBoneTM){
	// <boneMatrix r0c0 = "0,3" r0c1 = "0,3" r0c2 = "0,3" r1c0 = "0,3" r1c1 = "0,3" r1c2 = "0,3" r2c0 = "0,3" r2c1 = "0,3" r2c2 = "0,3" r3c0 = "0,3" r3c1 = "0,3" r3c2 = "0,3" / >
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theObjectTMElement = NULL;
	hr = theDocument->createElement(L"boneMatrix", &theObjectTMElement);
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r0c0"), _variant_t(theBoneTM.GetRow(0).x));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r0c1"), _variant_t(theBoneTM.GetRow(0).y));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r0c2"), _variant_t(theBoneTM.GetRow(0).z));

	hr = theObjectTMElement->setAttribute(SysAllocString(L"r1c0"), _variant_t(theBoneTM.GetRow(1).x));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r1c1"), _variant_t(theBoneTM.GetRow(1).y));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r1c2"), _variant_t(theBoneTM.GetRow(1).z));

	hr = theObjectTMElement->setAttribute(SysAllocString(L"r2c0"), _variant_t(theBoneTM.GetRow(2).x));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r2c1"), _variant_t(theBoneTM.GetRow(2).y));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r2c2"), _variant_t(theBoneTM.GetRow(2).z));

	hr = theObjectTMElement->setAttribute(SysAllocString(L"r3c0"), _variant_t(theBoneTM.GetRow(3).x));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r3c1"), _variant_t(theBoneTM.GetRow(3).y));
	hr = theObjectTMElement->setAttribute(SysAllocString(L"r3c2"), _variant_t(theBoneTM.GetRow(3).z));

	hr = boneNode->appendChild(theObjectTMElement, NULL);
	return theObjectTMElement;
}

IXMLDOMElement* insertDualQuatForBone(IXMLDOMElement* boneNode, float qs, float qx, float qy, float qz, float dqs, float dqx, float dqy, float dqz) {
	/*
	<boneDualQuat dqs="19.3022807402" dqx="-6.20234476474" dqy="-1.50649298003" dqz="-23.4315604127" qs="0.769934619861" qx="0.054620723471" qy="-0.107169469365" qz="0.626683303226" />
	*/
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theDualQuatElement = NULL;

	hr = theDocument->createElement(L"boneDualQuat", &theDualQuatElement);
	hr = theDualQuatElement->setAttribute(SysAllocString(L"qs"), _variant_t(qs));
	hr = theDualQuatElement->setAttribute(SysAllocString(L"qx"), _variant_t(qx));
	hr = theDualQuatElement->setAttribute(SysAllocString(L"qy"), _variant_t(qy));
	hr = theDualQuatElement->setAttribute(SysAllocString(L"qz"), _variant_t(qz));

	hr = theDualQuatElement->setAttribute(SysAllocString(L"dqs"), _variant_t(dqs));
	hr = theDualQuatElement->setAttribute(SysAllocString(L"dqx"), _variant_t(dqx));
	hr = theDualQuatElement->setAttribute(SysAllocString(L"dqy"), _variant_t(dqy));
	hr = theDualQuatElement->setAttribute(SysAllocString(L"dqz"), _variant_t(dqz));

	hr = boneNode->appendChild(theDualQuatElement, NULL);
	return theDualQuatElement;
}

IXMLDOMElement* insertVertexForSkin(IXMLDOMElement* skinNode, int vertexIndex, int numBonesForVertex){
	IXMLDOMDocument3* theDocument = NULL;
	IXMLDOMNode* theVerticesNode = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();

	hr = skinNode->selectSingleNode(SysAllocString(L"./vertices"), &theVerticesNode);

	if (hr == S_FALSE) return NULL;

	IXMLDOMElement* theVerticesElement = NULL;
	hr = theVerticesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theVerticesElement);

	IXMLDOMElement* theNewVertexElement = NULL;
	hr = theDocument->createElement(L"vertex", &theNewVertexElement);
	hr = theNewVertexElement->setAttribute(SysAllocString(L"vertexIndex"), _variant_t(vertexIndex));
	//hr = theNewVertexElement->setAttribute(SysAllocString(L"numBonesForVertex"), _variant_t(numBonesForVertex));

	hr = theVerticesElement->appendChild(theNewVertexElement, NULL);

	// bonesForVertex: numBonesForVertex
	IXMLDOMElement* theBonesForVertexElement = NULL;
	hr = theDocument->createElement(L"bonesForVertex", &theBonesForVertexElement);
	theBonesForVertexElement->setAttribute(SysAllocString(L"numBonesForVertex"), _variant_t(numBonesForVertex));
	theNewVertexElement->appendChild(theBonesForVertexElement, NULL);

	return theNewVertexElement;
}

IXMLDOMElement* insertBoneEntryForVertex(IXMLDOMElement* vertexNode, int boneIndexForVertex, int boneIndexForSkin, float weight){
	IXMLDOMDocument3* theDocument = NULL;
	IXMLDOMNode* theBonesForVertexNode = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();

	hr = vertexNode->selectSingleNode(SysAllocString(L"./bonesForVertex"), &theBonesForVertexNode);

	if (hr == S_FALSE) return NULL;

	IXMLDOMElement* theBonesForVertexElement = NULL;
	hr = theBonesForVertexNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theBonesForVertexElement);

	IXMLDOMElement* theNewBoneEntryElement = NULL;
	hr = theDocument->createElement(L"boneEntry", &theNewBoneEntryElement);
	hr = theNewBoneEntryElement->setAttribute(SysAllocString(L"boneIndexForVertex"), _variant_t(boneIndexForVertex));
	hr = theNewBoneEntryElement->setAttribute(SysAllocString(L"boneIndexForSkin"), _variant_t(boneIndexForSkin));
	hr = theNewBoneEntryElement->setAttribute(SysAllocString(L"weight"), _variant_t(weight));
	hr = theBonesForVertexElement->appendChild(theNewBoneEntryElement, NULL);

	return theNewBoneEntryElement;
}