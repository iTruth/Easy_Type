#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xmlParser.h"

int main(int argc, char **argv)
{
	if(argc != 2){
		printf("参数错误\n");
		return -1;
	}
	FILE *fp = fopen(argv[1], "rt+,ccs=UTF-8");
	if(fp == NULL){
		printf("无法打开文件\n");
		return -2;
	}
	FILE *parse_fp = NULL;
	wchar_t xmlLine[32767] = {0};
	while(!feof(fp))
	{
		readLine(xmlLine, fp);
		if(wcslen(xmlLine) < 3){
			continue;
		}
		xmlLineInfoHeader *h = xmlLineParse(xmlLine);
		if(parse_fp == NULL){
			wchar_t *name = xmlItem(h, L"alias");
			if(name == NULL){
				freeXmlNode(h);
				continue;
			}
			parse_fp = _wfopen(name, L"wt+,ccs=UTF-8");
			if(parse_fp == NULL){
				printf("创建文件失败\n");
				freeXmlNode(h);
				return -3;
			}
		} else {
			wchar_t *item = xmlItem(h, L"comparision");
			if(item != NULL){
				fwprintf(parse_fp, L"%s\n", item);
			}
		}
		freeXmlNode(h);
	}
	fclose(fp);
	fclose(parse_fp);
	printf("Successful translate %s\n", argv[1]);
	return 0;
}
