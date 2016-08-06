#include "processTransforms.h"
#include "printUtilities.h"

/*

	INode* -> Node Transformation matrix 'Matrix3' [ GetNodeTM(	TimeValue 	t, Interval * 	valid = NULL ) ]
	INode* -> Object Transformation matrix [  GetObjectTM	(	TimeValue 	time, Interval * 	valid = NULL ) ]
	Matrix3.m = float[4][3]
	Matrix Inverse -> [ matrix->Inverse(anotherMatrix) ]
	local transformation -> parentNode->GetNodeTM().inverse
*/


static char theString[100] = "";

void inline printMatrix( Matrix3 &m, FILE *f ){
	
	setOutputFile( f );
	indentLine(); //0
	for( int i = 0; i < 4; i++)
	{
		_snprintf( theString, 100, "%f	%f	%f\n", m.GetRow(i).x, m.GetRow(i).y, m.GetRow(i).z ); printToOutput( theString );
	}

	deIndentLine();
}

void getNodeTransformations( INode *theNode, TimeValue t, FILE *f){
	Matrix3 theNodeMatrix = theNode->GetNodeTM(t);

	setOutputFile( f );
	indentLine(); //0
	_snprintf( theString, 100, "theNodeMatrix:\n" ); printToOutput( theString );

	printMatrix(theNodeMatrix, f);

	deIndentLine();
}