#pragma once
#include "stateListNFA.h"
#include "stateListDFA.h"


extern listNode stateListNFA[STATELISTSIZE_NFA][128];
extern listNode endStateIndexListHeadDFA;
extern listNode DFA2NFASetHead[STATELISTSIZE_DFA];
extern int insertNode(listNode* head, int data);
extern int searchData(listNode* head, int data);
extern int freeList(listNode* head);
extern int listCmp(listNode* head1, listNode* head2);
extern int newStateDFA();
extern int getStateListSizeDFA();
extern int connectStateDFA(int srcIndex, int dstIndex, int input);


int insertSet(listNode* head, int state, char input) {
	listNode* cur = stateListNFA[state][input].next;
	// ������NFA��Ӧ��ÿһ�����ݲ��뵽head�ڵ���
	while (cur != nullptr) {
		insertNode(head, cur->data);
		cur = cur->next;
	}
	return 0;
}


int epsilonClosure(listNode* dstHead, listNode* srcHead) {
	listNode* curDst, * curSrc;
	// ʹ��һ����ʱ��ͷ�ڵ㣬����dstHead��srcHead��ͬʱ�Ĵ���
	listNode tempHead;
	tempHead.data = -1;
	tempHead.next = nullptr;
	curSrc = srcHead->next;
	curDst = &tempHead;
	while (curSrc != nullptr) {
		// ����ÿ��src�е����룬������뵽tempHead��
		insertNode(&tempHead, curSrc->data);
		while (curDst->next != nullptr) {
			// ��tempHead�е�ÿ�����ݵ�epsilon�հ����뵽tempHead��
			curDst = curDst->next;
			insertSet(&tempHead, curDst->data, '\0');
		}
		curSrc = curSrc->next;
	}
	// ���src��dst��ͬһ���ڵ㣬��Ҫ���ͷŵ��ڴ�Ȼ���ڰ�ָ��ת��
	if (dstHead == srcHead)
		freeList(dstHead);
	dstHead->next = tempHead.next;
	return 0;
}


int delta(listNode* dstHead, listNode* srcHead, char input) {
	listNode* cur= srcHead->next;
	// ��������һ����������Ȼ����������Ӧ�ò�����dst����src�����
	listNode tempHead;
	tempHead.data = -1;
	tempHead.next = nullptr;
	while (cur != nullptr) {
		insertSet(&tempHead, cur->data, input);
		cur = cur->next;
	}
	if (dstHead == srcHead)
		freeList(dstHead);
	dstHead->next = tempHead.next;
	return 0;
}


int createDFA(int startStateIndexNFA, int endStateIndexNFA) {
	// �Գ�ʼ�ڵ����һ����ʼ������ǰΨһԪ����NFA�ĳ�ʼ�ڵ�ıհ�
	int startStateIndexDFA = newStateDFA();
	insertNode(&DFA2NFASetHead[startStateIndexDFA], startStateIndexNFA);
	epsilonClosure(&DFA2NFASetHead[startStateIndexDFA], &DFA2NFASetHead[startStateIndexDFA]);
	int curIndexDFA = startStateIndexDFA;
	// ����һ����ʱ��ͷ�����ڽ���һ���µıհ�����
	listNode tempHead;
	tempHead.data = -1;
	tempHead.next = nullptr;
	while (curIndexDFA < getStateListSizeDFA()) {
		for (int i = 1; i < 128; i++) {
			// ����ÿһ���ǿ��ַ�
			tempHead.next = nullptr;
			delta(&tempHead, &DFA2NFASetHead[curIndexDFA], i);
			epsilonClosure(&tempHead, &tempHead);
			if (tempHead.next == nullptr)
				continue;	// ����հ�Ϊ�գ�����
			// ����Ƿ��Ѿ���������հ�����
			int flag = 0, j = 0;
			for (; j < getStateListSizeDFA(); j++) {
				flag = listCmp(&DFA2NFASetHead[j], &tempHead);
				if (flag == 1)
					break;	// ���ִ��ھ�ֱ������ѭ��
			}
			if (flag == 0) {
				// �����������ͬ�ģ����½�һ���ڵ㣬��ָ��ת�ƹ�ȥ
				j = newStateDFA();
				DFA2NFASetHead[j].next = tempHead.next;
			}
			else {
				// ���������ͬ�ģ����ͷŵ���ȡ�����µıհ����ϣ�������Ҫ������
				freeList(&tempHead);
			}
			connectStateDFA(curIndexDFA, j, i);
		}
		curIndexDFA++;
	}
	// ������һ�����������е���̬�����뵽��̬�б���
	for (int i = 0; i < getStateListSizeDFA(); i++) {
		if (searchData(&DFA2NFASetHead[i], endStateIndexNFA) == 1)
			insertNode(&endStateIndexListHeadDFA, i);
	}
	return 0;
}