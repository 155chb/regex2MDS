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
	bool flag = 1;	// ����һ����Ǳ�����1��ʾ��״̬��֮ǰ��״̬����ͬ
	for (int i = 0; i < stateListSizeDFA; i++) {
		flag = 1;
		for (int j = 0; j < i; j++) {
			for (int k = 0; k < 128; k++) {
				// �����п����������һ�飬���������򲻿�����
				flag = stateListDFA[i][k] != stateListDFA[j][k];
				if (flag)	// ������ֿ����֣���ȥ�˳�ѭ�������һ�� 
					break;
			}
			if (!flag) {
				// ����ҵ��˲������ֵľͿ��Ա�ǲ��˳�ѭ����
				DFA2MDS[i] = DFA2MDS[j];
				break;
			}
		}
		if (flag)	// ���û���ҵ��������ֵģ�����������һ���µ�MDS
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