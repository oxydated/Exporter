#include "xmlDocumentAnimation.h"
namespace oxyde {
	namespace exporter {
		namespace XML {

			oxyDualQuatTrackElement::oxyDualQuatTrackElement(oxyKeyframeElementPtr theParent, int inputNumKeys)
				: oxyDocumentElement(theParent, L"dualQuatTrack"),
				numKeys(inputNumKeys)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxyDualQuatTrackElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"numKeys", _variant_t(numKeys)));
			}

			oxyDualQuatKeyElement::oxyDualQuatKeyElement(oxyDualQuatTrackElementPtr theParent, int inputstartTime, int inputendTime)
				: oxyDocumentElement(theParent, L"dualQuatKey"),
				startTime(inputstartTime),
				endTime(inputendTime)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			std::shared_ptr<oxyDualQuatKeyElement> oxyDualQuatKeyElement::createDualQuatKey(oxyDualQuatTrackElementPtr  theParent,
				int inputstartTime, int inputendTime,
				float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz,
				float inputangle, float inputux, float inputuy, float inputuz, float inputslide, float inputmx, float inputmy, float inputmz
			) {
				oxyDualQuatKeyElementPtr theKey = std::shared_ptr<oxyDualQuatKeyElement>(new oxyDualQuatKeyElement(theParent, inputstartTime, inputendTime));
				std::shared_ptr<startingDualQuatElement> theSDQelemnt = std::shared_ptr<startingDualQuatElement>(new startingDualQuatElement(
					theKey, inputqs, inputqx, inputqy, inputqz, inputdqs, inputdqx, inputdqy, inputdqz));
				std::shared_ptr<interpolationDataElement> theInterpData = std::shared_ptr<interpolationDataElement>(new interpolationDataElement(
					theKey, inputangle, inputux, inputuy, inputuz, inputslide, inputmx, inputmy, inputmz));
				return theKey;
			}

			void oxyDualQuatKeyElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"startTime", _variant_t(startTime)));
				attributeList.push_back(elementAttribute(L"endTime", _variant_t(endTime)));
			}

			startingDualQuatElement::startingDualQuatElement(oxyDualQuatKeyElementPtr theParent, float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz) :
				oxyDocumentElement(theParent, L"startingDualQuat"),
				qs(inputqs), qx(inputqx), qy(inputqy), qz(inputqz), dqs(inputdqs), dqx(inputdqx), dqy(inputdqy), dqz(inputdqz)
			{
				buildListOfAttributes();
				setElementAttributes();
			}
			void startingDualQuatElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"qs", variantFromFloat(qs)));
				attributeList.push_back(elementAttribute(L"qx", variantFromFloat(qx)));
				attributeList.push_back(elementAttribute(L"qy", variantFromFloat(qy)));
				attributeList.push_back(elementAttribute(L"qz", variantFromFloat(qz)));
				attributeList.push_back(elementAttribute(L"dqs", variantFromFloat(dqs)));
				attributeList.push_back(elementAttribute(L"dqx", variantFromFloat(dqx)));
				attributeList.push_back(elementAttribute(L"dqy", variantFromFloat(dqy)));
				attributeList.push_back(elementAttribute(L"dqz", variantFromFloat(dqz)));
			}
			interpolationDataElement::interpolationDataElement(oxyDualQuatKeyElementPtr theParent, float inputangle, float inputux, float inputuy, float inputuz, float inputslide, float inputmx, float inputmy, float inputmz) :
				oxyDocumentElement(theParent, L"interpolationParam"),
				angle(inputangle), ux(inputux), uy(inputuy), uz(inputuz), slide(inputslide), mx(inputmx), my(inputmy), mz(inputmz)
			{
				buildListOfAttributes();
				setElementAttributes();
			}
			void interpolationDataElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"angle", variantFromFloat(angle)));
				attributeList.push_back(elementAttribute(L"ux", variantFromFloat(ux)));
				attributeList.push_back(elementAttribute(L"uy", variantFromFloat(uy)));
				attributeList.push_back(elementAttribute(L"uz", variantFromFloat(uz)));
				attributeList.push_back(elementAttribute(L"slide", variantFromFloat(slide)));
				attributeList.push_back(elementAttribute(L"mx", variantFromFloat(mx)));
				attributeList.push_back(elementAttribute(L"my", variantFromFloat(my)));
				attributeList.push_back(elementAttribute(L"mz", variantFromFloat(mz)));
			}

			baseSpinnerDataElement::baseSpinnerDataElement(oxyAnimationElementPtr theParent, std::wstring insourceName, float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz) :
				oxyDocumentElement(theParent, L"spinnerBase"), sourceName(insourceName),
				qs(inputqs), qx(inputqx), qy(inputqy), qz(inputqz), dqs(inputdqs), dqx(inputdqx), dqy(inputdqy), dqz(inputdqz)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void baseSpinnerDataElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"source", _variant_t(sourceName.data())));

				attributeList.push_back(elementAttribute(L"qs", variantFromFloat(qs)));
				attributeList.push_back(elementAttribute(L"qx", variantFromFloat(qx)));
				attributeList.push_back(elementAttribute(L"qy", variantFromFloat(qy)));
				attributeList.push_back(elementAttribute(L"qz", variantFromFloat(qz)));
				attributeList.push_back(elementAttribute(L"dqs", variantFromFloat(dqs)));
				attributeList.push_back(elementAttribute(L"dqx", variantFromFloat(dqx)));
				attributeList.push_back(elementAttribute(L"dqy", variantFromFloat(dqy)));
				attributeList.push_back(elementAttribute(L"dqz", variantFromFloat(dqz)));
			}

			tipSpinnerDataElement::tipSpinnerDataElement(oxyAnimationElementPtr theParent, std::wstring insourceName, float inputqs, float inputqx, float inputqy, float inputqz, float inputdqs, float inputdqx, float inputdqy, float inputdqz) :
				oxyDocumentElement(theParent, L"spinnerTip"), sourceName(insourceName),
				qs(inputqs), qx(inputqx), qy(inputqy), qz(inputqz), dqs(inputdqs), dqx(inputdqx), dqy(inputdqy), dqz(inputdqz)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void tipSpinnerDataElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"source", _variant_t(sourceName.data())));

				attributeList.push_back(elementAttribute(L"qs", variantFromFloat(qs)));
				attributeList.push_back(elementAttribute(L"qx", variantFromFloat(qx)));
				attributeList.push_back(elementAttribute(L"qy", variantFromFloat(qy)));
				attributeList.push_back(elementAttribute(L"qz", variantFromFloat(qz)));
				attributeList.push_back(elementAttribute(L"dqs", variantFromFloat(dqs)));
				attributeList.push_back(elementAttribute(L"dqx", variantFromFloat(dqx)));
				attributeList.push_back(elementAttribute(L"dqy", variantFromFloat(dqy)));
				attributeList.push_back(elementAttribute(L"dqz", variantFromFloat(dqz)));
			}
		}
	}
}
