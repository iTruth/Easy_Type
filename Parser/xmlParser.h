#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

typedef struct __xmlLineInfoHeader{
	wchar_t *label;
	int size;
	struct __xmlLineInfo* next;
} xmlLineInfoHeader;

typedef struct __xmlLineInfo{
	wchar_t *Key;
	wchar_t *Value;
	struct __xmlLineInfo* next;
} xmlLineInfo;

xmlLineInfo *xmlItemParse(wchar_t *xmlItem)
{
	xmlLineInfo *info = (xmlLineInfo*)malloc(sizeof(xmlLineInfo));
	wchar_t **item = split(xmlItem, '=');
	if(getChrNum(xmlItem, '=') == 1){
		int value_len = wcslen(item[1]);
		item[1][value_len-1] = '\0';
		info->Value = &item[1][1];
	} else {
		info->Value = NULL;
	}
	info->Key = item[0];
	return info;
}

xmlLineInfoHeader *xmlLineParse(wchar_t *xmlLine)
{
	xmlLine = wcschr(xmlLine, '<');
	xmlLineInfoHeader *h = (xmlLineInfoHeader*)malloc(sizeof(xmlLineInfoHeader));
	h->next = NULL;
	h->size = 0;
	int line_len = wcslen(xmlLine);
	if(wcscmp(&xmlLine[line_len-2], L"/>") == 0){
		xmlLine[line_len-2] = '\0';
	} else {
		xmlLine[line_len-1] = '\0';
	}
	wchar_t *finLine = &xmlLine[1];
	wchar_t **items = split(finLine, ' ');
	h->label = items[0];
	int count = getChrNum(finLine, ' ');
	if(count == 0){
		return h;
	}
	h->next = xmlItemParse(items[1]);
	++h->size;
	xmlLineInfo *xpNext = h->next;
	for(int i=1; i<count; ++i){
		xpNext->next = xmlItemParse(items[1+i]);
		xpNext = xpNext->next;
		++h->size;
	}
	xpNext->next = NULL;
	return h;
}

wchar_t* xmlItem(xmlLineInfoHeader *h, wchar_t *name)
{
	xmlLineInfo *info = h->next;
	while(info != NULL)
	{
		if(wcscmp(info->Key, name) == 0){
			return info->Value;
		}
		info = info->next;
	}
	return NULL;
}

void displayXmlInfo(xmlLineInfoHeader *h)
{
	printf("Label: %hs\n", h->label);
	int size = h->size;
	xmlLineInfo *info = h->next;
	for(int i=0; i<size; ++i){
		printf("Key: %hs Value: %hs\n", info->Key, (info->Value == NULL)?L"NULL":info->Value);
		info = info->next;
	}
}

void freeXmlLineInfo(xmlLineInfo *info)
{
	free(info->Key);
	free(info);
}

void freeXmlLineInfoHeader(xmlLineInfoHeader *header)
{
	free(header->label);
	free(header);
}

void freeXmlNode(xmlLineInfoHeader *header)
{
	xmlLineInfo *info = header->next;
	xmlLineInfo *curr_info = NULL;
	freeXmlLineInfoHeader(header);
	while(info != NULL)
	{
		curr_info = info;
		info = info->next;
		freeXmlLineInfo(curr_info);
	}
}
