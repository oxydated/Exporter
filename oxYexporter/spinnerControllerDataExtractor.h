#pragma once

#include "controllerDataExtractor.h"

namespace oxyde {
	namespace exporter {
		namespace controller {

			class spinnerControllerDataExtractor :public controllerDataExtractor {
			public:
				virtual void exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement);
				static void registerMe();
				static Class_ID getClass_ID();
				static controllerDataExtractor_ptr buildSpinnerControllerDataExtractor(Control* theControl);
			private:
				explicit spinnerControllerDataExtractor(Control* theControl) :controllerDataExtractor(theControl) {}
				void processBaseSpinningBoneControl(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement);
				void processTipSpinningBoneControl(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement);
			};

		}
	}
}
