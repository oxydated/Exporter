#include "processCamera.h"

namespace oxyde {
	namespace exporter {
		namespace camera {

			void exportCamera(INode * cameraNode, oxyde::exporter::XML::oxyCamerasInSceneElementPtr camerasInSceneElement)
			{

				Object* cameraNodeObject = cameraNode->GetObjectRef();
				//Class_ID theTopNodeObjectClassID = cameraNodeObject->ClassID();
				//SClass_ID theTopNodeObjectSuperClassID = theTopNodeObject->SuperClassID();
				//MSTR objectClassName;
				//theTopNodeObject->GetClassName(objectClassName);

				////oxyde::log::printText(std::wstring(L"Class Name: ") + std::wstring(objectClassName));

				if (cameraNodeObject->CanConvertToType(Class_ID(LOOKAT_CAM_CLASS_ID, 0))) {
					GenCamera* theLookAtCamera = reinterpret_cast<GenCamera*>(cameraNodeObject->ConvertToType(0, Class_ID(LOOKAT_CAM_CLASS_ID, 0)));
					//oxyde::log::printText(std::wstring(L"Class Name: ") + std::wstring(objectClassName));
					float FOV = theLookAtCamera->GetFOV(0);
					std::wstring cameraNodeName = cameraNode->NodeName();
					if (cameraNode->GetTarget() != nullptr) {
						std::wstring cameraTargetName = cameraNode->GetTarget()->NodeName();
						oxyde::exporter::XML::oxyCameraElementPtr theBezierKeyElement = std::make_shared<oxyde::exporter::XML::oxyCameraElement>(camerasInSceneElement, cameraNodeName, FOV, cameraTargetName);
					}
				}
			}

			void exportAllCameras(oxyde::exporter::XML::oxyDocumentPtr theDocument, const std::vector<INodePtr>& cameraNodes)
			{
				oxyde::exporter::XML::oxyCamerasInSceneElementPtr camerasInSceneElement = std::make_shared<oxyde::exporter::XML::oxyCamerasInSceneElement>(theDocument);
				for (auto &&camNode : cameraNodes) {
					exportCamera(camNode, camerasInSceneElement);
				}
			}
		}
	}
}
