#pragma once

#include <max.h>
#include "xmlDocumentCamera.h"

namespace oxyde {
	namespace exporter {
		namespace camera {

			void exportCamera(INode* cameraNode, oxyde::exporter::XML::oxyCamerasInSceneElementPtr camerasInScene);

			void exportAllCameras(oxyde::exporter::XML::oxyDocumentPtr theDocument, const std::vector<INodePtr> &cameraNodes);
		}
	}
}