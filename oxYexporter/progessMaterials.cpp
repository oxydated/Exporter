#include "processMaterials.h"
#include <stdio.h>

/*

INode* -> Mtl [ GetMtl() ]  ( check for NULL - No material assigned so far )
Mtl -> (Diffuse Texture) Texmap [ GetSubTexmap( ID_DI ) ] ( check for NULL )
Texmap -> UVGen [ GetTheUVGen() ] ( UVGen for texcoord transformations )
UVGen -> the actual axis [ GetAxis() ] ( return the values AXIS_UV, AXIS_VW, AXIS_WU )

*/


void getMaterialInformation( INode* theNode, FILE* expFile ){
	Mtl* theMtl = theNode->GetMtl();
	if( theMtl != NULL ){
		Texmap* theMap = theMtl->GetSubTexmap(1);
		if( theMap != NULL ){
			MSTR theMapName;
			theMap->GetClassName(theMapName);
			fprintf(expFile, "theMap class name: %s\n", theMapName.data());
			if( theMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0) ){
				fprintf(expFile, "it is a bitmap texture\n", theMapName.data());
				fprintf(expFile, "Image file: %s\n", ((BitmapTex*)theMap)->GetMapName() );
			}
			
			fprintf(expFile, "The texCoord axis: ");
			switch( theMap->GetTheUVGen()->GetAxis())
			{
				case AXIS_UV:
					fprintf(expFile, "AXIS_UV\n"); 
					break;
				case AXIS_VW:
					fprintf(expFile, "AXIS_UV\n"); 
					break;
				case AXIS_WU:
					fprintf(expFile, "AXIS_UV\n"); 
					break;
				default:
					fprintf(expFile, "NOT FOUND???\n");
					break;
			}
		}

	}
}