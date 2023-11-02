#pragma once
#include "production.h"
#include <string.h>

extern char regex[REGEXSIZE];
extern int endCharIndex;
extern bool M[REGEXSIZE / 5][128];
extern int MSize;

int regexFormat()
{
	int i = 0, in_brack = 0, bufferLen = 0;
	int len = strlen(regex);
	char buffer[100];
	char cur;
	while (i < len) {
		cur = regex[i++];	// 读取当前字符
		if (in_brack != 0) {
			// 如果在[]中，需要正确识别特殊字符\]-，以及转义字符\\\]\-\t\r\n
			// []里的东西，我全部需要将其化为字符形式交给M
			if (cur == ']') {
				in_brack = 0;
				if (MSize > 9) {
					buffer[bufferLen++] = '1';
				}
				buffer[bufferLen++] = MSize % 10 + '0';
				i--, MSize++;
				continue;
			}
			if (cur == '\\') {
				cur = regex[i++];
				if (cur == 'n')
					cur = '\n';
				else if (cur == 'r')
					cur = '\r';
				else if (cur == 't')
					cur = '\t';
			}
			else if (cur == '-') {
				// 如果识别到-，就需要将前一个字符和后一个字符之间的所有字符都加入到M中
				char c = regex[i - 2] + 1;
				cur = regex[i++];
				if (cur == '\\') {
					cur = regex[i++];
					if (cur == 'n')
						cur = '\n';
					else if (cur == 'r')
						cur = '\r';
					else if (cur == 't')
						cur = '\t';
				}
				for (; c < cur; c++)
					M[MSize][c] = true;
			}
			M[MSize][cur] = true;
		}
		else {
			// 而不在[]中的字符，对于\n\r\t转换形式，\[\]\+\*\|\?\(\)\\都需要保留形式，其余遇到\就跳过
			if (cur == '\\') {
				cur = regex[i++];
				if (cur == 'n')
					cur = '\n';
				else if (cur == 'r')
					cur = '\r';
				else if (cur == 't')
					cur = '\t';
				else if (cur == '[' || cur == ']' || cur == '+' || cur == '*' || cur == '|' || cur == '?' || cur == '(' || cur == ')' || cur == '\\') {
					buffer[bufferLen++] = '\\';
				}
				else {
					continue;
				}
			}
			else if (cur == '[') {
				in_brack = 1;
			}
			buffer[bufferLen++] = cur;
		}
	}
	buffer[bufferLen++] = '\0';
	memcpy_s(regex, 100, buffer, 100);
	endCharIndex = bufferLen - 2;
	return 0;
}