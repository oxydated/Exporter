#include "auxiliaryLogger.h"

namespace oxyde {
	namespace exporter {
		namespace log {

			std::map<int, std::wstring> bonesForSkin;

			std::map<std::wstring, int> nodeObjectsByName;

			std::map<int, std::set<int>> adjacentBonesForBone;

			std::map<int, int> nodeObjectToBoneTranslationMap;

			std::set<boneGraphEdge> boneGraphAdjacencyMap;
			std::set<std::pair<int, int>> nodeGraphAdjacencyMap;

			void startBuildingBonesForSkin() {
				bonesForSkin = std::map<int, std::wstring>();
			}

			void insertBonesForSkin(int boneIndexOnSkin, std::wstring nodeName)
			{
				if (bonesForSkin.find(boneIndexOnSkin) == bonesForSkin.end()) {
					bonesForSkin[boneIndexOnSkin] = nodeName;
				}
			}

			void startBuildingNodeObjectsForBone() {
				nodeObjectsByName = std::map<std::wstring, int>();
			}

			void insertNodeObjectAndName(int nodeObject, std::wstring nodeName)
			{
				if (nodeObjectsByName.find(nodeName) == nodeObjectsByName.end()) {
					nodeObjectsByName[nodeName] = nodeObject;
				}
			}

			void startBuildingAdjacentBonesForBone() {
				adjacentBonesForBone = std::map<int, std::set<int>>();
				for (auto &&bone : bonesForSkin) {
					adjacentBonesForBone[bone.first] = std::set<int>();
				}
			}

			void insertAdjacentBones(const std::set<int> &bonesForVertex) {
				for (const int &i : bonesForVertex) {
					for (const int &j : bonesForVertex) {
						if (j != i) {
							adjacentBonesForBone[i].insert(j);
						}
					}
				}
			}

			void startBuildingBoneGraphAdjacencyMap() {
				nodeObjectToBoneTranslationMap = std::map<int, int>();

				for (auto &&thePair : bonesForSkin) {
					nodeObjectToBoneTranslationMap[nodeObjectsByName[thePair.second]] = thePair.first;
				}

				boneGraphAdjacencyMap = std::set<boneGraphEdge>();
				nodeGraphAdjacencyMap = std::set < std::pair<int, int>>();

				for (auto &&thePair : adjacentBonesForBone) {
					int i = thePair.first;
					for (const int &j : thePair.second) {
						boneGraphAdjacencyMap.insert(boneGraphEdge(min(i, j), max(i, j)));

						int nodeObject_i = nodeObjectsByName[bonesForSkin[i]];
						int nodeObject_j = nodeObjectsByName[bonesForSkin[j]];

						nodeGraphAdjacencyMap.insert(std::pair<int, int>(min(nodeObject_i, nodeObject_j), max(nodeObject_i, nodeObject_j)));
					}
				}
			}

			void printBoneGraphAdjacencyMap()
			{
				std::wstringstream animStream;
				animStream << std::fixed;
				//	"{1 <->    3, 3 <->   5, 5 ->  2, 2 <->  3}"
				animStream << L"{ ";
				for (auto &&thePair : boneGraphAdjacencyMap) {
					animStream << std::to_wstring(thePair.first) << L" <-> " << std::to_wstring(thePair.second) << ", ";
				}
				animStream << L"}";

				oxyde::log::printText(animStream.str());
			}

			std::map<int, oxyde::DQ::dualQuat>skinPosesForBones;

			void startSkinPosesForBones() {
				skinPosesForBones = std::map<int, oxyde::DQ::dualQuat>();
			}

			void insertSkinPosesForBones(int boneIndexOnSkin, const oxyde::DQ::dualQuat &skinPose)
			{
				if (skinPosesForBones.find(boneIndexOnSkin) == skinPosesForBones.end()) {
					skinPosesForBones[boneIndexOnSkin] = skinPose;
				}
			}

			std::map<std::wstring, oxyde::DQ::dualQuat> localTransformForNodes;

			void startLocalTransformForNodes() {
				localTransformForNodes = std::map<std::wstring, oxyde::DQ::dualQuat>();
			}

			void insertLocalTransformForNodes(std::wstring nodeName, const oxyde::DQ::dualQuat &localTransform)
			{
				if (localTransformForNodes.find(nodeName) == localTransformForNodes.end()) {
					localTransformForNodes[nodeName] = localTransform;
				}
			}

			void processSkinPoseWithLocalTransform(INodePtr sceneRootNode) {

				std::map<std::wstring, oxyde::DQ::dualQuat> globalTransformForNodeName = std::map<std::wstring, oxyde::DQ::dualQuat>();

				globalTransformForNodeName[sceneRootNode->GetName()] = { 1.,0. ,0. ,0. ,0. ,0. ,0. ,0. };

				std::stack<INodePtr> nodeStack = std::stack<INodePtr>();
				
				nodeStack.push(sceneRootNode);

				while (!nodeStack.empty()) {
					INodePtr topNode = nodeStack.top();
					nodeStack.pop();

					for (int i = 0; i < topNode->NumChildren(); i++) {
						INodePtr child = topNode->GetChildNode(i);

						oxyde::DQ::dualQuat boneGlobalTransform;
						oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(globalTransformForNodeName[topNode->GetName()]),
							DUALQUAARRAY(localTransformForNodes[child->GetName()]),
							DUALQUAARRAY(boneGlobalTransform));

						globalTransformForNodeName[child->GetName()] = boneGlobalTransform;

						nodeStack.push(child);
					}
				}

				std::map<int, oxyde::DQ::dualQuat> globalTransformationForNodeObject = std::map<int, oxyde::DQ::dualQuat>();
				for (auto &&thePair : globalTransformForNodeName) {
					globalTransformationForNodeObject[nodeObjectsByName[thePair.first]] = thePair.second;
				}

				///////////////////////
				for (auto &&theEdge : nodeGraphAdjacencyMap) {
					int theParent = theEdge.first;
					int theChild = theEdge.second;

					oxyde::DQ::dualQuat transformedFromSkin;
					oxyde::DQ::dualQuat transformedFromSkinParent;

					//fromSkintoGlobalChild =
					//	Expand[MultiQuat[globalChild, ConjugateQuat[skinposeChild]]];

					//oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(skinPose), DUALQUAARRAY(boneGlobalTransform), DUALQUAARRAY(transformedFromSkin));

					oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(skinPosesForBones[nodeObjectToBoneTranslationMap[theChild]]), 
						DUALQUAARRAY(globalTransformationForNodeObject[theChild]),
						DUALQUAARRAY(transformedFromSkin));
					
					//fromSkintoGlobalParent =
					//	Expand[MultiQuat[globalParent, ConjugateQuat[skinposeParent]]];

					//oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(skinPoseParent), DUALQUAARRAY(parentGlobalTransform), DUALQUAARRAY(transformedFromSkinParent));

					oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(skinPosesForBones[nodeObjectToBoneTranslationMap[theParent]]), 
						DUALQUAARRAY(globalTransformationForNodeObject[theParent]),
						DUALQUAARRAY(transformedFromSkinParent));

					//fromSkinToGlobalLocal =
					//	Expand[MultiQuat[ConjugateQuat[fromSkintoGlobalParent],
					//	fromSkintoGlobalChild]];

					//Sign[flattenQuat[fromSkinToGlobalLocal][[1]]]) &

					oxyde::DQ::dualQuat transformedFromSkinParentConj;

					//DUALQUAVAR(conjSource);
					oxyde::DQ::dual_quaternion_conjugate(DUALQUAARRAY(transformedFromSkinParent), DUALQUAARRAY(transformedFromSkinParentConj));

					oxyde::DQ::dualQuat transformFromParentToChild;

					oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(transformedFromSkinParentConj), DUALQUAARRAY(transformedFromSkin), DUALQUAARRAY(transformFromParentToChild));

					if (transformFromParentToChild[0] < 0.) {
						oxyde::DQ::dualQuat  complQuat = skinPosesForBones[nodeObjectToBoneTranslationMap[theChild]];
						skinPosesForBones[nodeObjectToBoneTranslationMap[theChild]] = { -complQuat[0], -complQuat[1], -complQuat[2], -complQuat[3], -complQuat[4], -complQuat[5], -complQuat[6], -complQuat[7] };

						//oxyde::log::printNamedParameter(L"Signal inverted: ", topNode->GetName());

						oxyde::log::printText(L"Signal inverted: " + bonesForSkin[nodeObjectToBoneTranslationMap[theChild]] + L"[ " + 
							std::to_wstring(theChild) + L" ," + std::to_wstring(nodeObjectToBoneTranslationMap[theChild]) + L"]");
					}

				}
				///////////////////////

				oxyde::log::printText(L"Global Transformation and Skinpose");

				oxyde::log::printText(L"{ ");

				for (auto &&thePair : bonesForSkin) {
					std::wstringstream animStream;
					animStream << std::fixed;

					animStream << L"{ ";
					animStream << L"\"" << thePair.second << L"\", ";
					animStream << thePair.first << L", ";
					animStream << nodeObjectsByName[thePair.second] << L", ";
					oxyde::DQ::dualQuat &q = globalTransformForNodeName[thePair.second];
					animStream << L"{" << q[0] << L"," << q[1] << L"," << q[2] << L"," << q[3] << L"," << q[4] << L"," << q[5] << L"," << q[6] << L"," << q[7] << L"}.dualQuatUnit, ";
					oxyde::DQ::dualQuat &g = skinPosesForBones[thePair.first];
					animStream << L"{" << g[0] << L"," << g[1] << L"," << g[2] << L"," << g[3] << L"," << g[4] << L"," << g[5] << L"," << g[6] << L"," << g[7] << L"}.dualQuatUnit ";
					animStream << L"}, ";

					oxyde::log::printText(animStream.str());
				}
				oxyde::log::printText(L"}");

			}

			oxyde::DQ::dualQuat getFinalSkinPoseForBoneName(std::wstring boneName) {
				return skinPosesForBones[nodeObjectToBoneTranslationMap[nodeObjectsByName[boneName]]];
			}

		}
	}
}