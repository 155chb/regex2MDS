#pragma once
#include "stateListDFA.h"
#include "stateListMDS.h"


extern int stateListDFA[STATELISTSIZE_DFA][128];
extern int DFA2MDS[STATELISTSIZE_DFA];
extern int getStateListSizeDFA();
extern int newStateMDS();
extern int connectStateMDS(int srcIndex, int dstIndex, int input);


int initDFA2MDS() {
	int stateListSizeDFA = getStateListSizeDFA();
	bool flag = 1;	// 定义一个标记变量，1表示该状态与之前的状态不相同
	for (int i = 0; i < stateListSizeDFA; i++) {
		flag = 1;
		for (int j = 0; j < i; j++) {
			for (int k = 0; k < 128; k++) {
				// 把所有可能输入遍历一遍，如果均相等则不可区分
				flag = stateListDFA[i][k] != stateListDFA[j][k];
				if (flag)	// 如果发现可区分，就去退出循环检查下一个 
					break;
			}
			if (!flag) {
				// 如果找到了不可区分的就可以标记并退出循环了
				DFA2MDS[i] = DFA2MDS[j];
				break;
			}
		}
		if (flag)	// 如果没有找到不可区分的，就让它申请一个新的MDS
			DFA2MDS[i] = newStateMDS();
	}
	return 0;
}


int createMDS() {
	initDFA2MDS();
	for (int i = 0; i < getStateListSizeDFA(); i++) {
		for (int j = 0; j < 128; j++) {
			if (stateListDFA[i][j] != -1)
				connectStateMDS(DFA2MDS[i], DFA2MDS[stateListDFA[i][j]], j);
		}
	}
	listNode* cur = endStateIndexListHeadDFA.next;
	while (cur != nullptr) {
		insertNode(&endStateIndexListHeadMDS, DFA2MDS[cur->data]);
		cur = cur->next;
	}
	return 0;
}