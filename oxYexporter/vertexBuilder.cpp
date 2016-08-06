#include "processGeometry.h"

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

int finalFace::numFinalFaces = 0;

finalFace* finalFace::faces = NULL;

//

int finalVertex::numFinals = 0;

finalVertex* finalVertex::vertices = NULL;

//

Mesh* vertexBuilder::theMesh = NULL;

vertexBuilder* vertexBuilder::vertices = NULL;

int vertexBuilder::numVertices = 0;

int vertexBuilder::numFinalVert = 0;

bool vertexBuilder::usingNormalSpec = false;

int vertexBuilder::mp = -1;


void vertexBuilder::startBuilder( Mesh *in_theMesh, int in_mp ){
	if(vertices != NULL){
		mp = -1;
		numVertices = 0;
		numFinalVert = 0;
		usingNormalSpec = false;
		delete [] vertices;
		vertices = NULL;
	}
	mp = in_mp;
	theMesh = in_theMesh;


	/// getting the number of TVerts. that will be the number of vertices (vertexBuilder). Vertices will be indexed by the TVerts on mesh
	numFinalVert = theMesh->getNumMapVerts(mp);
	vertices = new vertexBuilder[numFinalVert];
	for( int i = 0; i < numFinalVert; i++){
		vertices[i].init(i);
	}

	/// checking for use of NormalSpec
	
	MeshNormalSpec* theSpec = theMesh->GetSpecifiedNormals();
	if(theSpec != NULL){
		bool support = theSpec->GetFlag(MESH_NORMAL_MODIFIER_SUPPORT);
		int numNormals = theSpec->GetNumNormals();
		int numFaces = theSpec->GetNumFaces();
		if( (numNormals > 0) && (numFaces > 0) ){
			usingNormalSpec = true;
		} else usingNormalSpec = false;
	} else usingNormalSpec = false;

	/// running through faces. Get the actual position vertices. get numFaces for each vertexBuilder in vertices. Initialize normals array for each vertexbuilder

	for( int i = 0; i < theMesh->numFaces; i++ ){
		for( int corner = 0; corner < 3; corner++ ){
			int theTVert = getTVertIndexPerFaceAndCorner( i, corner );
			vertices[theTVert].numFaces++;
			vertices[theTVert].theVertex = getVertexIndexPerFaceAndCorner( i, corner );
			vertices[theTVert].theTVertex = theTVert;
		}
	}

	for( int i = 0; i < numFinalVert; i++ ){
		if( vertices[i].numFaces > 0){
			vertices[i].finalVertex = new int[vertices[i].numFaces];
			vertices[i].normals = new int[vertices[i].numFaces];
			vertices[i].normalUsed = new bool[vertices[i].numFaces];
			for( int j = 0; j < vertices[i].numFaces; j++){
				vertices[i].finalVertex[j] = -1;
				vertices[i].normals[j] = -1;
				vertices[i].normalUsed[j] = false;
			}
		}
	}

	/// running through faces again. This time get every normal. it's a good moment to build the finalFaces array

	finalFace::startBuildFaces( theMesh->numFaces );

	for( int i = 0; i < theMesh->numFaces; i++ ){
		for( int corner = 0; corner < 3; corner++ ){
			int theTVert = getTVertIndexPerFaceAndCorner( i, corner );
			int thisCornerFinalIndex = vertices[theTVert].addNormalIndex( getNormalIndexPerFaceAndCorner( i, corner ));
			finalFace::getFace(i).setVertexForCorner( corner, thisCornerFinalIndex );
		}
	}

	/// now, build the finalVertices array, running through the vertexBuilder's own array
	finalVertex::startVertices( numVertices );

	for( int i = 0; i < numFinalVert; i++ ){
		int theVertex = vertices[i].theVertex;
		int theTVertex = vertices[i].theTVertex;

		if( vertices[i].numFaces > 0){
			for( int j = 0; j < vertices[i].numFaces; j++ ){
				if(vertices[i].normalUsed[j] ){
					int theNormal = vertices[i].normals[j];
					finalVertex::getVertex( vertices[i].finalVertex[j] ).set( theVertex, theTVertex, theNormal );
				}
			}
		}
	}
}

int vertexBuilder::addNormalIndex( int nIndex ){
	/// verify if nIndex is already in normals. check against normalused. Insert it and return true if not. return false otherwise
	bool foundEmptyPosition = false;
	int emptyPosition = -1;
	bool foundIndex = false;
	int theFinalIndex = -1;
	for( int i = 0; i < numFaces; i++){
		if( !foundEmptyPosition ){
			if( !normalUsed[i] ){
				emptyPosition = i;
				foundEmptyPosition = true;
			}
		}

		if(!foundIndex){
			foundIndex = ( (normals[i]==nIndex) && normalUsed[i] );
			if(foundIndex){
				theFinalIndex = finalVertex[i];
			}
		}
	}
	if(!foundIndex){
		if( foundEmptyPosition ){
			normals[emptyPosition] = nIndex;
			normalUsed[emptyPosition] = true;
			return finalVertex[emptyPosition] = numVertices++;
		}
	}
	return theFinalIndex;
}

int vertexBuilder::getNormalIndexPerFaceAndCorner( int theFace, int theCorner ){
	int theReturnIndex = -1;

	if(usingNormalSpec){
		MeshNormalSpec* theSpec = theMesh->GetSpecifiedNormals();
		theReturnIndex = theSpec->Face( theFace ).GetNormalID( theCorner );

	}else{
		RVertex & theRVertex = theMesh->getRVert( theMesh->faces[theFace].getVert(theCorner) );
		int numRNormals = theRVertex.rFlags & NORCT_MASK;
		bool specified = theRVertex.rFlags & SPECIFIED_NORMAL;

		if( (numRNormals == 1) || (specified)){
			theReturnIndex = 0;			
		}else{

			int rn = 0;
			bool normalFound = false;
			do{				
				RNormal &theRNormal = theRVertex.ern[rn];
				normalFound = (theRNormal.getSmGroup() & theMesh->faces[theFace].getSmGroup() ) != 0;
				if( !normalFound ){
					rn++;
				}else{
					theReturnIndex = rn;
				}
			}while( (!normalFound) && (rn < numRNormals) );
		}
	}
	
	return theReturnIndex;
}

int vertexBuilder::getVertexIndexPerFaceAndCorner( int theFace, int theCorner ){ return theMesh->faces[theFace].getVert(theCorner); }

int vertexBuilder::getTVertIndexPerFaceAndCorner( int theFace, int theCorner ){ return theMesh->mapFaces(mp)[theFace].getTVert(theCorner); }

Point3 vertexBuilder::getVertexPosForFinal( int finalIndex ){
	return theMesh->verts[finalVertex::getVertex(finalIndex).getVert()];
}

UVVert vertexBuilder::getTexCoordForFinal( int finalIndex ){
	return theMesh->mapVerts(mp)[finalVertex::getVertex(finalIndex).getTVert()];
}

Point3 vertexBuilder::getNormalForFinal( int finalIndex ){
	if(usingNormalSpec){
		MeshNormalSpec* theSpec = theMesh->GetSpecifiedNormals();
		return theSpec->GetNormalArray()[ finalVertex::getVertex(finalIndex).getNormal() ];
	} else {
		RVertex & theRVertex = theMesh->getRVert( finalVertex::getVertex(finalIndex).getVert() );
		int numRNormals = theRVertex.rFlags & NORCT_MASK;
		bool specified = theRVertex.rFlags & SPECIFIED_NORMAL;

		if( (numRNormals == 1) || (specified)){
			return theRVertex.rn.getNormal();
		}else{
			return theRVertex.ern[finalVertex::getVertex(finalIndex).getNormal()].getNormal();
		}
	}
}