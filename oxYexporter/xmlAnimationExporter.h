#ifndef __XMLANIMATIONEXPORTER_HEADER__
#define __XMLANIMATIONEXPORTER_HEADER__

#include "xmlExporter.h"
#include <max.h>

//<animaton>
//	<keyFrames>

IXMLDOMElement* insertAnimationForNode(IXMLDOMElement* theNode);

//		<positionTrack numKeys = "3">

IXMLDOMElement* insertPositionTrackForAnimation(IXMLDOMElement* theAnimationElement);

HRESULT insertNumKeysAttrForTrack(IXMLDOMElement* theTrackElement, int numKeys);

//			<positionKey time = "0" x = "22,037552" y = "-0,000006" z = "32,387222" / >
//			<positionKey time = "3520" x = "22,037552" y = "-88,648277" z = "69,299088" / >
//			<positionKey time = "10240" x = "22,037552" y = "-55,079979" z = "-42,108807" / >

IXMLDOMElement* insertPositionKeyForTrack(IXMLDOMElement* thePositionTrackElement, int time, float x, float y, float z);

//		< / positionTrack>
//		<rotationTrack numKeys = "3">

IXMLDOMElement* insertRotationTrackForAnimation(IXMLDOMElement* theAnimationElement);

//			<rotationKey time = "0" quatS = "0,673372" quatX = "-0,673372" quatY = "-0,215802" quatZ = "0,215802" / >
//			<rotationKey time = "1000" quatS = "0,5000" quatX = "-0,20000" quatY = "-0,215802" quatZ = "0,215802" / >
//			<rotationKey time = "2000" quatS = "0,5000" quatX = "0,40000" quatY = "-0,9802" quatZ = "0,215802" / >

IXMLDOMElement* insertRotationQuatKeyForTrack(IXMLDOMElement* theRotationTrackElement, int time, float quatS, float quatX, float quatY, float quatZ);

//			<rotationKey time = "2000" order = "XYZ" EulerX = "0,40000" EulerY = "-0,9802" EulerZ = "0,215802" / >

IXMLDOMElement* insertRotationEulerKeyForTrack(IXMLDOMElement* theRotationTrackElement, int time, TCHAR* order, float EulerX, float EulerY, float EulerZ);

//		< / rotationTrack>
//		<scaleTrack numKeys = "2">

IXMLDOMElement* insertScaleTrackForAnimation(IXMLDOMElement* theAnimationElement);

//			<scaleKey time = "4800" scaleX = "1,000000" scaleY = "1,000000" scaleZ = "1,000000" scaleQuatS = "1,000000" scaleQuatX = "0,000000" scaleQuatY = "0,000000" scaleQuatZ = "-0,000000" / >
//			<scaleKey time = "6400" scaleX = "1,000000" scaleY = "1,000000" scaleZ = "1,770371" scaleQuatS = "0,980785" scaleQuatX = "0,000000" scaleQuatY = "0,000000" scaleQuatZ = "-0,195090" / >

IXMLDOMElement* insertScaleKeyForTrack(IXMLDOMElement* theScaleTrackElement, int time, float scaleX, float scaleY, float scaleZ, float scaleQuatS, float scaleQuatX, float scaleQuatY, float scaleQuatZ);

//		< / scaleTrack>
//		<matrixTrack numKeys = "2">

IXMLDOMElement* insertMatrixTrackForAnimation(IXMLDOMElement* theAnimationElement);

//			<matrixKey time = "4800" l0c0="1.0" l0c1="1.0" l0c2="1.0" l1c0="1.0" l1c1="1.0" l1c2="1.0" l2c0="1.0" l2c1="1.0" l2c2="1.0" l3c0="1.0" l3c1="1.0" l3c2="1.0" / >
//			<matrixKey time = "6400" l0c0="1.0" l0c1="1.0" l0c2="1.0" l1c0="1.0" l1c1="1.0" l1c2="1.0" l2c0="1.0" l2c1="1.0" l2c2="1.0" l3c0="1.0" l3c1="1.0" l3c2="1.0" / >

IXMLDOMElement* insertMatrixKeyForTrack(IXMLDOMElement* theMatrixTrackElement, int time, float l0c0, float l0c1, float l0c2, float l1c0, float l1c1, float l1c2, float l2c0, float l2c1, float l2c2, float l3c0, float l3c1, float l3c2, bool isLocal=false);

//		< / matrixTrack>
//	< / keyFrames>


//	<dualQuatTrack numKeys = "4">

IXMLDOMElement* insertDualQuatTrackForAnimation(IXMLDOMElement* theAnimationElement, int numKeys);

//		<dualQuatKey endTime = "1920" startTime = "0">
//			<startingDualQuat dqs = "18.9034051923" dqx = "-1.80157197614" dqy = "4.73357663621" dqz = "18.3896327619" qs = "-0.704931942828" qx = "-0.0554221582912" qy = "0.0554220868291" qz = "0.704931012683" / >
//			<interpolationParam angle = "1.07338548242" mx = "5.19451577995" my = "-65.2207235594" mz = "26.2562274777" slide = "-3.22213938438" ux = "0.996814664527" uy = "0.0568904504045" uz = "-0.0558927655092" / >
//		< / dualQuatKey>

IXMLDOMElement* insertDualQuatKeyForTrack(IXMLDOMElement* theDualQuatTrackElement,
	int startTime, int endTime,
	float qs, float qx, float qy, float qz, float dqs, float dqx, float dqy, float dqz,
	float angle, float ux, float uy, float uz, float slide, float mx, float my, float mz
);

//	< / dualQuatTrack>

//	<baseSpinningBone source = "source" l0c0="1.0" l0c1="1.0" l0c2="1.0" l1c0="1.0" l1c1="1.0" l1c2="1.0" l2c0="1.0" l2c1="1.0" l2c2="1.0" l3c0="1.0" l3c1="1.0" l3c2="1.0" / >

IXMLDOMElement* insertBaseSpinningBoneForAnimation(IXMLDOMElement* theAnimationElement, _TCHAR* source, float l0c0, float l0c1, float l0c2, float l1c0, float l1c1, float l1c2, float l2c0, float l2c1, float l2c2, float l3c0, float l3c1, float l3c2);

//< / animaton>

#endif