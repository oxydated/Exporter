#include "processControllers.h"
#include "printUtilities.h"
#include "ikctrl.h"
#include <maxscript/foundation/numbers.h>

static char theString[100] = "";
static int indent = 1;

using namespace std;

void getControllerInformation(INode* theNode, FILE* expFile, IXMLDOMElement* theNodeElement ){
	setOutputFile( expFile );
	Control* theTMControl = theNode->GetTMController();
	if(theTMControl != NULL){
		Class_ID theTMControlClassID = theTMControl->ClassID();
		SClass_ID theTMControlSuperClassID = theTMControl->SuperClassID();
		IXMLDOMElement* theAnimationElement = insertAnimationForNode(theNodeElement);

		// for BIPED Controller
	
		IBipMaster12* theBipMaster = NULL;

		theBipMaster = (IBipMaster12*)theTMControl->GetInterface(IBipMaster12::I_BIPMASTER12);
		if (theBipMaster != NULL){
			++indent;
			DebugPrint("%*sit's a IBipMaster12\n", indent, " ");
			++indent;
			processBipedControl(theTMControl, theNode, theBipMaster, expFile, theAnimationElement);

			--indent;
			--indent;
		}

		///////////////////////
		visitController(theNode, theTMControl, expFile, theAnimationElement);

		DebugPrint("%*sParent Node TM for node %s\n", indent, " ", theNode->GetName());
		Matrix3 theMatrix = theNode->GetParentTM(0);
		DebugPrint("%*s		row 0: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z);
		DebugPrint("%*s		row 1: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z);
		DebugPrint("%*s		row 2: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z);
		DebugPrint("%*s		row 3: (%10f, %10f, %10f)\n\n", indent, " ", theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);

	} else {
		DebugPrint("%*sNo Control for node %s\n", indent, " ", theNode->GetName());
		Matrix3 theMatrix = theNode->GetNodeTM(0);
		DebugPrint("%*s		row 0: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z);
		DebugPrint("%*s		row 1: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z);
		DebugPrint("%*s		row 2: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z);
		DebugPrint("%*s		row 3: (%10f, %10f, %10f)\n\n", indent, " ", theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);
	}
}

void visitController(INode* theNode, Control* theControl, FILE* expFile, IXMLDOMElement* theAnimationElement){
	if(theControl == NULL) return;
	SClass_ID theControlSuperID = theControl->SuperClassID();
	Class_ID theControlClassID = theControl->ClassID();
	BOOL isLeaf = theControl->IsLeaf();
	
	IScriptCtrl* theScriptControl = NULL;
	theScriptControl = (IScriptCtrl*)theControl->GetInterface(IID_SCRIPT_CONTROL);
	if (theScriptControl != NULL){
		IBaseScriptControl8* theScript = (IBaseScriptControl8*)theControl;
		//IScriptCtrl* theNewScriptControl = (IScriptCtrl*)theScript->GetInterface(IID_SCRIPT_CONTROL);

		//MCHAR* theScriptText = theScript->get_script_text();

		int varCount = theScript->getVarCount();
		MSTR sourceVarName = _M("source");
		for (int i = 0; i < varCount; i++){
			MSTR &varName = theScript->getVarName(i);
			if (varName == sourceVarName){
				FPValue &theValue = theScript->getVarValue(i, 0);
				ParamType2 theType = theValue.type;
				if (theType == TYPE_INODE){
					char* sourceName = theValue.n->GetName(); 
					DebugPrint("%*sTarget: %s \n", indent, " ", theValue.n->GetName());
					int njkl = 0;
				}
			}
			int zerro = 0;
		}

		//FPInterfaceDesc* theDesc = theNewScriptControl->GetDescByID(IID_SCRIPT_CONTROL);
		//Interface_ID testID = theDesc->GetID();

		DebugPrint("%*sNode: %s ...is a scriptControl node\n", indent, " ", theNode->GetName());
		//MSTR theExpression = theScriptControl->GetExpression();
		//MSTR theScriptDescription = theScriptControl->GetDescription();
		MSTR theSourceVariableName = _M("source");
		BOOL bo = theScriptControl->VariableExists(theSourceVariableName);
		int theSourceVariableIndex = theScriptControl->GetVarIndex(theSourceVariableName);
		//Value* theSourceVariableIntegerIndex = Integer::intern(theSourceVariableIndex);
		//Value* theSourceVariableIntegerIndex = Integer::heap_intern(theSourceVariableIndex);
		Value* theSourceNodeVal = theScriptControl->GetVarValue(Integer::intern(theSourceVariableIndex+1), 0);
		INode* thesourceNode = theSourceNodeVal->to_node();
		char *theNodeName = thesourceNode->GetName();
		int ij = 0;
	}



	MSTR theControlClassName;
	theControl->GetClassName(theControlClassName);
	DebugPrint("%*sName of the control: %s ... ClassID: 0x%lX : 0x%lX\n", indent, " ", theControlClassName, theControlClassID.PartA(), theControlClassID.PartB());
	int numKeys = theControl->NumKeys();
	DebugPrint("%*sNumber of animation keys: %i\n", indent, " ", numKeys);
	Matrix3 theMatrix;
	IXMLDOMElement* theTrackElement = NULL;

	switch(theControlSuperID){
	case PRS_CONTROL_CLASS_ID:
		DebugPrint("%*sit's a PRS_CONTROL_CLASS_ID\n", indent, " ");
		break;
	case CTRL_MATRIX3_CLASS_ID:
		DebugPrint("%*sit's a CTRL_MATRIX3_CLASS_ID\n", indent, " ");
		theMatrix.IdentityMatrix();
		theControl->GetValue(0, (void*)&theMatrix, FOREVER, CTRL_RELATIVE);
		DebugPrint("%*s		row 0: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z);
		DebugPrint("%*s		row 1: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z);
		DebugPrint("%*s		row 2: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z);
		DebugPrint("%*s		row 3: (%10f, %10f, %10f)\n\n", indent, " ", theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);

		if (isLeaf != TRUE){
			++indent;
			Control* thePosControl = theControl->GetPositionController();
			if (thePosControl != NULL){
				visitController(theNode, thePosControl, expFile, theAnimationElement);
			}
			Control* theRotControl = theControl->GetRotationController();
			if (theRotControl != NULL){
				visitController(theNode, theRotControl, expFile, theAnimationElement);
			}
			Control* theScaleControl = theControl->GetScaleController();
			if (theScaleControl != NULL){
				visitController(theNode, theScaleControl, expFile, theAnimationElement);
			}
			--indent;
		}

		if (numKeys != NOT_KEYFRAMEABLE){
			++indent;
			theTrackElement = insertMatrixTrackForAnimation(theAnimationElement);
			processMatrixControl(theNode, theControl, expFile, theTrackElement);
			--indent;
		}
		else {
			DebugPrint("%*sIt hasn't a keyframe\n", indent, " ");
			++indent;
			theTrackElement = insertMatrixTrackForAnimation(theAnimationElement);
			processMatrixControl(theNode, theControl, expFile, theTrackElement);
			--indent;
		}

		break;
	case CTRL_FLOAT_CLASS_ID:
		DebugPrint("%*sit's a CTRL_FLOAT_CLASS_ID\n", indent, " ");
		break;
	case CTRL_POINT3_CLASS_ID:
		DebugPrint("%*sit's a CTRL_POINT3_CLASS_ID\n", indent, " ");
		break;
	case CTRL_POSITION_CLASS_ID:
		DebugPrint("%*sit's a CTRL_POSITION_CLASS_ID\n", indent, " ");
		++indent;
		theTrackElement = insertPositionTrackForAnimation(theAnimationElement);
		processXYZControl(theNode, theControl, expFile, theAnimationElement, theTrackElement);
		--indent;
		break;
	case CTRL_ROTATION_CLASS_ID:
		DebugPrint("%*sit's a CTRL_ROTATION_CLASS_ID\n", indent, " ");
		++indent;
		theTrackElement = insertRotationTrackForAnimation(theAnimationElement);
		processXYZControl(theNode, theControl, expFile, theAnimationElement, theTrackElement);
		--indent;
		break;
	case CTRL_SCALE_CLASS_ID:
		DebugPrint("%*sit's a CTRL_SCALE_CLASS_ID\n", indent, " ");
		++indent;
		theTrackElement = insertScaleTrackForAnimation(theAnimationElement);
		processScaleControl(theNode, theControl, expFile, theTrackElement);
		--indent;
		break;
	case CTRL_MORPH_CLASS_ID:
		DebugPrint("%*sit's a CTRL_MORPH_CLASS_ID\n", indent, " ");
		break;
	default:
		break;
	}
}


int getControllersKeys(Control* theControl, keySet& theKeys){
	int numKeysInserted = 0;
	IKeyControl* theKeyControl = GetKeyControlInterface(theControl);
	Class_ID theControlClassID = theControl->ClassID();

	if (theKeyControl != NULL){
		int numKeys = theKeyControl->GetNumKeys();
		if (numKeys == 0){
			DebugPrint("%*sit hasn't any key\n", indent, " ");
		}
		else {
			DebugPrint("%*sit has %i keys\n", indent, " ", numKeys);
		}
		for (int i = 0; i < numKeys; i++){
			IKey* aKey = NULL;
			if (theControlClassID == Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID, 0)){
				aKey = new IBezFloatKey();
			}
			if (theControlClassID == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0)){
				aKey = new IBezScaleKey();
			}

			if (aKey != NULL){
				theKeyControl->GetKey(i, aKey);
				theKeys.insert(aKey->time);
				numKeysInserted++;

				if (theControlClassID == Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID, 0)){
					//aKey = new IBezFloatKey();
					++indent;
					DebugPrint("%*sThe key in time[%i]: %f\n", indent, " ", aKey->time, ((IBezFloatKey*)aKey)->val);
					--indent;
				}
				delete aKey;
			}
		}
	} else {
		DebugPrint("%*sit hasn't a IKeyControl interface\n", indent, " ");
	}

	if (theControl->IsLeaf() == FALSE){
		DebugPrint("%*sit has subcontrollers\n", indent, " ");

	} else {
		DebugPrint("%*sit has NO subcontrollers at all\n", indent, " ");
	}
	return numKeysInserted;
}

void processXYZControl(INode* theNode, Control* theControl, FILE* expFile, IXMLDOMElement* theAnimationElement, IXMLDOMElement* theTrackElement){
	IEulerControl* theEulerControl = (IEulerControl*)theControl->GetInterface(I_EULERCTRL);
	keySet theKeys;
	theKeys.insert(0);
	if ((theEulerControl != NULL) || (theControl->ClassID() == POSITION_XYZ_CLASSID)){
		Control* theXControl = theControl->GetXController();
		if (theXControl != NULL){
			DebugPrint("%*s	theXControl\n", indent, " ");
			visitController(theNode, theXControl, expFile, theAnimationElement);
			getControllersKeys(theXControl, theKeys);
		}

		Control* theYControl = theControl->GetYController();
		if (theYControl != NULL){
			DebugPrint("%*s	theYControl\n", indent, " ");
			visitController(theNode, theYControl, expFile, theAnimationElement);
			getControllersKeys(theYControl, theKeys);
		}

		Control* theZControl = theControl->GetZController();
		if (theZControl != NULL){
			DebugPrint("%*s	theZControl\n", indent, " ");
			visitController(theNode, theZControl, expFile, theAnimationElement);
			getControllersKeys(theZControl, theKeys);
		}
	}
	insertNumKeysAttrForTrack(theTrackElement, theKeys.size());
	extractControlDataFromKeys(theNode, theKeys, theControl, theTrackElement);
}

void processScaleControl(INode* theNode, Control* theControl, FILE* expFile, IXMLDOMElement* theTrackElement){
	keySet theKeys;
	theKeys.insert(0);
	getControllersKeys(theControl, theKeys);
	insertNumKeysAttrForTrack(theTrackElement, theKeys.size());
	extractControlDataFromKeys(theNode, theKeys, theControl, theTrackElement);
}

void processMatrixControl(INode* theNode, Control* theControl, FILE* expFile, IXMLDOMElement* theTrackElement){
	keySet theKeys;
	if (theControl->NumKeys() != NOT_KEYFRAMEABLE)
	{
		for (int currentKey = 0; currentKey < theControl->NumKeys(); currentKey++){
			theKeys.insert(theControl->GetKeyTime(currentKey));
		}
	}
	else {
		theKeys.insert((TimeValue)0);
	}
	insertNumKeysAttrForTrack(theTrackElement, theKeys.size());
	extractControlDataFromKeys(theNode, theKeys, theControl, theTrackElement);
}

void extractControlDataFromKeys(INode* theNode, keySet& theKeys, Control* theControl, IXMLDOMElement* theTrackElement){
	for (keyIterator aKey = theKeys.begin(); aKey != theKeys.end(); aKey++){
		TimeValue theTime = (*aKey);
		Point3 thePoint;
		Quat theQuat;
		Matrix3 theMatrix;
		Matrix3 localMatrix;
		Point3 theEulerAngle;
		int theOrder = -1;
		char* theOrderString = NULL;
		ScaleValue theScale;

		switch (theControl->SuperClassID()){
		case CTRL_POSITION_CLASS_ID:
			getPositionFromControllerByTime(theControl, theTime, thePoint);
			DebugPrint("%*s		Time: %i, Point: (%f, %f, %f)\n", indent, " ", theTime, thePoint.x, thePoint.y, thePoint.z);
			insertPositionKeyForTrack(theTrackElement, theTime, thePoint.x, thePoint.y, thePoint.z);
			break;
		case CTRL_ROTATION_CLASS_ID:
			//getQuaternionFromControllerByTime(theControl, theTime, theQuat); 
			getEulerAnglesFromControllerByTime(theControl, theTime, theEulerAngle, theOrder);
			theOrderString = new char[5];
			eulerOrderString(&theOrderString, theOrder);
			insertRotationEulerKeyForTrack(theTrackElement, theTime, theOrderString, theEulerAngle.x, theEulerAngle.y, theEulerAngle.z);
			DebugPrint("%*s		Time: %i, Order: %s, Euler Angle: (%f, %f, %f)\n", indent, " ", theTime, theOrderString, theEulerAngle.x, theEulerAngle.y, theEulerAngle.z);
			//DebugPrint("%*s		Time: %i, Quat: [%f], (%f, %f, %f)\n", indent, " ", theTime, theQuat.w, theQuat.x, theQuat.y, theQuat.z);
			//insertRotationQuatKeyForTrack(theTrackElement, theTime, theQuat.w, theQuat.x, theQuat.y, theQuat.z);
			delete theOrderString;
			break;
		case CTRL_SCALE_CLASS_ID:
			getScaleFromControllerByTime(theControl, theTime, theScale);
			DebugPrint("%*s		Time: %i, Scale: (%f, %f, %f), Axis: [%f], (%f, %f, %f)\n", indent, " ", theTime, theScale.s.x, theScale.s.y, theScale.s.z, theScale.q.w, theScale.q.x, theScale.q.y, theScale.q.z);
			insertScaleKeyForTrack(theTrackElement, theTime, theScale.s.x, theScale.s.y, theScale.s.z, theScale.q.w, theScale.q.x, theScale.q.y, theScale.q.z);
			break;
		case CTRL_MATRIX3_CLASS_ID:
			getMatrixFromControllerByTime(theNode, theControl, theTime, theMatrix, localMatrix);
			DebugPrint("%*s		Time: %i, row 0: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z);
			DebugPrint("%*s		          row 1: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z);
			DebugPrint("%*s		          row 2: (%10f, %10f, %10f)\n", indent, " ", theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z);
			DebugPrint("%*s		          row 3: (%10f, %10f, %10f)\n\n", indent, " ", theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);

			insertMatrixKeyForTrack(theTrackElement, theTime,	theMatrix.GetRow(0).x, theMatrix.GetRow(0).y, theMatrix.GetRow(0).z,
																theMatrix.GetRow(1).x, theMatrix.GetRow(1).y, theMatrix.GetRow(1).z,
																theMatrix.GetRow(2).x, theMatrix.GetRow(2).y, theMatrix.GetRow(2).z,
																theMatrix.GetRow(3).x, theMatrix.GetRow(3).y, theMatrix.GetRow(3).z);

			insertMatrixKeyForTrack(theTrackElement, theTime,	localMatrix.GetRow(0).x, localMatrix.GetRow(0).y, localMatrix.GetRow(0).z,
																localMatrix.GetRow(1).x, localMatrix.GetRow(1).y, localMatrix.GetRow(1).z,
																localMatrix.GetRow(2).x, localMatrix.GetRow(2).y, localMatrix.GetRow(2).z,
																localMatrix.GetRow(3).x, localMatrix.GetRow(3).y, localMatrix.GetRow(3).z, true);
			break;
		default:
			break;
		}
	}
}

bool getQuaternionFromControllerByTime(Control* theControl, TimeValue t, Quat& theQuat){
	bool itReturnedAQuat = false;
	IEulerControl* theEulerControl = (IEulerControl*)theControl->GetInterface(I_EULERCTRL);
	if (theEulerControl != NULL){
		int theEulerType = theEulerControl->GetOrder();
		theControl->GetValue(t, (void*)&theQuat, FOREVER);
		itReturnedAQuat = true;
	}
	return itReturnedAQuat;
}

bool getEulerAnglesFromControllerByTime(Control* theControl, TimeValue t, Point3& theAngle, int& theOrder){
	bool itReturnedAAngle = false;
	IEulerControl* theEulerControl = (IEulerControl*)theControl->GetInterface(I_EULERCTRL);
	if (theEulerControl != NULL){
		theOrder = theEulerControl->GetOrder();

		Control* theXControl = theControl->GetXController();
		if (theXControl != NULL){
			//DebugPrint("%*s	theXControl\n", indent, " ");
			theXControl->GetValue(t, (void*)&(theAngle.x), FOREVER);
		}

		Control* theYControl = theControl->GetYController();
		if (theYControl != NULL){
			//DebugPrint("%*s	theXControl\n", indent, " ");
			theYControl->GetValue(t, (void*)&(theAngle.y), FOREVER);
		}

		Control* theZControl = theControl->GetZController();
		if (theZControl != NULL){
			//DebugPrint("%*s	theXControl\n", indent, " ");
			theZControl->GetValue(t, (void*)&(theAngle.z), FOREVER);
		}
		
		DebugPrint("%*s	the Euler angle: (%f, %f, %f)\n", indent, " ", theAngle.x, theAngle.y, theAngle.z );

		itReturnedAAngle = true;
	}
	return itReturnedAAngle;
}

bool getPositionFromControllerByTime(Control* theControl, TimeValue t, Point3& thePoint){
	bool itReturnedAPoint = false;
	if (theControl->SuperClassID() == CTRL_POSITION_CLASS_ID){
		theControl->GetValue(t, (void*)&thePoint, FOREVER);
		itReturnedAPoint = true;
	}
	return itReturnedAPoint;
}

bool getScaleFromControllerByTime(Control* theControl, TimeValue t, ScaleValue& theScale){
	bool itReturnedAScale = false;
	if (theControl->SuperClassID() == CTRL_SCALE_CLASS_ID){
		theControl->GetValue(t, (void*)&theScale, FOREVER);
		itReturnedAScale = true;
	}
	return itReturnedAScale;
}

bool getMatrixFromControllerByTime(INode* theNode, Control* theControl, TimeValue t, Matrix3& theMatrix, Matrix3& localMatrix){
	bool itReturnedAMatrix = false;
	if (theControl->SuperClassID() == CTRL_MATRIX3_CLASS_ID){
		theMatrix.IdentityMatrix();
		//Matrix3 localMat;
		localMatrix.IdentityMatrix();

		Matrix3 parentMat = theNode->GetParentTM(t);
		theControl->GetValue(t, (void*)&theMatrix, FOREVER, CTRL_RELATIVE);
		localMatrix = theMatrix * Inverse(parentMat);
		itReturnedAMatrix = true;
	}
	return itReturnedAMatrix;
}

void eulerOrderString(char** theOrderString, int theOrder){
	if (theOrderString != NULL){
		switch (theOrder){
		case EULERTYPE_XYZ:
			strcpy(*theOrderString, "XYZ");
			break;
		case EULERTYPE_XZY:
			strcpy(*theOrderString, "XZY");
			break;
		case EULERTYPE_YZX:
			strcpy(*theOrderString, "YZX");
			break;
		case EULERTYPE_YXZ:
			strcpy(*theOrderString, "YXZ");
			break;
		case EULERTYPE_ZXY:
			strcpy(*theOrderString, "ZXY");
			break;
		case EULERTYPE_ZYX:
			strcpy(*theOrderString, "ZYX");
			break;
		case EULERTYPE_XYX:
			strcpy(*theOrderString, "XYX");
			break;
		case EULERTYPE_YZY:
			strcpy(*theOrderString, "YZY");
			break;
		case EULERTYPE_ZXZ:
			strcpy(*theOrderString, "ZXZ");
			break;
		case EULERTYPE_RF:
			strcpy(*theOrderString, "RF");
			break;
		}
	}
}

void processBipedControl(Control* theControl, INode* theNode, IBipMaster12* theBipMaster, FILE* expFile, IXMLDOMElement* theTrackElement){
	keySet theKeys;
	for (int currentKey = 0; currentKey < theControl->NumKeys(); currentKey++){
		theKeys.insert(theControl->GetKeyTime(currentKey));
	}
	DebugPrint("%*snum of biped keys: [%i]\n", indent, " ", theKeys.size());
	//insertNumKeysAttrForTrack(theTrackElement, theKeys.size());
	//extractControlDataFromKeys(theKeys, theControl, theTrackElement);
	++indent;
	for (keyIterator aKey = theKeys.begin(); aKey != theKeys.end(); aKey++){
		TimeValue theTime = (*aKey);
		Point3 thePoint;
		Quat theQuat;
		ScaleValue theScale;

		thePoint = theBipMaster->GetBipedPos(theTime, theNode);
		DebugPrint("%*s		Time: %i, Point: (%f, %f, %f)\n", indent, " ", theTime, thePoint.x, thePoint.y, thePoint.z);
		//insertPositionKeyForTrack(theTrackElement, theTime, thePoint.x, thePoint.y, thePoint.z);

		theQuat = theBipMaster->GetBipedRot(theTime, theNode);
		DebugPrint("%*s		Time: %i, Quat: [%f], (%f, %f, %f)\n", indent, " ", theTime, theQuat.w, theQuat.x, theQuat.y, theQuat.z);
		//insertRotationQuatKeyForTrack(theTrackElement, theTime, theQuat.w, theQuat.x, theQuat.y, theQuat.z);
		
		theScale = theBipMaster->GetBipedScale(theTime, theNode);
		DebugPrint("%*s		Time: %i, Scale: (%f, %f, %f), Axis: [%f], (%f, %f, %f)\n", indent, " ", theTime, theScale.s.x, theScale.s.y, theScale.s.z, theScale.q.w, theScale.q.x, theScale.q.y, theScale.q.z);
		//insertScaleKeyForTrack(theTrackElement, theTime, theScale.s.x, theScale.s.y, theScale.s.z, theScale.q.w, theScale.q.x, theScale.q.y, theScale.q.z);
			
	}
	--indent;
}