#ifndef __NODENAVIGATION_H__
#define __NODENAVIGATION_H__

#include <stack>
#include <utility>
#include <msxml6.h>
#include <stdio.h>
#include <max.h>

using namespace std;

//typedef stack<INode*> nodeStack;
typedef pair<IXMLDOMElement*, INode*> nodePair;
typedef stack<nodePair> nodeStack;

void visitNodes(INode* rootNode, TimeValue t, FILE* expFile = stdout);

#endif