#include "dllFunc.h"

HINSTANCE thisInstance = 0;
HINSTANCE getThisDLLModule(){ return thisInstance; }

BOOL WINAPI DllMain( __in HINSTANCE hInstDll, __in DWORD fdwReason, __in LPVOID lpvReserved ){
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
			thisInstance = hInstDll;
			DisableThreadLibraryCalls( thisInstance );
			return TRUE;
		break;
	case DLL_PROCESS_DETACH:
			return FALSE;
		break;
	case DLL_THREAD_ATTACH:
			return FALSE;
		break;
	case DLL_THREAD_DETACH:
			return FALSE;
		break;
	}
}

__declspec(dllexport) const TCHAR* LibDescription(){

	static TCHAR lpBuffer[256];
	LoadString( thisInstance, IDS_LIBDESCRIPTION_STRING, (LPTSTR)lpBuffer, 256);
	return lpBuffer;
}

__declspec(dllexport) int LibNumberClasses(){
	return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc( int i ){
	return new oxyExpDesc( thisInstance );
}

__declspec(dllexport) ULONG LibVersion(){
	return Get3DSMAXVersion();
}

__declspec(dllexport) int LibInitialize(){
	return TRUE;
}

__declspec(dllexport) int LibShutdown(){
	return TRUE;
}

