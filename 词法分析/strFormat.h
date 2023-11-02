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
		cur = regex[i++];	// ��ȡ��ǰ�ַ�
		if (in_brack != 0) {
			// �����[]�У���Ҫ��ȷʶ�������ַ�\]-���Լ�ת���ַ�\\\]\-\t\r\n
			// []��Ķ�������ȫ����Ҫ���仯Ϊ�ַ���ʽ����M
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
				// ���ʶ��-������Ҫ��ǰһ���ַ��ͺ�һ���ַ�֮��������ַ������뵽M��
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
			// ������[]�е��ַ�������\n\r\tת����ʽ��\[\]\+\*\|\?\(\)\\����Ҫ������ʽ����������\������
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