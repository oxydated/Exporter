#pragma once

#include "timeControllerDataExtraction.h"

namespace oxyde {
	namespace exporter {
		namespace controller {

			class perFrameControllerDataExtractor : public matrixControllerDataExtractor {
			public:
				virtual std::set<TimeValue> getKeyTimes() override;
				virtual void exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement) override;
				//static Class_ID getClass_ID();
				static void registerMe(Class_ID);
				static controllerDataExtractor_ptr buildPerFrameControllerDataExtractor(Control* theControl);

				bool isPerFrame() override;

			protected:
				void getLocalMatrixForTime(TimeValue t, double m[]);
				void getDualQuatForTime(TimeValue t, float quat[]);
				IKeyControl* GetKeyControlInterfacePointer();

				explicit perFrameControllerDataExtractor(Control* theControl) :matrixControllerDataExtractor(theControl) {}
			};

		}
	}
}

