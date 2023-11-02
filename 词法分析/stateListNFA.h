#pragma once
#include "listNode.h"
#define STATELISTSIZE_NFA 100


listNode stateListNFA[STATELISTSIZE_NFA][128];
int stateListSizeNFA = 0;


extern int insertNode(listNode* list, int data);
extern int freeList(listNode* list);
extern int printfData(listNode* list);


int initStateListNFA() {
	for (int i = 0; i < STATELISTSIZE_NFA; i++) {
		for (int j = 0; j < 128; j++) {
			stateListNFA[i][j].data = -1;
			stateListNFA[i][j].next = nullptr;
		}
	}
	return 0;
}


int freeStateListNFA() {
	for (int i = 0; i < STATELISTSIZE_NFA; i++) {
		for (int j = 0; j < 128; j++) {
			freeList(&stateListNFA[i][j]);
		}
	}
	return 0;
}

int newStateNFA() {
	return stateListSizeNFA++;
}


int connectStateNFA(int srcIndex, int dstIndex, int input) {
	return insertNode(&stateListNFA[srcIndex][input], dstIndex);
}


int printfStateNFA() {
	for (int i = 0; i < stateListSizeNFA; i++) {
		for (int j = 0; j < 128; j++) {
			if (stateListNFA[i][j].next != nullptr) {
				printf("state:%5d\tinput:   %c\tnext:    ", i, (char)j);
				printfData(&stateListNFA[i][j]);
				cout << endl;
			}
		}
	}
	return 0;
}