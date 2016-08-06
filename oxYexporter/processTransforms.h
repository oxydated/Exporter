#ifndef __PROCESSTRANSFORMS_H__
#define __PROCESSTRANSFORMS_H__

#include <max.h>
#include <inode.h>
#include <object.h>
#include <matrix3.h>
#include <matrix2.h>


/*

	INode* -> Node Transformation matrix 'Matrix3' [ GetNodeTM(	TimeValue 	t, Interval * 	valid = NULL ) ]
	INode* -> Object Transformation matrix [  GetObjectTM	(	TimeValue 	time, Interval * 	valid = NULL ) ]
	Matrix3.m = float[4][3]
	local transformation -> parentNode->GetNodeTM().inverse
*/

void getNodeTransformations( INode *theNode, TimeValue t, FILE *f);

#endif