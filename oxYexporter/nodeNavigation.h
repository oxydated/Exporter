#ifndef __NODENAVIGATION_H__
#define __NODENAVIGATION_H__

#include <stack>
#include <utility>
//#include <msxml6.h>
#include "xmlDocument.h"
#include "xmlDocumentScene.h"
#include <stdio.h>
#include <max.h>

using namespace std;

//typedef pair<IXMLDOMElement*, INode*> nodePair;
typedef pair<oxyde::exporter::XML::oxyNodeKeeperPtr, INode*> nodePair;
typedef stack<nodePair> nodeStack;

//void visitNodes(INode* rootNode, TimeValue t, FILE* expFile = stdout);
void visitNodes(INode* rootNode, TimeValue t, oxyde::exporter::XML::oxyDocumentPtr theDocument);

#endif