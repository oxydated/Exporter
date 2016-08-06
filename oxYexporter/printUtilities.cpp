#include <string.h>
#include "printUtilities.h"

static FILE* outputFile = stdout;

static int indent = 0;

static char binStr[33] = "00000000000000000000000000000000";

void setOutputFile( FILE* outFile ) { outputFile = outFile;}

FILE* getoutFile() { return outputFile; }

void printToOutput( char* theString ){
	char indentation[100] = "";

	for( int i = 0; i < indent; i++){
		strcat( indentation, "	" );
	}

	char* next = strtok(theString, "\n");
	while( next != NULL){
		fprintf(outputFile,"%s%s\n", indentation, next );
		next = strtok( NULL, "\n");
	};	
}

int indentLine() { return indent++; }

int deIndentLine() { if (indent > 0 ) indent--; return indent; }


const char* printBinary( DWORD theWord ){
	for(int i=0; i<(8*sizeof(DWORD)); i++){
		binStr[32-i] = ( theWord & (1 << i) ) != 0 ? '1' : '0';
	}
	return binStr;
}