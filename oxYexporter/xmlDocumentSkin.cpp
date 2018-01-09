#include <memory>
#include "xmlDocumentSkin.h"

namespace oxyde {
	namespace exporter {
		namespace XML {

			oxyObjectList::oxyObjectList(oxyDocumentPtr theInputDocument) :oxyDocumentElement(theInputDocument, L"objectList"), objectCounter(-1){}

			int oxyObjectList::getObjectID()
			{
				return ++objectCounter;
			}

			oxySkinElementPtr oxySkinElement::createSkinElement(oxyObjectListPtr theObjectList, std::wstring inputSkinNodeName, int inputMesh,
				int inputnumBones,
				int inputnumVertices,
				float inr0c0, float inr0c1, float inr0c2,
				float inr1c0, float inr1c1, float inr1c2,
				float inr2c0, float inr2c1, float inr2c2,
				float inr3c0, float inr3c1, float inr3c2)
			{
				oxySkinElementPtr theSkinElement = std::shared_ptr<oxySkinElement>(new oxySkinElement(theObjectList, inputSkinNodeName, inputMesh));
				theSkinElement->theSkinBonesElement = std::shared_ptr<oxySkinBonesElement>(new oxySkinBonesElement(theSkinElement, inputnumBones));
				theSkinElement->theSkinVerticesElement = std::shared_ptr<oxySkinVerticesElement>(new oxySkinVerticesElement(theSkinElement, inputnumVertices));
				theSkinElement->theSkinMatrixElement = std::shared_ptr<oxySkinMatrixElement>(new oxySkinMatrixElement(theSkinElement, 
					inr0c0, inr0c1, inr0c2,
					inr1c0, inr1c1, inr1c2,
					inr2c0, inr2c1, inr2c2,
					inr3c0, inr3c1, inr3c2));
				return theSkinElement;
			}

			oxySkinElement::oxySkinElement(oxyObjectListPtr theObjectList, std::wstring inputSkinNodeName, int inputMesh):
				oxyDocumentElement(theObjectList, L"skin"),
				skinNodeName(inputSkinNodeName),
				mesh(inputMesh)
			{
				skinID = theObjectList->getObjectID();
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxySkinElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"ID", _variant_t(skinID)));
				attributeList.push_back(elementAttribute(L"skinNodeName", _variant_t(skinNodeName.data())));
				attributeList.push_back(elementAttribute(L"mesh", _variant_t(mesh)));
			}

			void oxySkinElement::addBone(std::wstring inputBoneName, int inputBoneIndex,
				float inr0c0, float inr0c1, float inr0c2, float inr1c0, float inr1c1, float inr1c2, float inr2c0, float inr2c1, float inr2c2, float inr3c0, float inr3c1, float inr3c2,
				float inqs, float inqx, float inqy, float inqz, float indqs, float indqx, float indqy, float indqz
			)
			{
				oxySkinBoneDataElementPtr newBone = std::make_shared<oxySkinBoneDataElement>(theSkinBonesElement, inputBoneName, inputBoneIndex);
				oxySkinBoneMatrixElementPtr newMatrixForBone = std::make_shared<oxySkinBoneMatrixElement>(newBone,  inr0c0,  inr0c1,  inr0c2,  inr1c0,  inr1c1,  inr1c2,  inr2c0,  inr2c1,  inr2c2,  inr3c0,  inr3c1,  inr3c2 );
				oxySkinBoneQuatElementPtr newQuatForBone = std::make_shared<oxySkinBoneQuatElement>(newBone, inqs, inqx, inqy, inqz, indqs, indqx, indqy, indqz);
			}

			oxySkinElement::oxySkinVertexPtr oxySkinElement::addVertex(int inputVertexIndex, int inputnumBonesForVertex)
			{
				return std::make_shared<oxySkinVertex>(theSkinVerticesElement, inputVertexIndex, inputnumBonesForVertex);
			}

			oxySkinBonesElement::oxySkinBonesElement(oxySkinElementPtr theSkinElement, int inputnumBones) :
				oxyDocumentElement(theSkinElement, L"bones"),
				numBones(inputnumBones)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxySkinBonesElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"numBones", _variant_t(numBones)));
			}

			oxySkinVerticesElement::oxySkinVerticesElement(oxySkinElementPtr theSkinElement, int inputnumVertices) :
				oxyDocumentElement(theSkinElement, L"vertices"),
				numVertices(inputnumVertices)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxySkinVerticesElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"numVertices", _variant_t(numVertices)));
			}

			oxySkinMatrixElement::oxySkinMatrixElement(oxySkinElementPtr theSkinElement, float inr0c0, float inr0c1, float inr0c2, float inr1c0, float inr1c1, float inr1c2, float inr2c0, float inr2c1, float inr2c2, float inr3c0, float inr3c1, float inr3c2) :
				oxyDocumentElement(theSkinElement, L"objectMatrix"),
				r0c0(inr0c0), r0c1(inr0c1), r0c2(inr0c2), r1c0(inr1c0), r1c1(inr1c1), r1c2(inr1c2), r2c0(inr2c0), r2c1(inr2c1), r2c2(inr2c2), r3c0(inr3c0), r3c1(inr3c1), r3c2(inr3c2)
			{
				buildListOfAttributes();
				setElementAttributes();
			}
			void oxySkinMatrixElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"r0c0", variantFromFloat(r0c0)));
				attributeList.push_back(elementAttribute(L"r0c1", variantFromFloat(r0c1)));
				attributeList.push_back(elementAttribute(L"r0c2", variantFromFloat(r0c2)));

				attributeList.push_back(elementAttribute(L"r1c0", variantFromFloat(r1c0)));
				attributeList.push_back(elementAttribute(L"r1c1", variantFromFloat(r1c1)));
				attributeList.push_back(elementAttribute(L"r1c2", variantFromFloat(r1c2)));

				attributeList.push_back(elementAttribute(L"r2c0", variantFromFloat(r2c0)));
				attributeList.push_back(elementAttribute(L"r2c1", variantFromFloat(r2c1)));
				attributeList.push_back(elementAttribute(L"r2c2", variantFromFloat(r2c2)));

				attributeList.push_back(elementAttribute(L"r3c0", variantFromFloat(r3c0)));
				attributeList.push_back(elementAttribute(L"r3c1", variantFromFloat(r3c1)));
				attributeList.push_back(elementAttribute(L"r3c2", variantFromFloat(r3c2)));
			}
			oxySkinElement::oxySkinBoneDataElement::oxySkinBoneDataElement(oxySkinBonesElementPtr theSkinBonesElement, std::wstring inputBoneName, int inputBoneIndex) :
				oxyDocumentElement(theSkinBonesElement, L"bone"),
				boneName(inputBoneName),
				boneIndex(inputBoneIndex)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxySkinElement::oxySkinBoneDataElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"boneName", _variant_t(boneName.data())));
				attributeList.push_back(elementAttribute(L"boneIndex", _variant_t(boneIndex)));
			}

			oxySkinElement::oxySkinBoneMatrixElement::oxySkinBoneMatrixElement(oxySkinBoneDataElementPtr theSkinBoneDataElement, float inr0c0, float inr0c1, float inr0c2, float inr1c0, float inr1c1, float inr1c2, float inr2c0, float inr2c1, float inr2c2, float inr3c0, float inr3c1, float inr3c2) :
				oxyDocumentElement(theSkinBoneDataElement, L"boneMatrix"),
				r0c0(inr0c0), r0c1(inr0c1), r0c2(inr0c2), r1c0(inr1c0), r1c1(inr1c1), r1c2(inr1c2), r2c0(inr2c0), r2c1(inr2c1), r2c2(inr2c2), r3c0(inr3c0), r3c1(inr3c1), r3c2(inr3c2)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxySkinElement::oxySkinBoneMatrixElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"r0c0", variantFromFloat(r0c0)));
				attributeList.push_back(elementAttribute(L"r0c1", variantFromFloat(r0c1)));
				attributeList.push_back(elementAttribute(L"r0c2", variantFromFloat(r0c2)));

				attributeList.push_back(elementAttribute(L"r1c0", variantFromFloat(r1c0)));
				attributeList.push_back(elementAttribute(L"r1c1", variantFromFloat(r1c1)));
				attributeList.push_back(elementAttribute(L"r1c2", variantFromFloat(r1c2)));

				attributeList.push_back(elementAttribute(L"r2c0", variantFromFloat(r2c0)));
				attributeList.push_back(elementAttribute(L"r2c1", variantFromFloat(r2c1)));
				attributeList.push_back(elementAttribute(L"r2c2", variantFromFloat(r2c2)));

				attributeList.push_back(elementAttribute(L"r3c0", variantFromFloat(r3c0)));
				attributeList.push_back(elementAttribute(L"r3c1", variantFromFloat(r3c1)));
				attributeList.push_back(elementAttribute(L"r3c2", variantFromFloat(r3c2)));
			}

			oxySkinElement::oxySkinBoneQuatElement::oxySkinBoneQuatElement(oxySkinBoneDataElementPtr theSkinBoneDataElement, float inqs, float inqx, float inqy, float inqz, float indqs, float indqx, float indqy, float indqz):
				oxyDocumentElement(theSkinBoneDataElement, L"boneDualQuat"),
				qs(inqs), qx(inqx), qy(inqy), qz(inqz), dqs(indqs), dqx(indqx), dqy(indqy), dqz(indqz)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxySkinElement::oxySkinBoneQuatElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"qs", variantFromFloat(qs)));
				attributeList.push_back(elementAttribute(L"qx", variantFromFloat(qx)));
				attributeList.push_back(elementAttribute(L"qy", variantFromFloat(qy)));
				attributeList.push_back(elementAttribute(L"qz", variantFromFloat(qz)));
				attributeList.push_back(elementAttribute(L"dqs", variantFromFloat(dqs)));
				attributeList.push_back(elementAttribute(L"dqx", variantFromFloat(dqx)));
				attributeList.push_back(elementAttribute(L"dqy", variantFromFloat(dqy)));
				attributeList.push_back(elementAttribute(L"dqz", variantFromFloat(dqz)));
			}

			oxySkinElement::oxySkinVertex::oxySkinVertexData::oxySkinVertexData(oxySkinVerticesElementPtr theVerticesElement, int inputVertexIndex) :
				oxyDocumentElement(theVerticesElement, L"vertex"),
				vertexIndex(inputVertexIndex)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxySkinElement::oxySkinVertex::oxySkinVertexData::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"vertexIndex", _variant_t(vertexIndex)));
			}

			oxySkinElement::oxySkinVertex::oxySkinVertex(oxySkinVerticesElementPtr theVerticesElement, int inputVertexIndex, int inputnumBonesForVertex)
			{
				theSkinVertexData = std::make_shared<oxySkinVertexData>(theVerticesElement, inputVertexIndex);
				theSkinBonesForVertex = std::make_shared<oxySkinBonesForVertex>(theSkinVertexData, inputnumBonesForVertex);
			}

			void oxySkinElement::oxySkinVertex::addBoneEntry(int inputboneIndexForVertex, int inputboneIndexForSkin, float inputweight)
			{
				oxySkinBoneEntryPtr theBoneEntry = std::make_shared<oxySkinBoneEntry>(theSkinBonesForVertex, inputboneIndexForVertex, inputboneIndexForSkin, inputweight);
			}

			oxySkinElement::oxySkinVertex::oxySkinBonesForVertex::oxySkinBonesForVertex(oxySkinVertexDataPtr theVertexData, int inputnumBonesForVertex) :
				oxyDocumentElement(theVertexData, L"bonesForVertex"),
				numBonesForVertex(inputnumBonesForVertex)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxySkinElement::oxySkinVertex::oxySkinBonesForVertex::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"numBonesForVertex", _variant_t(numBonesForVertex)));
			}

			oxySkinElement::oxySkinVertex::oxySkinBoneEntry::oxySkinBoneEntry(oxySkinBonesForVertexPtr theSkinBonesForVertex, int inputboneIndexForVertex, int inputboneIndexForSkin, float inputweight) :
				oxyDocumentElement(theSkinBonesForVertex, L"boneEntry"),
				boneIndexForVertex(inputboneIndexForVertex),
				boneIndexForSkin(inputboneIndexForSkin),
				weight(inputweight)
			{
				buildListOfAttributes();
				setElementAttributes();
			}
			void oxySkinElement::oxySkinVertex::oxySkinBoneEntry::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"boneIndexForVertex", _variant_t(boneIndexForVertex)));
				attributeList.push_back(elementAttribute(L"boneIndexForSkin", _variant_t(boneIndexForSkin)));
				attributeList.push_back(elementAttribute(L"weight", variantFromFloat(weight)));
			}
}
	}
}
