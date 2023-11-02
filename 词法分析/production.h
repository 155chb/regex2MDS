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
		result = 0;	// state等于0这里其实也没啥用，因为只有一种产生式
	}
	else if (state == 1) {
		result = input == '|' ? 1 : 2;
		if (regex[endCharIndex - 1] == '\\')	// 如果前一项是'\\'，无论如何都应该判断为字符
			result = 2;
	}
	else if (state == 2){
		switch (input) {
		case '*':	result = 3; break;
		case '+':	result = 4; break;
		case '?':	result = 5; break;
		default:	result = 6;
		}
		if (regex[endCharIndex - 1] == '\\')	// 如果前一项是'\\'，无论如何都应该判断为字符
			result = 6;
	}
	else if (state == 3) {
		switch (input) {
		case ')':	result = 7; break;
		case ']':	result = 8; break;
		case '.':	result = 9; break;
		default:	result = 10;
		}
		if (regex[endCharIndex - 1] == '\\')	// 如果前一项是'\\'，无论如何都应该判断为字符
			result = 10;
	}
	return result;
}


int do_0(int& startStateIndex, int& endStateIndex) {
	// 在E0中，需要获取到E1的产生式，根据产生式的不同，做不同的连接操作
	int startIndex2, endIndex2, startIndex1, endIndex1;
	do_2(startIndex2, endIndex2);
	int productionIndex1 = do_1(startIndex1, endIndex1);
	if (productionIndex1 == -1) {
		// 产生式为-1时，说明E1为空，直接将E0的开始状态和结束状态设置为E2的开始状态和结束状态
		startStateIndex = startIndex2;
		endStateIndex = endIndex2;
	}
	else if (productionIndex1 == 1) {
		// 产生式为1时，说明E1为E0 |，新建两个节点，然后连接
		// 这里之前自以为是，改了一下，果然出问题了，人麻了，所以改回了课上讲的方法
		startStateIndex = newStateNFA();
		endStateIndex = newStateNFA();
		connectStateNFA(startStateIndex, startIndex1, '\0');
		connectStateNFA(startStateIndex, startIndex2, '\0');
		connectStateNFA(endIndex1, endStateIndex, '\0');
		connectStateNFA(endIndex2, endStateIndex, '\0');
	}
	else if (productionIndex1 == 2) {
		// 产生式为2时，说明E1为E0，需要将E1和E2拼接起来
		startStateIndex = startIndex1;
		endStateIndex = endIndex2;
		connectStateNFA(endIndex1, startIndex2, '\0');
	}
	return 0;
}


int do_1(int& startStateIndex, int& endStateIndex) {
	// 在E1中，不需要做任何的连接操作
	// 只需要注意一下，当执行产生式2时将字符的位置向前移动一位
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
		// 对于产生式3，需要将E3的结束状态连接到E3的开始状态
		// 并将E3的开始状态连接到E3的结束状态
		// （改了一下，不是上课讲的那种了）
		endCharIndex--;
		do_3(startStateIndex, endStateIndex);
		connectStateNFA(startStateIndex, endStateIndex, '\0');
		connectStateNFA(endStateIndex, startStateIndex, '\0');
	}
	else if (productionIndex == 4) {
		// 对于产生式4，需要将E3的结束状态连接到E3的开始状态
		endCharIndex--;
		do_3(startStateIndex, endStateIndex);
		connectStateNFA(endStateIndex, startStateIndex, '\0');
	}
	else if (productionIndex == 5) {
		// 对于产生式5，需要将E3的开始状态连接到E3的结束状态
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