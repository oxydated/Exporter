#ifndef __PROCESSSKIN_HEADER__
#define __PROCESSSKIN_HEADER__

#include "iskin.h"
#include <modstack.h>

bool isThereASkinModifier(IDerivedObject* theDerivedObj);

bool extractSkinDataFromObj(IDerivedObject* theDerivedObj, INode* theNode, _TCHAR* skinNodeName, FILE* expFile);

int extractTargetMesh(IDerivedObject* theDerivedObj, int skinIndexInModifierStack, FILE* expFile);

#endif