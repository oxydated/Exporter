#ifndef __PROCESSCONTROLLERS_H_
#define __PROCESSCONTROLLERS_H_

#include <max.h>
#include <control.h>
#include <istdplug.h>
#include <set>
#include <CS/Biped12Api.h>
#include <maxscript/maxwrapper/scriptcontroller.h>

#include "xmlAnimationExporter.h"

using namespace std;

typedef set<TimeValue> keySet;
typedef keySet::iterator keyIterator;

#define POSITION_XYZ_CLASSID Class_ID(0x118F7E02, 0xFFEE238A)


void getControllerInformation(INode* theNode, FILE* expFile, IXMLDOMElement* theNodeElement);

void visitController(INode* theNode, Control* theControl, FILE* expFile, IXMLDOMElement* theAnimationElement);

keySet getControllersKeys(Control* theControl);

void processXYZControl(INode* theNode, Control* theControl, FILE* expFile, IXMLDOMElement* theAnimationElement, IXMLDOMElement* theTrackElement);

void processScaleControl(INode* theNode, Control* theControl, FILE* expFile, IXMLDOMElement* theTrackElement);

void processMatrixControl(INode* theNode, Control* theControl, FILE* expFile, IXMLDOMElement* theTrackElement);

void processBipedControl(Control* theControl, INode* theNode, IBipMaster12* theBipMaster, FILE* expFile, IXMLDOMElement* theTrackElement);

void processScriptControl(Control* theControl, IXMLDOMElement* theAnimationElement);

void processBaseSpinningBoneControl(Control* theControl, IXMLDOMElement* theAnimationElement);

//void processRotationControl(Control* theControl, FILE* expFile);

//void processPositionControl(Control* theControl, FILE* expFile);

void extractControlDataFromKeys(INode* theNode, keySet& theKeys, Control* theControl, IXMLDOMElement* theTrackElement);

bool getQuaternionFromControllerByTime(Control* theControl, TimeValue t, Quat& theQuat);

bool getEulerAnglesFromControllerByTime(Control* theControl, TimeValue t, Point3& theAngle, int& theOrder);

bool getPositionFromControllerByTime(Control* theControl, TimeValue t, Point3& thePoint);

bool getScaleFromControllerByTime(Control* theControl, TimeValue t, ScaleValue& theScale); 

bool getMatrixFromControllerByTime(INode* theNode, Control* theControl, TimeValue t, Matrix3& theMatrix, Matrix3& localMatrix);

void eulerOrderString(_TCHAR** theOrderString, int theOrder);

#endif