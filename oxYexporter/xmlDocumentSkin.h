#pragma once
#include <memory>
#include <max.h>
#include "xmlDocument.h"

namespace oxyde {
	namespace exporter {
		namespace XML {

			class oxyObjectList : public oxyDocumentElement {
			public:
				oxyObjectList(oxyDocumentPtr theInputDocument);
				oxyObjectList() = delete;
				int getObjectID();

			private:
				int objectCounter;
			};

			using oxyObjectListPtr = std::shared_ptr<oxyObjectList>;

			class oxySkinElement;
			using oxySkinElementPtr = std::shared_ptr<oxySkinElement>;


			class oxySkinBonesElement : std::enable_shared_from_this<oxySkinBonesElement>, public oxyDocumentElement {
			public:
				friend oxySkinElement;

			private:
				oxySkinBonesElement(oxySkinElementPtr theSkinElement, int inputnumBones);
				oxySkinBonesElement() = delete;

				int numBones;
				void buildListOfAttributes() override;
			};

			using oxySkinBonesElementPtr = std::shared_ptr<oxySkinBonesElement>;

			class oxySkinVerticesElement : public oxyDocumentElement {
			public:
				friend oxySkinElement;
			private:
				//vertices numVertices = "2674"
				oxySkinVerticesElement(oxySkinElementPtr theSkinElement, int inputnumVertices);
				oxySkinVerticesElement() = delete;

				int numVertices;
				void buildListOfAttributes() override;
			};

			using oxySkinVerticesElementPtr = std::shared_ptr<oxySkinVerticesElement>;

			//<objectMatrix r0c0 = "1" r0c1 = "0" r0c2 = "4.768372E-07" r1c0 = "-4.768372E-07" r1c1 = "-1.629207E-07" r1c2 = "1" r2c0 = "7.768664E-14" r2c1 = "-1" r2c2 = "-1.629207E-07" r3c0 = "0" r3c1 = "18.66828" r3c2 = "1.837147" / >

			class oxySkinMatrixElement : public oxyDocumentElement {
			public:
				friend oxySkinElement;
			private:
				//vertices numVertices = "2674"
				oxySkinMatrixElement(oxySkinElementPtr theSkinElement,
					float inr0c0, float inr0c1, float inr0c2,
					float inr1c0, float inr1c1, float inr1c2,
					float inr2c0, float inr2c1, float inr2c2,
					float inr3c0, float inr3c1, float inr3c2
				);
				oxySkinMatrixElement() = delete;

				float r0c0, r0c1, r0c2;
				float r1c0, r1c1, r1c2;
				float r2c0, r2c1, r2c2;
				float r3c0, r3c1, r3c2;

				void buildListOfAttributes() override;
			};

			using oxySkinMatrixElementPtr = std::shared_ptr<oxySkinMatrixElement>;
			
			
			//<bones numBones = "19">
			class oxySkinElement : public oxyDocumentElement, std::enable_shared_from_this<oxySkinElement> {
			public:
				static oxySkinElementPtr createSkinElement(oxyObjectListPtr theObjectList, std::wstring inputSkinNodeName, int inputMesh,
					INodePtr theSkinNode,
					int inputnumBones,
					int inputnumVertices,
					float inr0c0, float inr0c1, float inr0c2,
					float inr1c0, float inr1c1, float inr1c2,
					float inr2c0, float inr2c1, float inr2c2,
					float inr3c0, float inr3c1, float inr3c2);

				class oxySkinVertex;
				using oxySkinVertexPtr = std::shared_ptr<oxySkinVertex>;

			private:
				class oxySkinBoneDataElement;
				class oxySkinBoneMatrixElement;
				class oxySkinBoneQuatElement;

				using oxySkinBoneDataElementPtr = std::shared_ptr<oxySkinBoneDataElement>;
				using oxySkinBoneMatrixElementPtr = std::shared_ptr<oxySkinBoneMatrixElement>;
				using oxySkinBoneQuatElementPtr = std::shared_ptr<oxySkinBoneQuatElement>;

				oxySkinElement(oxyObjectListPtr theObjectList, std::wstring inputSkinNodeName, int inputMesh);
				oxySkinElement() = delete;

				oxySkinBonesElementPtr theSkinBonesElement;
				oxySkinVerticesElementPtr theSkinVerticesElement;
				oxySkinMatrixElementPtr theSkinMatrixElement;
				int skinID;
				std::wstring skinNodeName;
				int mesh;
				void buildListOfAttributes() override;

			public:
				void addBone(std::wstring inputBoneName, int inputBoneIndex,
					float inr0c0, float inr0c1, float inr0c2, float inr1c0, float inr1c1, float inr1c2, float inr2c0, float inr2c1, float inr2c2, float inr3c0, float inr3c1, float inr3c2,
					float inqs, float inqx, float inqy, float inqz, float indqs, float indqx, float indqy, float indqz
				);

				oxySkinVertexPtr addVertex(int inputVertexIndex, int inputnumBonesForVertex);
			};

			class oxySkinElement::oxySkinBoneDataElement : public oxyDocumentElement {
			public:
				oxySkinBoneDataElement(oxySkinBonesElementPtr theSkinBonesElement, std::wstring inputBoneName, int inputBoneIndex);
				oxySkinBoneDataElement() = delete;

			private:
				//<bone boneName = "Bone001" boneIndex = "0">

				std::wstring boneName;
				int boneIndex;
				void buildListOfAttributes() override;
			};

			class oxySkinElement::oxySkinBoneMatrixElement : public oxyDocumentElement {
				//<boneMatrix r0c0 = "0.108436" r0c1 = "-0.000001" r0c2 = "-0.994104" r1c0 = "0.994104" r1c1 = "0.000000" r1c2 = "0.108436" r2c0 = "0.000000" r2c1 = "-1.000000" r2c2 = "0.000001" r3c0 = "175.474976" r3c1 = "0.000054" r3c2 = "72.193451" / >
			public:
				oxySkinBoneMatrixElement(oxySkinBoneDataElementPtr theSkinBoneDataElement,
					float inr0c0, float inr0c1, float inr0c2,
					float inr1c0, float inr1c1, float inr1c2,
					float inr2c0, float inr2c1, float inr2c2,
					float inr3c0, float inr3c1, float inr3c2
				);
				oxySkinBoneMatrixElement() = delete;

			private:
				float r0c0, r0c1, r0c2;
				float r1c0, r1c1, r1c2;
				float r2c0, r2c1, r2c2;
				float r3c0, r3c1, r3c2;

				void buildListOfAttributes() override;
			};

			class oxySkinElement::oxySkinBoneQuatElement : public oxyDocumentElement {
				//<boneDualQuat qs="0.085822" qx="-0.085812" qy="-0.701881" qz="-0.701879" dqs="-14.455140" dqx="-14.455166" dqy="1.521235" dqz="-1.521442"/>
			public:
				oxySkinBoneQuatElement(oxySkinBoneDataElementPtr theSkinBoneDataElement,
					float inqs, float inqx, float inqy, float inqz, float indqs, float indqx, float indqy, float indqz
				);
				oxySkinBoneQuatElement() = delete;

			private:
				float qs, qx, qy, qz, dqs, dqx, dqy, dqz;

				void buildListOfAttributes() override;
			};

			class oxySkinElement::oxySkinVertex {
			private:
				class oxySkinVertexData : public oxyDocumentElement{
					//<vertex vertexIndex="0">
				public:
					oxySkinVertexData(oxySkinVerticesElementPtr theVerticesElement, int inputVertexIndex);
					oxySkinVertexData() = delete;

				private:
					int vertexIndex;
					void buildListOfAttributes() override;
				};

				using oxySkinVertexDataPtr = std::shared_ptr<oxySkinVertexData>;
				oxySkinVertexDataPtr theSkinVertexData;

				class oxySkinBonesForVertex : public oxyDocumentElement {
					//<bonesForVertex numBonesForVertex = "1">
				public:
					oxySkinBonesForVertex(oxySkinVertexDataPtr theVertexData, int inputnumBonesForVertex);
					oxySkinBonesForVertex() = delete;
				private:
					int numBonesForVertex;
					void buildListOfAttributes() override;
				};

				using oxySkinBonesForVertexPtr = std::shared_ptr<oxySkinBonesForVertex>;
				oxySkinBonesForVertexPtr theSkinBonesForVertex;

				class oxySkinBoneEntry : public oxyDocumentElement {
					//<boneEntry boneIndexForVertex = "0" boneIndexForSkin = "0" weight = "1" / >
				public:
					oxySkinBoneEntry(oxySkinBonesForVertexPtr theSkinBonesForVertex, int inputboneIndexForVertex, int inputboneIndexForSkin, float inputweight);
					oxySkinBoneEntry() = delete;
				private:
					int boneIndexForVertex;
					int boneIndexForSkin;
					float weight;
					void buildListOfAttributes() override;
				};

				using oxySkinBoneEntryPtr = std::shared_ptr<oxySkinBoneEntry>;

			public:
				oxySkinVertex(oxySkinVerticesElementPtr theVerticesElement, int inputVertexIndex, int inputnumBonesForVertex);
				void addBoneEntry(int inputboneIndexForVertex, int inputboneIndexForSkin, float inputweight);
			};

		}
	}
}
