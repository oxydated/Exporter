#pragma once
#include "xmlDocument.h"

namespace oxyde {
	namespace exporter {
		namespace XML {

			class oxyGeometryElement : public oxyDocumentElement {
			public:
				oxyGeometryElement(oxyDocumentPtr theInputDocument);
				oxyGeometryElement() = delete;
				int getMeshID();

			private:
				int meshCounter;
			};

			using oxyGeometryElementPtr = std::shared_ptr<oxyGeometryElement>;

			class oxyMeshSection {

			public:

				class oxyMeshFaces;
				using oxyMeshFacesPtr = std::shared_ptr<oxyMeshFaces>;

				class oxyMeshVertices;
				using oxyMeshVerticesPtr = std::shared_ptr<oxyMeshVertices>;

				class oxyMeshTexture;
				using oxyMeshTexturePtr = std::shared_ptr<oxyMeshTexture>;

			private:

				class oxyMeshElement : public oxyDocumentElement {
				public:
					oxyMeshElement(oxyGeometryElementPtr theGeometrySection);
					oxyMeshElement() = delete;
					int getMeshID() { return meshID; }

				private:
					int meshID;
					void buildListOfAttributes() override;
				};

				using oxyMeshElementPtr = std::shared_ptr<oxyMeshElement>;
				oxyMeshElementPtr theMeshElement;

			public:
				oxyMeshSection(oxyGeometryElementPtr theGeometrySection);
				int getMeshID() { return theMeshElement->getMeshID(); }
				oxyMeshFacesPtr setMeshFaces(int inputnumFaces);
				oxyMeshVerticesPtr setMeshVertices(int inputnumVertices);
				oxyMeshTexturePtr setMeshTexture(std::wstring inputMap);
			};

			using oxyMeshSectionPtr = std::shared_ptr<oxyMeshSection>;

			class oxyMeshSection::oxyMeshFaces {
			private:
				class oxyMeshFacesElement : public oxyDocumentElement {
				//<faces numFaces = "5344">
				public:
					oxyMeshFacesElement(oxyMeshElementPtr theMeshElement, int inputnumFaces);
					oxyMeshFacesElement() = delete;

				private:
					int numFaces;
					void buildListOfAttributes() override;
				};

				using oxyMeshFacesElementPtr = std::shared_ptr<oxyMeshFacesElement>;
				oxyMeshFacesElementPtr theFacesElement;

				class oxyMeshFaceDataElement : public oxyDocumentElement {
					//<face faceID = "0" v0 = "0" v1 = "1" v2 = "2" / >
				public:
					oxyMeshFaceDataElement(oxyMeshFacesElementPtr theFacesElement, int inputfaceID, int inputv0, int inputv1, int inputv2);
					oxyMeshFaceDataElement() = delete;

				private:
					int faceID;
					int v0, v1, v2;
					void buildListOfAttributes() override;
				};

				using oxyMeshFaceDataElementPtr = std::shared_ptr<oxyMeshFaceDataElement>;
			public:
				oxyMeshFaces(oxyMeshElementPtr theMeshElement, int inputnumFaces);
				void addFace(int inputfaceID, int inputv0, int inputv1, int inputv2);
			};
			
			class oxyMeshSection::oxyMeshVertices {
			private:
				class oxyMeshVerticesElement : public oxyDocumentElement {
					//<vertices numVertices = "2882">
				public:
					oxyMeshVerticesElement(oxyMeshElementPtr theMeshElement, int inputnumVertices);
					oxyMeshVerticesElement() = delete;

				private:
					int numVertices;
					void buildListOfAttributes() override;
				};

				using oxyMeshVerticesElementPtr = std::shared_ptr<oxyMeshVerticesElement>;
				oxyMeshVerticesElementPtr theVerticesElement;

				class oxyMeshVertexDataElement : public oxyDocumentElement {
					//<vertex vertexID = "0" theVert = "0" x = "-2.235174E-08" y = "-38.86385" z = "1.054212" theTVert = "0" u = "0.5" v = "0" theNormal = "0" nx = "4.569807E-05" ny = "-0.2200946" nz = "-0.9754785" / >
				public:
					oxyMeshVertexDataElement(oxyMeshVerticesElementPtr theVerticesElement, int inputvertexID, int inputtheVert, float inputx, float inputy, float inputz, int inputtheTVert, float inputu, float inputv, int inputtheNormal, float inputnx, float inputny, float inputnz);
					oxyMeshVertexDataElement() = delete;

				private:
					int vertexID;
					int theVert;
					float x, y, z;
					int theTVert;
					float u, v;
					int theNormal;
					float nx, ny, nz;

					void buildListOfAttributes() override;
				};

				using oxyMeshVertexDataElementPtr = std::shared_ptr<oxyMeshVertexDataElement>;
			public:
				oxyMeshVertices(oxyMeshElementPtr theMeshElement, int inputnumVertices);
				void addVertex(int inputvertexID, int inputtheVert, float inputx, float inputy, float inputz, int inputtheTVert, float inputu, float inputv, int inputtheNormal, float inputnx, float inputny, float inputnz);
			};

			class oxyMeshSection::oxyMeshTexture {
			private:
				class oxyMeshTextureElement : oxyDocumentElement {
					//<vertices numVertices = "2882">
				public:
					oxyMeshTextureElement(oxyMeshElementPtr theMeshElement, std::wstring inputMap);
					oxyMeshTextureElement() = delete;

				private:
					std::wstring theMap;
					void buildListOfAttributes() override;
				};

				using oxyMeshTextureElementPtr = std::shared_ptr<oxyMeshTextureElement>;
				oxyMeshTextureElementPtr theTextureElement;

			public:
				oxyMeshTexture(oxyMeshElementPtr theMeshElement, std::wstring inputMap);

			};

		}
	}
}
