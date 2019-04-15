#include <max.h>
#include <control.h>
#include <istdplug.h>
#include <array>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <cmath>
#include "matrixUtility.h"
#include "linAlg.h"
#include "dualQuaternionFunctions.h"
#include "debugLog.h"
#include "timeControllerDataExtraction.h"
#include "skinDataExtraction.h"
#include "xmlDocumentPRS.h"
#include "xmlDocumentLookAt.h"

namespace oxyde {
	namespace exporter {
		namespace controller {

			Class_ID defaultControllerDataExtractor::getClass_ID() {
				return Class_ID();
			}

			void defaultControllerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement) {
				DebugPrint(L"This is a default extractor test... ClassID: 0x%lX : 0x%lX\n", m_Control->ClassID().PartA(), m_Control->ClassID().PartB());
			}

			void defaultControllerDataExtractor::registerMe() {
				registerFactory(defaultControllerDataExtractor::buildDefaultControllerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr defaultControllerDataExtractor::buildDefaultControllerDataExtractor(Control* theControl) {
				return std::dynamic_pointer_cast<controllerDataExtractor>(std::make_shared<defaultControllerDataExtractor>(defaultControllerDataExtractor(theControl)));
			}

			//float floatControllerDataExtractor::getValueForTime(TimeValue theTime) {
			//	float value = 0.0;
			//	m_Control->GetValue(theTime, (void*)&value, FOREVER, CTRL_ABSOLUTE);
			//	return 0.0;
			//}

			void floatControllerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement) {
				//DebugPrint(L"This is a float extractor... ClassID: 0x%lX : 0x%lX\n", m_Control->ClassID().PartA(), m_Control->ClassID().PartB());
				IKeyControl* theKeyControl = GetKeyControlInterface(m_Control);

				if (theKeyControl != NULL) {
					int numKeys = theKeyControl->GetNumKeys();

					if (numKeys > 0) {
						oxyde::exporter::XML::oxyBezierTrackElementPtr theBezierTrackElement = std::make_shared<oxyde::exporter::XML::oxyBezierTrackElement>(theAnimationElement, numKeys - 1);

						IKey* theKey = (IKey*)(new char[theKeyControl->GetKeySize()]);
						bool firstFrame = true;

						float startTime;
						float outTangent;
						float outTangentLength;
						float startValue;

						float endTime;
						float inTangent;
						float inTangentLength;
						float endValue;

						for (int i = 0; i < numKeys; i++) {
							theKeyControl->GetKey(i, theKey);
							IBezFloatKey* theBezFloatKey = static_cast<IBezFloatKey*>(theKey);

							endTime = theBezFloatKey->time;
							inTangent = theBezFloatKey->intan;
							inTangentLength = theBezFloatKey->inLength;
							endValue = theBezFloatKey->val;

							if (!firstFrame) {
								float b0 = startValue;
								float b1 = outTangent*outTangentLength*(endTime - startTime) + startValue;
								float b2 = endValue + inTangent*inTangentLength*(endTime - startTime);
								float b3 = endValue;

								float B0, B1, B2, B3;

								int outTanType = GetOutTanType(theBezFloatKey->flags);
								int inTanType = GetInTanType(theBezFloatKey->flags);


								//Smooth - Smooth			t*(3 * b1 + t*(-6 * b1 + 3 * b2 + (3 * b1 - 3 * b2 + b3)*t)) + b0*(1 + t*(-3 + (3 - t)*t))
								if ((outTanType != BEZKEY_LINEAR) && (inTanType != BEZKEY_LINEAR)) {
									B0 = b0;
									B1 = -3 * b0 + 3 * b1;
									B2 = 3 * b0 - 6 * b1 + 3 * b2;
									B3 = -b0 + 3 * b1 - 3 * b2 + b3;
								}

								//Linear - Smooth			b0 + t*(-2 * b0 + 2 * b1 + t*(2 * b0 - 4 * b1 + 2 * b3 + (-b0 + 2 * b1 - b3)*t))
								if ((outTanType == BEZKEY_LINEAR) && (inTanType != BEZKEY_LINEAR)) {
									B0 = b0;
									B1 = -2 * b0 + 2 * b1;
									B2 = 2 * b0 - 4 * b1 + 2 * b3;
									B3 = -b0 + 2 * b1 - b3;
								}

								//Smooth - Linear			b0 + t*(-b0 + b3 + t*(-b0 + 2 * b2 - b3 + (b0 - 2 * b2 + b3)*t))
								if ((outTanType != BEZKEY_LINEAR) && (inTanType == BEZKEY_LINEAR)) {
									B0 = b0;
									B1 = -b0 + b3;
									B2 = -b0 + 2 * b2 - b3;
									B3 = b0 - 2 * b2 + b3;
								}

								//Linear - Linear			b0 - b0*t + b3*t
								if ((outTanType == BEZKEY_LINEAR) && (inTanType == BEZKEY_LINEAR)) {
									B0 = b0;
									B1 = -b0 + b3;
									B2 = 0.;
									B3 = 0.;
								}

								oxyde::exporter::XML::oxyBezierKeyElementPtr theBezierKeyElement = std::make_shared<oxyde::exporter::XML::oxyBezierKeyElement>(theBezierTrackElement, startTime, endTime, B3, B2, B1, B0);

							}

							startTime = theBezFloatKey->time;
							outTangent = theBezFloatKey->outtan;
							outTangentLength = theBezFloatKey->outLength;
							startValue = theBezFloatKey->val;

							firstFrame = false;
						}
					}
					else {
						oxyde::exporter::XML::oxyBezierTrackElementPtr theBezierTrackElement = std::make_shared<oxyde::exporter::XML::oxyBezierTrackElement>(theAnimationElement, numKeys);
					}
				}


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

			void matrixControllerDataExtractor::buildTrack(oxyde::exporter::XML::oxyAnimationElementPtr theAnimationElement)
			{
				using dualQuat = std::array<double, 8>;
				using dualQuatKey = std::pair<TimeValue, dualQuat>;

				std::set<TimeValue> keyTimes = getKeyTimes();
				if (keyTimes.size() == 0) {
					keyTimes.insert(TimeValue(0));
				}
				std::vector<std::pair<dualQuatKey, dualQuatKey>> intervals;

				dualQuatKey formerValue;

				double formerMatrix[16];

				for (auto timeIntervalEnd = keyTimes.begin(); timeIntervalEnd != keyTimes.end(); timeIntervalEnd++) {

					std::wstring outStr;
					std::wostringstream outStream(outStr);

					outStream << L"		" << *timeIntervalEnd << std::endl;

					float quat[8];

					getDualQuatForTime(*timeIntervalEnd, quat);

					double m[16];
					getLocalMatrixForTime(*timeIntervalEnd, m);

					oxyde::log::printLine();
					float mFloat[16];

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

					DUALQUAVAR(r);

					oxyde::DQ::dual_Versor(angle,
						nx, ny, nz,
						slide,
						mx, my, mz,
						DUALQUACOMP(r));

					oxyde::log::printDualQuat(L"r", DUALQUACOMP(r));

					dualQuat currentQuat = { DUALQUACOMP(r) };
					dualQuatKey currentKey(*timeIntervalEnd, currentQuat);

					oxyde::log::printDualQuat(L"currentQuat", DUALQUAARRAY(currentQuat));
					oxyde::log::printLine();

					std::wstring resultString(outStream.str());
					DebugOutputString(resultString.c_str());

					if (timeIntervalEnd != keyTimes.begin()) {

						oxyde::log::printNamedParameter(L"formerTime", formerValue.first);
						oxyde::log::printNamedParameter(L"CurrentTime", currentKey.first);

						float angleForPoint = 0;
						float quatAngle = 0;

						float startQ[8] = { formerValue.second[0], formerValue.second[1], formerValue.second[2], formerValue.second[3],
							formerValue.second[4],formerValue.second[5], formerValue.second[6], formerValue.second[7] };

						float endQ[8] = { currentKey.second[0], currentKey.second[1], currentKey.second[2],currentKey.second[3],
							currentKey.second[4], currentKey.second[5], currentKey.second[6], currentKey.second[7] };

						float transQ[8] = { 0., 0. , 0. , 0. , 0. , 0. , 0. , 0. };

						oxyde::DQ::transformFromSourceToDestinationAxis(
							startQ[0], startQ[1], startQ[2], startQ[3], startQ[4], startQ[5], startQ[6], startQ[7],
							endQ[0], endQ[1], endQ[2], endQ[3], endQ[4], endQ[5], endQ[6], endQ[7],
							transQ[0], transQ[1], transQ[2], transQ[3], transQ[4], transQ[5], transQ[6], transQ[7]
						);

						oxyde::log::printDualQuat(L"startQ", startQ);
						oxyde::log::printDualQuat(L"endQ", endQ);
						oxyde::log::printDualQuat(L"transQ", transQ);

						oxyde::DQ::dualQuatParameters transQParameters;

						oxyde::DQ::extractDualVersorParameters(
							DUALQUAARRAY(transQ),
							transQParameters
						);

						double angleQ = 2 * oxyde::math::getAngleFromCosAndSin(transQParameters.qS, transQParameters.theSin);
						double complementAngleQ = 2 * oxyde::math::getAngleFromCosAndSin(-transQParameters.qS, -transQParameters.theSin);

						oxyde::log::printNamedParameter(L"angleQ", float(angleQ));
						oxyde::log::printNamedParameter(L"complementAngleQ", float(complementAngleQ));

						double lm[16];

						double inverseFormerMatrix[16];
						oxyde::math::invertMatrix(formerMatrix, inverseFormerMatrix);

						float formerMatrixFloat[16] = { formerMatrix[0], formerMatrix[1], formerMatrix[2], formerMatrix[3],
							formerMatrix[4], formerMatrix[5], formerMatrix[6], formerMatrix[7],
							formerMatrix[8], formerMatrix[9], formerMatrix[10], formerMatrix[11],
							formerMatrix[12], formerMatrix[13], formerMatrix[14], formerMatrix[15] };

						float inverseFormerMatrixFloat[16] = { inverseFormerMatrix[0], inverseFormerMatrix[1], inverseFormerMatrix[2], inverseFormerMatrix[3],
							inverseFormerMatrix[4], inverseFormerMatrix[5], inverseFormerMatrix[6], inverseFormerMatrix[7],
							inverseFormerMatrix[8], inverseFormerMatrix[9], inverseFormerMatrix[10], inverseFormerMatrix[11],
							inverseFormerMatrix[12], inverseFormerMatrix[13], inverseFormerMatrix[14], inverseFormerMatrix[15] };

						oxyde::log::printMatrix(L"formerMatrix", formerMatrixFloat);
						oxyde::log::printMatrix(L"inverseFormerMatrix", inverseFormerMatrixFloat);

						oxyde::log::printLine();
						oxyde::log::printText(L"{");

						double angleDeviation = 0;
						double complementAngleDeviation = 0;

						for (int i = 0; i < 11; i++) {
							float alpha = float(i) / 10.0;
							TimeValue iTime = formerValue.first*(1 - alpha) + currentKey.first*(alpha);
							double interpolatedAngle = angleQ*alpha;
							double interpolatedComplementAngle = complementAngleQ*alpha;

							getLocalMatrixForTime(iTime, lm);

							float matrixAtHalfTime[16] = { lm[0], lm[1], lm[2], lm[3], lm[4], lm[5], lm[6], lm[7], lm[8], lm[9], lm[10], lm[11], lm[12], lm[13], lm[14], lm[15] };

							oxyde::log::printMatrix(L"lm", matrixAtHalfTime);

							//
							double intervalMatrix[16];
							oxyde::math::multiplyMatrices(lm, inverseFormerMatrix, intervalMatrix);

							float intervalMatrixFloat[16] = { intervalMatrix[0], intervalMatrix[1], intervalMatrix[2], intervalMatrix[3],
								intervalMatrix[4], intervalMatrix[5], intervalMatrix[6], intervalMatrix[7],
								intervalMatrix[8], intervalMatrix[9], intervalMatrix[10], intervalMatrix[11],
								intervalMatrix[12], intervalMatrix[13], intervalMatrix[14], intervalMatrix[15] };

							oxyde::log::printMatrix(L"intervalMatrix", intervalMatrixFloat);

							float px = intervalMatrix[12], py = intervalMatrix[13], pz = intervalMatrix[14];

							oxyde::log::printPointInSpace(L"pPoint", px, py, pz);
							oxyde::DQ::getAngleForPointAroundQuatAxis(DUALQUAARRAY(transQ), px, py, pz, angleForPoint, quatAngle);

							oxyde::log::printNamedParameter(L"angleForPoint", angleForPoint);
							oxyde::log::printNamedParameter(L"interpolatedAngle", float(interpolatedAngle));
							oxyde::log::printNamedParameter(L"interpolatedComplementAngle", float(interpolatedComplementAngle));

							angleDeviation += std::abs(angleForPoint - interpolatedAngle);
							complementAngleDeviation += std::abs(angleForPoint - interpolatedComplementAngle);
						}
						oxyde::log::printText(L"}");
						oxyde::log::printLine();

						//
						if (std::fabs(transQ[0] - 1.0) > 0.0009) {

							bool complementQuat = angleDeviation > complementAngleDeviation;

							if (complementQuat) {
								oxyde::log::printText(L"complementQuat");
								currentKey.second[0] = -currentKey.second[0];
								currentKey.second[1] = -currentKey.second[1];
								currentKey.second[2] = -currentKey.second[2];
								currentKey.second[3] = -currentKey.second[3];
								currentKey.second[4] = -currentKey.second[4];
								currentKey.second[5] = -currentKey.second[5];
								currentKey.second[6] = -currentKey.second[6];
								currentKey.second[7] = -currentKey.second[7];
							}
						}

						intervals.push_back(std::pair<dualQuatKey, dualQuatKey>(formerValue, currentKey));

					}
					formerValue = currentKey;
					formerMatrix[0] = m[0];
					formerMatrix[1] = m[1];
					formerMatrix[2] = m[2];
					formerMatrix[3] = m[3];
					formerMatrix[4] = m[4];
					formerMatrix[5] = m[5];
					formerMatrix[6] = m[6];
					formerMatrix[7] = m[7];
					formerMatrix[8] = m[8];
					formerMatrix[9] = m[9];
					formerMatrix[10] = m[10];
					formerMatrix[11] = m[11];
					formerMatrix[12] = m[12];
					formerMatrix[13] = m[13];
					formerMatrix[14] = m[14];
					formerMatrix[15] = m[15];
				}

				if (keyTimes.size() == 1) {
					intervals.push_back(std::pair<dualQuatKey, dualQuatKey>(formerValue, formerValue));
				}

				oxyde::log::printLine();

				oxyde::exporter::XML::oxyKeyframeElementPtr theKeyframeElement = std::make_shared<oxyde::exporter::XML::oxyKeyframeElement>(theAnimationElement);
				oxyde::exporter::XML::oxyDualQuatTrackElementPtr theDualQuatTrackElement = std::make_shared<oxyde::exporter::XML::oxyDualQuatTrackElement>(theKeyframeElement, intervals.size());
				//IXMLDOMElement* theDualQuatTrackElement = insertDualQuatTrackForAnimation( theAnimationElement, intervals.size());

				for (auto &&pair : intervals) {
					std::wstring outStr;
					std::wostringstream outStream(outStr);

					oxyde::log::printLine();
					outStream << L"		" << pair.first.first << "  :  " << pair.second.first << std::endl;

					float startQ[8] = { pair.first.second[0], pair.first.second[1], pair.first.second[2], pair.first.second[3],
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

					oxyde::log::printDualQuat(L"halfQ", halfQ);

					float fromStartToHalfQ[8];
					oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(startQ), DUALQUAARRAY(halfQ), DUALQUAARRAY(fromStartToHalfQ));

					oxyde::log::printDualQuat(L"fromStartToHalfQ", fromStartToHalfQ);

					oxyde::DQ::dualQuatParameters theHalfwayParameters;

					oxyde::DQ::extractDualVersorParameters(
						DUALQUAARRAY(fromStartToHalfQ),
						theHalfwayParameters
					);

					double halfwayAngle = 2 * oxyde::math::getAngleFromCosAndSin(theHalfwayParameters.qS, theHalfwayParameters.theSin);

					////////////////////////////////////////////

					oxyde::log::printDualQuat(L"startQ", startQ);

					oxyde::log::printDualQuat(L"endQ", endQ);

					oxyde::DQ::dualQuatParameters transQParameters;

					oxyde::DQ::extractDualVersorParameters(
						DUALQUAARRAY(transQ),
						transQParameters
					);

					double angleQ = 2 * oxyde::math::getAngleFromCosAndSin(transQParameters.qS, transQParameters.theSin);

					float sanityCheckQ[8];
					oxyde::DQ::dual_Versor(angleQ,
						transQParameters.Ux, transQParameters.Uy, transQParameters.Uz,
						transQParameters.theSfactor,
						transQParameters.Mx, transQParameters.My, transQParameters.Mz,
						DUALQUAARRAY(sanityCheckQ));


					///////////////////////////////////////////////////////////////////////
					float fromParametersQ[8];
					oxyde::DQ::dual_Versor(angleQ / 2.0,
						transQParameters.Ux, transQParameters.Uy, transQParameters.Uz,
						transQParameters.theSfactor / 2.0,
						transQParameters.Mx, transQParameters.My, transQParameters.Mz,
						DUALQUAARRAY(fromParametersQ));

					///////////////////////////////////////////////////////////////////////

					oxyde::DQ::dualQuatParameters transQcParameters;
					oxyde::DQ::complementDualVersorParameters(transQParameters, transQcParameters);


					double angleQc = 2 * oxyde::math::getAngleFromCosAndSin(transQcParameters.qS, transQcParameters.theSin);

					float sanityCheckQc[8];
					oxyde::DQ::dual_Versor(angleQc,
						transQcParameters.Ux, transQcParameters.Uy, transQcParameters.Uz,
						transQcParameters.theSfactor,
						transQcParameters.Mx, transQcParameters.My, transQcParameters.Mz,
						DUALQUAARRAY(sanityCheckQc));


					///////////////////////////////////////////////////////////////////////
					float fromParametersQc[8];
					oxyde::DQ::dual_Versor(angleQc / 2.0,
						transQcParameters.Ux, transQcParameters.Uy, transQcParameters.Uz,
						transQcParameters.theSfactor / 2.0,
						transQcParameters.Mx, transQcParameters.My, transQcParameters.Mz,
						DUALQUAARRAY(fromParametersQc));

					///////////////////////////////////////////////////////////////////////
					float zeroPoint[8];
					oxyde::DQ::point_quaternion(0., 0., 0., DUALQUAARRAY(zeroPoint));

					float transformedByQ[8];
					oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(fromParametersQ),
						DUALQUAARRAY(zeroPoint),
						DUALQUAARRAY(transformedByQ));

					float transformedByQc[8];
					oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(fromParametersQc),
						DUALQUAARRAY(zeroPoint),
						DUALQUAARRAY(transformedByQc));

					float pointHalfWay[8];
					oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(fromStartToHalfQ),
						DUALQUAARRAY(zeroPoint),
						DUALQUAARRAY(pointHalfWay));

					oxyde::log::printDualQuat(L"pointHalfWay", pointHalfWay);

					float halfwayToQ[3] = { pointHalfWay[5] - transformedByQ[5], pointHalfWay[6] - transformedByQ[6], pointHalfWay[7] - transformedByQ[7] };
					float distFromHalfwayToQ = 0.;
					oxyde::linAlg::norm(halfwayToQ, &distFromHalfwayToQ);


					float halfwayToQc[3] = { pointHalfWay[5] - transformedByQc[5], pointHalfWay[6] - transformedByQc[6], pointHalfWay[7] - transformedByQc[7] };
					float distFromHalfwayToQc = 0.;
					oxyde::linAlg::norm(halfwayToQc, &distFromHalfwayToQc);

					oxyde::log::printText(distFromHalfwayToQ > distFromHalfwayToQc ? L"transQc nearer to halfway point" : L"transQ nearer to halfway point");
					///////////////////////////////////////////////////////////////////////

					///Some code for skin pose correction
					float angleInterp = 0;
					float UxInterp = 1, UyInterp = 0, UzInterp = 0;
					float sInterp = 0;
					float MxInterp = 0, MyInterp = 0, MzInterp = 0;


					float transQSanityCheck[8];

					if (pair.first.first != pair.second.first) {

						//insertDualQuatKeyForTrack(theDualQuatTrackElement,
						//	pair.first.first, pair.second.first,
						//	startQ[0], startQ[1], startQ[2], startQ[3], startQ[4], startQ[5], startQ[6], startQ[7],
						//	angleQ, transQParameters.Ux, transQParameters.Uy, transQParameters.Uz, transQParameters.theSfactor, transQParameters.Mx, transQParameters.My, transQParameters.Mz
						//);

						//oxyde::exporter::XML::oxyDualQuatTrackElementPtr theDualQuatTrackElement
						if ((pair.second.first - pair.first.first) <= GetTicksPerFrame()) {
							if (angleQ > pi) {
								angleQ = angleQ - 2 * pi;
							}
						}

						oxyde::exporter::XML::oxyDualQuatKeyElementPtr theKey = oxyde::exporter::XML::oxyDualQuatKeyElement::createDualQuatKey(theDualQuatTrackElement,
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

						angleInterp = angleQ;
						UxInterp = transQParameters.Ux, UyInterp = transQParameters.Uy, UzInterp = transQParameters.Uz;
						sInterp = transQParameters.theSfactor;
						MxInterp = transQParameters.Mx, MyInterp = transQParameters.My, MzInterp = transQParameters.Mz;

					}
					else {
						oxyde::exporter::XML::oxyDualQuatKeyElementPtr theKey = oxyde::exporter::XML::oxyDualQuatKeyElement::createDualQuatKey(theDualQuatTrackElement,
							pair.first.first, pair.second.first,
							startQ[0], startQ[1], startQ[2], startQ[3], startQ[4], startQ[5], startQ[6], startQ[7],
							0., 1., 0., 0., 0., 0., 0., 0.
						);

						//oxyde::log::printDualQuatParameters(L"transQSanityCheckParameters",
						//	1., 0., 0., 0., 0., 0., 0., 0.
						//);

						//oxyde::DQ::dual_Versor(0., 1., 0., 0., 0., 0., 0., 0.,
						//	DUALQUAARRAY(transQSanityCheck)
						//);
					}

					if (pair.first.first == 0) {
						//float step = 0.0;
						//if (pair.second.first > 0) {
						//	step = 0.001 / double(pair.second.first);
						//}
						float step = (pair.second.first > 0) ? 0.001 / double(pair.second.first) : 0.0;

						float interpQuat[8];
						oxyde::DQ::dual_Versor(
							angleInterp * step,
							UxInterp, UyInterp, UzInterp,
							sInterp * step,
							MxInterp, MyInterp, MzInterp,
							DUALQUAARRAY(interpQuat)
						);

						float localQuat[8];
						oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(interpQuat),
							DUALQUAARRAY(startQ),
							DUALQUAARRAY(localQuat));

						oxyde::log::printText(currentNode->GetName());
						oxyde::log::printDualQuat(L"interpQuat", interpQuat);
						oxyde::log::printDualQuat(L"localQuat", localQuat);

						dualQuat localQuatDouble = { DUALQUAARRAY(localQuat) };
						oxyde::exporter::skin::skinPoseCorrector::addLocalTransform(currentNode, localQuatDouble);
					}

					oxyde::log::printDualQuat(L"transQSanityCheck", transQSanityCheck);
					outStream << "Expand[MultiQuat[transQSanityCheck, startQ]]" << std::endl;

					std::wstring resultString(outStream.str());
					DebugOutputString(resultString.c_str());
				}

				oxyde::log::printLine();
			}

			//void PRScontrollerDataExtractor::getLocalMatrixForTime(TimeValue t, double m[]) {
			//	Matrix3 localMatrix;
			//	localMatrix.IdentityMatrix();
			//	m_Control->GetValue(t, (void*)&localMatrix, FOREVER, CTRL_RELATIVE);

			//	m[0] = localMatrix.GetRow(0).x;		m[1] = localMatrix.GetRow(0).y;		m[2] = localMatrix.GetRow(0).z;		m[3] = 0.;
			//	m[4] = localMatrix.GetRow(1).x;		m[5] = localMatrix.GetRow(1).y;		m[6] = localMatrix.GetRow(1).z;		m[7] = 0.;
			//	m[8] = localMatrix.GetRow(2).x;		m[9] = localMatrix.GetRow(2).y;		m[10] = localMatrix.GetRow(2).z;	m[11] = 0.;
			//	m[12] = localMatrix.GetRow(3).x;	m[13] = localMatrix.GetRow(3).y;	m[14] = localMatrix.GetRow(3).z;	m[15] = 1.;
			//}

			//void PRScontrollerDataExtractor::getDualQuatForTime(TimeValue t, float quat[])
			//{
			//	std::wstring outStr;
			//	std::wostringstream outStream(outStr);
			//	outStream << "__________________________________________________[" << t << "]" << std::endl;

			//	float eulerX = 0.0, eulerY = 0.0, eulerZ = 0.0;
			//	Quat q;
			//	if (m_rotationController) {
			//		auto rotationControl = std::dynamic_pointer_cast<rotationControllerDataExtractor>(m_rotationController);
			//		if (rotationControl) {
			//			rotationControl->getValueForTime(t, q);
			//		}
			//	}

			//	float rotQuat[8];
			//	oxyde::DQ::pure_Real_quaternion(q.w, q.x, q.y, q.z, DUALQUAARRAY(rotQuat));
			//	//outStream << "rotQuat = {";
			//	//outStream << rotQuat[0] << "," << rotQuat[1] << "," << rotQuat[2] << "," << rotQuat[3] << ",";
			//	//outStream << rotQuat[4] << "," << rotQuat[5] << "," << rotQuat[6] << "," << rotQuat[7];
			//	//outStream << "}.dualQuatUnit";
			//	//outStream << std::endl;

			//	float x = 0.0, y = 0.0, z = 0.0;
			//	if (m_positionController) {
			//		auto positionControl = std::dynamic_pointer_cast<positionControllerDataExtractor>(m_positionController);
			//		if (positionControl) {
			//			positionControl->getValueForTime(t, x, y, z);
			//		}
			//	}

			//	//outStream << "posControl = {" << x << ", " << y << ", " << z << "}" << std::endl;

			//	float transQuat[8];
			//	oxyde::DQ::translation_quaternion(x, y, z, DUALQUAARRAY(transQuat));
			//	//outStream << "transQuat = {";
			//	//outStream << transQuat[0] << "," << transQuat[1] << "," << transQuat[2] << "," << transQuat[3] << ",";
			//	//outStream << transQuat[4] << "," << transQuat[5] << "," << transQuat[6] << "," << transQuat[7];
			//	//outStream << "}.dualQuatUnit";
			//	//outStream << std::endl;

			//	oxyde::DQ::dual_quaternion_product( DUALQUAARRAY(transQuat), DUALQUAARRAY(rotQuat),DUALQUAARRAY(quat));
			//	//outStream << "finalQuat = {";
			//	//outStream << quat[0] << "," << quat[1] << "," << quat[2] << "," << quat[3] << ",";
			//	//outStream << quat[4] << "," << quat[5] << "," << quat[6] << "," << quat[7];
			//	//outStream << "}.dualQuatUnit";
			//	//outStream << std::endl;

			//	//std::wstring resultString(outStream.str());
			//	//DebugOutputString(resultString.c_str());
			//}

			void PRScontrollerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement)
			{
				//std::shared_ptr<oxyde::exporter::XML::baseSpinnerDataElement> baseElement = std::make_shared<oxyde::exporter::XML::baseSpinnerDataElement>(
				//	theAnimationElement, std::wstring(sourceName), quat[0], quat[1], quat[2], quat[3], quat[4], quat[5], quat[6], quat[7]);

				oxyde::exporter::XML::oxyPRSElementPtr thePRSElement = std::make_shared<oxyde::exporter::XML::oxyPRSElement>(std::dynamic_pointer_cast<oxyde::exporter::XML::oxyAnimationElement>(theAnimationElement));

				for (int i = 0; i < m_Control->NumSubs(); i++) {
					Animatable* controlAnim = m_Control->SubAnim(i);
					Control* animAsControl = dynamic_cast<Control*>(controlAnim);
					if (animAsControl != nullptr) {
						auto theControllerExtractor = oxyde::exporter::controller::controllerDataExtractor::buildExtractorAndSetCurrentNode(animAsControl, currentNode);
						theControllerExtractor->exportController(thePRSElement);
					}
				}



				//	//DebugPrint(L"This is a PRS Controller Extractor test\n");
				//	std::set<TimeValue> keyTimes = getKeyTimes();
				//	Matrix3 localMatrix;
				//	for (auto&& t : keyTimes) {
				//		localMatrix.IdentityMatrix();
				//		m_Control->GetValue(t, (void*)&localMatrix, FOREVER, CTRL_RELATIVE);

				//		//std::wstring outStr;
				//		//std::wostringstream outStream(outStr);
				//		//outStream << L"	Time: " << t << std::endl;

				//		//outStream << L"		" << localMatrix.GetRow(0).x << "   " << localMatrix.GetRow(0).y << "   " << localMatrix.GetRow(0).z << std::endl;
				//		//outStream << L"		" << localMatrix.GetRow(1).x << "   " << localMatrix.GetRow(1).y << "   " << localMatrix.GetRow(1).z << std::endl;
				//		//outStream << L"		" << localMatrix.GetRow(2).x << "   " << localMatrix.GetRow(2).y << "   " << localMatrix.GetRow(2).z << std::endl;
				//		//outStream << L"		" << localMatrix.GetRow(3).x << "   " << localMatrix.GetRow(3).y << "   " << localMatrix.GetRow(3).z << std::endl;

				//		//outStream << std::endl;

				//		double m[16] = {
				//			localMatrix.GetRow(0).x ,localMatrix.GetRow(0).y, localMatrix.GetRow(0).z, 0.,
				//			localMatrix.GetRow(1).x ,localMatrix.GetRow(1).y, localMatrix.GetRow(1).z, 0.,
				//			localMatrix.GetRow(2).x ,localMatrix.GetRow(2).y, localMatrix.GetRow(2).z, 0.,
				//			localMatrix.GetRow(3).x ,localMatrix.GetRow(3).y, localMatrix.GetRow(3).z, 1.
				//		};
				//		double nx, ny, nz;
				//		double cosTheta, sinTheta;
				//		double slide;
				//		double mx, my, mz;

				//		oxyde::math::getDualQuaternionParametersFromMatrix(m, cosTheta, sinTheta, nx, ny, nz, slide, mx, my, mz);

				//		//outStream << "Normal = {" << nx << ", " << ny << ", " << nz << "}" << std::endl;
				//		//outStream << "Cos, Sin = {" << cosTheta << ", " << sinTheta << "}" << std::endl;

				//		//outStream << "Slide = " << slide << std::endl;
				//		//outStream << "Axis vector m = {" << mx << ", " << my << ", " << mz << "}" << std::endl;

				//		//std::wstring resultString(outStream.str());
				//		//DebugOutputString(resultString.c_str());
				//	}
				//	keyTimes.clear();

				//	if (m_positionController)m_positionController->exportController(theAnimationElement);
				//	if (m_rotationController)m_rotationController->exportController(theAnimationElement);
				//	if (m_scaleController)m_scaleController->exportController(theAnimationElement);
				//	buildTrack(std::dynamic_pointer_cast<oxyde::exporter::XML::oxyAnimationElement>(theAnimationElement));
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

			PRScontrollerDataExtractor::PRScontrollerDataExtractor(Control* theControl) : controllerDataExtractor(theControl)
			{
				//m_positionController = nullptr;
				//m_rotationController = nullptr;
				//m_scaleController = nullptr;

				//if (!m_Control->IsLeaf()) {
				//	if (m_Control->GetPositionController() != nullptr) m_positionController = buildExtractor(m_Control->GetPositionController());
				//	if (m_Control->GetRotationController() != nullptr) m_rotationController = buildExtractor(m_Control->GetRotationController());
				//	if (m_Control->GetScaleController() != nullptr) m_scaleController = buildExtractor(m_Control->GetScaleController());
				//}
			}

			IKeyControl * PRScontrollerDataExtractor::GetKeyControlInterfacePointer()
			{
				return GetKeyControlInterface(m_Control);
			}

			//std::set<TimeValue> PRScontrollerDataExtractor::getKeyTimes()
			//{
			//	std::set<TimeValue> keyTimes;
			//	keyTimes.insert(TimeValue(0));

			//	std::shared_ptr<XYZControllerDataExtractor>posController_ptr = std::dynamic_pointer_cast<XYZControllerDataExtractor>(m_positionController);
			//	if (posController_ptr)
			//	{
			//		std::set<TimeValue> tempSet = posController_ptr->getKeyTimes();
			//		for (auto&& t : tempSet) {
			//			keyTimes.insert(t);
			//		}
			//		tempSet.clear();
			//	}

			//	std::shared_ptr<XYZControllerDataExtractor>rotController_ptr = std::dynamic_pointer_cast<XYZControllerDataExtractor>(m_rotationController);
			//	if (rotController_ptr)
			//	{
			//		std::set<TimeValue> tempSet = rotController_ptr->getKeyTimes();
			//		for (auto&& t : tempSet) {
			//			keyTimes.insert(t);
			//		}
			//		tempSet.clear();
			//	}

			//	//std::set<TimeValue> PRSset = keyControllerDataExtractor::getKeyTimes();
			//	//for (auto&& t : PRSset) {
			//	//	keyTimes.insert(t);
			//	//}
			//	//PRSset.clear();

			//	IKeyControl* keyControl = GetKeyControlInterfacePointer();
			//	if (keyControl != nullptr) {
			//		int numKeys = keyControl->GetNumKeys();
			//		for (int i = 0; i < numKeys; i++) {
			//			keyTimes.insert(m_Control->GetKeyTime(i));
			//		}
			//	}

			//	// Not treating Scale Transform this time. So I don't care about scale keyframes.

			//	return keyTimes;
			//}

			std::set<TimeValue> XYZControllerDataExtractor::getKeyTimes()
			{
				std::set<TimeValue> keyTimes;
				keyTimes.insert(TimeValue(0));

				if (!m_Control->IsLeaf()) {

					if (m_XController) {
						std::shared_ptr<keyControllerDataExtractor>XKeyController_ptr = std::static_pointer_cast<keyControllerDataExtractor>(m_XController);
						if (XKeyController_ptr)
						{
							std::set<TimeValue> tempSet = XKeyController_ptr->getKeyTimes();
							for (auto&& t : tempSet) {
								keyTimes.insert(t);
							}
							tempSet.clear();
						}
					}

					if (m_YController) {
						std::shared_ptr<keyControllerDataExtractor>YKeyController_ptr = std::static_pointer_cast<keyControllerDataExtractor>(m_YController);
						if (YKeyController_ptr)
						{
							std::set<TimeValue> tempSet = YKeyController_ptr->getKeyTimes();
							for (auto&& t : tempSet) {
								keyTimes.insert(t);
							}
							tempSet.clear();
						}
					}

					if (m_ZController) {
						std::shared_ptr<keyControllerDataExtractor>ZKeyController_ptr = std::static_pointer_cast<keyControllerDataExtractor>(m_ZController);
						if (ZKeyController_ptr)
						{
							std::set<TimeValue> tempSet = ZKeyController_ptr->getKeyTimes();
							for (auto&& t : tempSet) {
								keyTimes.insert(t);
							}
							tempSet.clear();
						}
					}
				}
				else {
					int isKeyable = m_Control->IsKeyable();
					int test = isKeyable == 1;

					std::set<TimeValue> tempSet = keyControllerDataExtractor::getKeyTimes();
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

			IKeyControl * XYZControllerDataExtractor::GetKeyControlInterfacePointer()
			{
				return GetKeyControlInterface(m_Control);
			}

			XYZControllerDataExtractor::XYZControllerDataExtractor(Control *theControl) : keyControllerDataExtractor(theControl)
			{
				if (!m_Control->IsLeaf()) {
					//DebugPrint(L"XYZControllerDataExtractor is NOT leaf\n");
					if (m_Control->GetXController() != nullptr) m_XController = buildExtractor(m_Control->GetXController());
					if (m_Control->GetYController() != nullptr) m_YController = buildExtractor(m_Control->GetYController());
					if (m_Control->GetZController() != nullptr) m_ZController = buildExtractor(m_Control->GetZController());
				}
				else {
					//DebugPrint(L"XYZControllerDataExtractor is leaf\n");
				}
			}

			void positionControllerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement)
			{
				//DebugPrint(L"This is a position Controller Extractor\n");
				//std::set<TimeValue> keyTimes = getKeyTimes();
				//DebugPrint(L"Total Keys for position controller: %i\n", keyTimes.size());
				//keyTimes.clear();



				//oxyde::exporter::XML::oxyPositionElementPtr thePositionElement = std::make_shared<oxyde::exporter::XML::oxyPositionElement>(std::dynamic_pointer_cast<oxyde::exporter::XML::oxyDocumentElementPtr>(theAnimationElement));
				oxyde::exporter::XML::oxyPositionElementPtr thePositionElement = std::make_shared<oxyde::exporter::XML::oxyPositionElement>(theAnimationElement);

				for (int i = 0; i < m_Control->NumSubs(); i++) {
					Animatable* controlAnim = m_Control->SubAnim(i);
					Control* animAsControl = dynamic_cast<Control*>(controlAnim);
					if (animAsControl != nullptr) {
						auto theControllerExtractor = oxyde::exporter::controller::controllerDataExtractor::buildExtractorAndSetCurrentNode(animAsControl, currentNode);
						std::wstring subControlName(m_Control->SubAnimName(i));
						for (std::wstring::size_type i = subControlName.find_first_of(' '); i != std::wstring::npos; i = subControlName.find_first_of(' ')) {
							subControlName.erase(i, 1);
						}

						oxyde::exporter::XML::oxyDocumentElementPtr subControlElement = std::make_shared<oxyde::exporter::XML::oxyDocumentElement>(thePositionElement, subControlName);
						theControllerExtractor->exportController(subControlElement);
					}
				}
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

			positionControllerDataExtractor::positionControllerDataExtractor(Control *theControl) : XYZControllerDataExtractor(theControl) {}

			void rotationControllerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement)
			{
				//DebugPrint(L"This is a ROTATION Controller Extractor\n");
				//std::set<TimeValue> keyTimes = getKeyTimes();
				//DebugPrint(L"Total Keys for ROTATION controller: %i\n", keyTimes.size());
				//keyTimes.clear();



				oxyde::exporter::XML::oxyRotationElementPtr theRotationElement = std::make_shared<oxyde::exporter::XML::oxyRotationElement>(std::dynamic_pointer_cast<oxyde::exporter::XML::oxyPRSElement>(theAnimationElement), getEulerAngleOrder());

				for (int i = 0; i < m_Control->NumSubs(); i++) {
					Animatable* controlAnim = m_Control->SubAnim(i);
					Control* animAsControl = dynamic_cast<Control*>(controlAnim);
					if (animAsControl != nullptr) {
						auto theControllerExtractor = oxyde::exporter::controller::controllerDataExtractor::buildExtractorAndSetCurrentNode(animAsControl, currentNode);
						std::wstring subControlName(m_Control->SubAnimName(i));
						for (std::wstring::size_type i = subControlName.find_first_of(' '); i != std::wstring::npos; i = subControlName.find_first_of(' ')) {
							subControlName.erase(i, 1);
						}

						oxyde::exporter::XML::oxyDocumentElementPtr subControlElement = std::make_shared<oxyde::exporter::XML::oxyDocumentElement>(theRotationElement, subControlName);
						theControllerExtractor->exportController(subControlElement);
					}
				}
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
				registerFactory(rotationControllerDataExtractor::buildRotationControllerDataExtractor, Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0x0));
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

			void bipSlaveControllerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement) {
				//DebugPrint(L"This is a bipSlave extractor... ClassID: 0x%lX : 0x%lX\n", m_Control->ClassID().PartA(), m_Control->ClassID().PartB());
				std::set<TimeValue> keyTimes = getKeyTimes();
				Matrix3 theMatrix;
				Matrix3 localMatrix;
				Matrix3 ParentMatrix;
				for (auto&& t : keyTimes) {
					theMatrix.IdentityMatrix();
					localMatrix.IdentityMatrix();

					ParentMatrix = m_bipNode->GetParentTM(t);
					m_Control->GetValue(t, (void*)&theMatrix, FOREVER, CTRL_RELATIVE);
					localMatrix = theMatrix * Inverse(ParentMatrix);

					//std::wstring outStr;
					//std::wostringstream outStream(outStr);
					//outStream << L"	Time: " << t << std::endl;

					//outStream << L"		" << localMatrix.GetRow(0).x << "   " << localMatrix.GetRow(0).y << "   " << localMatrix.GetRow(0).z << std::endl;
					//outStream << L"		" << localMatrix.GetRow(1).x << "   " << localMatrix.GetRow(1).y << "   " << localMatrix.GetRow(1).z << std::endl;
					//outStream << L"		" << localMatrix.GetRow(2).x << "   " << localMatrix.GetRow(2).y << "   " << localMatrix.GetRow(2).z << std::endl;
					//outStream << L"		" << localMatrix.GetRow(3).x << "   " << localMatrix.GetRow(3).y << "   " << localMatrix.GetRow(3).z << std::endl;

					//outStream << std::endl;

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

					//outStream << "Normal = {" << nx << ", " << ny << ", " << nz << "}" << std::endl;
					//outStream << "Cos, Sin = {" << cosTheta << ", " << sinTheta << "}" << std::endl;

					//outStream << "Slide = " << slide << std::endl;
					//outStream << "Axis vector m = {" << mx << ", " << my << ", " << mz << "}" << std::endl;

					//std::wstring resultString(outStream.str());
					//DebugOutputString(resultString.c_str());


				}
				keyTimes.clear();
				buildTrack(std::dynamic_pointer_cast<oxyde::exporter::XML::oxyAnimationElement>(theAnimationElement));
				return;
			}

			void bipSlaveControllerDataExtractor::getDualQuatForTime(TimeValue t, float quat[])
			{
				//oxyde::log::printLine();

				IBipMaster12* theBipMaster = NULL;
				theBipMaster = (IBipMaster12*)m_Control->GetInterface(IBipMaster12::I_BIPMASTER12);
				if (theBipMaster != NULL) {
					Quat q = theBipMaster->GetBipedRot(t, m_bipNode, TRUE);

					float rotQuat[8];
					oxyde::DQ::pure_Real_quaternion(q.w, q.x, q.y, q.z, DUALQUAARRAY(rotQuat));
					//oxyde::log::printDualQuat(L"rotFromBip", rotQuat);

					Point3 pGlobal = theBipMaster->GetBipedPos(t, m_bipNode);
					Matrix3 parentTM = m_bipNode->GetParentTM(t);
					parentTM.Invert();
					Point3 p = parentTM.PointTransform(pGlobal);

					float transQuat[8];
					oxyde::DQ::translation_quaternion(p.x, p.y, p.z, DUALQUAARRAY(transQuat));
					//oxyde::log::printDualQuat(L"transFromBip", transQuat);

					oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(transQuat), DUALQUAARRAY(rotQuat), DUALQUAARRAY(quat));
					//oxyde::log::printDualQuat(L"finalFromBip", quat);
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

			void bipBodyControllerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement) {
				//DebugPrint(L"This is a bipBody extractor... ClassID: 0x%lX : 0x%lX\n", m_Control->ClassID().PartA(), m_Control->ClassID().PartB());

				if (!m_Control->IsLeaf()) {
					//DebugPrint(L"bipBodyControllerDataExtractor is NOT leaf\n");
				}
				else {
					//DebugPrint(L"bipBodyControllerDataExtractor is leaf\n");
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

			void lookAtcontrollerDataExtractor::exportController(oxyde::exporter::XML::oxyDocumentElementPtr theAnimationElement)
			{
				oxyde::exporter::XML::oxyLookAtElementPtr theLookAtElement = std::make_shared<oxyde::exporter::XML::oxyLookAtElement>(std::dynamic_pointer_cast<oxyde::exporter::XML::oxyAnimationElement>(theAnimationElement));

				for (int i = 0; i < m_Control->NumSubs(); i++) {
					Animatable* controlAnim = m_Control->SubAnim(i);
					Control* animAsControl = dynamic_cast<Control*>(controlAnim);
					if (animAsControl != nullptr) {
						auto theControllerExtractor = oxyde::exporter::controller::controllerDataExtractor::buildExtractorAndSetCurrentNode(animAsControl, currentNode);

						std::wstring subControlName(m_Control->SubAnimName(i));

						if (subControlName == L"Roll Angle") {
							oxyde::exporter::XML::oxyDocumentElementPtr subControlElement = std::make_shared<oxyde::exporter::XML::oxyDocumentElement>(theLookAtElement, L"RollAngle");
							theControllerExtractor->exportController(subControlElement);
						}
						else {
							theControllerExtractor->exportController(theLookAtElement);
						}
					}
				}
			}

			Class_ID lookAtcontrollerDataExtractor::getClass_ID()
			{
				return Class_ID(LOOKAT_CONTROL_CLASS_ID, 0x0);
			}

			void lookAtcontrollerDataExtractor::registerMe()
			{
				registerFactory(lookAtcontrollerDataExtractor::buildlookAtcontrollerDataExtractor, getClass_ID());
			}

			controllerDataExtractor_ptr lookAtcontrollerDataExtractor::buildlookAtcontrollerDataExtractor(Control * theControl)
			{
				return std::dynamic_pointer_cast<controllerDataExtractor>(
					std::make_shared<lookAtcontrollerDataExtractor>(lookAtcontrollerDataExtractor(theControl))
					);
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
			oxyde::exporter::controller::lookAtcontrollerDataExtractor::registerMe();
		}
	};

	factoryInstaller theFactoryInstaller;
}

