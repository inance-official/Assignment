/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : key, name 파일 저장
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "kntable.h"

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
			  const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
	KEYNAME *k1 = (KEYNAME *)aa;
	KEYNAME *k2 = (KEYNAME *)bb;

	return strcmp(k1->key, k2->key);
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
	char       rbuff[64] = {0,};
	int        num       = 0;

	fp = fopen("./file.txt", "r");
	if (fp == NULL)
	{
		printf("file open error[%d]\n", errno);
		return -1;
	}

	while (fgets(rbuff, sizeof(rbuff), fp) != NULL)
	{
		num++;
	}

	fclose(fp);

	return num;
}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 메모리로 데이터 로드
PARAMETERS  : KEYNAME *knlist - 로드한 데이터를 저장할 메모리
			  int num         - 파일 내 레코드 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int insert()
{
	FILE      *fp        = NULL;

	KEYNAME   *knlist    = NULL, *tkey = NULL;
	int        rcnt      = 0;
	int        ii        = 0;
	char       rbuff[64] = { 0, };
	int        count     = 0;

	/*--- 사용자 데이터 INSERT ---*/
	fp = fopen("./file.txt", "a+");
	if (fp == NULL)
	{
		printf("file open error[%d]\n", errno);
		return -1;
	}

	for (ii = 0; ii < rcnt + 1; ii++)
	{
		rcnt++;
		knlist = (KEYNAME *)malloc(sizeof(KEYNAME) * rcnt);
		memset(knlist, 0x00, sizeof(KEYNAME) * rcnt);

		printf("\n");

		printf("Input key[9] : ");
		scanf("%s", knlist[ii].key);

		if (strcmp(knlist[ii].key, "q") == 0)
			break;

		printf("Input name[40] : ");
		scanf("%s", knlist[ii].name);

		fwrite(&knlist[ii], 1, sizeof(KEYNAME), fp);
		fwrite("\n", 1, 1, fp);

		fflush(fp);

		if (knlist != NULL)
			free(knlist);
	}

	fclose(fp);

	/*--- 전체 데이터 메모리 로드 및 정렬 ---*/
	fp = fopen("./file.txt", "r");
	if (fp == NULL)
	{
		printf("file open error[%d]\n", errno);
		return -1;
	}

	count = FileCount();
	knlist = (KEYNAME *)malloc(sizeof(KEYNAME) * count);
	memset(knlist, 0x00, sizeof(KEYNAME) * count);

	for (ii = 0; ii < count; ii++)
	{
		fgets(rbuff, sizeof(rbuff), fp);

		tkey = (KEYNAME *)rbuff;

		memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
	}

	fclose(fp);

	// 배열을 정렬
	qsort(knlist, count, sizeof(KEYNAME), Cmp);

	/*--- 전체 데이터 신규 저장 ---*/
	fp = fopen("./file.txt", "w+");
	if (fp == NULL)
	{
		printf("file open error[%d]\n", errno);
		return -1;
	}

	for (ii = 0; ii < count; ii++)
	{
		fwrite(&knlist[ii], 1, sizeof(KEYNAME), fp);
		fwrite("\n", 1, 1, fp);
		fflush(fp);
	}

	free(knlist);

	fclose(fp);

	return 1;
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
	printf("                                 \n");
	printf("---------------------------------\n");
	printf("                                 \n");
	printf("        Insert Key & Name        \n");
	printf("                                 \n");
	printf("If you want to quit input key [q]\n");
	printf("---------------------------------\n");
	printf("                                 \n");

	insert();

	printf("                                 \n");
	printf("Exit the Program.                \n");
	printf("                                 \n");

	return 0;
}
