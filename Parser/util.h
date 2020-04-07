#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void readLine(wchar_t *strDest, FILE *fp)
{
	while((*strDest++ = fgetwc(fp))!='\n' && !feof(fp) && !ferror(fp));
	*--strDest = '\0';
}

int getChrNum(wchar_t *line, int chr)
{
	int size = 0;
	int strFlag = 0;
	int line_size = wcslen(line);
	for(int i=0; i<line_size; ++i){
		if(strFlag){
			if(line[i] != '\"'){
				continue;
			} else {
				++i;
				strFlag = 0;
			}
		}
		if(line[i] == '\"'){
			strFlag = 1;
			continue;
		}
		if(line[i] == chr){
			++size;
		}
	}
	return size;
}

wchar_t **split(wchar_t *line, wchar_t chr)
{
	int line_size = wcslen(line)+1;
	wchar_t *line_cpy = (wchar_t*)malloc(line_size*sizeof(wchar_t));
	int strFlag = 0;
	wcscpy(line_cpy, line);
	wchar_t **szStr = (wchar_t**)malloc((getChrNum(line, chr)+1)*sizeof(wchar_t*));
	int ref_p = 0;
	int size = 0;
	for(int i=0; i<line_size; ++i){
		if(strFlag){
			if(line_cpy[i] != '\"'){
				continue;
			} else {
				++i;
				strFlag = 0;
			}
		}
		if(line_cpy[i] == '\"'){
			strFlag = 1;
			continue;
		}
		if(line_cpy[i] == chr){
			line_cpy[i] = '\0';
			szStr[size++] = &line_cpy[ref_p];
			ref_p = i+1;
		} else if(line_cpy[i] == '\0'){
			szStr[size++] = &line_cpy[ref_p];
		}
	}
	return szStr;
}

void freeSplitRet(wchar_t **splitStr)
{
	free(&splitStr[0]);
}

