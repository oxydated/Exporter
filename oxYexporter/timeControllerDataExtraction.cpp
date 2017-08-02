#include <max.h>
#include <control.h>
#include <istdplug.h>
#include <array>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include "matrixUtility.h"
#include "linAlg.h"
#include "dualQuaternionFunctions.h"
#include "debugLog.h"
#include "timeControllerDataExtraction.h"

namespace oxyde {
	namespace exporter {
		namespace controller {

			Class_ID defaultControllerDataExtractor::getClass_ID() {
				return Class_ID();
			}

			void defaultControllerDataExtractor::exportController(IXMLDOMElement* theAnimationElement) {
				DebugPrint(L"This is a default extractor test... ClassID: 0x%lX : 0x%lX\n", m_Control->ClassID().PartA(), m_Control->ClassID().PartB());
			}

			void defaultControllerDataExtractor::registerMe() {
				registerFactory(defaultControllerDataExtractor::buildDefaultControllerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr defaultControllerDataExtractor::buildDefaultControllerDataExtractor(Control* theControl) {
				return std::dynamic_pointer_cast<controllerDataExtractor>(std::make_shared<defaultControllerDataExtractor>(defaultControllerDataExtractor(theControl)));
			}

			float floatControllerDataExtractor::getValueForTime(TimeValue theTime) {
				float value = 0.0;
				m_Control->GetValue(theTime, (void*)&value, FOREVER, CTRL_ABSOLUTE);
				return 0.0;
			}

			void floatControllerDataExtractor::exportController(IXMLDOMElement* theAnimationElement) {
				DebugPrint(L"This is a float extractor... ClassID: 0x%lX : 0x%lX\n", m_Control->ClassID().PartA(), m_Control->ClassID().PartB());
				return;
			}

			Class_ID floatControllerDataExtractor::getClass_ID() {
				return Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID, 0x0);
			}

			void floatControllerDataExtractor::registerMe() {
				registerFactory(floatControllerDataExtractor::buildFloatControllerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr floatControllerDataExtractor::buildFloatControllerDataExtractor(Control * theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(std::make_shared<floatControllerDataExtractor>(floatControllerDataExtractor(theControl)));
			}
			IKeyControl * floatControllerDataExtractor::GetKeyControlInterfacePointer()
			{
				return GetKeyControlInterface(m_Control);
			}

			void matrixControllerDataExtractor::buildTrack(IXMLDOMElement* theAnimationElement)
			{
				using dualQuat = std::array<double, 8>;
				using dualQuatKey = std::pair<TimeValue, dualQuat>;

				std::set<TimeValue> keyTimes = getKeyTimes();
				if (keyTimes.size() == 0) {
					keyTimes.insert(TimeValue(0));
				}
				std::vector<std::pair<dualQuatKey, dualQuatKey>> intervals;

				dualQuatKey formerValue;
				for (auto timeIntervalEnd = keyTimes.begin(); timeIntervalEnd != keyTimes.end(); timeIntervalEnd++) {

					std::wstring outStr;
					std::wostringstream outStream(outStr);

					outStream << L"		" << *timeIntervalEnd << std::endl;

					float quat[8];

					//getDualQuatForTime(*timeIntervalEnd, quat);

					//oxyde::log::printLine();
					//oxyde::log::printDualQuat(L"fromControllers", quat);

					double m[16];
					getLocalMatrixForTime(*timeIntervalEnd, m);

					oxyde::log::printLine();

					//outStream << std::endl << "______________________________________________________" << std::endl << std::endl;
					float mFloat[16];
					//outStream << "thisMatrix = Transpose[{";
					//outStream << "{" << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << "}" << ",";
					//outStream << "{" << m[4] << ", " << m[5] << ", " << m[6] << ", " << m[7] << "}" << ",";
					//outStream << "{" << m[8] << ", " << m[9] << ", " << m[10] << ", " << m[11] << "}" << ",";
					//outStream << "{" << m[12] << ", " << m[13] << ", " << m[14] << ", " << m[15] << "}" << "}]";
					//outStream << std::endl << std::endl;

					mFloat[0] = m[0];	mFloat[4] = m[4]; mFloat[8] = m[8];		mFloat[12] = m[12];
					mFloat[1] = m[1];	mFloat[5] = m[5]; mFloat[9] = m[9];		mFloat[13] = m[13];
					mFloat[2] = m[2];	mFloat[6] = m[6]; mFloat[10] = m[10];	mFloat[14] = m[14];
					mFloat[3] = m[3];	mFloat[7] = m[7]; mFloat[11] = m[11];	mFloat[15] = m[15];

					oxyde::log::printMatrix(L"thisMatrix", mFloat);

					double nx, ny, nz;
					double angle;
					double slide;
					double mx, my, mz;

					oxyde::math::getDualQuaternionParametersFromMatrix(m, angle, nx, ny, nz, slide, mx, my, mz);

					oxyde::log::printDualQuatParameters(L"rParameters",
						nx, ny, nz,
						angle, slide,
						mx, my, mz
					);
					//outStream << "N[makeDualVersor[" << angle << ", {" << nx << "," << ny << "," << nz << " }, " << slide << ", {" << mx << "," << my << "," << mz << " }]]" << std::endl;
					//outStream << std::endl;

					//DUALQUAVAR(rC);
					DUALQUAVAR(r);

					oxyde::DQ::dual_Versor(angle,
						nx, ny, nz,
						slide,
						mx, my, mz,
						DUALQUACOMP(r));

					oxyde::log::printDualQuat(L"r", DUALQUACOMP(r));

					//oxyde::DQ::dual_quaternion_complement(DUALQUACOMP(rC), DUALQUACOMP(r));

					dualQuat currentQuat = { DUALQUACOMP(r) };
					//dualQuat currentQuat = { DUALQUAARRAY(quat) };
					dualQuatKey currentKey(*timeIntervalEnd, currentQuat);

					//outStream << "{";
					//outStream << currentQuat[0] << "," << currentQuat[1] << "," << currentQuat[2] << "," << currentQuat[3] << ",";
					//outStream << currentQuat[4] << "," << currentQuat[5] << "," << currentQuat[6] << "," << currentQuat[7];
					//outStream << "}";
					//outStream << std::endl;

					oxyde::log::printDualQuat(L"currentQuat", DUALQUAARRAY(currentQuat));
					oxyde::log::printLine();

					//outStream << std::endl << "______________________________________________________" << std::endl << std::endl;

					std::wstring resultString(outStream.str());
					DebugOutputString(resultString.c_str());

					if (timeIntervalEnd != keyTimes.begin()) {
						intervals.push_back(std::pair<dualQuatKey, dualQuatKey>(formerValue, currentKey));
					}
					formerValue = currentKey;
				}

				if (keyTimes.size() == 1) {
					intervals.push_back(std::pair<dualQuatKey, dualQuatKey>(formerValue, formerValue));
				}

				oxyde::log::printLine();

				IXMLDOMElement* theDualQuatTrackElement = insertDualQuatTrackForAnimation( theAnimationElement, intervals.size());

				for (auto &&pair : intervals) {
					std::wstring outStr;
					std::wostringstream outStream(outStr);

					oxyde::log::printLine();
					outStream << L"		" << pair.first.first << "  :  " << pair.second.first << std::endl;
										
					float startQ[8] = {	pair.first.second[0], pair.first.second[1], pair.first.second[2], pair.first.second[3], 
									pair.first.second[4], pair.first.second[5], pair.first.second[6], pair.first.second[7] };

					float endQ[8] = { pair.second.second[0], pair.second.second[1], pair.second.second[2], pair.second.second[3],
						pair.second.second[4], pair.second.second[5], pair.second.second[6], pair.second.second[7] };

					float transQ[8] = { 0., 0. , 0. , 0. , 0. , 0. , 0. , 0. };

					oxyde::DQ::transformFromSourceToDestinationAxis(
						startQ[0], startQ[1], startQ[2], startQ[3], startQ[4], startQ[5], startQ[6], startQ[7],
						endQ[0], endQ[1], endQ[2], endQ[3], endQ[4], endQ[5], endQ[6], endQ[7],
						transQ[0], transQ[1], transQ[2], transQ[3], transQ[4], transQ[5], transQ[6], transQ[7]
					);

					////////////////////////////////////////////
					TimeValue halfway = pair.first.first + (pair.second.first - pair.first.first) / 2;
					double m[16];
					getLocalMatrixForTime(halfway, m);
					//oxyde::log::printText(std::to_wstring(halfway));
					float halfQ[8];
					oxyde::math::getDualQuaternionFromMatrix(m, DUALQUAARRAY(halfQ));

					//oxyde::log::printDualQuat(L"halfQ", halfQ);

					float fromStartToHalfQ[8];
					oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(startQ), DUALQUAARRAY(halfQ), DUALQUAARRAY(fromStartToHalfQ));

					//oxyde::log::printDualQuat(L"fromStartToHalfQ", fromStartToHalfQ);

					oxyde::DQ::dualQuatParameters theHalfwayParameters;

					oxyde::DQ::extractDualVersorParameters(
						DUALQUAARRAY(fromStartToHalfQ),
						theHalfwayParameters
					);

					double halfwayAngle = 2 * oxyde::math::getAngleFromCosAndSin(theHalfwayParameters.qS, theHalfwayParameters.theSin);

					//oxyde::log::printDualQuatParameters(L"theHalfwayParameters",
					//	theHalfwayParameters.Ux, theHalfwayParameters.Uy, theHalfwayParameters.Uz,
					//	halfwayAngle, theHalfwayParameters.theSfactor,
					//	theHalfwayParameters.Mx, theHalfwayParameters.My, theHalfwayParameters.Mz
					//);
					////////////////////////////////////////////

					oxyde::log::printDualQuat(L"startQ", startQ);

					oxyde::log::printDualQuat(L"endQ", endQ);

					//oxyde::log::printDualQuat(L"transQ", transQ);

					oxyde::DQ::dualQuatParameters transQParameters;

					oxyde::DQ::extractDualVersorParameters(
						DUALQUAARRAY(transQ),
						transQParameters
					);

					double angleQ = 2*oxyde::math::getAngleFromCosAndSin(transQParameters.qS, transQParameters.theSin);

					//oxyde::log::printDualQuatParameters(L"transQParameters",
					//	transQParameters.Ux, transQParameters.Uy, transQParameters.Uz,
					//	angleQ, transQParameters.theSfactor,
					//	transQParameters.Mx, transQParameters.My, transQParameters.Mz
					//);

					float sanityCheckQ[8];
					oxyde::DQ::dual_Versor(angleQ,
						transQParameters.Ux, transQParameters.Uy, transQParameters.Uz,
						transQParameters.theSfactor,
						transQParameters.Mx, transQParameters.My, transQParameters.Mz,
						DUALQUAARRAY(sanityCheckQ));

					//oxyde::log::printDualQuat(L"sanityCheckQ", sanityCheckQ);

					///////////////////////////////////////////////////////////////////////
					float fromParametersQ[8];
					oxyde::DQ::dual_Versor(angleQ / 2.0,
						transQParameters.Ux, transQParameters.Uy, transQParameters.Uz,
						transQParameters.theSfactor / 2.0,
						transQParameters.Mx, transQParameters.My, transQParameters.Mz,
						DUALQUAARRAY(fromParametersQ));

					//oxyde::log::printDualQuat(L"fromParametersQ", fromParametersQ);
					///////////////////////////////////////////////////////////////////////

					//oxyde::log::printDualQuat(L"transQc", transQc);

					oxyde::DQ::dualQuatParameters transQcParameters;
					oxyde::DQ::complementDualVersorParameters(transQParameters, transQcParameters);

					//oxyde::DQ::extractDualVersorParameters(
					//	DUALQUAARRAY(transQc),
					//	transQcParameters
					//);

					double angleQc = 2 * oxyde::math::getAngleFromCosAndSin(transQcParameters.qS, transQcParameters.theSin);

					//oxyde::log::printDualQuatParameters(L"transQcParameters",
					//	transQcParameters.Ux, transQcParameters.Uy, transQcParameters.Uz,
					//	angleQc, transQcParameters.theSfactor,
					//	transQcParameters.Mx, transQcParameters.My, transQcParameters.Mz
					//);

					float sanityCheckQc[8];
					oxyde::DQ::dual_Versor(angleQc,
						transQcParameters.Ux, transQcParameters.Uy, transQcParameters.Uz,
						transQcParameters.theSfactor,
						transQcParameters.Mx, transQcParameters.My, transQcParameters.Mz,
						DUALQUAARRAY(sanityCheckQc));

					//oxyde::log::printDualQuat(L"sanityCheckQc", sanityCheckQc);

					///////////////////////////////////////////////////////////////////////
					float fromParametersQc[8];
					oxyde::DQ::dual_Versor(angleQc / 2.0,
						transQcParameters.Ux, transQcParameters.Uy, transQcParameters.Uz,
						transQcParameters.theSfactor / 2.0,
						transQcParameters.Mx, transQcParameters.My, transQcParameters.Mz,
						DUALQUAARRAY(fromParametersQc));

					//oxyde::log::printDualQuat(L"fromParametersQc", fromParametersQc);

					///////////////////////////////////////////////////////////////////////
					float zeroPoint[8];
					oxyde::DQ::point_quaternion(0., 0., 0., DUALQUAARRAY(zeroPoint));

					float transformedByQ[8];					
					oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(fromParametersQ),
						DUALQUAARRAY(zeroPoint),
						DUALQUAARRAY(transformedByQ));

					//oxyde::log::printDualQuat(L"transformedByQ", transformedByQ);

					float transformedByQc[8];
					oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(fromParametersQc),
						DUALQUAARRAY(zeroPoint),
						DUALQUAARRAY(transformedByQc));

					//oxyde::log::printDualQuat(L"transformedByQc", transformedByQc);

					float pointHalfWay[8];
					oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(fromStartToHalfQ),
						DUALQUAARRAY(zeroPoint),
						DUALQUAARRAY(pointHalfWay));

					//oxyde::log::printDualQuat(L"pointHalfWay", pointHalfWay);

					float halfwayToQ[3] = { pointHalfWay[5] - transformedByQ[5], pointHalfWay[6] - transformedByQ[6], pointHalfWay[7] - transformedByQ[7] };
					float distFromHalfwayToQ = 0.;
					oxyde::linAlg::norm(halfwayToQ, &distFromHalfwayToQ);


					float halfwayToQc[3] = { pointHalfWay[5] - transformedByQc[5], pointHalfWay[6] - transformedByQc[6], pointHalfWay[7] - transformedByQc[7] };
					float distFromHalfwayToQc = 0.;
					oxyde::linAlg::norm(halfwayToQc, &distFromHalfwayToQc);

					oxyde::log::printText(distFromHalfwayToQ > distFromHalfwayToQc ? L"transQc nearer to halfway point" : L"transQ nearer to halfway point");
					///////////////////////////////////////////////////////////////////////
					
					float transQSanityCheck[8];

					if (pair.first.first != pair.second.first) {
						if (distFromHalfwayToQ > distFromHalfwayToQc) {
							insertDualQuatKeyForTrack(theDualQuatTrackElement,
								pair.first.first, pair.second.first,
								startQ[0], startQ[1], startQ[2], startQ[3], startQ[4], startQ[5], startQ[6], startQ[7],
								angleQc, transQcParameters.Ux, transQcParameters.Uy, transQcParameters.Uz, transQcParameters.theSfactor, transQcParameters.Mx, transQcParameters.My, transQcParameters.Mz
							);
							
							oxyde::log::printDualQuatParameters(L"transQSanityCheckParameters",
								transQcParameters.Ux, transQcParameters.Uy, transQcParameters.Uz,
								angleQc, transQcParameters.theSfactor,
								transQcParameters.Mx, transQcParameters.My, transQcParameters.Mz
							);

							oxyde::DQ::dual_Versor(
								angleQc, 
								transQcParameters.Ux, transQcParameters.Uy, transQcParameters.Uz, 
								transQcParameters.theSfactor, 
								transQcParameters.Mx, transQcParameters.My, transQcParameters.Mz,
								DUALQUAARRAY(transQSanityCheck)
							);
						}
						else {
							insertDualQuatKeyForTrack(theDualQuatTrackElement,
								pair.first.first, pair.second.first,
								startQ[0], startQ[1], startQ[2], startQ[3], startQ[4], startQ[5], startQ[6], startQ[7],
								angleQ, transQParameters.Ux, transQParameters.Uy, transQParameters.Uz, transQParameters.theSfactor, transQParameters.Mx, transQParameters.My, transQParameters.Mz
							);

							oxyde::log::printDualQuatParameters(L"transQSanityCheckParameters",
								transQParameters.Ux, transQParameters.Uy, transQParameters.Uz,
								angleQ, transQParameters.theSfactor,
								transQParameters.Mx, transQParameters.My, transQParameters.Mz
							);

							oxyde::DQ::dual_Versor(
								angleQ, 
								transQParameters.Ux, transQParameters.Uy, transQParameters.Uz, 
								transQParameters.theSfactor, 
								transQParameters.Mx, transQParameters.My, transQParameters.Mz,
								DUALQUAARRAY(transQSanityCheck)
							);
						}
					}
					else {
						insertDualQuatKeyForTrack(theDualQuatTrackElement,
							pair.first.first, pair.second.first,
							startQ[0], startQ[1], startQ[2], startQ[3], startQ[4], startQ[5], startQ[6], startQ[7],
							0., 1., 0., 0., 0., 0., 0., 0.
						);

						oxyde::log::printDualQuatParameters(L"transQSanityCheckParameters",
							1., 0., 0., 0., 0., 0., 0., 0.
						);

						oxyde::DQ::dual_Versor(0., 1., 0., 0., 0., 0., 0., 0.,
							DUALQUAARRAY(transQSanityCheck)
						);
					}

					oxyde::log::printDualQuat(L"transQSanityCheck", transQSanityCheck);
					outStream << "Expand[MultiQuat[transQSanityCheck, startQ]]" << std::endl;

					std::wstring resultString(outStream.str());
					DebugOutputString(resultString.c_str());
				}

				oxyde::log::printLine();
			}

			void PRScontrollerDataExtractor::getLocalMatrixForTime(TimeValue t, double m[]) {
				Matrix3 localMatrix;
				localMatrix.IdentityMatrix();
				m_Control->GetValue(t, (void*)&localMatrix, FOREVER, CTRL_RELATIVE);

				m[0] = localMatrix.GetRow(0).x;		m[1] = localMatrix.GetRow(0).y;		m[2] = localMatrix.GetRow(0).z;		m[3] = 0.;
				m[4] = localMatrix.GetRow(1).x;		m[5] = localMatrix.GetRow(1).y;		m[6] = localMatrix.GetRow(1).z;		m[7] = 0.;
				m[8] = localMatrix.GetRow(2).x;		m[9] = localMatrix.GetRow(2).y;		m[10] = localMatrix.GetRow(2).z;	m[11] = 0.;
				m[12] = localMatrix.GetRow(3).x;	m[13] = localMatrix.GetRow(3).y;	m[14] = localMatrix.GetRow(3).z;	m[15] = 1.;
			}

			void PRScontrollerDataExtractor::getDualQuatForTime(TimeValue t, float quat[])
			{
				std::wstring outStr;
				std::wostringstream outStream(outStr);
				outStream << "__________________________________________________[" << t << "]" << std::endl;

				float eulerX = 0.0, eulerY = 0.0, eulerZ = 0.0;
				Quat q;
				if (m_rotationController) {
					auto rotationControl = std::dynamic_pointer_cast<rotationControllerDataExtractor>(m_rotationController);
					if (rotationControl) {
						rotationControl->getValueForTime(t, q);
						//rotationControl->getValueForTime(t, eulerX, eulerY, eulerZ);
						//int EulerOrder = rotationControl->getEulerAngleOrder();
						//outStream << "EulerOrder = " << EulerOrder << std::endl;

						//float eulerRotation[3] = { eulerX, eulerY, eulerZ };
						//EulerToQuat(eulerRotation, q, EulerOrder);
					}
				}

				//outStream << "rotControl = {" << eulerX << ", " << eulerY << ", " << eulerZ << "}" << std::endl;

				float rotQuat[8];
				oxyde::DQ::pure_Real_quaternion(q.w, q.x, q.y, q.z, DUALQUAARRAY(rotQuat));
				outStream << "rotQuat = {";
				outStream << rotQuat[0] << "," << rotQuat[1] << "," << rotQuat[2] << "," << rotQuat[3] << ",";
				outStream << rotQuat[4] << "," << rotQuat[5] << "," << rotQuat[6] << "," << rotQuat[7];
				outStream << "}.dualQuatUnit";
				outStream << std::endl;

				float x = 0.0, y = 0.0, z = 0.0;
				if (m_positionController) {
					auto positionControl = std::dynamic_pointer_cast<positionControllerDataExtractor>(m_positionController);
					if (positionControl) {
						positionControl->getValueForTime(t, x, y, z);
					}
				}

				outStream << "posControl = {" << x << ", " << y << ", " << z << "}" << std::endl;

				float transQuat[8];
				oxyde::DQ::translation_quaternion(x, y, z, DUALQUAARRAY(transQuat));
				outStream << "transQuat = {";
				outStream << transQuat[0] << "," << transQuat[1] << "," << transQuat[2] << "," << transQuat[3] << ",";
				outStream << transQuat[4] << "," << transQuat[5] << "," << transQuat[6] << "," << transQuat[7];
				outStream << "}.dualQuatUnit";
				outStream << std::endl;

				//oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(transQuat), DUALQUAARRAY(rotQuat), DUALQUAARRAY(quat));
				oxyde::DQ::dual_quaternion_product( DUALQUAARRAY(transQuat), DUALQUAARRAY(rotQuat),DUALQUAARRAY(quat));
				outStream << "finalQuat = {";
				outStream << quat[0] << "," << quat[1] << "," << quat[2] << "," << quat[3] << ",";
				outStream << quat[4] << "," << quat[5] << "," << quat[6] << "," << quat[7];
				outStream << "}.dualQuatUnit";
				outStream << std::endl;

				std::wstring resultString(outStream.str());
				DebugOutputString(resultString.c_str());
			}

			void PRScontrollerDataExtractor::exportController(IXMLDOMElement* theAnimationElement)
			{
				DebugPrint(L"This is a PRS Controller Extractor test\n");
				std::set<TimeValue> keyTimes = getKeyTimes();
				Matrix3 localMatrix;
				for (auto&& t : keyTimes) {
					localMatrix.IdentityMatrix();
					m_Control->GetValue(t, (void*)&localMatrix, FOREVER, CTRL_RELATIVE);

					std::wstring outStr;
					std::wostringstream outStream(outStr);
					outStream << L"	Time: " << t << std::endl;

					outStream << L"		" << localMatrix.GetRow(0).x << "   " << localMatrix.GetRow(0).y << "   " << localMatrix.GetRow(0).z << std::endl;
					outStream << L"		" << localMatrix.GetRow(1).x << "   " << localMatrix.GetRow(1).y << "   " << localMatrix.GetRow(1).z << std::endl;
					outStream << L"		" << localMatrix.GetRow(2).x << "   " << localMatrix.GetRow(2).y << "   " << localMatrix.GetRow(2).z << std::endl;
					outStream << L"		" << localMatrix.GetRow(3).x << "   " << localMatrix.GetRow(3).y << "   " << localMatrix.GetRow(3).z << std::endl;

					outStream << std::endl;

					double m[16] = {
						localMatrix.GetRow(0).x ,localMatrix.GetRow(0).y, localMatrix.GetRow(0).z, 0.,
						localMatrix.GetRow(1).x ,localMatrix.GetRow(1).y, localMatrix.GetRow(1).z, 0.,
						localMatrix.GetRow(2).x ,localMatrix.GetRow(2).y, localMatrix.GetRow(2).z, 0.,
						localMatrix.GetRow(3).x ,localMatrix.GetRow(3).y, localMatrix.GetRow(3).z, 1.
					};
					double nx, ny, nz;
					double cosTheta, sinTheta;
					double slide;
					double mx, my, mz;

					oxyde::math::getDualQuaternionParametersFromMatrix(m, cosTheta, sinTheta, nx, ny, nz, slide, mx, my, mz);

					outStream << "Normal = {" << nx << ", " << ny << ", " << nz << "}" << std::endl;
					outStream << "Cos, Sin = {" << cosTheta << ", " << sinTheta << "}" << std::endl;

					outStream << "Slide = " << slide << std::endl;
					outStream << "Axis vector m = {" << mx << ", " << my << ", " << mz << "}" << std::endl;

					std::wstring resultString(outStream.str());
					DebugOutputString(resultString.c_str());
				}
				keyTimes.clear();

				if (m_positionController)m_positionController->exportController(theAnimationElement);
				if (m_rotationController)m_rotationController->exportController(theAnimationElement);
				if (m_scaleController)m_scaleController->exportController(theAnimationElement);
				buildTrack(theAnimationElement);
			}

			Class_ID PRScontrollerDataExtractor::getClass_ID()
			{
				return Class_ID(PRS_CONTROL_CLASS_ID, 0x0);
			}

			void PRScontrollerDataExtractor::registerMe()
			{
				registerFactory(PRScontrollerDataExtractor::buildPRScontrollerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr PRScontrollerDataExtractor::buildPRScontrollerDataExtractor(Control * theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(
					std::make_shared<PRScontrollerDataExtractor>(PRScontrollerDataExtractor(theControl))
					);
			}

			PRScontrollerDataExtractor::PRScontrollerDataExtractor(Control* theControl) : matrixControllerDataExtractor(theControl)
			{
				if (!m_Control->IsLeaf()) {
					m_positionController = buildExtractor(m_Control->GetPositionController());
					m_rotationController = buildExtractor(m_Control->GetRotationController());
					m_scaleController = buildExtractor(m_Control->GetScaleController());
				}
			}

			IKeyControl * PRScontrollerDataExtractor::GetKeyControlInterfacePointer()
			{
				return GetKeyControlInterface(m_Control);
			}

			std::set<TimeValue> PRScontrollerDataExtractor::getKeyTimes()
			{
				std::set<TimeValue> keyTimes;

				std::shared_ptr<XYZControllerDataExtractor>posController_ptr = std::dynamic_pointer_cast<XYZControllerDataExtractor>(m_positionController);
				if (posController_ptr)
				{
					std::set<TimeValue> tempSet = posController_ptr->getKeyTimes();
					for (auto&& t : tempSet) {
						keyTimes.insert(t);
					}
					tempSet.clear();
				}

				std::shared_ptr<XYZControllerDataExtractor>rotController_ptr = std::dynamic_pointer_cast<XYZControllerDataExtractor>(m_rotationController);
				if (rotController_ptr)
				{
					std::set<TimeValue> tempSet = rotController_ptr->getKeyTimes();
					for (auto&& t : tempSet) {
						keyTimes.insert(t);
					}
					tempSet.clear();
				}

				// Not treating Scale Transform this time. So I don't care about scale keyframes.

				return keyTimes;
			}

			std::set<TimeValue> XYZControllerDataExtractor::getKeyTimes()
			{
				std::set<TimeValue> keyTimes;

				std::shared_ptr<keyControllerDataExtractor>XKeyController_ptr = std::dynamic_pointer_cast<keyControllerDataExtractor>(m_XController);
				if (XKeyController_ptr)
				{
					//m_XController->exportController();
					std::set<TimeValue> tempSet = XKeyController_ptr->getKeyTimes();
					for (auto&& t : tempSet) {
						keyTimes.insert(t);
					}
					tempSet.clear();
				}

				std::shared_ptr<keyControllerDataExtractor>YKeyController_ptr = std::dynamic_pointer_cast<keyControllerDataExtractor>(m_YController);
				if (YKeyController_ptr)
				{
					//m_XController->exportController();
					std::set<TimeValue> tempSet = YKeyController_ptr->getKeyTimes();
					for (auto&& t : tempSet) {
						keyTimes.insert(t);
					}
					tempSet.clear();
				}

				std::shared_ptr<keyControllerDataExtractor>ZKeyController_ptr = std::dynamic_pointer_cast<keyControllerDataExtractor>(m_ZController);
				if (ZKeyController_ptr)
				{
					//m_XController->exportController();
					std::set<TimeValue> tempSet = ZKeyController_ptr->getKeyTimes();
					for (auto&& t : tempSet) {
						keyTimes.insert(t);
					}
					tempSet.clear();
				}
				return keyTimes;
			}

			void XYZControllerDataExtractor::getValueForTime(TimeValue theTime, float & x, float & y, float & z)
			{
				Point3 *p = new Point3();
				m_Control->GetValue(theTime, (void*)p, FOREVER, CTRL_ABSOLUTE);
				x = p->x;
				y = p->y;
				z = p->z;
				delete p;
			}

			void XYZControllerDataExtractor::getValueForTime(TimeValue theTime, Quat & q)
			{
				m_Control->GetValue(theTime, (void*)(&q), FOREVER, CTRL_ABSOLUTE);
			}

			//Class_ID XYZControllerDataExtractor::getClass_ID() 
			//{
			//}

			//void XYZControllerDataExtractor::registerMe() {
			//	
			//}

			//controllerDataExtractor_ptr XYZControllerDataExtractor::buildXYZControllerDataExtractor(Control* theControl) 
			//{
			//	return std::dynamic_pointer_cast<controllerDataExtractor>(
			//		std::make_shared<XYZControllerDataExtractor>(XYZControllerDataExtractor(theControl))
			//		);
			//}

			XYZControllerDataExtractor::XYZControllerDataExtractor(Control *theControl) : controllerDataExtractor(theControl)
			{
				if (!m_Control->IsLeaf()) {
					DebugPrint(L"XYZControllerDataExtractor is NOT leaf\n");
					m_XController = buildExtractor(m_Control->GetXController());
					m_YController = buildExtractor(m_Control->GetYController());
					m_ZController = buildExtractor(m_Control->GetZController());
				}
				else {
					DebugPrint(L"XYZControllerDataExtractor is leaf\n");
				}
			}

			void positionControllerDataExtractor::exportController(IXMLDOMElement* theAnimationElement)
			{
				DebugPrint(L"This is a position Controller Extractor\n");
				std::set<TimeValue> keyTimes = getKeyTimes();
				DebugPrint(L"Total Keys for position controller: %i\n", keyTimes.size());
				keyTimes.clear();
			}

			Class_ID positionControllerDataExtractor::getClass_ID()
			{
				return Class_ID(0x118F7E02, 0xFFEE238A);
			}

			void positionControllerDataExtractor::registerMe()
			{
				registerFactory(positionControllerDataExtractor::buildPositionControllerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr positionControllerDataExtractor::buildPositionControllerDataExtractor(Control* theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(
					std::make_shared<positionControllerDataExtractor>(positionControllerDataExtractor(theControl))
					);
			}

			positionControllerDataExtractor::positionControllerDataExtractor(Control *theControl) : XYZControllerDataExtractor(theControl){}

			void rotationControllerDataExtractor::exportController(IXMLDOMElement* theAnimationElement)
			{
				DebugPrint(L"This is a ROTATION Controller Extractor\n");
				std::set<TimeValue> keyTimes = getKeyTimes();
				DebugPrint(L"Total Keys for ROTATION controller: %i\n", keyTimes.size());
				keyTimes.clear();
			}

			int rotationControllerDataExtractor::getEulerAngleOrder()
			{
				IEulerControl* eulerControl = (IEulerControl*)m_Control->GetInterface(I_EULERCTRL);
				return eulerControl->GetOrder();
			}

			Class_ID rotationControllerDataExtractor::getClass_ID()
			{
				return Class_ID(EULER_CONTROL_CLASS_ID, 0x0);
			}

			void rotationControllerDataExtractor::registerMe()
			{
				registerFactory(rotationControllerDataExtractor::buildRotationControllerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr rotationControllerDataExtractor::buildRotationControllerDataExtractor(Control* theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(
					std::make_shared<rotationControllerDataExtractor>(rotationControllerDataExtractor(theControl))
					);
			}

			rotationControllerDataExtractor::rotationControllerDataExtractor(Control *theControl) : XYZControllerDataExtractor(theControl) {}
			
			void bipSlaveControllerDataExtractor::getLocalMatrixForTime(TimeValue t, double m[]) {
				Matrix3 theMatrix;
				Matrix3 localMatrix;
				Matrix3 ParentMatrix;

				theMatrix.IdentityMatrix();
				localMatrix.IdentityMatrix();

				ParentMatrix = m_bipNode->GetParentTM(t);
				m_Control->GetValue(t, (void*)&theMatrix, FOREVER, CTRL_RELATIVE);
				localMatrix = theMatrix * Inverse(ParentMatrix);

				m[0] = localMatrix.GetRow(0).x;		m[1] = localMatrix.GetRow(0).y;		m[2] = localMatrix.GetRow(0).z;		m[3] = 0.;
				m[4] = localMatrix.GetRow(1).x;		m[5] = localMatrix.GetRow(1).y;		m[6] = localMatrix.GetRow(1).z;		m[7] = 0.;
				m[8] = localMatrix.GetRow(2).x;		m[9] = localMatrix.GetRow(2).y;		m[10] = localMatrix.GetRow(2).z;	m[11] = 0.;
				m[12] = localMatrix.GetRow(3).x;	m[13] = localMatrix.GetRow(3).y;	m[14] = localMatrix.GetRow(3).z;	m[15] = 1.;
			}

			void bipSlaveControllerDataExtractor::exportController(IXMLDOMElement* theAnimationElement) {
				DebugPrint(L"This is a bipSlave extractor... ClassID: 0x%lX : 0x%lX\n", m_Control->ClassID().PartA(), m_Control->ClassID().PartB());
				std::set<TimeValue> keyTimes = getKeyTimes();
				Matrix3 theMatrix;
				Matrix3 localMatrix;
				Matrix3 ParentMatrix;
				for (auto&& t : keyTimes) {
					//std::array<double, 16> &&mArray = getLocalMatrixForTime(t);
					theMatrix.IdentityMatrix();
					localMatrix.IdentityMatrix();

					ParentMatrix = m_bipNode->GetParentTM(t);
					m_Control->GetValue(t, (void*)&theMatrix, FOREVER, CTRL_RELATIVE);
					localMatrix = theMatrix * Inverse(ParentMatrix);

					std::wstring outStr;
					std::wostringstream outStream(outStr);
					outStream << L"	Time: " << t << std::endl;

					outStream << L"		" << localMatrix.GetRow(0).x << "   " << localMatrix.GetRow(0).y << "   " << localMatrix.GetRow(0).z << std::endl;
					outStream << L"		" << localMatrix.GetRow(1).x << "   " << localMatrix.GetRow(1).y << "   " << localMatrix.GetRow(1).z << std::endl;
					outStream << L"		" << localMatrix.GetRow(2).x << "   " << localMatrix.GetRow(2).y << "   " << localMatrix.GetRow(2).z << std::endl;
					outStream << L"		" << localMatrix.GetRow(3).x << "   " << localMatrix.GetRow(3).y << "   " << localMatrix.GetRow(3).z << std::endl;

					outStream << std::endl;

					double m[16] = {
						localMatrix.GetRow(0).x ,localMatrix.GetRow(0).y, localMatrix.GetRow(0).z, 0.,
						localMatrix.GetRow(1).x ,localMatrix.GetRow(1).y, localMatrix.GetRow(1).z, 0.,
						localMatrix.GetRow(2).x ,localMatrix.GetRow(2).y, localMatrix.GetRow(2).z, 0.,
						localMatrix.GetRow(3).x ,localMatrix.GetRow(3).y, localMatrix.GetRow(3).z, 1.
					};
					double nx, ny, nz;
					double cosTheta, sinTheta;
					double slide;
					double mx, my, mz;

					oxyde::math::getDualQuaternionParametersFromMatrix(m, cosTheta, sinTheta, nx, ny, nz, slide, mx, my, mz);

					outStream << "Normal = {" << nx << ", " << ny << ", " << nz << "}" << std::endl;
					outStream << "Cos, Sin = {" << cosTheta << ", " << sinTheta << "}" << std::endl;

					outStream << "Slide = " << slide << std::endl;
					outStream << "Axis vector m = {" << mx << ", " << my << ", " << mz << "}" << std::endl;

					std::wstring resultString(outStream.str());
					DebugOutputString(resultString.c_str());


				}
				keyTimes.clear();
				buildTrack(theAnimationElement);
				return;
			}

			void bipSlaveControllerDataExtractor::getDualQuatForTime(TimeValue t, float quat[])
			{
				oxyde::log::printLine();

				IBipMaster12* theBipMaster = NULL;
				theBipMaster = (IBipMaster12*)m_Control->GetInterface(IBipMaster12::I_BIPMASTER12);
				if (theBipMaster != NULL) {
					Quat q = theBipMaster->GetBipedRot(t, m_bipNode, TRUE);

					float rotQuat[8];
					oxyde::DQ::pure_Real_quaternion(q.w, q.x, q.y, q.z, DUALQUAARRAY(rotQuat));
					oxyde::log::printDualQuat(L"rotFromBip", rotQuat);

					Point3 pGlobal = theBipMaster->GetBipedPos(t, m_bipNode);
					Matrix3 parentTM= m_bipNode->GetParentTM(t);
					parentTM.Invert();
					Point3 p = parentTM.PointTransform(pGlobal);

					float transQuat[8];
					oxyde::DQ::translation_quaternion(p.x, p.y, p.z, DUALQUAARRAY(transQuat));
					oxyde::log::printDualQuat(L"transFromBip", transQuat);

					oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(transQuat), DUALQUAARRAY(rotQuat), DUALQUAARRAY(quat));
					oxyde::log::printDualQuat(L"finalFromBip", quat);
				}
				else {
					double m[16];
					getLocalMatrixForTime(t, m);
					oxyde::math::getDualQuaternionFromMatrix(m, DUALQUAARRAY(quat));
				}
			}

			Class_ID bipSlaveControllerDataExtractor::getClass_ID() {
				return BIPSLAVE_CONTROL_CLASS_ID;
			}

			void bipSlaveControllerDataExtractor::registerMe() {
				registerFactory(bipSlaveControllerDataExtractor::buildBipSlaveControllerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr bipSlaveControllerDataExtractor::buildBipSlaveControllerDataExtractor(Control * theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(std::make_shared<bipSlaveControllerDataExtractor>(bipSlaveControllerDataExtractor(theControl)));
			}

			IKeyControl * bipSlaveControllerDataExtractor::GetKeyControlInterfacePointer()
			{
				return GetKeyControlInterface(m_Control);
			}

			void bipBodyControllerDataExtractor::exportController(IXMLDOMElement* theAnimationElement) {
				DebugPrint(L"This is a bipBody extractor... ClassID: 0x%lX : 0x%lX\n", m_Control->ClassID().PartA(), m_Control->ClassID().PartB());

				if (!m_Control->IsLeaf()) {
					DebugPrint(L"bipBodyControllerDataExtractor is NOT leaf\n");
				}
				else {
					DebugPrint(L"bipBodyControllerDataExtractor is leaf\n");
				}

				if (m_bipSlaveController)m_bipSlaveController->exportController(theAnimationElement);

				return;
			}

			Class_ID bipBodyControllerDataExtractor::getClass_ID() {
				return BIPBODY_CONTROL_CLASS_ID;
			}

			void bipBodyControllerDataExtractor::registerMe() {
				registerFactory(bipBodyControllerDataExtractor::buildBipBodyControllerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr bipBodyControllerDataExtractor::buildBipBodyControllerDataExtractor(Control * theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(std::make_shared<bipBodyControllerDataExtractor>(bipBodyControllerDataExtractor(theControl)));
			}

			bipBodyControllerDataExtractor::bipBodyControllerDataExtractor(Control *theControl) :controllerDataExtractor(theControl)
			{
				Control* subControl = theControl->GetRotationController();
				if (subControl != nullptr) {
					m_bipSlaveController = controllerDataExtractor::buildExtractor(subControl);
				}
			}

		}
	}
}

namespace {
	class factoryInstaller {
	public:
		factoryInstaller() {
			oxyde::exporter::controller::floatControllerDataExtractor::registerMe();
			oxyde::exporter::controller::defaultControllerDataExtractor::registerMe();
			oxyde::exporter::controller::PRScontrollerDataExtractor::registerMe();
			oxyde::exporter::controller::positionControllerDataExtractor::registerMe();
			oxyde::exporter::controller::rotationControllerDataExtractor::registerMe();
			oxyde::exporter::controller::bipSlaveControllerDataExtractor::registerMe();
			oxyde::exporter::controller::bipBodyControllerDataExtractor::registerMe();
		}
	};

	factoryInstaller theFactoryInstaller;
}
