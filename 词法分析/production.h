#pragma once
#define REGEXSIZE 100
#include "stateListNFA.h"


char regex[REGEXSIZE];
int endCharIndex;
bool M[REGEXSIZE / 5][128] = { 0 };
int MSize = 0;


extern int newStateNFA();
extern int connectStateNFA(int srcIndex, int dstIndex, int input);
int do_0(int& startStateIndex, int& endStateIndex);
int do_1(int& startStateIndex, int& endStateIndex);
int do_2(int& startStateIndex, int& endStateIndex);
int do_3(int& startStateIndex, int& endStateIndex);


int scanfRegex() {
	cin.getline(regex, REGEXSIZE);
	return 0;
}


int printfRegex() {
	cout << regex << endl;
	return 0;
}


int printfM() {
	for (int i = 0; i < MSize; i++) {
		printf("row: %d\n", i);
		for (int j = 0; j < 128; j++)
			if (M[i][j]) printf("%c ", (char)j);
		cout << endl;
	}
	return 0;
}


int createNFA(int& startStateIndex, int& endStateIndex) {
	do_0(startStateIndex, endStateIndex);
	return 0;
}


/**
*  0:	E0 -> E1 E2
*  1:	E1 -> E0 |
*  2:	E1 -> E0
*  3:	E2 -> E3 *
*  4:	E2 -> E3 +
*  5:	E2 -> E3 ?
*  6:	E2 -> E3
*  7:	E3 -> ( E0 )
*  8:	E3 -> [ E0 ]
*  9:	E3 -> E0 .
* 10:	E3 -> char
* -1:	E1 -> null
**/

int getProductionIndex(int state) {
	if (endCharIndex < 0 || (regex[endCharIndex] == '(' && regex[endCharIndex - 1] != '\\'))
		return -1;
	int result;
	char input = regex[endCharIndex];
	if (state == 0) {
		result = 0;	// state����0������ʵҲûɶ�ã���Ϊֻ��һ�ֲ���ʽ
	}
	else if (state == 1) {
		result = input == '|' ? 1 : 2;
		if (regex[endCharIndex - 1] == '\\')	// ���ǰһ����'\\'��������ζ�Ӧ���ж�Ϊ�ַ�
			result = 2;
	}
	else if (state == 2){
		switch (input) {
		case '*':	result = 3; break;
		case '+':	result = 4; break;
		case '?':	result = 5; break;
		default:	result = 6;
		}
		if (regex[endCharIndex - 1] == '\\')	// ���ǰһ����'\\'��������ζ�Ӧ���ж�Ϊ�ַ�
			result = 6;
	}
	else if (state == 3) {
		switch (input) {
		case ')':	result = 7; break;
		case ']':	result = 8; break;
		case '.':	result = 9; break;
		default:	result = 10;
		}
		if (regex[endCharIndex - 1] == '\\')	// ���ǰһ����'\\'��������ζ�Ӧ���ж�Ϊ�ַ�
			result = 10;
	}
	return result;
}


int do_0(int& startStateIndex, int& endStateIndex) {
	// ��E0�У���Ҫ��ȡ��E1�Ĳ���ʽ�����ݲ���ʽ�Ĳ�ͬ������ͬ�����Ӳ���
	int startIndex2, endIndex2, startIndex1, endIndex1;
	do_2(startIndex2, endIndex2);
	int productionIndex1 = do_1(startIndex1, endIndex1);
	if (productionIndex1 == -1) {
		// ����ʽΪ-1ʱ��˵��E1Ϊ�գ�ֱ�ӽ�E0�Ŀ�ʼ״̬�ͽ���״̬����ΪE2�Ŀ�ʼ״̬�ͽ���״̬
		startStateIndex = startIndex2;
		endStateIndex = endIndex2;
	}
	else if (productionIndex1 == 1) {
		// ����ʽΪ1ʱ��˵��E1ΪE0 |���½������ڵ㣬Ȼ������
		// ����֮ǰ����Ϊ�ǣ�����һ�£���Ȼ�������ˣ������ˣ����ԸĻ��˿��Ͻ��ķ���
		startStateIndex = newStateNFA();
		endStateIndex = newStateNFA();
		connectStateNFA(startStateIndex, startIndex1, '\0');
		connectStateNFA(startStateIndex, startIndex2, '\0');
		connectStateNFA(endIndex1, endStateIndex, '\0');
		connectStateNFA(endIndex2, endStateIndex, '\0');
	}
	else if (productionIndex1 == 2) {
		// ����ʽΪ2ʱ��˵��E1ΪE0����Ҫ��E1��E2ƴ������
		startStateIndex = startIndex1;
		endStateIndex = endIndex2;
		connectStateNFA(endIndex1, startIndex2, '\0');
	}
	return 0;
}


int do_1(int& startStateIndex, int& endStateIndex) {
	// ��E1�У�����Ҫ���κε����Ӳ���
	// ֻ��Ҫע��һ�£���ִ�в���ʽ2ʱ���ַ���λ����ǰ�ƶ�һλ
	int productionIndex = getProductionIndex(1);
	if (productionIndex == -1)
		return -1;
	if (productionIndex == 1)
		endCharIndex--;
	do_0(startStateIndex, endStateIndex);
	return productionIndex;
}


int do_2(int& startStateIndex, int& endStateIndex) {
	int productionIndex = getProductionIndex(2);
	if (productionIndex == 3) {
		// ���ڲ���ʽ3����Ҫ��E3�Ľ���״̬���ӵ�E3�Ŀ�ʼ״̬
		// ����E3�Ŀ�ʼ״̬���ӵ�E3�Ľ���״̬
		// ������һ�£������Ͽν��������ˣ�
		endCharIndex--;
		do_3(startStateIndex, endStateIndex);
		connectStateNFA(startStateIndex, endStateIndex, '\0');
		connectStateNFA(endStateIndex, startStateIndex, '\0');
	}
	else if (productionIndex == 4) {
		// ���ڲ���ʽ4����Ҫ��E3�Ľ���״̬���ӵ�E3�Ŀ�ʼ״̬
		endCharIndex--;
		do_3(startStateIndex, endStateIndex);
		connectStateNFA(endStateIndex, startStateIndex, '\0');
	}
	else if (productionIndex == 5) {
		// ���ڲ���ʽ5����Ҫ��E3�Ŀ�ʼ״̬���ӵ�E3�Ľ���״̬
		endCharIndex--;
		do_3(startStateIndex, endStateIndex);
		connectStateNFA(startStateIndex, endStateIndex, '\0');
	}
	else if (productionIndex == 6) {
		do_3(startStateIndex, endStateIndex);
	}
	return 0;
}


int do_3(int& startStateIndex, int& endStateIndex) {
	int productionIndex = getProductionIndex(3);
	if (productionIndex == 7) {
		endCharIndex--;
		do_0(startStateIndex, endStateIndex);
		endCharIndex--;
	}
	else if (productionIndex == 8) {
		startStateIndex = newStateNFA();
		endStateIndex = newStateNFA();
		endCharIndex--;
		int index = regex[endCharIndex--] - '0';
		if (regex[endCharIndex] != '[')
			index += 10, endCharIndex--;
		for (int i = 0; i < 128; i++)
			if (M[index][i])
				connectStateNFA(startStateIndex, endStateIndex, (char)i);
		endCharIndex--;
	}
	else if (productionIndex == 9) {
		startStateIndex = newStateNFA();
		endStateIndex = newStateNFA();
		for (int i = 1; i < 128; i++)
			connectStateNFA(startStateIndex, endStateIndex, (char)i);
		endCharIndex--;
	}
	else if (productionIndex == 10) {
		startStateIndex = newStateNFA();
		endStateIndex = newStateNFA();
		connectStateNFA(startStateIndex, endStateIndex, regex[endCharIndex--]);
		if (regex[endCharIndex] == '\\')
			endCharIndex--;
	}
	return 0;
}