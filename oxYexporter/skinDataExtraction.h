#pragma once

#include "iskin.h"
#include <modstack.h>
#include <utility>
#include <map>
#include <vector>
#include <array>
#include "max.h"
#include "xmlDocument.h"
#include "auxiliaryLogger.h"

namespace oxyde {
	namespace exporter {
		namespace skin {

			class skinPoseCorrector {
				using dualQuat = std::array<double, 8>;
				using matrix4x4 = std::array<double, 16>;
				
			protected:

				INodePtr skeletonRoot;

				static std::map<INodePtr, dualQuat> nodeToLocalTransformDict;

				std::map<INodePtr, dualQuat> nodeToGlobalTransformDict;
				std::map<INodePtr, dualQuat> nodeToSkinPoseDict;
				std::map<INodePtr, matrix4x4> skinPoseMatricesDict;

			public:
				static void addLocalTransform(INodePtr boneNode, dualQuat localTransform) {
					nodeToLocalTransformDict[boneNode] = localTransform;

					oxyde::DQ::dualQuat localQuatfloat = { DUALQUAARRAY(localTransform) };
					oxyde::exporter::log::insertLocalTransformForNodes(boneNode->GetName(), localQuatfloat);
				}

				skinPoseCorrector(ISkin* theSkinInterface, INodePtr theSkinNode);

				matrix4x4 getSkinPoseMatrixForNode(INodePtr boneNode) {
					return skinPoseMatricesDict[boneNode];
				}

				dualQuat getSkinPosedualQuatForNode(INodePtr boneNode) {
					return nodeToSkinPoseDict[boneNode];
				}

			};


			class skinObjectsList {
			private:
				//std::map<INodePtr, skinObject> nodeToSkinMap;

				static skinObjectsList* theSingleton;

				std::vector<INodePtr> nodeToSkinVector;
				INodePtr sceneRootNode;

				const INodePtr getRootNode() { return sceneRootNode; }
				
				skinObjectsList(INodePtr theSceneRootNode):sceneRootNode(theSceneRootNode) {}

			public:
				static void initializeSkinObjectsList(INodePtr theSceneRootNode) {
					theSingleton = new skinObjectsList(theSceneRootNode);
				}

				static void addSkinObjectToList(INodePtr skinNode) {
					theSingleton->addSkinObject(skinNode);
				}

				static const INodePtr getRootNodeForScene() {
					return theSingleton->getRootNode();
				}

				void addSkinObject(INodePtr skinNode) {
					nodeToSkinVector.push_back(skinNode);
				}

				void buildSkinObjects( oxyde::exporter::XML::oxyDocumentPtr theDocument);

				static void buildSkinObjectsInList( oxyde::exporter::XML::oxyDocumentPtr theDocument) {
					theSingleton->buildSkinObjects(theDocument);
				}
			};
		}
	}
}
