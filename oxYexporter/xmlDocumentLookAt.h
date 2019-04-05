#pragma once
#include "xmlDocumentAnimation.h"

namespace oxyde {
	namespace exporter {
		namespace XML {

			class oxyLookAtElement : public oxyDocumentElement {
			public:
				oxyLookAtElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyLookAtElement(oxyAnimationElementPtr  theParent) :oxyDocumentElement(theParent, L"LookAt") {}
			};

			using oxyLookAtElementPtr = std::shared_ptr<oxyLookAtElement>;

		}
	}
}
