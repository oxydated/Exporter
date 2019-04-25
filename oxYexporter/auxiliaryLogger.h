#pragma once
#include <array>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <sstream>
#include <memory>
#include <cmath>
#include "matrixUtility.h"
#include "linAlg.h"
#include "dualQuaternionFunctions.h"
#include "debugLog.h"
#include <max.h>


namespace oxyde {
	namespace exporter {
		namespace log {

			using boneIndexForSkin = std::pair<int, std::wstring>;
			using nodeObjectAndName = std::pair<std::wstring, int>;
			
			void startBuildingBonesForSkin();
			void insertBonesForSkin(int boneIndexOnSkin, std::wstring nodeName);

			void startBuildingNodeObjectsForBone();
			void insertNodeObjectAndName(int nodeObject, std::wstring nodeName);

			void startBuildingAdjacentBonesForBone();

			void insertAdjacentBones(const std::set<int> &bonesForVertex);

			using boneGraphEdge = std::pair<int, int>;

			void startBuildingBoneGraphAdjacencyMap();

			void printBoneGraphAdjacencyMap();

			void startSkinPosesForBones();

			void insertSkinPosesForBones(int boneIndexOnSkin, const oxyde::DQ::dualQuat &skinPose);

			void startLocalTransformForNodes();

			void insertLocalTransformForNodes(std::wstring nodeName, const oxyde::DQ::dualQuat &localTransform);

			void processSkinPoseWithLocalTransform(INodePtr sceneRootNode);

			oxyde::DQ::dualQuat getFinalSkinPoseForBoneName(std::wstring boneName);
		}
	}
}
