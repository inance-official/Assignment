/******************************************************************************
* PROGRAM NAME : search
* SOURCE NAME  : search.c
* DESCRIPTION  : code, name 파일 검색
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cntable.h"

CODENAME   *g_CnList = NULL;

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
			  const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
	CODENAME *k1 = (CODENAME*)aa;
	CODENAME *k2 = (CODENAME*)bb;
	return strcmp(k1 -> code, k2 -> code);
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    : num - 파일 내 레코드 수
******************************************************************************/
int FileCount()
{
	FILE      *fp        = NULL;
	char       sbuff[64] = {0,};
	int        num       = 0;

	fp = fopen("./file.txt", "r");
	if (fp == NULL)
	{
		printf("file open error[%d]\n", errno);
		return -1;
	}

	while (fgets(sbuff, sizeof(sbuff), fp) != NULL)
	{
		num++;
	}

	fclose(fp);

	return num;
}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 데이터 로드
PARAMETERS  : CODENAME *cnlist - 로드한 데이터를 저장할 메모리
			  int num         - 파일 내 레코드 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(CODENAME *cnlist, int num)
{
	CODENAME   *tcode		= NULL;
	FILE      *fp           = NULL;
	char       sbuff[64]    = {0,};
	int        ii;

	// 파일 읽기모드로 열기
	fp = fopen("./file.txt", "r");
	if (fp == NULL)
	{
		printf("file open error[%d]\n", errno);
		return -1;
	}

	for (ii = 0; ii < num; ii++)
	{
		fgets(sbuff, sizeof(sbuff), fp);

		tcode = (CODENAME *)sbuff;

		memcpy(&cnlist[ii], tcode, sizeof(CODENAME));
	}

	fclose(fp);

	return 1;
}

/******************************************************************************
FUNCTION    : Search
DESCRIPTION : 검색 키에 해당하는 데이터 조회
PARAMETERS  : CODENAME *cnlist - 검색할 데이터가 저장된 메모리
			  int num         - 검색할 데이터의 수
			  char *code       - 검색할 키
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Search(CODENAME *cnlist, int num, char *code)
{
	CODENAME codename;
	CODENAME *result; // 검색 결과
	CODENAME *start;  // 검색 결과의 시작 위치
	CODENAME *end;    // 검색 결과의 끝 위치
	CODENAME *now;    // 검색 결과의 현재 위치

	/*--- 검색 키 설정 ---*/
	strcpy(codename.code, code);

	/*--- bsearch ---*/
	result = (CODENAME *)bsearch(&codename, cnlist, num, sizeof(CODENAME), Cmp);
	if (result == NULL) // 검색 실패
	{
		return -1;
	}
	else // 검색 성공
	{
		// 검색 결과의 시작 위치
		start = result;
		while (start > cnlist && Cmp(start-1, &codename) == 0)
			start--;

		// 검색 결과의 끝 위치
		end = result;
		while (end < cnlist + num && Cmp(end+1, &codename) == 0)
			end++;

		// 검색 결과 출력
		printf("---------------------------------\n");
		for (now = start; now <= end; now++)
		{
			printf("code : %-*.*s Name : %-*.*s\n",
				   sizeof(now->code)-1, sizeof(now->code)-1, now->code,
				   sizeof(now->name)-1, sizeof(now->name)-1, now->name);
		}
		printf("---------------------------------\n");

		return 1;
	}
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
	char    code[CODESIZE] = {0,};
	int     user          = 0;
	int     num           = 0;
	int		rtn;

	printf("                                 \n");
	printf("---------------------------------\n");
	printf("                                 \n");
	printf("    Enter a code to find name    \n");
	printf("                                 \n");
	printf("---------------------------------\n");
	printf("                                 \n");

	while (1)
	{
		/*--- 메모리 할당 ---*/
		num = FileCount();

		g_CnList = (CODENAME *)malloc(sizeof(CODENAME) * num);
		memset(g_CnList, 0x00, sizeof(CODENAME) * num);

		/*--- 파일에서 데이터 로드 ---*/
		rtn = FileToMem(g_CnList, num);
		if (rtn < 0)
		{
			printf("file open error[%d]\n", errno);
				return -1;
		}

		/*--- 검색 키 입력 ---*/
		printf("Input code(9) : ");
		scanf("%s", code);
		printf("\n");

		/*--- 검색 수행 ---*/
		rtn = Search(g_CnList, num, code);
		if (rtn < 0) // 검색 실패
		{
			printf("---------------------------------\n");
			printf("Not Found. code[%s]\n", code);
			printf("Search failed. Please try again. \n");
			printf("---------------------------------\n");
		}

		printf("                                 \n");
		printf("Do you search more? (1)yes (2)no : ");
		scanf("%d", &user); // 재검색 여부
		printf("                                 \n");

		if (user == 2) // 2:no 종료
			break;

		// 동적 할당한 메모리 해제
		if (g_CnList != NULL)
			free(g_CnList);
	}

	return 0;
}
