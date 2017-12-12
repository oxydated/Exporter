#pragma once

#include <max.h>
#include <control.h>
#include <istdplug.h>
#include <set>
#include <map>
#include <set>
#include <utility>
#include <memory>
#include <CS/Biped12Api.h>
#include <maxscript/maxwrapper/scriptcontroller.h>
//#include "xmlAnimationExporter.h"

namespace oxyde {
	namespace exporter {

		namespace controller {

			class controllerDataExtractor;

			using controllerDataExtractor_ptr = std::shared_ptr<controllerDataExtractor>;

			class controllerDataExtractor {
			public:
				virtual void exportController(IXMLDOMElement* theAnimationElement) = 0;

				static controllerDataExtractor_ptr buildExtractor(Control* theControl);
				static controllerDataExtractor_ptr buildExtractorAndSetCurrentNode(Control* theControl, INode* theNode);

			protected:
				controllerDataExtractor() = default;
				explicit controllerDataExtractor(Control *theControl) {
					m_Control = theControl;
				};
				static void registerFactory(controllerDataExtractor_ptr(*theFactory)(Control*), Class_ID theClassID);

				static std::map<Class_ID, controllerDataExtractor_ptr(*)(Control*)> factoriesMap;
				static INode* currentNode;

				Control* m_Control;
			};

			class keyControllerDataExtractor : public controllerDataExtractor {
			public:
				virtual std::set<TimeValue> getKeyTimes();

			protected:
				explicit keyControllerDataExtractor(Control* theControl) :controllerDataExtractor(theControl) {}
				virtual IKeyControl* GetKeyControlInterfacePointer() = 0;
			};

		}
	}
}
