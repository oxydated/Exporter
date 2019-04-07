#include "xmlDocumentCamera.h"

namespace oxyde {
	namespace exporter {
		namespace XML {

			oxyCameraElement::oxyCameraElement(oxyCamerasInSceneElementPtr theParent, std::wstring in_nodeName, float in_fov, std::wstring in_target):
				oxyDocumentElement(theParent, L"Camera"), nodeName(in_nodeName), fov(in_fov), target(in_target)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxyCameraElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"nodeName", _variant_t(nodeName.data())));
				attributeList.push_back(elementAttribute(L"fov", variantFromFloat(fov)));
				attributeList.push_back(elementAttribute(L"target", _variant_t(target.data())));
			}

		}
	}
}
