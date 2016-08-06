#ifndef __OXYEXP_H__
#define __OXYEXP_H__

#include <max.h>
#include <impexp.h>
#include <Windows.h>
#include "resource.h"

class oxyExp: public SceneExport{
public:
	oxyExp();

	oxyExp(HINSTANCE hInst);

	virtual int ExtCount();

	virtual const MCHAR* Ext(int n);
	
	virtual const MCHAR* LongDesc();
	
	virtual const MCHAR* ShortDesc();
	
	virtual const MCHAR* AuthorName();
	
	virtual const MCHAR* CopyrightMessage();
	
	virtual const MCHAR* OtherMessage1();
	
	virtual const MCHAR* OtherMessage2();
	
	virtual unsigned int Version();
	
	virtual void ShowAbout(HWND hWnd);
	
	virtual int DoExport( const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
	
	virtual BOOL supportOptions( int ext, DWORD options );
	
	virtual ~oxyExp();

private:
	HINSTANCE hModule;

};

#endif