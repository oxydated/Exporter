#include "perFrameControllerDataExtractor.h"
#include "matrixUtility.h"
#include "linAlg.h"
#include "dualQuaternionFunctions.h"
#include <iiksys.h>

namespace oxyde {
	namespace exporter {
		namespace controller {

			std::set<TimeValue> perFrameControllerDataExtractor::getKeyTimes()
			{
				std::set<TimeValue> timeSet;
				timeSet.insert(TimeValue(0));

				Interval animationInterval = GetCOREInterface()->GetAnimRange();
				int ticksPerFrame = GetTicksPerFrame();
				for (TimeValue i = animationInterval.Start(); i <= animationInterval.End(); i += ticksPerFrame) {

					timeSet.insert(i);
				}
				return timeSet;
			}
			void perFrameControllerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement)
			{
				buildTrack(std::dynamic_pointer_cast<oxyde::exporter::XML::oxyAnimationElement>(theAnimationElement));
			}

			void perFrameControllerDataExtractor::registerMe(Class_ID theID)
			{
				registerFactory(perFrameControllerDataExtractor::buildPerFrameControllerDataExtractor, theID, true);
			}

			controllerDataExtractor_ptr perFrameControllerDataExtractor::buildPerFrameControllerDataExtractor(Control * theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(std::make_shared<perFrameControllerDataExtractor>(perFrameControllerDataExtractor(theControl)));
			}

			void perFrameControllerDataExtractor::getLocalMatrixForTime(TimeValue t, double m[]) 
			{
				Matrix3 localMatrix;
				localMatrix.IdentityMatrix();
				m_Control->GetValue(t, (void*)&localMatrix, FOREVER, CTRL_RELATIVE);
				//m_Control->GetValue(t, (void*)&localMatrix, FOREVER, CTRL_ABSOLUTE);

				m[0] = localMatrix.GetRow(0).x;		m[1] = localMatrix.GetRow(0).y;		m[2] = localMatrix.GetRow(0).z;		m[3] = 0.;
				m[4] = localMatrix.GetRow(1).x;		m[5] = localMatrix.GetRow(1).y;		m[6] = localMatrix.GetRow(1).z;		m[7] = 0.;
				m[8] = localMatrix.GetRow(2).x;		m[9] = localMatrix.GetRow(2).y;		m[10] = localMatrix.GetRow(2).z;	m[11] = 0.;
				m[12] = localMatrix.GetRow(3).x;	m[13] = localMatrix.GetRow(3).y;	m[14] = localMatrix.GetRow(3).z;	m[15] = 1.;
			}

			void perFrameControllerDataExtractor::getDualQuatForTime(TimeValue t, float quat[])
			{
				double m[16];
				getLocalMatrixForTime(t, m);
				oxyde::math::getDualQuaternionFromMatrix(m, DUALQUAARRAY(quat));
			}

			IKeyControl * perFrameControllerDataExtractor::GetKeyControlInterfacePointer()
			{
				return GetKeyControlInterface(m_Control);
			}

			bool perFrameControllerDataExtractor::isPerFrame()
			{
				return true;
			}

		}
	}
}

namespace {
	class factoryInstaller {
	public:
		factoryInstaller() {
			oxyde::exporter::controller::perFrameControllerDataExtractor::registerMe(IKCONTROL_CLASS_ID);

			oxyde::exporter::controller::perFrameControllerDataExtractor::registerMe(Class_ID(0x1CBB1D2F, 0x30EA5264));

			oxyde::exporter::controller::perFrameControllerDataExtractor::registerMe(Class_ID(0xDAB5C9D, 0x5DB56555));

			oxyde::exporter::controller::perFrameControllerDataExtractor::registerMe(Class_ID(3895672849L, 2930979016L));

			oxyde::exporter::controller::perFrameControllerDataExtractor::registerMe(Class_ID(8209L, 0L));
		}
	};

	factoryInstaller theFactoryInstaller;
}
