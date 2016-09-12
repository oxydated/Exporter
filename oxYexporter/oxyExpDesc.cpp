#include "oxyExpDesc.h"
#include "constants.h"

#define SETMCHARSTRING(ids)		static MCHAR lpBuffer[256];	\
								LoadString( hModule, ##ids##, (LPTSTR)lpBuffer, 256);	\
								return lpBuffer;

oxyExpDesc::oxyExpDesc(){}

oxyExpDesc::oxyExpDesc( HINSTANCE hInst ){ hModule = hInst;}

int oxyExpDesc::IsPublic() { return TRUE; }

void* oxyExpDesc::Create( BOOL loading ){
	DebugPrint(L"Creating the exporter plugin\n");
	return new oxyExp( hModule );
}

const MCHAR* oxyExpDesc::ClassName(){ SETMCHARSTRING(IDS_CLASSNAME_STRING) }

SClass_ID oxyExpDesc::SuperClassID() { return SCENE_EXPORT_CLASS_ID; }

Class_ID oxyExpDesc::ClassID() { return OXYEXP_CLASS_ID; }

const MCHAR* oxyExpDesc::Category() { return _M(""); }

oxyExpDesc::~oxyExpDesc() { }
