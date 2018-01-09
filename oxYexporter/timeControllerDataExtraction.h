#pragma once
#include "controllerDataExtractor.h"

namespace oxyde {
	namespace exporter {
		namespace controller {

			class defaultControllerDataExtractor : public controllerDataExtractor {
			public:
				virtual void exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement) override;
				static Class_ID getClass_ID();
				static void registerMe();
				static controllerDataExtractor_ptr buildDefaultControllerDataExtractor(Control* theControl);

			protected:
				explicit defaultControllerDataExtractor(Control* theControl) :controllerDataExtractor(theControl) {}
			};

			class floatControllerDataExtractor : public keyControllerDataExtractor {
			public:
				float getValueForTime(TimeValue theTime);
				void exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement) override;

				static Class_ID getClass_ID();
				static void registerMe();
				static controllerDataExtractor_ptr buildFloatControllerDataExtractor(Control* theControl);

			protected:

				explicit floatControllerDataExtractor(Control* theControl) :keyControllerDataExtractor(theControl) {}
				IKeyControl* GetKeyControlInterfacePointer();
				
			};

			class matrixControllerDataExtractor : public keyControllerDataExtractor {
			public:
				void buildTrack(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement);
			protected:
				virtual void getDualQuatForTime(TimeValue t, float quat[]) = 0;
				virtual void getLocalMatrixForTime(TimeValue t, double m[]) = 0;
				explicit matrixControllerDataExtractor(Control* theControl) :keyControllerDataExtractor(theControl){}
			};

			class PRScontrollerDataExtractor : public matrixControllerDataExtractor {
			public:
				virtual void exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement) override;
				std::set<TimeValue> getKeyTimes();

				static Class_ID getClass_ID();
				static void registerMe();
				static controllerDataExtractor_ptr buildPRScontrollerDataExtractor(Control* theControl);

			protected:
				void getLocalMatrixForTime(TimeValue t, double m[]);
				void getDualQuatForTime(TimeValue t, float quat[]);
				IKeyControl* GetKeyControlInterfacePointer();
				explicit PRScontrollerDataExtractor(Control *theControl);
				controllerDataExtractor_ptr m_positionController;
				controllerDataExtractor_ptr m_rotationController;
				controllerDataExtractor_ptr m_scaleController;
			};

			class XYZControllerDataExtractor : public controllerDataExtractor {
			public:
				std::set<TimeValue> getKeyTimes();
				void getValueForTime(TimeValue theTime, float &x, float &y, float &z);
				void getValueForTime(TimeValue theTime, Quat &q);

			protected:
				explicit XYZControllerDataExtractor(Control *theControl);
				controllerDataExtractor_ptr m_XController;
				controllerDataExtractor_ptr m_YController;
				controllerDataExtractor_ptr m_ZController;
			};

			class positionControllerDataExtractor : public XYZControllerDataExtractor {
			public:
				virtual void exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement) override;
				static Class_ID getClass_ID();
				static void registerMe();
				static controllerDataExtractor_ptr buildPositionControllerDataExtractor(Control* theControl);

			protected:
				explicit positionControllerDataExtractor(Control *theControl);
			};

			class rotationControllerDataExtractor : public XYZControllerDataExtractor {
			public:
				virtual void exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement) override;
				int getEulerAngleOrder();
				static Class_ID getClass_ID();
				static void registerMe();
				static controllerDataExtractor_ptr buildRotationControllerDataExtractor(Control* theControl);

			protected:
				explicit rotationControllerDataExtractor(Control *theControl);
			};

			class bipSlaveControllerDataExtractor : public matrixControllerDataExtractor {
			public:
				virtual void exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement) override;
				static Class_ID getClass_ID();
				static void registerMe();
				static controllerDataExtractor_ptr buildBipSlaveControllerDataExtractor(Control* theControl);

			protected:
				void getLocalMatrixForTime(TimeValue t, double m[]);
				void getDualQuatForTime(TimeValue t, float quat[]);
				IKeyControl* GetKeyControlInterfacePointer();
				INode* m_bipNode;
				explicit bipSlaveControllerDataExtractor(Control* theControl):matrixControllerDataExtractor(theControl), m_bipNode(currentNode) {}
			};

			class bipBodyControllerDataExtractor : public controllerDataExtractor {
			public:
				virtual void exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement) override;
				static Class_ID getClass_ID();
				static void registerMe();
				static controllerDataExtractor_ptr buildBipBodyControllerDataExtractor(Control* theControl);

			protected:
				explicit bipBodyControllerDataExtractor(Control *theControl);
				controllerDataExtractor_ptr m_bipSlaveController;
			};


		}
	}

}