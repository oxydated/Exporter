#ifndef __PRINTUTILITIES_H__
#define __PRINTUTILITIES_H__

#include <stdio.h>
#include <Windows.h>

void setOutputFile( FILE* outFile = stdout );

FILE* getoutFile();

void printToOutput( char* theString );

int indentLine();

int deIndentLine();

const char* printBinary( DWORD theWord );

#endif