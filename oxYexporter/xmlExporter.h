#pragma once

#ifndef __XMLEXPORTER_h__
#define __XMLEXPORTER_h__

#include <Windows.h>
#include <stdio.h>
#include <ObjBase.h>
#include <msxml6.h>
#include <comutil.h>
#include <tchar.h>

IXMLDOMDocument3* getTheDocument();
//<document>

IXMLDOMElement* getRootElement();

//	<scene>

IXMLDOMElement* getSceneElement();

//		<node nodeName="str" nodeMatrix="int"	nodeObject="int">
IXMLDOMElement* insertNode( IXMLDOMElement* parent, _TCHAR* nodeName, int nodeObject );
//			<node	...

//		</node>

//	</scene>

//	<objectList>

IXMLDOMElement* getObjectListElement();


//		<object  	objectID="int"	objType="int">

//			<objectMesh id="int" />

//		</object>

//	</objectList>

//	<geometry>

IXMLDOMElement* getGeometryElement();

//		<mesh meshID:>

IXMLDOMElement* insertMeshByID( int ID, int numFaces, int numVertices );

//			<faces numFaces: "int" >

//				<face	faceID="int"	v0= "int"	v1= "int"	v2= "int"  />
//				<face 	...

IXMLDOMElement* insertFaceForMesh( IXMLDOMElement* meshNode, int ID, int v0, int v1, int v2 );

//			</faces>

//			<vertices numVertices: >

//				<vertex		vertexID="int" 		theVert="int"	x="float" y="float" z="float"	theTVert="int"		u="float" v="float"	 theNormal="int" nx="float" ny="float" nz="float" />
//				<vertex		...

IXMLDOMElement* insertVertexForMesh( IXMLDOMElement* meshNode, int ID, int theVert, float x, float y, float z, int theTVert, float u, float v, int theNormal, float nx, float ny, float nz );

//			</vertices>

//		</mesh>

//	</geometry>

//	<transformations>

IXMLDOMElement* getTransformationsElement();

//		<matrix matrixID="int"	m0="float"	m1="float"	m2="float"	m3="float"	
//								m4="float"	m5="float"	m6="float"	m7="float"	
//								m8="float"	m9="float"	m10="float"	m11="float"	
//								m12="float"	m13="float"	m14="float"	m15="float" />

//	</transformations>

//</document>
void closeFile(const _TCHAR *name);

#endif