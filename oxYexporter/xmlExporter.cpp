#include "xmlExporter.h"
#include "resource.h"
#include "dllFunc.h"

IXMLDOMDocument3* theDocument = NULL;
IXMLDOMElement* theRootElement = NULL;
IXMLDOMElement* theSceneElement = NULL;
IXMLDOMElement* theObjectListElement = NULL;
IXMLDOMElement* theGeometryElement = NULL;
IXMLDOMElement* theTransformationsElement = NULL;
HRESULT hr = S_OK;

IXMLDOMDocument3* getTheDocument(){
	//IXMLDOMDocument3* retRoot = NULL;
	if( theDocument == NULL ){
		CoInitialize(NULL);
		hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&theDocument));
				
		theDocument->put_async(VARIANT_FALSE);
		theDocument->put_validateOnParse(VARIANT_FALSE);
		theDocument->put_resolveExternals(VARIANT_FALSE);
		theDocument->put_preserveWhiteSpace(VARIANT_TRUE);

		IXMLDOMProcessingInstruction* thePI = NULL;
		hr = theDocument->createProcessingInstruction( L"xml", L"version='1.0'", &thePI);
		hr = theDocument->appendChild(thePI, NULL);
	}
	return theDocument;
}

IXMLDOMElement* getRootElement(){
	/*IXMLDOMElement* retRoot = NULL;
	if( theDocument == NULL ){
		CoInitialize(NULL);
		hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&theDocument));
				
		theDocument->put_async(VARIANT_FALSE);
		theDocument->put_validateOnParse(VARIANT_FALSE);
		theDocument->put_resolveExternals(VARIANT_FALSE);
		theDocument->put_preserveWhiteSpace(VARIANT_TRUE);

		IXMLDOMProcessingInstruction* thePI = NULL;
		hr = theDocument->createProcessingInstruction( L"xml", L"version='1.0'", &thePI);
		hr = theDocument->appendChild(thePI, NULL);
		
		IXMLDOMElement* theElement = NULL;
		hr = theDocument->createElement(L"document", &theElement);
		hr = theDocument->appendChild(theElement, NULL);
		hr =  theDocument->get_documentElement(&theRootElement);
		retRoot = theRootElement;
	} else {
		retRoot = theRootElement;
	}*/
	theDocument = getTheDocument();

	if( theRootElement == NULL ){
		IXMLDOMElement* theElement = NULL;
		hr = theDocument->createElement(L"document", &theElement);
		hr = theDocument->appendChild(theElement, NULL);
		hr =  theDocument->get_documentElement(&theRootElement);
	}

	return theRootElement;
}

IXMLDOMElement* getSceneElement(){
	IXMLDOMElement* retScene = NULL;

	if(theSceneElement == NULL){		
		theRootElement = getRootElement();
		theDocument->createElement(SysAllocString(L"scene"), &theSceneElement);
		theRootElement->appendChild(theSceneElement, NULL);
		retScene = theSceneElement;
	}else{
		retScene = theSceneElement;
	}
	return retScene;
};

IXMLDOMElement* getObjectListElement(){
	IXMLDOMElement* retObjectLis = NULL;

	if(theObjectListElement == NULL){		
		theRootElement = getRootElement();
		theDocument->createElement(SysAllocString(L"objectList"), &theObjectListElement);
		theRootElement->appendChild(theObjectListElement, NULL);
		retObjectLis = theObjectListElement;
	}else{
		retObjectLis = theObjectListElement;
	}
	return retObjectLis;
};

IXMLDOMElement* getGeometryElement(){
	IXMLDOMElement* retGeometry = NULL;

	if(theGeometryElement == NULL){		
		theRootElement = getRootElement();
		theDocument->createElement(SysAllocString(L"geometry"), &theGeometryElement);
		theRootElement->appendChild(theGeometryElement, NULL);
		retGeometry = theGeometryElement;
	}else{
		retGeometry = theGeometryElement;
	}
	return retGeometry;
};

IXMLDOMElement* getTransformationsElement(){
	IXMLDOMElement* retTransformations = NULL;

	if(theTransformationsElement == NULL){		
		theRootElement = getRootElement();
		theDocument->createElement(SysAllocString(L"transformations"), &theTransformationsElement);
		theRootElement->appendChild(theTransformationsElement, NULL);
		retTransformations = theTransformationsElement;
	}else{
		retTransformations = theTransformationsElement;
	}
	return retTransformations;
};

IXMLDOMElement* insertNode( IXMLDOMElement* parent, _TCHAR* nodeName, int nodeObject ){
	IXMLDOMElement* retElement = NULL;
	HRESULT hr = S_OK;
	hr = theDocument->createElement(SysAllocString(L"node"), &retElement);
	hr = retElement->setAttribute(SysAllocString(L"nodeName"), _variant_t(nodeName));
	hr = retElement->setAttribute(SysAllocString(L"nodeObject"), _variant_t(nodeObject));
	hr = parent->appendChild(retElement, NULL);
	return retElement;
}

void closeFile(const _TCHAR *name){
	if( theDocument != NULL ){
		IXMLDOMDocument3* xslDoc = NULL;
		IXMLDOMDocument3* outDoc = NULL;
		hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&xslDoc));
		xslDoc->put_async(VARIANT_FALSE);
		xslDoc->put_validateOnParse(VARIANT_FALSE);
		xslDoc->put_resolveExternals(VARIANT_FALSE);
		xslDoc->put_preserveWhiteSpace(VARIANT_FALSE);
		LPTSTR theStyle = new _TCHAR[800];
		LoadString( getThisDLLModule(), IDS_STYLE_INDENT, theStyle, 800);
		BSTR theInputStyle = _bstr_t(theStyle);

		VARIANT loaded = _variant_t(false).Detach();

		hr = xslDoc->loadXML(theInputStyle, &loaded.boolVal);
		bool itWorked = (loaded.boolVal == VARIANT_TRUE);
		VariantClear(&loaded);

		hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&outDoc));
		outDoc->put_async(VARIANT_FALSE);
		outDoc->put_validateOnParse(VARIANT_FALSE);
		outDoc->put_resolveExternals(VARIANT_FALSE);
		outDoc->put_preserveWhiteSpace(VARIANT_FALSE);

		IDispatch* dispOutDoc = NULL;
		hr = outDoc->QueryInterface(__uuidof(IDispatch), (void**)&dispOutDoc);
		_variant_t vOutDoc(dispOutDoc);

		hr = theDocument->transformNodeToObject( xslDoc, vOutDoc);

		//hr = theDocument->save(_variant_t(L"myExportTestOriginal.xml"));
		//hr = outDoc->save(_variant_t("myExportTestXSL.xml"));

		hr = outDoc->save(_variant_t(name));

		if(theRootElement != NULL){
			theRootElement->Release();
			theRootElement = NULL;
		}
		if (theSceneElement != NULL){
			theSceneElement->Release();
			theSceneElement = NULL;
		}
		if (theObjectListElement != NULL){
			theObjectListElement->Release();
			theObjectListElement = NULL;
		}
		if (theGeometryElement != NULL){
			theGeometryElement->Release();
			theGeometryElement = NULL;
		}
		if (theTransformationsElement != NULL){
			theTransformationsElement->Release();
			theTransformationsElement = NULL;
		}

		xslDoc->Release();
		outDoc->Release();
		theDocument->Release();

		xslDoc = NULL;
		outDoc = NULL;
		theDocument = NULL;

		CoUninitialize();
	}
	return;
}

IXMLDOMElement* insertMeshByID( int ID, int numFaces, int numVertices ){	
//		<mesh meshID:>
	theDocument = getTheDocument();
	theGeometryElement = getGeometryElement();

	IXMLDOMElement* theMeshElement = NULL;
	hr = theDocument->createElement(L"mesh", &theMeshElement);
	hr = theMeshElement->setAttribute(SysAllocString(L"meshID"), _variant_t(ID));
	hr = theGeometryElement->appendChild(theMeshElement, NULL);

//			<faces numFaces: "int" >
	IXMLDOMElement* theFacesElement = NULL;
	hr = theDocument->createElement(L"faces", &theFacesElement);
	hr = theFacesElement->setAttribute(SysAllocString(L"numFaces"), _variant_t(numFaces));
	hr = theMeshElement->appendChild(theFacesElement, NULL);

//			<vertices numVertices: >
	IXMLDOMElement* theVerticesElement = NULL;
	hr = theDocument->createElement(L"vertices", &theVerticesElement);
	hr = theVerticesElement->setAttribute(SysAllocString(L"numVertices"), _variant_t(numVertices));
	hr = theMeshElement->appendChild(theVerticesElement, NULL);

	return theMeshElement;
}

//			<faces numFaces: "int" >

//				<face	faceID="int"	v0= "int"	v1= "int"	v2= "int"  />
//				<face 	...

IXMLDOMElement* insertFaceForMesh( IXMLDOMElement* meshNode, int ID, int v0, int v1, int v2 ){
	theDocument = getTheDocument();
	IXMLDOMNode* theFacesNode = NULL;
	hr = meshNode->selectSingleNode(SysAllocString(L"./faces"), &theFacesNode);

	if(hr==S_FALSE) return NULL;

	IXMLDOMElement* theFacesElement = NULL;
	hr = theFacesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theFacesElement);

	IXMLDOMElement* theNewFaceElement = NULL;
	hr = theDocument->createElement(L"face", &theNewFaceElement);
	hr = theNewFaceElement->setAttribute(SysAllocString(L"faceID"), _variant_t(ID));
	hr = theNewFaceElement->setAttribute(SysAllocString(L"v0"), _variant_t(v0));
	hr = theNewFaceElement->setAttribute(SysAllocString(L"v1"), _variant_t(v1));
	hr = theNewFaceElement->setAttribute(SysAllocString(L"v2"), _variant_t(v2));

	hr = theFacesElement->appendChild(theNewFaceElement, NULL);
	
	return theNewFaceElement;
}

//			<vertices numVertices: >

//				<vertex		vertexID="int" 		theVert="int"	x="float" y="float" z="float"	theTVert="int"		u="float" v="float"	 theNormal="int" nx="float" ny="float" nz="float" />
//				<vertex		...

IXMLDOMElement* insertVertexForMesh( IXMLDOMElement* meshNode, int ID, int theVert, float x, float y, float z, int theTVert, float u, float v, int theNormal, float nx, float ny, float nz ){
	theDocument = getTheDocument();
	IXMLDOMNode* theVerticesNode = NULL;
	hr = meshNode->selectSingleNode(SysAllocString(L"./vertices"), &theVerticesNode);

	if(hr==S_FALSE) return NULL;

	IXMLDOMElement* theVerticesElement = NULL;
	hr = theVerticesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theVerticesElement);

	IXMLDOMElement* theNewVerticeElement = NULL;
	hr = theDocument->createElement(L"vertex", &theNewVerticeElement);
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"vertexID"), _variant_t(ID));

	hr = theNewVerticeElement->setAttribute(SysAllocString(L"theVert"), _variant_t(theVert));
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"x"), _variant_t(x));
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"y"), _variant_t(y));
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"z"), _variant_t(z));

	hr = theNewVerticeElement->setAttribute(SysAllocString(L"theTVert"), _variant_t(theTVert));
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"u"), _variant_t(u));
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"v"), _variant_t(v));

	hr = theNewVerticeElement->setAttribute(SysAllocString(L"theNormal"), _variant_t(theNormal));
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"nx"), _variant_t(nx));
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"ny"), _variant_t(ny));
	hr = theNewVerticeElement->setAttribute(SysAllocString(L"nz"), _variant_t(nz));

	hr = theVerticesElement->appendChild(theNewVerticeElement, NULL);
	
	return theNewVerticeElement;
}