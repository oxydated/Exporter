#include "xmlDocumentMesh.h"
namespace oxyde {
	namespace exporter {
		namespace XML {

			oxyGeometryElement::oxyGeometryElement(oxyDocumentPtr theInputDocument) :oxyDocumentElement(theInputDocument, L"geometry"), meshCounter(-1) {}

			int oxyGeometryElement::getMeshID()
			{
				return ++meshCounter;
			}

			oxyMeshSection::oxyMeshElement::oxyMeshElement(oxyGeometryElementPtr theGeometrySection) :
				oxyDocumentElement(theGeometrySection, L"mesh")
			{
				meshID = theGeometrySection->getMeshID();
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxyMeshSection::oxyMeshElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"meshID", _variant_t(meshID)));
			}

			oxyMeshSection::oxyMeshSection(oxyGeometryElementPtr theGeometrySection)
			{
				theMeshElement = std::make_shared<oxyMeshElement>(theGeometrySection);
			}

			oxyMeshSection::oxyMeshFacesPtr oxyMeshSection::setMeshFaces(int inputnumFaces)
			{
				return std::make_shared<oxyMeshFaces>(theMeshElement, inputnumFaces);
			}

			oxyMeshSection::oxyMeshVerticesPtr oxyMeshSection::setMeshVertices(int inputnumVertices)
			{
				return std::make_shared<oxyMeshVertices>(theMeshElement, inputnumVertices);
			}

			oxyMeshSection::oxyMeshFaces::oxyMeshFacesElement::oxyMeshFacesElement(oxyMeshElementPtr theMeshElement, int inputnumFaces) :
				oxyDocumentElement(theMeshElement, L"faces"),
				numFaces(inputnumFaces)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxyMeshSection::oxyMeshFaces::oxyMeshFacesElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"numFaces", _variant_t(numFaces)));
			}

			oxyMeshSection::oxyMeshFaces::oxyMeshFaceDataElement::oxyMeshFaceDataElement(oxyMeshFacesElementPtr theFacesElement, int inputfaceID, int inputv0, int inputv1, int inputv2) :
				oxyDocumentElement(theFacesElement, L"face"),
				faceID(inputfaceID),
				v0(inputv0), v1(inputv1), v2(inputv2)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxyMeshSection::oxyMeshFaces::oxyMeshFaceDataElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"faceID", _variant_t(faceID)));
				attributeList.push_back(elementAttribute(L"v0", _variant_t(v0)));
				attributeList.push_back(elementAttribute(L"v1", _variant_t(v1)));
				attributeList.push_back(elementAttribute(L"v2", _variant_t(v2)));
			}

			oxyMeshSection::oxyMeshFaces::oxyMeshFaces(oxyMeshElementPtr theMeshElement, int inputnumFaces)
			{
				theFacesElement = std::make_shared<oxyMeshFacesElement>(theMeshElement, inputnumFaces);
			}
			void oxyMeshSection::oxyMeshFaces::addFace(int inputfaceID, int inputv0, int inputv1, int inputv2)
			{
				oxyMeshFaceDataElementPtr theFaceElement = std::make_shared<oxyMeshFaceDataElement>(theFacesElement, inputfaceID, inputv0, inputv1, inputv2);
			}
			oxyMeshSection::oxyMeshVertices::oxyMeshVerticesElement::oxyMeshVerticesElement(oxyMeshElementPtr theMeshElement, int inputnumVertices) :
				oxyDocumentElement(theMeshElement, L"vertices"),
				numVertices(inputnumVertices)
			{
				buildListOfAttributes();
				setElementAttributes();
			}
			void oxyMeshSection::oxyMeshVertices::oxyMeshVerticesElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"numVertices", _variant_t(numVertices)));
			}
			oxyMeshSection::oxyMeshVertices::oxyMeshVertexDataElement::oxyMeshVertexDataElement(oxyMeshVerticesElementPtr theVerticesElement, int inputvertexID, int inputtheVert, float inputx, float inputy, float inputz, int inputtheTVert, float inputu, float inputv, int inputtheNormal, float inputnx, float inputny, float inputnz) :
				oxyDocumentElement(theVerticesElement, L"vertex"),
				vertexID(inputvertexID),
				theVert(inputtheVert),
				x(inputx), y(inputy), z(inputz),
				theTVert(inputtheTVert),
				u(inputu), v(inputv),
				theNormal(inputtheNormal),
				nx(inputnx), ny(inputny), nz(inputnz)
			{
				buildListOfAttributes();
				setElementAttributes();
			}
			void oxyMeshSection::oxyMeshVertices::oxyMeshVertexDataElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"vertexID", _variant_t(vertexID)));
				attributeList.push_back(elementAttribute(L"theVert", _variant_t(theVert)));
				attributeList.push_back(elementAttribute(L"x", variantFromFloat(x)));
				attributeList.push_back(elementAttribute(L"y", variantFromFloat(y)));
				attributeList.push_back(elementAttribute(L"z", variantFromFloat(z)));
				attributeList.push_back(elementAttribute(L"theTVert", _variant_t(theTVert)));
				attributeList.push_back(elementAttribute(L"u", variantFromFloat(u)));
				attributeList.push_back(elementAttribute(L"v", variantFromFloat(v)));
				attributeList.push_back(elementAttribute(L"theNormal", _variant_t(theNormal)));
				attributeList.push_back(elementAttribute(L"nx", variantFromFloat(nx)));
				attributeList.push_back(elementAttribute(L"ny", variantFromFloat(ny)));
				attributeList.push_back(elementAttribute(L"nz", variantFromFloat(nz)));
			}
			oxyMeshSection::oxyMeshVertices::oxyMeshVertices(oxyMeshElementPtr theMeshElement, int inputnumVertices)
			{
				theVerticesElement = std::make_shared<oxyMeshVerticesElement>(theMeshElement, inputnumVertices);
			}
			void oxyMeshSection::oxyMeshVertices::addVertex(int inputvertexID, int inputtheVert, float inputx, float inputy, float inputz, int inputtheTVert, float inputu, float inputv, int inputtheNormal, float inputnx, float inputny, float inputnz)
			{
				oxyMeshVertexDataElementPtr theVertexElement = std::make_shared<oxyMeshVertexDataElement>(theVerticesElement, inputvertexID, inputtheVert, inputx, inputy, inputz, inputtheTVert, inputu, inputv, inputtheNormal, inputnx, inputny, inputnz);
			}
}
	}
}
