#include "skinDataExtraction.h"
#include "nodeNavigation.h"
#include "processGeometry.h"
#include "processMaterials.h"
#include "processTransforms.h"
#include "processControllers.h"
//#include "xmlExporter.h"
#include "iskin.h"
#include <modstack.h>
#include <stack>
#include "processSkin.h"
#include "matrixUtility.h"
#include "debugLog.h"
#include "dualQuaternionMath.h"
#include "dualQuaternionFunctions.h"
#include "xmlDocumentSkin.h"
#include "xmlDocumentMesh.h"

namespace oxyde {
	namespace exporter {
		namespace skin {

			using dualQuat = std::array<double, 8>;
			using dualQuatFloat = std::array<float, 8>;
			using matrix4x4 = std::array<double, 16>;

			std::map<INodePtr, dualQuat> skinPoseCorrector::nodeToLocalTransformDict;

			skinPoseCorrector::skinPoseCorrector(ISkin* theSkinInterface) {
				int numBones = theSkinInterface->GetNumBones();
				oxyde::log::printText(L"skinPoseConversion = Association[{");

				for (int i = 0; i < numBones; i++) {
					INodePtr theBoneNode = theSkinInterface->GetBone(i);
					Matrix3 theMatrix;

					oxyde::log::printText(theBoneNode->GetName());

					theSkinInterface->GetBoneInitTM(theBoneNode, theMatrix);

					matrix4x4 m;
					m[0] = theMatrix.GetRow(0).x;		m[1] = theMatrix.GetRow(0).y;		m[2] = theMatrix.GetRow(0).z;		m[3] = 0.;
					m[4] = theMatrix.GetRow(1).x;		m[5] = theMatrix.GetRow(1).y;		m[6] = theMatrix.GetRow(1).z;		m[7] = 0.;
					m[8] = theMatrix.GetRow(2).x;		m[9] = theMatrix.GetRow(2).y;		m[10] = theMatrix.GetRow(2).z;		m[11] = 0.;
					m[12] = theMatrix.GetRow(3).x;		m[13] = theMatrix.GetRow(3).y;		m[14] = theMatrix.GetRow(3).z;		m[15] = 1.;

					skinPoseMatricesDict[theBoneNode] = m;			
					
					float qs, qx, qy, qz, dqs, dqx, dqy, dqz;
					oxyde::math::getDualQuaternionFromMatrix(m.data(), qs, qx, qy, qz, dqs, dqx, dqy, dqz);
					nodeToSkinPoseDict[theBoneNode] = { qs, qx, qy, qz, dqs, dqx, dqy, dqz };

					float mFloat[] = { m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15] };
					
					oxyde::log::printText(std::to_wstring(i) + L" -> {");
					oxyde::log::printMatrix(L"skinPoseMatrix", mFloat);
					oxyde::log::printText(L",");

					oxyde::log::printDualQuat(L"skinPose", qs, qx, qy, qz, dqs, dqx, dqy, dqz);
					oxyde::log::printText(L"},");
				}

				oxyde::log::printText(L"}]");

				INodePtr sceneRoot = skinObjectsList::getRootNodeForScene();

				std::stack<INodePtr> theStack;

				theStack.push(sceneRoot);

				INodePtr skeletonRoot = nullptr;

				while (!theStack.empty()) {
					INodePtr topNode = theStack.top();
					theStack.pop();

					oxyde::log::printText(topNode->GetName());

					if (!skeletonRoot) {
						if (nodeToSkinPoseDict.find(topNode) != nodeToSkinPoseDict.end()) {
							skeletonRoot = topNode;
							nodeToGlobalTransformDict[skeletonRoot] = nodeToLocalTransformDict[skeletonRoot];

							oxyde::log::printNamedParameter(L"SkeletonRoot", topNode->GetName());
						}
					}
					else {
						dualQuatFloat localTransform = { DUALQUAARRAY(nodeToLocalTransformDict[topNode]) };
						dualQuatFloat parentGlobalTransform = { DUALQUAARRAY(nodeToGlobalTransformDict[topNode->GetParentNode()]) };
						dualQuatFloat boneGlobalTransform;

						oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(parentGlobalTransform),
							DUALQUAARRAY(localTransform),
							DUALQUAARRAY(boneGlobalTransform));

						nodeToGlobalTransformDict[topNode] = { DUALQUAARRAY(boneGlobalTransform) };

						if (nodeToSkinPoseDict.find(topNode) != nodeToSkinPoseDict.end() && nodeToSkinPoseDict.find(topNode->GetParentNode()) != nodeToSkinPoseDict.end()) {
							dualQuatFloat skinPose = { DUALQUAARRAY(nodeToSkinPoseDict[topNode]) };
							dualQuatFloat transformedFromSkin;
							dualQuatFloat skinPoseParent = { DUALQUAARRAY(nodeToSkinPoseDict[topNode->GetParentNode()]) };
							dualQuatFloat transformedFromSkinParent;

							oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(skinPose), DUALQUAARRAY(boneGlobalTransform), DUALQUAARRAY(transformedFromSkin));

							oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(skinPoseParent), DUALQUAARRAY(parentGlobalTransform), DUALQUAARRAY(transformedFromSkinParent));

							//fromSkinToGlobalLocal =
							//	Expand[MultiQuat[ConjugateQuat[fromSkintoGlobalParent],
							//	fromSkintoGlobalChild]];
							//Sign[flattenQuat[fromSkinToGlobalLocal][[1]]]) &

							dualQuatFloat transformedFromSkinParentConj;
							dualQuatFloat transformFromParentToChild;

							DUALQUAVAR(conjSource);
							oxyde::DQ::dual_quaternion_conjugate(DUALQUAARRAY(transformedFromSkinParent), DUALQUAARRAY(transformedFromSkinParentConj));

							oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(transformedFromSkinParentConj), DUALQUAARRAY(transformedFromSkin), DUALQUAARRAY(transformFromParentToChild));

							if (transformFromParentToChild[0] < 0.) {
								dualQuat complQuat = nodeToSkinPoseDict[topNode];
								nodeToSkinPoseDict[topNode] = { -complQuat[0], -complQuat[1], -complQuat[2], -complQuat[3], -complQuat[4], -complQuat[5], -complQuat[6], -complQuat[7] };

								oxyde::log::printNamedParameter(L"Signal inverted: ", topNode->GetName());
							}
						}

					}

					int numChildren = topNode->NumberOfChildren();
					for (int i = 0; i < numChildren; i++) {

						INodePtr child = topNode->GetChildNode(i);
						theStack.push(child);
					}
				}

				for (auto i = nodeToSkinPoseDict.begin(); i != nodeToSkinPoseDict.end(); ++i) {
					oxyde::log::printNamedParameter(L"boneNode", i->first->GetName());
					oxyde::log::printDualQuat(L"SkinPose", DUALQUAARRAY(i->second));
				}

			}

			skinObjectsList*  skinObjectsList::theSingleton = NULL;

			void skinObjectsList::buildSkinObjects(oxyde::exporter::XML::oxyDocumentPtr theDocument) {

				//** create skin section and geometry section here
				oxyde::exporter::XML::oxyObjectListPtr theObjectList = std::make_shared<oxyde::exporter::XML::oxyObjectList>(theDocument);
				oxyde::exporter::XML::oxyGeometryElementPtr theGeometrySection = std::make_shared<oxyde::exporter::XML::oxyGeometryElement>(theDocument);


				for (auto &&skinNode : nodeToSkinVector) {
					if (!skinNode->IsRootNode()) {
						bool derivedTest = false;
						Object* theTopNodeObject = skinNode->GetObjectRef();
						Class_ID theTopNodeObjectClassID = theTopNodeObject->ClassID();
						SClass_ID theTopNodeObjectSuperClassID = theTopNodeObject->SuperClassID();
						derivedTest = (GEN_DERIVOB_CLASS_ID == theTopNodeObjectSuperClassID);
						if (derivedTest) {
							IDerivedObject* theDerivedObj = (IDerivedObject*)theTopNodeObject;

							if (isThereASkinModifier(theDerivedObj)) {
								//extractSkinDataFromObj(theDerivedObj, skinNode, (_TCHAR*)skinNode->GetName(), expFile);

								//** passdown skin and geometry sections to this function
								extractSkinDataFromObj(theDerivedObj, skinNode, (_TCHAR*)skinNode->GetName(), theObjectList, theGeometrySection);
							}
						}
					}
				}
			}
		}
	}
}
