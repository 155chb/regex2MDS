#pragma once
#include <iostream>
using namespace std;


struct listNode {
	int data;
	listNode *next;
};


int printfData(listNode* head) {
	listNode* cur = head->next;
	while (cur != nullptr) {
		cout << cur->data << " ";
		cur = cur->next;
	}
	return 0;
}


int insertNode(listNode* head, int data) {	// 这里的head是头节点，由主程序控制，不插入数据
	listNode* cur = head;
	while (cur->next != nullptr && cur->data != data) {
		cur = cur->next;
	}
	if (cur->next != nullptr) {
		// cout << "data already exists" << endl;
		return -1;
	}
	cur->next = (listNode*)malloc(sizeof(listNode));
	cur = cur->next;
	cur->data = data;
	cur->next = nullptr;
	// printfData(head);
	return 0;
}


int freeList(listNode* head) {	// 这里的head是头节点，不释放
	listNode* cur = head->next;
	listNode* next = nullptr;
	while (cur != nullptr) {
		next = cur->next;
		free(cur);
		cur = next;
	}
	return 0;
}


int searchData(listNode* head, int data) {
	listNode* cur = head->next;
	while (cur != nullptr) {
		if (cur->data == data)
			return 1;
		cur = cur->next;
	}
	return 0;
}


int listLen(listNode* head) {
	listNode* cur = head->next;
	int len = 0;
	while (cur != nullptr) {
		len++;
		cur = cur->next;
	}
	return len;
}


int listCmp(listNode* head1, listNode* head2) {
	listNode* cur = head1->next;
	int len1 = listLen(head1);
	int len2 = listLen(head2);
	if (len1 != len2)
		return 0;
	while (cur != nullptr) {
		if (searchData(head2, cur->data) == 0)
			return 0;
		cur = cur->next;
	}
	return 1;
}