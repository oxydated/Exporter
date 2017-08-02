#include <string>
#include <sstream>
#include "xmlAnimationExporter.h"

IXMLDOMElement* insertAnimationForNode(IXMLDOMElement* theNode){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theAnimatonElement = NULL;
	hr = theDocument->createElement(L"animaton", &theAnimatonElement);
	hr = theNode->appendChild(theAnimatonElement, NULL);
	IXMLDOMElement* theKeyFramesElement = NULL;
	hr = theDocument->createElement(L"keyFrames", &theKeyFramesElement);
	hr = theAnimatonElement->appendChild(theKeyFramesElement, NULL);

	return theAnimatonElement;
}

IXMLDOMElement* insertPositionTrackForAnimation(IXMLDOMElement* theAnimationElement){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMNode* theKeyFramesNode = NULL;
	hr = theAnimationElement->selectSingleNode(SysAllocString(L"./keyFrames"), &theKeyFramesNode);
	if (hr == S_FALSE) return NULL;
	IXMLDOMElement* theKeyFramesElement = NULL;
	hr = theKeyFramesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theKeyFramesElement);

	IXMLDOMElement* thePositionTrackElement = NULL;
	hr = theDocument->createElement(L"positionTrack", &thePositionTrackElement);

	hr = theKeyFramesElement->appendChild(thePositionTrackElement, NULL);
	return thePositionTrackElement;
}

IXMLDOMElement* insertRotationTrackForAnimation(IXMLDOMElement* theAnimationElement){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMNode* theKeyFramesNode = NULL;
	hr = theAnimationElement->selectSingleNode(SysAllocString(L"./keyFrames"), &theKeyFramesNode);
	if (hr == S_FALSE) return NULL;
	IXMLDOMElement* theKeyFramesElement = NULL;
	hr = theKeyFramesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theKeyFramesElement);

	IXMLDOMElement* theRotationTrackElement = NULL;
	hr = theDocument->createElement(L"rotationTrack", &theRotationTrackElement);

	hr = theKeyFramesElement->appendChild(theRotationTrackElement, NULL);
	return theRotationTrackElement;
}

IXMLDOMElement* insertScaleTrackForAnimation(IXMLDOMElement* theAnimationElement){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMNode* theKeyFramesNode = NULL;
	hr = theAnimationElement->selectSingleNode(SysAllocString(L"./keyFrames"), &theKeyFramesNode);
	if (hr == S_FALSE) return NULL;
	IXMLDOMElement* theKeyFramesElement = NULL;
	hr = theKeyFramesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theKeyFramesElement);

	IXMLDOMElement* theScaleTrackElement = NULL;
	hr = theDocument->createElement(L"scaleTrack", &theScaleTrackElement);

	hr = theKeyFramesElement->appendChild(theScaleTrackElement, NULL);
	return theScaleTrackElement;
}

IXMLDOMElement* insertMatrixTrackForAnimation(IXMLDOMElement* theAnimationElement){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMNode* theKeyFramesNode = NULL;
	hr = theAnimationElement->selectSingleNode(SysAllocString(L"./keyFrames"), &theKeyFramesNode);
	if (hr == S_FALSE) return NULL;
	IXMLDOMElement* theKeyFramesElement = NULL;
	hr = theKeyFramesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theKeyFramesElement);

	IXMLDOMElement* theMatrixTrackElement = NULL;
	hr = theDocument->createElement(L"matrixTrack", &theMatrixTrackElement);

	hr = theKeyFramesElement->appendChild(theMatrixTrackElement, NULL);
	return theMatrixTrackElement;
}

HRESULT insertNumKeysAttrForTrack(IXMLDOMElement* theTrackElement, int numKeys){
	return theTrackElement->setAttribute(SysAllocString(L"numKeys"), _variant_t(numKeys));
}

IXMLDOMElement* insertDualQuatTrackForAnimation(IXMLDOMElement* theAnimationElement, int numKeys) {
	/*
		<dualQuatTrack numKeys="4">
	*/
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMNode* theKeyFramesNode = NULL;
	hr = theAnimationElement->selectSingleNode(SysAllocString(L"./keyFrames"), &theKeyFramesNode);
	if (hr == S_FALSE) return NULL;

	IXMLDOMElement* theKeyFramesElement = NULL;
	hr = theKeyFramesNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theKeyFramesElement);

	IXMLDOMElement* theDualQuatTrackElement = NULL;
	hr = theDocument->createElement(L"dualQuatTrack", &theDualQuatTrackElement);
	if (hr == S_OK) {
		hr = theDualQuatTrackElement->setAttribute(SysAllocString(L"numKeys"), _variant_t(numKeys));
	}

	hr = theKeyFramesElement->appendChild(theDualQuatTrackElement, NULL);

	return theDualQuatTrackElement;
}

IXMLDOMElement* insertPositionKeyForTrack(IXMLDOMElement* thePositionTrackElement, int time, float x, float y, float z){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* thePositionKeyElement = NULL;

	//			<positionKey time = "0" x = "22,037552" y = "-0,000006" z = "32,387222" / >
	hr = theDocument->createElement(L"positionKey", &thePositionKeyElement);
	hr = thePositionTrackElement->appendChild(thePositionKeyElement, NULL);
	hr = thePositionKeyElement->setAttribute(SysAllocString(L"time"), _variant_t(time));
	hr = thePositionKeyElement->setAttribute(SysAllocString(L"x"), _variant_t(x));
	hr = thePositionKeyElement->setAttribute(SysAllocString(L"y"), _variant_t(y));
	hr = thePositionKeyElement->setAttribute(SysAllocString(L"z"), _variant_t(z));

	return thePositionKeyElement;
}

IXMLDOMElement* insertRotationQuatKeyForTrack(IXMLDOMElement* theRotationTrackElement, int time, float quatS, float quatX, float quatY, float quatZ){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theRotationKeyElement = NULL;

	//			<rotationKey time = "0" quatS = "0,673372" quatX = "-0,673372" quatY = "-0,215802" quatZ = "0,215802" / >
	hr = theDocument->createElement(L"rotationKey", &theRotationKeyElement);
	hr = theRotationTrackElement->appendChild(theRotationKeyElement, NULL);
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"time"), _variant_t(time));
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"quatS"), _variant_t(quatS));
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"quatX"), _variant_t(quatX));
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"quatY"), _variant_t(quatY));
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"quatZ"), _variant_t(quatZ));

	return theRotationKeyElement;
}

IXMLDOMElement* insertRotationEulerKeyForTrack(IXMLDOMElement* theRotationTrackElement, int time, TCHAR* order, float EulerX, float EulerY, float EulerZ){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theRotationKeyElement = NULL;

	//			<rotationKey time = "2000" order = "XYZ" EulerX = "0,40000" EulerY = "-0,9802" EulerZ = "0,215802" / >
	hr = theDocument->createElement(L"rotationKey", &theRotationKeyElement);
	hr = theRotationTrackElement->appendChild(theRotationKeyElement, NULL);
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"time"), _variant_t(time));
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"order"), _variant_t(order));
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"EulerX"), _variant_t(EulerX));
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"EulerY"), _variant_t(EulerY));
	hr = theRotationKeyElement->setAttribute(SysAllocString(L"EulerZ"), _variant_t(EulerZ));

	return theRotationKeyElement;
}

IXMLDOMElement* insertScaleKeyForTrack(IXMLDOMElement* theScaleTrackElement, int time, float scaleX, float scaleY, float scaleZ, float scaleQuatS, float scaleQuatX, float scaleQuatY, float scaleQuatZ){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theScaleKeyElement = NULL;
	//			<scaleKey time = "4800" scaleX = "1,000000" scaleY = "1,000000" scaleZ = "1,000000" scaleQuatS = "1,000000" scaleQuatX = "0,000000" scaleQuatY = "0,000000" scaleQuatZ = "-0,000000" / >
	hr = theDocument->createElement(L"scaleKey", &theScaleKeyElement);
	hr = theScaleTrackElement->appendChild(theScaleKeyElement, NULL);
	hr = theScaleKeyElement->setAttribute(SysAllocString(L"time"), _variant_t(time));
	hr = theScaleKeyElement->setAttribute(SysAllocString(L"scaleX"), _variant_t(scaleX));
	hr = theScaleKeyElement->setAttribute(SysAllocString(L"scaleY"), _variant_t(scaleY));
	hr = theScaleKeyElement->setAttribute(SysAllocString(L"scaleZ"), _variant_t(scaleZ));

	hr = theScaleKeyElement->setAttribute(SysAllocString(L"scaleQuatS"), _variant_t(scaleQuatS));
	hr = theScaleKeyElement->setAttribute(SysAllocString(L"scaleQuatX"), _variant_t(scaleQuatX));
	hr = theScaleKeyElement->setAttribute(SysAllocString(L"scaleQuatY"), _variant_t(scaleQuatY));
	hr = theScaleKeyElement->setAttribute(SysAllocString(L"scaleQuatZ"), _variant_t(scaleQuatZ));

	return theScaleKeyElement;
}

IXMLDOMElement* insertMatrixKeyForTrack(IXMLDOMElement* theMatrixTrackElement, int time, float l0c0, float l0c1, float l0c2, float l1c0, float l1c1, float l1c2, float l2c0, float l2c1, float l2c2, float l3c0, float l3c1, float l3c2, bool isLocal){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theMatrixKeyElement = NULL;
	//			<matrixKey time = "4800" l0c0="1.0" l0c1="1.0" l0c2="1.0" l1c0="1.0" l1c1="1.0" l1c2="1.0" l2c0="1.0" l2c1="1.0" l2c2="1.0" l3c0="1.0" l3c1="1.0" l3c2="1.0" / >
	if (isLocal){
		hr = theDocument->createElement(L"localMatrixKey", &theMatrixKeyElement);
	}
	else {
		hr = theDocument->createElement(L"matrixKey", &theMatrixKeyElement);
	}
	hr = theMatrixTrackElement->appendChild(theMatrixKeyElement, NULL);
	hr = theMatrixKeyElement->setAttribute(SysAllocString(L"time"), _variant_t(time));

	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l0c0"), _variant_t(l0c0));
	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l0c1"), _variant_t(l0c1));
	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l0c2"), _variant_t(l0c2));

	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l1c0"), _variant_t(l1c0));
	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l1c1"), _variant_t(l1c1));
	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l1c2"), _variant_t(l1c2));

	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l2c0"), _variant_t(l2c0));
	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l2c1"), _variant_t(l2c1));
	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l2c2"), _variant_t(l2c2));

	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l3c0"), _variant_t(l3c0));
	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l3c1"), _variant_t(l3c1));
	//hr = theMatrixKeyElement->setAttribute(SysAllocString(L"l3c2"), _variant_t(l3c2));


	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l0c0", l0c0);
	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l0c1", l0c1);
	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l0c2", l0c2);

	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l1c0", l1c0);
	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l1c1", l1c1);
	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l1c2", l1c2);

	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l2c0", l2c0);
	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l2c1", l2c1);
	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l2c2", l2c2);

	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l3c0", l3c0);
	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l3c1", l3c1);
	hr = oxyde::exporter::XML::insertFloatAttributeInElement(theMatrixKeyElement, L"l3c2", l3c2);

	return theMatrixKeyElement;
}

IXMLDOMElement* insertDualQuatKeyForTrack(IXMLDOMElement* theDualQuatTrackElement,
	int startTime,  int endTime,
	float qs, float qx, float qy, float qz, float dqs, float dqx, float dqy, float dqz,
	float angle, float ux, float uy, float uz, float slide, float mx, float my, float mz
) 
{	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();
	IXMLDOMElement* theDualQuatKeyElement = NULL;

	//		<dualQuatKey endTime = "1920" startTime = "0">

	hr = theDocument->createElement(L"dualQuatKey", &theDualQuatKeyElement);
	hr = theDualQuatKeyElement->setAttribute(SysAllocString(L"startTime"), _variant_t(startTime));
	hr = theDualQuatKeyElement->setAttribute(SysAllocString(L"endTime"), _variant_t(endTime));

	//			<startingDualQuat dqs = "18.9034051923" dqx = "-1.80157197614" dqy = "4.73357663621" dqz = "18.3896327619" qs = "-0.704931942828" qx = "-0.0554221582912" qy = "0.0554220868291" qz = "0.704931012683" / >

	std::wstring outStr;
	std::wstring resultString;
	std::wostringstream outStream(outStr);
	
	IXMLDOMElement* theStartingDualQuatElement = NULL;
	hr = theDocument->createElement(L"startingDualQuat", &theStartingDualQuatElement);

	oxyde::exporter::XML::insertFloatAttributeInElement(theStartingDualQuatElement, L"qs", qs);
	oxyde::exporter::XML::insertFloatAttributeInElement(theStartingDualQuatElement, L"qx", qx);
	oxyde::exporter::XML::insertFloatAttributeInElement(theStartingDualQuatElement, L"qy", qy);
	oxyde::exporter::XML::insertFloatAttributeInElement(theStartingDualQuatElement, L"qz", qz);
	oxyde::exporter::XML::insertFloatAttributeInElement(theStartingDualQuatElement, L"dqs", dqs);
	oxyde::exporter::XML::insertFloatAttributeInElement(theStartingDualQuatElement, L"dqx", dqx);
	oxyde::exporter::XML::insertFloatAttributeInElement(theStartingDualQuatElement, L"dqy", dqy);
	oxyde::exporter::XML::insertFloatAttributeInElement(theStartingDualQuatElement, L"dqz", dqz);

	//outStream << std::fixed << qs;
	//resultString = outStream.str();
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"qs"), _variant_t(resultString.c_str()));
	//outStream.str(outStr);

	//outStream << std::fixed << qx;
	//resultString = outStream.str();
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"qx"), _variant_t(resultString.c_str()));
	//outStream.str(outStr);

	//outStream << std::fixed << qy;
	//resultString = outStream.str();
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"qy"), _variant_t(resultString.c_str()));
	//outStream.str(outStr);

	//outStream << std::fixed << qz;
	//resultString = outStream.str();
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"qz"), _variant_t(resultString.c_str()));
	//outStream.str(outStr);

	//outStream << std::fixed << dqs;
	//resultString = outStream.str();
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"dqs"), _variant_t(resultString.c_str()));
	//outStream.str(outStr);

	//outStream << std::fixed << dqx;
	//resultString = outStream.str();
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"dqx"), _variant_t(resultString.c_str()));
	//outStream.str(outStr);

	//outStream << std::fixed << dqy;
	//resultString = outStream.str();
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"dqy"), _variant_t(resultString.c_str()));
	//outStream.str(outStr);

	//outStream << std::fixed << dqz;
	//resultString = outStream.str();
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"dqz"), _variant_t(resultString.c_str()));
	//outStream.str(outStr);


	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"qs"), _variant_t(qs));
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"qx"), _variant_t(qx));
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"qy"), _variant_t(qy));
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"qz"), _variant_t(qz));
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"dqs"), _variant_t(dqs));
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"dqx"), _variant_t(dqx));
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"dqy"), _variant_t(dqy));
	//hr = theStartingDualQuatElement->setAttribute(SysAllocString(L"dqz"), _variant_t(dqz));
	hr = theDualQuatKeyElement->appendChild(theStartingDualQuatElement, NULL);

	//			<interpolationParam angle = "1.07338548242" mx = "5.19451577995" my = "-65.2207235594" mz = "26.2562274777" slide = "-3.22213938438" ux = "0.996814664527" uy = "0.0568904504045" uz = "-0.0558927655092" / >
	
	IXMLDOMElement* theInterpolationParamElement = NULL;
	hr = theDocument->createElement(L"interpolationParam", &theInterpolationParamElement);

	//hr = theInterpolationParamElement->setAttribute(SysAllocString(L"angle"), _variant_t(angle));

	oxyde::exporter::XML::insertFloatAttributeInElement(theInterpolationParamElement, L"angle", angle);
	
	//hr = theInterpolationParamElement->setAttribute(SysAllocString(L"ux"), _variant_t(ux));
	//hr = theInterpolationParamElement->setAttribute(SysAllocString(L"uy"), _variant_t(uy));
	//hr = theInterpolationParamElement->setAttribute(SysAllocString(L"uz"), _variant_t(uz));

	oxyde::exporter::XML::insertFloatAttributeInElement(theInterpolationParamElement, L"ux", ux);
	oxyde::exporter::XML::insertFloatAttributeInElement(theInterpolationParamElement, L"uy", uy);
	oxyde::exporter::XML::insertFloatAttributeInElement(theInterpolationParamElement, L"uz", uz);

	//hr = theInterpolationParamElement->setAttribute(SysAllocString(L"slide"), _variant_t(slide));

	oxyde::exporter::XML::insertFloatAttributeInElement(theInterpolationParamElement, L"slide", slide);

	//hr = theInterpolationParamElement->setAttribute(SysAllocString(L"mx"), _variant_t(mx));
	//hr = theInterpolationParamElement->setAttribute(SysAllocString(L"my"), _variant_t(my));
	//hr = theInterpolationParamElement->setAttribute(SysAllocString(L"mz"), _variant_t(mz));

	oxyde::exporter::XML::insertFloatAttributeInElement(theInterpolationParamElement, L"mx", mx);
	oxyde::exporter::XML::insertFloatAttributeInElement(theInterpolationParamElement, L"my", my);
	oxyde::exporter::XML::insertFloatAttributeInElement(theInterpolationParamElement, L"mz", mz);

	hr = theDualQuatKeyElement->appendChild(theInterpolationParamElement, NULL);

	//		< / dualQuatKey>

	hr = theDualQuatTrackElement->appendChild(theDualQuatKeyElement, NULL);

	return theDualQuatKeyElement;
}

IXMLDOMElement * insertBaseSpinningBoneForAnimation(IXMLDOMElement * theAnimationElement, _TCHAR * source, float l0c0, float l0c1, float l0c2, float l1c0, float l1c1, float l1c2, float l2c0, float l2c1, float l2c2, float l3c0, float l3c1, float l3c2)
{
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	theDocument = getTheDocument();

	IXMLDOMElement* theBaseSpinningBoneElement = NULL;
	hr = theDocument->createElement(L"baseSpinningBone", &theBaseSpinningBoneElement);

	hr = theAnimationElement->appendChild(theBaseSpinningBoneElement, NULL);

	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"source"), _variant_t(source));

	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l0c0"), _variant_t(l0c0));
	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l0c1"), _variant_t(l0c1));
	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l0c2"), _variant_t(l0c2));

	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l1c0"), _variant_t(l1c0));
	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l1c1"), _variant_t(l1c1));
	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l1c2"), _variant_t(l1c2));

	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l2c0"), _variant_t(l2c0));
	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l2c1"), _variant_t(l2c1));
	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l2c2"), _variant_t(l2c2));

	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l3c0"), _variant_t(l3c0));
	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l3c1"), _variant_t(l3c1));
	hr = theBaseSpinningBoneElement->setAttribute(SysAllocString(L"l3c2"), _variant_t(l3c2));

	return theBaseSpinningBoneElement;
}
