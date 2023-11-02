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
	// 遍历将NFA对应的每一个数据插入到head节点中
	while (cur != nullptr) {
		insertNode(head, cur->data);
		cur = cur->next;
	}
	return 0;
}


int epsilonClosure(listNode* dstHead, listNode* srcHead) {
	listNode* curDst, * curSrc;
	// 使用一个临时的头节点，避免dstHead和srcHead相同时的错误
	listNode tempHead;
	tempHead.data = -1;
	tempHead.next = nullptr;
	curSrc = srcHead->next;
	curDst = &tempHead;
	while (curSrc != nullptr) {
		// 对于每个src中的输入，将其插入到tempHead中
		insertNode(&tempHead, curSrc->data);
		while (curDst->next != nullptr) {
			// 将tempHead中的每个数据的epsilon闭包插入到tempHead中
			curDst = curDst->next;
			insertSet(&tempHead, curDst->data, '\0');
		}
		curSrc = curSrc->next;
	}
	// 如果src和dst是同一个节点，需要先释放掉内存然后在把指针转移
	if (dstHead == srcHead)
		freeList(dstHead);
	dstHead->next = tempHead.next;
	return 0;
}


int delta(listNode* dstHead, listNode* srcHead, char input) {
	listNode* cur= srcHead->next;
	// 类似于上一个函数，虽然正常程序中应该不存在dst等于src的情况
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
	// 对初始节点进行一个初始化，当前唯一元素是NFA的初始节点的闭包
	int startStateIndexDFA = newStateDFA();
	insertNode(&DFA2NFASetHead[startStateIndexDFA], startStateIndexNFA);
	epsilonClosure(&DFA2NFASetHead[startStateIndexDFA], &DFA2NFASetHead[startStateIndexDFA]);
	int curIndexDFA = startStateIndexDFA;
	// 定义一个临时的头，用于接收一个新的闭包集合
	listNode tempHead;
	tempHead.data = -1;
	tempHead.next = nullptr;
	while (curIndexDFA < getStateListSizeDFA()) {
		for (int i = 1; i < 128; i++) {
			// 遍历每一个非空字符
			tempHead.next = nullptr;
			delta(&tempHead, &DFA2NFASetHead[curIndexDFA], i);
			epsilonClosure(&tempHead, &tempHead);
			if (tempHead.next == nullptr)
				continue;	// 如果闭包为空，跳过
			// 检查是否已经存在这个闭包集合
			int flag = 0, j = 0;
			for (; j < getStateListSizeDFA(); j++) {
				flag = listCmp(&DFA2NFASetHead[j], &tempHead);
				if (flag == 1)
					break;	// 发现存在就直接跳出循环
			}
			if (flag == 0) {
				// 如果不存在相同的，就新建一个节点，把指针转移过去
				j = newStateDFA();
				DFA2NFASetHead[j].next = tempHead.next;
			}
			else {
				// 如果存在相同的，就释放掉获取到的新的闭包集合，它不需要保存了
				freeList(&tempHead);
			}
			connectStateDFA(curIndexDFA, j, i);
		}
		curIndexDFA++;
	}
	// 最后遍历一遍结果，把所有的终态都加入到终态列表中
	for (int i = 0; i < getStateListSizeDFA(); i++) {
		if (searchData(&DFA2NFASetHead[i], endStateIndexNFA) == 1)
			insertNode(&endStateIndexListHeadDFA, i);
	}
	return 0;
}