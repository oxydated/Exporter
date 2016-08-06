#ifndef __OXYEXPDESC_H__
#define __OXYEXPDESC_H__

#include <iparamb2.h>
#include "oxyExp.h"

class oxyExpDesc: public ClassDesc{

public:
	oxyExpDesc();

	oxyExpDesc( HINSTANCE hInst );

	virtual int IsPublic();

	virtual void* Create( BOOL loading = FALSE );

	virtual const MCHAR* ClassName();

	virtual SClass_ID SuperClassID();

	virtual Class_ID ClassID();

	virtual const MCHAR* Category();

	virtual ~oxyExpDesc();

private:
	HINSTANCE hModule;

};

#endif