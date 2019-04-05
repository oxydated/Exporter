#pragma once
#include "xmlDocumentAnimation.h"

namespace oxyde {
	namespace exporter {
		namespace XML {
			
			class oxyPRSElement : public oxyDocumentElement {
			public:
				oxyPRSElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyPRSElement(oxyAnimationElementPtr  theParent) :oxyDocumentElement(theParent, L"PRS") {}
			};

			using oxyPRSElementPtr = std::shared_ptr<oxyPRSElement>;

			class oxyPositionElement : public oxyDocumentElement {
			public:
				oxyPositionElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyPositionElement(oxyDocumentElementPtr  theParent) :oxyDocumentElement(theParent, L"Position") {}
			};

			using oxyPositionElementPtr = std::shared_ptr<oxyPositionElement>;

			class oxyRotationElement : public oxyDocumentElement {
			public:
				oxyRotationElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyRotationElement(oxyPRSElementPtr  theParent, int in_eulerOrder);

			protected:
				int eulerOrder;
				void buildListOfAttributes() override;
			};

			using oxyRotationElementPtr = std::shared_ptr<oxyRotationElement>;


			class oxyBezierTrackElement : public oxyDocumentElement {
			public:
				oxyBezierTrackElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyBezierTrackElement(oxyDocumentElementPtr  theParent, int in_numKeys);

			protected:
				int numKeys;
				void buildListOfAttributes() override;
			};

			using oxyBezierTrackElementPtr = std::shared_ptr<oxyBezierTrackElement>;


			class oxyBezierKeyElement : public oxyDocumentElement {
			public:
				oxyBezierKeyElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyBezierKeyElement(oxyBezierTrackElementPtr  theParent, int in_startTime, int in_endTime, float in_coeff_B3, float in_coeff_B2, float in_coeff_B1, float in_coeff_B0);

			protected:
				int startTime, endTime;
				float coeff_B3, coeff_B2, coeff_B1, coeff_B0;

				void buildListOfAttributes() override;
			};

			using oxyBezierKeyElementPtr = std::shared_ptr<oxyBezierKeyElement>;
		}
	}
}
