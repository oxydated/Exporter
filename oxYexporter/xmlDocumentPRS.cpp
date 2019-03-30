#include "xmlDocumentPRS.h"

namespace oxyde {
	namespace exporter {
		namespace XML {

			oxyRotationElement::oxyRotationElement(oxyPRSElementPtr  theParent, int in_eulerOrder) :oxyDocumentElement(theParent, L"Rotation"), eulerOrder(in_eulerOrder) {
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxyRotationElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"eulerOrder", _variant_t(eulerOrder)));
			}

			oxyBezierTrackElement::oxyBezierTrackElement(oxyDocumentElementPtr theParent, int in_numKeys) :oxyDocumentElement(theParent, L"BezierTrack"), numKeys(in_numKeys) {
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxyBezierTrackElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"numKeys", _variant_t(numKeys)));
			}

			oxyBezierKeyElement::oxyBezierKeyElement(oxyBezierTrackElementPtr theParent, int in_startTime, int in_endTime, float in_coeff_B3, float in_coeff_B2, float in_coeff_B1, float in_coeff_B0):
				oxyDocumentElement(theParent, L"BezierKey"), startTime(in_startTime), endTime(in_endTime), coeff_B3(in_coeff_B3), coeff_B2(in_coeff_B2), coeff_B1(in_coeff_B1), coeff_B0(in_coeff_B0)
			{
				buildListOfAttributes();
				setElementAttributes();
			}

			void oxyBezierKeyElement::buildListOfAttributes()
			{
				attributeList.push_back(elementAttribute(L"startTime", _variant_t(startTime)));
				attributeList.push_back(elementAttribute(L"endTime", _variant_t(endTime)));
				attributeList.push_back(elementAttribute(L"coeff_B3", variantFromFloat(coeff_B3)));
				attributeList.push_back(elementAttribute(L"coeff_B2", variantFromFloat(coeff_B2)));
				attributeList.push_back(elementAttribute(L"coeff_B1", variantFromFloat(coeff_B1)));
				attributeList.push_back(elementAttribute(L"coeff_B0", variantFromFloat(coeff_B0)));
			}
		}
	}
}