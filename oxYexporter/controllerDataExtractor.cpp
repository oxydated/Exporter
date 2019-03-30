#include <sstream>
#include <stack>
#include "controllerDataExtractor.h"
#include "debugLog.h"
#include "iparamb2.h"

namespace oxyde {
	namespace exporter {

		namespace controller {

			std::map<Class_ID, controllerDataExtractor_ptr(*)(Control*)> controllerDataExtractor::factoriesMap;
			std::vector<Class_ID> controllerDataExtractor::perFrameFactories;
			INode* controllerDataExtractor::currentNode = nullptr;

			controllerDataExtractor_ptr controllerDataExtractor::buildExtractor(Control* theControl) {

				Class_ID theClassID = theControl->ClassID();

				if (Class_ID(PRS_CONTROL_CLASS_ID, 0x0) == theClassID) {
					if (!theControl->IsLeaf()) {
						Control* positionControl = theControl->GetPositionController();
						if (positionControl != nullptr) {
							Class_ID positionCotrolClassID = positionControl->ClassID();

							if (std::find(perFrameFactories.begin(), perFrameFactories.end(), positionCotrolClassID) != perFrameFactories.end()) {
								theClassID = positionCotrolClassID;
							}
						}
					}
				}

				using intAnim = std::pair<int, Animatable*>;
				std::stack<intAnim> stackIntAnim = std::stack<intAnim>();
				stackIntAnim.push(intAnim(0, theControl));
				while (!stackIntAnim.empty()) {
					intAnim theTopIntAnim = stackIntAnim.top();
					stackIntAnim.pop();

					for (int i = 0; i < theTopIntAnim.second->NumSubs(); i++) {
						stackIntAnim.push(intAnim(theTopIntAnim.first + 1, theTopIntAnim.second->SubAnim(i)));
					}
					std::wstringstream animStream;
					for (int i = 0; i < theTopIntAnim.first; i++) {
						animStream << L"	";
					}
					animStream << std::wstring(theTopIntAnim.second->ClassName()) << L">  ";
					Class_ID theClassIDTopAnim = theTopIntAnim.second->ClassID();
					animStream << std::hex << theClassIDTopAnim.PartA() << L":" << std::hex << theClassIDTopAnim.PartB();
					Control* theTopAnimAsControl = dynamic_cast<Control*>(theTopIntAnim.second);
					if (theTopAnimAsControl != nullptr) {
						animStream << L"	it's a controller   ";
						animStream << L"	is it LEAF? [" << theTopAnimAsControl->IsLeaf() << L"]   ";
						animStream << L"is it KEYABLE? [" << theTopAnimAsControl->IsKeyable() << L"]  ";
						IKeyControl* theKeyControl = GetKeyControlInterface(theTopAnimAsControl);

						ISurfPosition* theSurfPosInterface = dynamic_cast<ISurfPosition*>(theTopIntAnim.second);
						if (theSurfPosInterface) {
							INode* theSurfTarget = dynamic_cast<INode*>(theSurfPosInterface->GetReference(SURFCONT_SURFOBJ_REF));
							if (theSurfTarget) {
								animStream <<L" the Surface name:  "<< std::wstring(theSurfTarget->GetName()) << L"]  ";
							}
						}

						if (theKeyControl != NULL) {
							animStream << L"Num Keys [" << theKeyControl->GetNumKeys() << L"]  ";
							if (theKeyControl->GetNumKeys() > 0) {
								IKey* theKey = (IKey*)(new char[theKeyControl->GetKeySize()]);
								//if (theTopAnimAsControl->IsSubClassOf(Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0x0))) {
								if (theTopAnimAsControl->IsSubClassOf(Class_ID(0x9154, 0x0))) {
									theKeyControl->GetKey(0, theKey);
									ITCBRotKey* theRotKey = static_cast<ITCBRotKey*>(theKey);
									float testAngle = theRotKey->val.angle;
									float testAxisX = theRotKey->val.axis.x;
								}
							}
						}
						else {
							animStream << L"it has no keys  ";
						}
					}
					int numParamBlocks = theTopIntAnim.second->NumParamBlocks();
					for (int i = 0; i < numParamBlocks; i++) {
						IParamBlock2* theParam = theTopIntAnim.second->GetParamBlock(i);
						animStream << L"ParamBlocks Name: " << std::wstring(theParam->GetLocalName()) << L"]		";
						for (int j = 0; j < theParam->NumParams(); j++) {
							animStream << L"Param Name: " << std::wstring(theParam->GetParamDefByIndex(j)->int_name) << L"]		";
							if (theParam->GetAnimNum(theParam->IndextoID(j)) != -1) {
								animStream << L"Param SubAnim: " << theParam->GetAnimNum(theParam->IndextoID(j)) << L"]		";
							}
						}
					}

					for (int i = 0; i < theTopIntAnim.second->NumSubs(); i++) {
						animStream << L"SubAnim Name: " << std::wstring(theTopIntAnim.second->SubAnimName(i)) << L"]		";
					}

					//animStream << L"	is it LEAF? [" << theTopIntAnim.second->IsLeaf() << L"]   ";
					//animStream << L"is it KEYABLE? [" << theTopIntAnim.second->IsKeyable() << L"]  \n";
					oxyde::log::printText(animStream.str());

				}

				//auto foundFactory = factoriesMap.find(theControl->ClassID());
				auto foundFactory = factoriesMap.find(theClassID);
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

			void controllerDataExtractor::registerFactory(controllerDataExtractor_ptr(*theFactory)(Control*), Class_ID theClassID, bool isPerFrame) {
				factoriesMap[theClassID] = theFactory;
				if (isPerFrame) {
					perFrameFactories.push_back(theClassID);
				}
			}

			bool controllerDataExtractor::isPerFrame()
			{
				return false;
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

			bool keyControllerDataExtractor::doesItHaveKeyTime() {
				std::set<TimeValue> timeSet;
				timeSet.insert(TimeValue(0));

				IKeyControl* keyControl = GetKeyControlInterfacePointer();
				int numKeys = keyControl->GetNumKeys();

				return numKeys > 0;
			}

		}
	}
}