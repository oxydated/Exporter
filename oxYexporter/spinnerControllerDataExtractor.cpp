#include "spinnerControllerDataExtractor.h"
#include "matrixUtility.h"
#include "linAlg.h"
#include "dualQuaternionFunctions.h"
#include "xmlDocumentAnimation.h"

namespace oxyde {
	namespace exporter {
		namespace controller {

			void spinnerControllerDataExtractor::exportController(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement)
			{
				IScriptCtrl* theScriptControl = NULL;
				theScriptControl = (IScriptCtrl*)m_Control->GetInterface(IID_SCRIPT_CONTROL);
				if (theScriptControl != NULL) {
					MSTR theScriptControlDescription = theScriptControl->GetDescription();
					if (theScriptControlDescription == _M("BaseBoneController")) {
						processBaseSpinningBoneControl(theAnimationElement);
					}
					if (theScriptControlDescription == _M("SpinningBoneController")) {
						processTipSpinningBoneControl(theAnimationElement);
					}
				}
			}

			Class_ID spinnerControllerDataExtractor::getClass_ID()
			{
				return PRS_SCRIPT_CONTROL_CLASS_ID;
			}

			void spinnerControllerDataExtractor::registerMe()
			{
				registerFactory(spinnerControllerDataExtractor::buildSpinnerControllerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr spinnerControllerDataExtractor::buildSpinnerControllerDataExtractor(Control * theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(std::make_shared<spinnerControllerDataExtractor>(spinnerControllerDataExtractor(theControl)));
			}

			void spinnerControllerDataExtractor::processBaseSpinningBoneControl(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement)
			{
				IBaseScriptControl8* theScript = (IBaseScriptControl8*)m_Control;

				int varCount = theScript->getVarCount();
				MSTR sourceVarName = _M("source");
				MSTR sourceName;
				MSTR localTransformVarName = _M("local_transform_for_newBone_On_Creation");
				Matrix3 localTransform;
				float quat[8];

				for (int i = 0; i < varCount; i++) {
					MSTR &varName = theScript->getVarName(i);
					if (varName == sourceVarName) {
						FPValue &theValue = theScript->getVarValue(i, 0);
						ParamType2 theType = theValue.type;
						if (theType == TYPE_INODE) {
							sourceName = theValue.n->GetName();

						}
					}
					if (varName == localTransformVarName) {
						FPValue &theValue = theScript->getVarValue(i, 0);
						ParamType2 theType = theValue.type;
						if (theType == TYPE_MATRIX3_BV) {
							localTransform = *(theValue.m);

							double m[16];
							m[0] = localTransform.GetRow(0).x;		m[1] = localTransform.GetRow(0).y;		m[2] = localTransform.GetRow(0).z;		m[3] = 0.;
							m[4] = localTransform.GetRow(1).x;		m[5] = localTransform.GetRow(1).y;		m[6] = localTransform.GetRow(1).z;		m[7] = 0.;
							m[8] = localTransform.GetRow(2).x;		m[9] = localTransform.GetRow(2).y;		m[10] = localTransform.GetRow(2).z;		m[11] = 0.;
							m[12] = localTransform.GetRow(3).x;		m[13] = localTransform.GetRow(3).y;		m[14] = localTransform.GetRow(3).z;		m[15] = 1.;

							oxyde::math::getDualQuaternionFromMatrix(m, DUALQUAARRAY(quat));
						}
					}
				}

				std::shared_ptr<oxyde::exporter::XML::baseSpinnerDataElement> baseElement = std::make_shared<oxyde::exporter::XML::baseSpinnerDataElement>(
					theAnimationElement, std::wstring(sourceName), quat[0], quat[1], quat[2], quat[3], quat[4], quat[5], quat[6], quat[7]	);
			}

			void spinnerControllerDataExtractor::processTipSpinningBoneControl(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement)
			{
				IBaseScriptControl8* theScript = (IBaseScriptControl8*)m_Control;

				int varCount = theScript->getVarCount();
				MSTR sourceVarName = _M("source");
				MSTR sourceName;
				MSTR localTransformVarName = _M("newBoneTip_local_transform");
				Matrix3 localTransform;
				float quat[8];

				for (int i = 0; i < varCount; i++) {
					MSTR &varName = theScript->getVarName(i);
					if (varName == sourceVarName) {
						FPValue &theValue = theScript->getVarValue(i, 0);
						ParamType2 theType = theValue.type;
						if (theType == TYPE_INODE) {
							sourceName = theValue.n->GetName();

						}
					}
					if (varName == localTransformVarName) {
						FPValue &theValue = theScript->getVarValue(i, 0);
						ParamType2 theType = theValue.type;
						if (theType == TYPE_MATRIX3_BV) {
							localTransform = *(theValue.m);

							double m[16];
							m[0] = localTransform.GetRow(0).x;		m[1] = localTransform.GetRow(0).y;		m[2] = localTransform.GetRow(0).z;		m[3] = 0.;
							m[4] = localTransform.GetRow(1).x;		m[5] = localTransform.GetRow(1).y;		m[6] = localTransform.GetRow(1).z;		m[7] = 0.;
							m[8] = localTransform.GetRow(2).x;		m[9] = localTransform.GetRow(2).y;		m[10] = localTransform.GetRow(2).z;		m[11] = 0.;
							m[12] = localTransform.GetRow(3).x;		m[13] = localTransform.GetRow(3).y;		m[14] = localTransform.GetRow(3).z;		m[15] = 1.;

							oxyde::math::getDualQuaternionFromMatrix(m, DUALQUAARRAY(quat));
						}
					}
				}

				std::shared_ptr<oxyde::exporter::XML::tipSpinnerDataElement> baseElement = std::make_shared<oxyde::exporter::XML::tipSpinnerDataElement>(
					theAnimationElement, std::wstring(sourceName), quat[0], quat[1], quat[2], quat[3], quat[4], quat[5], quat[6], quat[7]);
			}

		}
	}
}

namespace {
	class factoryInstaller {
	public:
		factoryInstaller() {
			oxyde::exporter::controller::spinnerControllerDataExtractor::registerMe();
		}
	};

	factoryInstaller theFactoryInstaller;
}


