#pragma once
#include "xmlDocumentScene.h"
namespace oxyde {
	namespace exporter {
		namespace XML {

			class oxyAnimationElement : public oxyDocumentElement {
			public:
				oxyAnimationElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyAnimationElement(oxyNodeElementPtr  theParent):oxyDocumentElement(theParent, L"animaton") {}
			};

			using oxyAnimationElementPtr = std::shared_ptr<oxyAnimationElement>;

			class oxyKeyframeElement : public oxyDocumentElement {
			public:
				oxyKeyframeElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyKeyframeElement(oxyAnimationElementPtr  theParent) :oxyDocumentElement(theParent, L"keyFrames") {}
			};

			using oxyKeyframeElementPtr = std::shared_ptr<oxyKeyframeElement>;

			class oxyDualQuatTrackElement : public oxyDocumentElement {
			public:
				oxyDualQuatTrackElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyDualQuatTrackElement(oxyKeyframeElementPtr  theParent, int inputNumKeys);
			protected:
				int numKeys;
				void buildListOfAttributes() override;
			};

			using oxyDualQuatTrackElementPtr = std::shared_ptr<oxyDualQuatTrackElement>;

			class oxyDualQuatKeyElement : public oxyDocumentElement {
			public:
				oxyDualQuatKeyElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;
				oxyDualQuatKeyElement(oxyDualQuatTrackElementPtr  theParent, int inputstartTime, int inputendTime);
				//	float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz,
				//	float inputangle, float inputux, float inputuy, float inputuz, float inputslide, float inputmx, float inputmy, float inputmz
				//);

				static std::shared_ptr<oxyDualQuatKeyElement> createDualQuatKey(oxyDualQuatTrackElementPtr  theParent, 
					int inputstartTime, int inputendTime,
					float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz,
					float inputangle, float inputux, float inputuy, float inputuz, float inputslide, float inputmx, float inputmy, float inputmz
				);

			protected:
				int startTime; int endTime;
				//float qs; float qx; float qy; float qz; float dqs; float dqx; float dqy; float dqz;
				//float angl; float ux; float uy; float uz; float slide; float mx; float my; float mz;

				void buildListOfAttributes() override;
			};

			using oxyDualQuatKeyElementPtr = std::shared_ptr<oxyDualQuatKeyElement>;

			class startingDualQuatElement :public oxyDocumentElement {
			public:
				friend std::shared_ptr<oxyDualQuatKeyElement> oxyDualQuatKeyElement::createDualQuatKey(oxyDualQuatTrackElementPtr  theParent,
					int inputstartTime, int inputendTime,
					float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz,
					float inputangle, float inputux, float inputuy, float inputuz, float inputslide, float inputmx, float inputmy, float inputmz
				);

				startingDualQuatElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;

			protected:
				float qs; float qx; float qy; float qz; float dqs; float dqx; float dqy; float dqz;

				startingDualQuatElement(oxyDualQuatKeyElementPtr  theParent, float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz);
				void buildListOfAttributes() override;
			};

			class interpolationDataElement :public oxyDocumentElement {
			public:
				friend std::shared_ptr<oxyDualQuatKeyElement> oxyDualQuatKeyElement::createDualQuatKey(oxyDualQuatTrackElementPtr  theParent,
					int inputstartTime, int inputendTime,
					float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz,
					float inputangle, float inputux, float inputuy, float inputuz, float inputslide, float inputmx, float inputmy, float inputmz
				);

				interpolationDataElement(oxyDocumentPtr theInputDocument, std::wstring theElementName) = delete;

			protected:
				float angle; float ux; float uy; float uz; float slide; float mx; float my; float mz;

				interpolationDataElement(oxyDualQuatKeyElementPtr  theParent, float inputangle, float inputux, float inputuy, float inputuz, float inputslide, float inputmx, float inputmy, float inputmz);
				void buildListOfAttributes() override;
			};
		}
	}
}
