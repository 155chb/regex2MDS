#include "strFormat.h"
#include "NFA2DFA.h"
#include "DFA2MDS.h"


/* listNode,h */
extern int printfData(listNode* head);

/* strFormat.h */
extern int scanfRegex();
extern int printfRegex();
extern int printfM();
extern int regexFormat();

/* stateListNFA,h */
extern int initStateListNFA();
extern int freeStateListNFA();
extern int printfStateNFA();

/* production.h */
extern int createNFA(int& startStateIndex, int& endStateIndex);

/* stateListDFA.h */
extern int initStateListDFA();
extern int freeStateListDFA();
extern int printfStateDFA();

/* NFA2DFA.h */
extern int createDFA(int startStateIndex, int endStateIndex);

/* stateListMDS.h */
extern int initStateListMDS();
extern int freeStateListMDS();
extern int printfStateMDS();

/* DFA2MDS.h */
extern int createMDS();


int Regex() {
	scanfRegex();
	printf("---------------------format---------------------\n");
	regexFormat();
	printfRegex();
	printfM();
	int startStateIndex, endStateIndex;
	printf("---------------------NFA---------------------\n");
	createNFA(startStateIndex, endStateIndex);
	printf("startStateIndex:%d\n", startStateIndex);
	printf("endStateIndex:%d\n", endStateIndex);
	printfStateNFA();
	printf("---------------------DFA---------------------\n");
	createDFA(startStateIndex, endStateIndex);
	printf("startStateIndex:0\n");
	printf("endStateIndex:");
	printfData(&endStateIndexListHeadDFA);
	printf("\n");
	printfStateDFA();
	printf("---------------------MDS---------------------\n");
	createMDS();
	printf("startStateIndex:0\n");
	printf("endStateIndex:");
	printfData(&endStateIndexListHeadMDS);
	printf("\n");
	printfStateMDS();
	return 0;
}


int main() {
	initStateListNFA();
	initStateListDFA();
	initStateListMDS();
	Regex();
	freeStateListNFA();
	freeStateListDFA();
	freeStateListMDS();
	system("pause");
	return 0;
}