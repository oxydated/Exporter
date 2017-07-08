#ifndef __XMLSKINEXPORTER_HEADER__
#define __XMLSKINEXPORTER_HEADER__

#include "xmlExporter.h"
#include <matrix3.h>

IXMLDOMElement* insertSkinByID(int ID, TCHAR* skinNodeName, int meshID, int numBones, int numVertices);

IXMLDOMElement* insertObjectTMforSkin(IXMLDOMElement* skinNode, Matrix3 theObjectTM);

IXMLDOMElement* insertBoneForSkin(IXMLDOMElement* skinNode, _TCHAR* boneName, int boneIndex);

IXMLDOMElement* insertObjectTMforBone(IXMLDOMElement* boneNode, Matrix3 theBoneTM);

IXMLDOMElement* insertDualQuatForBone(IXMLDOMElement* boneNode, float qs, float qx, float qy, float qz, float dqs, float dqx, float dqy, float dqz);

IXMLDOMElement* insertVertexForSkin(IXMLDOMElement* skinNode, int vertexIndex, int numBonesForVertex);

IXMLDOMElement* insertBoneEntryForVertex(IXMLDOMElement* vertexNode, int boneIndexForVertex, int boneIndexForSkin, float weight);

#endif