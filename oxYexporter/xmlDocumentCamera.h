#pragma once
#include "xmlDocument.h"

namespace oxyde {
	namespace exporter {
		namespace XML {

			class oxyCamerasInSceneElement : public oxyDocumentElement {
			public:

				//oxyCamerasInSceneElement(oxyDocumentElementPtr theParent, std::wstring theElementName) = delete;
				oxyCamerasInSceneElement() = delete;
				oxyCamerasInSceneElement(oxyDocumentPtr theInputDocument) :oxyDocumentElement(theInputDocument, L"camerasInScene") {}

				//static oxyDocumentElementPtr branchNodeElement(std::shared_ptr<oxySceneElement> theParent, std::wstring inputNodeName);
			};

			using oxyCamerasInSceneElementPtr = std::shared_ptr<oxyCamerasInSceneElement>;

			class oxyCameraElement : public oxyDocumentElement {
			public:
				oxyCameraElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyCameraElement(oxyCamerasInSceneElementPtr  theParent, std::wstring in_nodeName, float in_fov, std::wstring in_target);

			protected:
				std::wstring nodeName;
				float fov;
				std::wstring target;

				void buildListOfAttributes() override;
			};
			
			using oxyCameraElementPtr = std::shared_ptr<oxyCameraElement>;
		}
	}
}
