#include "processGeometry.h"
#include "printUtilities.h"
#include "MeshNormalSpec.h"
#include "xmlExporter.h"

/*
	notes about geometry in 3dsmax:

	TriObject->ObjectState [ TriObject::Eval() ] ---> every object that can be deformed must be able to be converted to a TriObject

	INode->ObjectState [ EvalWorldState() ] ---> this is the final geometry, the one that actually shows up in the display

	(Object*) ObjectState::obj -> TriObject [ ConvertToType() ] ---> from a ObjectState to TriObject [ need to check CanConvertToType() ]

	TriObject->Mesh [ GetMesh() ] ---> access the underlying Mesh of the TriObject

	Mesh->AdjEdgeList [ AdjEdgeList (Mesh &amesh) ] ---> construct a Edge Adjacency list

	Mesh, AdjEdgeList ->  AdjFaceList  [ AdjFaceList(	Mesh & 	mesh, AdjEdgeList & el ) ]

	Mesh-> MNMesh  [ MNMesh (const Mesh &from) ] ---> Mesh based on a Winged Edge Structure

	Texture Coordinates:

	[ getNumMapVerts( int mp ) ] --> return the number of UVVerts for a given mapping channel

	Mesh->TVFace* [ mapFaces( int mp ) ] --> TVFaces array for a given mapping channel

	Mesh->UVVert* [ mapVerts( int mp) ] --> UVVerts array for a given mapping channel

	UVVert* has no correlation to Mesh::verts. For each TVFace in TVFace*, there is one exclusive Face in Mesh::faces

	workaround: to get the mesh vertex for each UVvert in the TVFace.

	Normals:

	buildNormals() to prepare the normals

	getting the RVertex --> Mesh->getRVert( int i ) --> RVertex for vertex i

	in 3DSMAX, a vertex may have several normals:

		RNormal RVertex::rn (in the case of a single normal)

		RNormals RVertex::ern ( in the case of more than one normal)

	relating RNormals to Faces:

		Face->getSmGroup()

		RNormal->getSmGroup()

	BUILDING THE VERTEX:
		
		for each Face in faces:

		pos (x, y, z) = verts[ Face->getVert(i) ]
		
		texcoord(u, v) = (mapFaces(mp)[ index of Face ]).getVert(i)

		normal (x, y, z) = mesh->getRVert( Face->getVert(i) ).ern[x] --> with the same smoothing group from Face

		each Face should have only one normal within the same smooth group in each of its vertices

*/

static char theString[100] = "";

Mesh& extractMeshFromObjectState( ObjectState theState, TimeValue t, bool &isGeometry ){
	Object* theObject = theState.obj;
	if ((theObject != NULL) && (theObject->SuperClassID() == GEOMOBJECT_CLASS_ID)){
		if (((GeomObject*)theObject)->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))){
			TriObject* theTriObject = (TriObject*)(
				((GeomObject*)theObject)->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0))
				);
			isGeometry = true;
			return theTriObject->mesh;
		}
	}
	else {
		isGeometry = false;
	}
	return Mesh();
}

Mesh& extractMeshFromNode( INode* theNode, TimeValue t, bool &isGeometry, FILE* expFile ){
	/*Object* theObject = theNode->EvalWorldState(t).obj;
	if( (theObject != NULL) && (theObject->SuperClassID() == GEOMOBJECT_CLASS_ID) ){
		if( ((GeomObject*)theObject)->CanConvertToType( Class_ID( TRIOBJ_CLASS_ID, 0 ) ) ){
			TriObject* theTriObject = (TriObject*)(
													((GeomObject*)theObject)->ConvertToType( t, Class_ID( TRIOBJ_CLASS_ID, 0 ) )
													);
			isGeometry = true;
			return theTriObject->mesh;
		}
	} else {
		isGeometry = false;
	}
	return Mesh();*/

	return extractMeshFromObjectState(theNode->EvalWorldState(t), t, isGeometry );
}

int processMesh( Mesh &theMesh, FILE* expFile ){
	static int meshID = 0;
	IXMLDOMElement *theMeshElement = NULL;

	setOutputFile( expFile );
	indentLine(); //0
	
	deIndentLine(); 
	
	int mp = 1;

	theMesh.buildNormals();
	
	indentLine();//0

	for( int i=0; i< theMesh.getNumFaces(); i++){
		
		getNormalsPerMeshFace( theMesh, i );
	}
	deIndentLine();

	_snprintf( theString, 100, "\n/////////////////////////////////////////////////////////////////" ); printToOutput( theString );
	_snprintf( theString, 100, "//////////////////////////////////////////////      vertexBuilder" ); printToOutput( theString );
	_snprintf( theString, 100, "\n" ); printToOutput( theString );
	
	vertexBuilder::startBuilder( &theMesh, mp );

	int thisMesh = meshID++;
	theMeshElement = insertMeshByID(thisMesh, finalFace::getNumFinalFaces(), finalVertex::getNumFinals());
	
	_snprintf( theString, 100, "faces: \n%i\n", finalFace::getNumFinalFaces() ); printToOutput( theString );

	indentLine();//0

	for( int theFace = 0; theFace < finalFace::getNumFinalFaces(); theFace++ ){
		int v0 = finalFace::getFace(theFace).getVertexForCorner(0);
		int v1 = finalFace::getFace(theFace).getVertexForCorner(1);
		int v2 = finalFace::getFace(theFace).getVertexForCorner(2);

		_snprintf( theString, 100, "%i		%i %i %i\n",  theFace, v0, v1, v2 ); printToOutput( theString );
		insertFaceForMesh( theMeshElement, theFace, v0, v1, v2 );
	}

	deIndentLine();

	_snprintf( theString, 100, "vertices: \n%i\n", finalVertex::getNumFinals() ); printToOutput( theString );

	indentLine();//0

	for( int theFinal = 0; theFinal < finalVertex::getNumFinals(); theFinal++ ){
		int theVert = finalVertex::getVertex(theFinal).getVert();
		int theTVert = finalVertex::getVertex(theFinal).getTVert();		
		int theNormal = finalVertex::getVertex(theFinal).getNormal();

		Point3 vertPos = vertexBuilder::getVertexPosForFinal(theFinal);
		UVVert texCoord = vertexBuilder::getTexCoordForFinal(theFinal);
		Point3 NormalVec = vertexBuilder::getNormalForFinal(theFinal);

		_snprintf( theString, 100, "%i		%i		%f	%f	%f \n", theFinal, theVert, vertPos.x, vertPos.y, vertPos.z ); printToOutput( theString );
		_snprintf( theString, 100, "		%i		%f	%f \n", theTVert, texCoord.x, texCoord.y ); printToOutput( theString );
		_snprintf( theString, 100, "		%i		%f	%f	%f \n", theNormal, NormalVec.x, NormalVec.y, NormalVec.z  ); printToOutput( theString );

		insertVertexForMesh( theMeshElement, theFinal,	theVert,	vertPos.x, vertPos.y, vertPos.z, 
														theTVert,	texCoord.x, texCoord.y, 
														theNormal,	NormalVec.x, NormalVec.y, NormalVec.z );
	}
	
	deIndentLine();

	return thisMesh;
}


void getNormalsPerMeshFace( Mesh &theMesh, int theFace ){
	
	indentLine();//0

	MeshNormalSpec* theSpec = theMesh.GetSpecifiedNormals();

	if(theSpec != NULL){
		bool support = theSpec->GetFlag(MESH_NORMAL_MODIFIER_SUPPORT);
		int numNormals = theSpec->GetNumNormals();
		int numFaces = theSpec->GetNumFaces();
		if( (numNormals > 0) && (numFaces > 0) ){
			MeshNormalFace &theNormalFace = theSpec->Face( theFace );


			for( int i=0; i< 3; i++){
				Point3 &theNormal = theSpec->GetNormalArray()[ theNormalFace.GetNormalID(i) ];
			}

			deIndentLine();		

			return;
		}
	}

	for( int i=0; i< 3; i++){
		RVertex & theRVertex = theMesh.getRVert( theMesh.faces[theFace].getVert(i) );
		int numRNormals = theRVertex.rFlags & NORCT_MASK;
		bool specified = theRVertex.rFlags & SPECIFIED_NORMAL;

		if( (numRNormals == 1) || (specified)){
			RNormal &theRNormal = theRVertex.rn;
			
		}else{
			int rn = 0;
			bool normalFound = false;
			do{				
				RNormal &theRNormal = theRVertex.ern[rn];
				normalFound = (theRNormal.getSmGroup() & theMesh.faces[theFace].getSmGroup() ) != 0;
				if( !normalFound ){
					rn++;
				}
			}while( (!normalFound) && (rn < numRNormals) );

		}
		
	}
	
	deIndentLine();
}