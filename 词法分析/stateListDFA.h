#pragma once
#include "listNode.h"
#define STATELISTSIZE_DFA 100


int stateListDFA[STATELISTSIZE_DFA][128];
int stateListSizeDFA = 0;
listNode endStateIndexListHeadDFA;
listNode DFA2NFASetHead[STATELISTSIZE_DFA];
int DFA2MDS[STATELISTSIZE_DFA];


extern int insertNode(listNode* list, int data);
extern int freeList(listNode* list);
extern int printfData(listNode* list);


int initStateListDFA() {
	for (int i = 0; i < STATELISTSIZE_DFA; i++) {
		for (int j = 0; j < 128; j++) {
			stateListDFA[i][j] = -1;
		}
		DFA2NFASetHead[i].data = -1;
		DFA2NFASetHead[i].next = nullptr;
		DFA2MDS[i] = -1;
	}
	endStateIndexListHeadDFA.data = -1;
	endStateIndexListHeadDFA.next = nullptr;
	return 0;
}


int freeStateListDFA() {
	for (int i = 0; i < STATELISTSIZE_DFA; i++) {
		freeList(&DFA2NFASetHead[i]);
	}
	freeList(&endStateIndexListHeadDFA);
	return 0;
}


int newStateDFA() {
	return stateListSizeDFA++;
}


int getStateListSizeDFA() {
	return stateListSizeDFA;
}


int connectStateDFA(int srcIndex, int dstIndex, int input) {
	if (stateListDFA[srcIndex][input] != -1) {
		// cout << "error: stateListDFA[" << srcIndex << "][" << input << "] is not empty\n";
		return -1;
	}
	stateListDFA[srcIndex][input] = dstIndex;
	return 0;
}


int printfStateDFA() {
	for (int i = 0; i < stateListSizeDFA; i++) {
		for (int j = 0; j < 128; j++) {
			if (stateListDFA[i][j] != -1) {
				printf("state:%5d\tinput:   %c\tnext:%5d\n", i, (char)j, stateListDFA[i][j]);
			}
		}
	}
	return 0;
}