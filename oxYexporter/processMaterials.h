#ifndef __PROCESSMATERIALS_H__
#define __PROCESSMATERIALS_H__

#include <max.h>
#include <triobj.h>
#include <mesh.h>
#include <object.h>
#include <imtl.h>
#include <stdmat.h>

/*

INode* -> Mtl [ GetMtl() ]  ( check for NULL - No material assigned so far )
Mtl -> (Diffuse Texture) Texmap [ GetSubTexmap( ID_DI ) ] ( check for NULL )
Texmap -> UVGen [ GetTheUVGen() ] ( UVGen for texcoord transformations )
UVGen -> the actual axis [ GetAxis() ] ( return the values AXIS_UV, AXIS_VW, AXIS_WU )

*/

void getMaterialInformation( INode* theNode, FILE* expFile );

#endif