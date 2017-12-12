#include <stdio.h>
#include "oxyExp.h"
#include "nodeNavigation.h"
//#include "xmlExporter.h"
#include "xmlDocument.h"

#define SETMCHARSTRING(ids)		static MCHAR lpBuffer[256];	\
								LoadString( hModule, ##ids##, (LPTSTR)lpBuffer, 256);	\
								return lpBuffer;

oxyExp::oxyExp(){ }

oxyExp::oxyExp( HINSTANCE hInst ){ hModule = hInst;}

int oxyExp::ExtCount(){ return 1; }

const MCHAR* oxyExp::Ext(int n){
		
	/*MCHAR lpBuffer[256];
	LoadString( hModule, IDS_EXTENSION_STRING, (LPTSTR)lpBuffer, 256);
	return lpBuffer;*/

	if( n == 0 ){
		return _T("OXY");
	} else {
		return _T("");
	}

	//SETMCHARSTRING(IDS_EXTENSION_STRING)
}
	
const MCHAR* oxyExp::LongDesc(){

	SETMCHARSTRING(IDS_LONGDESC_STRING)
}
	
const MCHAR* oxyExp::ShortDesc(){

	SETMCHARSTRING(IDS_SHORTDESC_STRING)
}
	
const MCHAR* oxyExp::AuthorName(){

	SETMCHARSTRING(IDS_AUTHORNAME_STRING)
}
	
const MCHAR* oxyExp::CopyrightMessage(){

	SETMCHARSTRING(IDS_COPYRIGHT_MESSAGE_STRING)
}
	
const MCHAR* oxyExp::OtherMessage1(){

	SETMCHARSTRING(IDS_LONGDESC_STRING)
}
	
const MCHAR* oxyExp::OtherMessage2(){

	SETMCHARSTRING(IDS_LONGDESC_STRING)
}
	
unsigned int oxyExp::Version(){
	return 100;
}
	
void oxyExp::ShowAbout(HWND hWnd){
	MessageBox( hWnd, (LPCWSTR)L"oxYde exporter", (LPCWSTR)L"oxyExporter", MB_OK );
}
	
int oxyExp::DoExport( const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options){
	FILE* expFile = fopen( "debug.yxo", "w");
	if(expFile != NULL){
		DebugPrint(L"It worked so far...\n");
		INode* rootNode = i->GetRootNode();
		oxyde::exporter::XML::oxyDocument::createDocument();
		oxyde::exporter::XML::oxyDocumentPtr theDocument = oxyde::exporter::XML::oxyDocument::getTheInstance();
		visitNodes(rootNode, i->GetTime(), theDocument);
		theDocument->save(std::wstring(name));
		//closeFile(name);
		oxyde::exporter::XML::oxyDocument::releaseDocument();

		fclose(expFile);

		return IMPEXP_SUCCESS;
	}

	return IMPEXP_FAIL;
}
	
BOOL oxyExp::supportOptions( int ext, DWORD options ){
	return FALSE;
}
	
oxyExp::~oxyExp(){ }