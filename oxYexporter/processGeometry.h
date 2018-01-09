#ifndef __PROCESSGEOMETRY_H__
#define __PROCESSGEOMETRY_H__

#include <max.h>
#include <triobj.h>
#include <mesh.h>
#include <MeshNormalSpec.h>
#include <object.h>
#include "xmlDocumentMesh.h"

/*
	notes about geometry in 3dsmax:

	TriObject->ObjectState [ TriObject::Eval() ] ---> every object that can be deformed must be able to be converted to a TriObject

	INode->ObjectState [ EvalWorldState() ] ---> this is the final geometry, the one that actually shows up in the display

	(Object*) ObjectState::obj -> TriObject [ ConvertToType() ] ---> from a ObjectState to TriObject [ need to check CanConvertToType() ]

	TriObject->Mesh [ GetMesh() ] ---> access the underlying Mesh of the TriObject

	Mesh->AdjEdgeList [ AdjEdgeList (Mesh &amesh) ] ---> construct a Edge Adjacency list

	Mesh, AdjEdgeList ->  AdjFaceList  [ AdjFaceList(	Mesh & 	mesh, AdjEdgeList & el ) ]

	Mesh-> MNMesh  [ MNMesh (const Mesh &from) ] ---> Mesh based on a Winged Edge Structure
*/

/*
	identifying the unique vertices

	 theMesh.faces[theFace].getVert(i) ... vertex in mesh.verts and in 

	 RVertex & theRVertex = theMesh.getRVert( theMesh.faces[theFace].getVert(i) );

	 RNormal &theRNormal = theRVertex.ern[rn];
				if( theRNormal.getSmGroup() & theMesh.faces[theFace].getSmGroup() )   .... looping and looking for it
	
	theMesh.mapFaces(mp)[i].getTVert(0) ... i is the same as the index of the mesh face. return index de TVert in :

	theMesh.mapVerts(mp)[i] ... to get the texcoord

	****************
	in the case the mesh was modified by Edit Normals:

	MeshNormalSpec* theSpec = theMesh.GetSpecifiedNormals();

	MeshNormalFace &theNormalFace = theSpec->Face( theFace );   ... the same face index as in theMesh.faces

	theSpec->GetNormalArray()[ theNormalFace.GetNormalID(i) ]

	*****************
	So:

	theMesh.mapFaces(mp)[i].getTVert(0)   --> primary key to vertex

	.... get shared position in theMesh.faces[theFace].getVert(i) --> that makes necessary to visit face to get vertex actual position
	     
		 ....RNormal &theRNormal = theRVertex.ern[rn];
				if( theRNormal.getSmGroup() & theMesh.faces[theFace].getSmGroup() ) ---> rn as the secondary key to vertex

		 OR

		 ....MeshNormalFace &theNormalFace = theSpec->Face( theFace );

				theNormalFace.GetNormalID(i) ---> index of normal as the secondary key to vertex

	CONCLUSION: each final vertex indentified by two keys: the TVert and the normal (RNormal or MeshNormalSpec normal)

*/
class finalFace{
private:
	static int numFinalFaces;
	static finalFace* faces;
public:
	static void startBuildFaces( int in_numFinalFaces ){ 
		if(faces != NULL){
			numFinalFaces = 0;
			delete faces;
			faces = NULL;
		}
		numFinalFaces = in_numFinalFaces; 
		faces = new finalFace[numFinalFaces]; 
	}

	static void setFace( int index, int corner, int vert ){ faces[index].setVertexForCorner( corner, vert ); }
	
	static finalFace& getFace( int index ){ return faces[index]; }

	static int getNumFinalFaces(){ return numFinalFaces; }

private:
	int v0;
	int v1;
	int v2;

public:
	finalFace():v0(-1), v1(-1), v2(-1){}

	int getVertexForCorner( int corner ){
		switch(corner){
		case 0: return v0; break;
		case 1: return v1; break;
		case 2: return v2; break;
		}
		return -1;
	}
	
	void setVertexForCorner( int corner, int vert ){
		switch(corner){
		case 0: v0 = vert; break;
		case 1: v1 = vert; break;
		case 2: v2 = vert; break;
		}
	}
};

class finalVertex{
private:
	static int numFinals;
	static finalVertex* vertices;

public:
	static void startVertices( int in_numFinals ){ 
		if(vertices != NULL){
			numFinals = 0;
			delete vertices;
			vertices = NULL;
		}
		numFinals = in_numFinals; 
		vertices = new finalVertex[numFinals]; 
	}

	static void setVertex( int index, int in_vertex, int in_tVertex, int in_normal ){
		vertices[index].set( in_vertex, in_tVertex, in_normal );
	}

	static finalVertex& getVertex( int index ){ return vertices[index]; }

	static int getNumFinals(){ return numFinals; }

private:
	int theVertex;
	int theTVertex;
	int theNormal;
	int numIncidentFaces;

	finalVertex(): theVertex(-1), theTVertex(-1), theNormal(-1), numIncidentFaces(0) {}

public:
	int getNumIncidentFaces(){ return numIncidentFaces; }

	int getVert() { return theVertex; }

	int getTVert() { return theTVertex; }

	int getNormal() { return theNormal; }

	void set( int in_vertex, int in_tVertex, int in_normal ){
		theVertex = in_vertex;
		theTVertex = in_tVertex;
		theNormal = in_normal;
		numIncidentFaces++;
	}
};

class vertexBuilder{

public:
	static void startBuilder( Mesh *in_theMesh, int mp );

	static int getVertexIndexPerFaceAndCorner( int theFace, int theCorner );

	static int getTVertIndexPerFaceAndCorner( int theFace, int theCorner );

	static int getNormalIndexPerFaceAndCorner( int theFace, int theCorner );

	static Point3 getVertexPosForFinal( int finalIndex );

	static Point3 getNormalForFinal( int finalIndex );

	static UVVert getTexCoordForFinal( int finalIndex );

private:
	static Mesh *theMesh;
	static vertexBuilder *vertices;
	static int numVertices;
	static int numFinalVert;
	static bool usingNormalSpec;
	static int mp;

public:

	int addNormalIndex( int nIndex );

	~vertexBuilder(){
		if( normals != NULL) delete [] normals;
		if( normalUsed != NULL) delete [] normalUsed;
		if( finalVertex != NULL) delete [] finalVertex;
	}

private:
	int numFaces;
	int theVertex;
	int theTVertex;
	int *normals;
	bool *normalUsed;
	int *finalVertex;

	vertexBuilder():theTVertex(-1), theVertex(-1), numFaces(0), normals(NULL), normalUsed(NULL), finalVertex(NULL){}

	void init( int in_theTVertex ){
		theTVertex = in_theTVertex;
	}
};

Mesh& extractMeshFromObjectState(ObjectState theState, TimeValue t, bool &isGeometry);

//Mesh& extractMeshFromNode( INode* theNode, TimeValue t, bool &isGeometry, FILE* expFile = stdout );
Mesh& extractMeshFromNode(INode* theNode, TimeValue t, bool &isGeometry);

//int processMesh( Mesh &theMesh, FILE* expFile = stdout );
int processMesh(Mesh &theMesh, oxyde::exporter::XML::oxyGeometryElementPtr theGeometrySection);

void getNormalsPerMeshFace( Mesh &theMesh, int theFace );

#endif