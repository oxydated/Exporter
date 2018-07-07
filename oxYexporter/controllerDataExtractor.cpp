#include <sstream>
#include "controllerDataExtractor.h"
#include "debugLog.h"

namespace oxyde {
	namespace exporter {

		namespace controller {

			std::map<Class_ID, controllerDataExtractor_ptr(*)(Control*)> controllerDataExtractor::factoriesMap;
			INode* controllerDataExtractor::currentNode = nullptr;

			controllerDataExtractor_ptr controllerDataExtractor::buildExtractor(Control* theControl) {

				std::wstringstream theStream;
				theStream << std::wstring(theControl->NodeName()) << L" ... ";
				Class_ID theClassID = theControl->ClassID();
				theStream << std::hex << theClassID.PartA() << L":" << std::hex << theClassID.PartB();
				oxyde::log::printText(theStream.str());

				auto foundFactory = factoriesMap.find(theControl->ClassID());
				return foundFactory != factoriesMap.end() ?
					foundFactory->second(theControl) :
					factoriesMap[Class_ID()](theControl);
			}

			controllerDataExtractor_ptr controllerDataExtractor::buildExtractorAndSetCurrentNode(Control* theControl, INode* theNode) {
				currentNode = theNode;
				return buildExtractor(theControl);
			}

			void controllerDataExtractor::registerFactory(controllerDataExtractor_ptr(*theFactory)(Control*), Class_ID theClassID) {
				factoriesMap[theClassID] = theFactory;
			}

			std::set<TimeValue> keyControllerDataExtractor::getKeyTimes() {
				std::set<TimeValue> timeSet;
				timeSet.insert(TimeValue(0));

				IKeyControl* keyControl = GetKeyControlInterfacePointer();
				int numKeys = keyControl->GetNumKeys();
				for (int i = 0; i < numKeys; i++) {
					timeSet.insert(m_Control->GetKeyTime(i));
				}
				return timeSet;
			}

		}
	}
}