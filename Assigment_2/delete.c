/******************************************************************************
* PROGRAM NAME : delete
* SOURCE NAME  : delete.c
* DESCRIPTION  : 파일에서 데이터 삭제
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "kntable.h"

KEYNAME   *g_KnList = NULL;

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
			  const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
	KEYNAME *k1 = (KEYNAME*)aa;
	KEYNAME *k2 = (KEYNAME*)bb;
	return strcmp(k1 -> key, k2 -> key);
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
DESCRIPTION : 파일에서 메모리로 데이터 로드
PARAMETERS  : KEYNAME *knlist - 로드한 데이터를 저장할 메모리
			  int num         - 파일 내 레코드 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
	KEYNAME   *tkey         = NULL;
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

		tkey = (KEYNAME *)sbuff;

		memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
	}

	fclose(fp);

	return 1;
}

/******************************************************************************
FUNCTION    : SortSave
DESCRIPTION : 정렬 및 저장
PARAMETERS  : KEYNAME *knlist - 정렬할 데이터가 저장된 메모리
			  int count       - 메모리 내 데이터 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int SortSave(KEYNAME *knlist, int count)
{
	FILE     *fp = NULL;
	int       ii;

	// 배열을 정렬
	qsort(knlist, count, sizeof(KEYNAME), Cmp);

	/*--- 전체 데이터 신규 저장 --*/
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

	printf("Save Complete                    \n");
	printf("                                 \n");

	fclose(fp);

	return 1;
}

/******************************************************************************
FUNCTION    : Delete
DESCRIPTION : 키를 입력받아 해당 키를 가진 데이터 삭제
PARAMETERS  : KEYNAME *knlist - 삭제할 데이터가 저장된 메모리
			  int count       - 메모리 내 데이터 수
			  char *delkey    - 삭제할 데이터의 키
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Delete(KEYNAME *knlist, int count, char *delkey)
{
	int chk                 = 0;      // 입력 키 비교
	int user                = 0;      // 사용자 입력
	int matchcnt            = 0;      // 키 일치 수
	char delname[NAMESIZE] = {0,};    // 삭제할 데이터의 이름
	int ii;

	/*--- 키 일치 및 중복 ---*/
	for (ii = 0; ii < count; ii++)
	{
		if (strcmp(knlist[ii].key, delkey) == 0)
		{
			matchcnt++;
		}
	}

	/*--- 삭제 ---*/
	if (matchcnt > 1) // 키 중복
	{
		printf("---------------------------------\n");
		for (ii = 0; ii < count; ii++) // 중복 키에 대한 목록 출력
		{
			if (strcmp(knlist[ii].key, delkey) == 0)
			{
				printf("Key : %s\tName : %s\n", knlist[ii].key, knlist[ii].name);
			}
		}
		printf("---------------------------------\n");
		printf("                                 \n");

		printf("Enter the name to delete: ");
		scanf("%s", delname);
		printf("                                 \n");
	   	printf("Key : %s\tName : %s\n", delkey, delname);
		printf("                                 \n");

		// 삭제 여부
		printf("Are you sure? (1)yes (2)no : ");
		scanf("%d", &user);
		printf("                                 \n");

		if (user == 1) // 1:yes 삭제
		{
			for (ii = 0; ii < count; ii++)
			{
				if (strcmp(knlist[ii].key, delkey) == 0 &&
					strcmp(knlist[ii].name, delname) == 0)
				{
					memmove(&knlist[ii], &knlist[ii+1], sizeof(KEYNAME) * (count - ii - 1));
					count--;
					printf("Delete Complete                  \n");
					printf("                                 \n");
					break;
				}
			}
		}
		else if (user == 2) // 2:no 취소
		{
			printf("Delete canceled                  \n");
			printf("                                 \n");
			return 1;
		}
	}
	else // 단일 키
	{
		for (ii = 0; ii < count; ii++)
		{
			chk = strcmp(knlist[ii].key, delkey);

			if (chk == 0)
			{
				printf("Key : %s\tName : %s\n", knlist[ii].key, knlist[ii].name);
				printf("                                 \n");
				printf("Are you sure? (1)yes (2)no : ");
				scanf("%d", &user);
				printf("                                 \n");

				if (user == 1) // 1:yes 삭제
				{
					memmove(&knlist[ii], &knlist[ii+1], sizeof(KEYNAME) * (count - ii));
					count--;
					printf("Delete Complete                  \n");
					printf("                                 \n");
					break;
				}
				else if (user == 2) // 2:no 취소
				{
					printf("Delete canceled                  \n");
					printf("                                 \n");
					return 1;
				}
			}
		}
	}

	if(chk != 0)
	{
		printf("Not Found. key[%s]\n", delkey);
		printf("                                 \n");
		return -1;
	}

	// 정렬 및 저장
	SortSave(knlist, count);

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
	char 	delkey[KEYSIZE]  = {0,};
	int     user         	 = 0;
	int		rtn;
	int 	count			 = 0;

	printf("                                 \n");
	printf("---------------------------------\n");
	printf("                                 \n");
	printf("  Enter a key to Delete recode   \n");
	printf("                                 \n");
	printf("---------------------------------\n");
	printf("                                 \n");

	while (1)
	{
		/*--- 메모리 할당 ---*/
		count = FileCount();

   		g_KnList = (KEYNAME *)malloc(sizeof(KEYNAME) * count);
		memset(g_KnList, 0x00, sizeof(KEYNAME) * count);

		/*--- 파일에서 데이터 로드 ---*/
		rtn = FileToMem(g_KnList, count);
		if (rtn < 0)
		{
			return -1;
		}

		/*--- 키 입력 ---*/
		printf("Input key(9) : ");
		scanf("%s", delkey);
		printf("                                 \n");

		/*--- 삭제하고 재배열 ---*/
		rtn = Delete(g_KnList, count, delkey);
		if (rtn < 0) // 삭제 실패
		{
			printf("Delete failed. Please try again. \n");
			printf("                                 \n");
		}

		printf("Do you delete more? (1)yes (2)no : ");
		scanf("%d", &user);
		printf("---------------------------------\n");
		printf("                                 \n");
		if (user == 2)	// 2:no 종료
		{
			break;
			printf("Exit the Program                 \n");
			printf("                                 \n");
		}

		if (g_KnList != NULL)
			free(g_KnList);
	}

	return 0;
}



