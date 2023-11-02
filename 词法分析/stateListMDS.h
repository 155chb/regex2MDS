#pragma once
#include "listNode.h"
#define STATELISTSIZE_MDS 100


int stateListMDS[STATELISTSIZE_MDS][128];
int stateListSizeMDS = 0;
listNode endStateIndexListHeadMDS;


int initStateListMDS() {
	for (int i = 0; i < STATELISTSIZE_MDS; i++) {
		for (int j = 0; j < 128; j++) {
			stateListMDS[i][j] = -1;
		}
	}
	endStateIndexListHeadMDS.data = -1;
	endStateIndexListHeadMDS.next = nullptr;
	return 0;
}


int freeStateListMDS() {
	freeList(&endStateIndexListHeadMDS);
	return 0;
}


int newStateMDS() {
	return stateListSizeMDS++;
}

int connectStateMDS(int srcIndex, int dstIndex, int input) {
	if (stateListMDS[srcIndex][input] != -1) {
		// cout << "error: stateListDFA[" << srcIndex << "][" << input << "] is not empty\n";
		return -1;
	}
	stateListMDS[srcIndex][input] = dstIndex;
	return 0;
}


int printfStateMDS() {
	for (int i = 0; i < stateListSizeMDS; i++) {
		for (int j = 0; j < 128; j++) {
			if (stateListMDS[i][j] != -1) {
				printf("state:%5d\tinput:   %c\tnext:%5d\n", i, (char)j, stateListMDS[i][j]);
			}
		}
	}
	return 0;
}