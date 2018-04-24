#ifndef __PROCESSSKIN_HEADER__
#define __PROCESSSKIN_HEADER__

#include <modstack.h>
#include "iskin.h"
#include "xmlDocumentSkin.h"
#include "xmlDocumentMesh.h"

bool isThereASkinModifier(IDerivedObject* theDerivedObj);

//bool extractSkinDataFromObj(IDerivedObject* theDerivedObj, INode* theNode, _TCHAR* skinNodeName, FILE* expFile);
bool extractSkinDataFromObj(IDerivedObject* theDerivedObj, INode* theNode, _TCHAR* skinNodeName,
	oxyde::exporter::XML::oxyObjectListPtr theObjectList, oxyde::exporter::XML::oxyGeometryElementPtr theGeometrySection);

//int extractTargetMesh(IDerivedObject* theDerivedObj, int skinIndexInModifierStack, FILE* expFile);
int extractTargetMesh(IDerivedObject* theDerivedObj, INode* theNode, int skinIndexInModifierStack, oxyde::exporter::XML::oxyGeometryElementPtr theGeometrySection);

#endif