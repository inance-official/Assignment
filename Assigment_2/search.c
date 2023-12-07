/******************************************************************************
* PROGRAM NAME : search
* SOURCE NAME  : search.c
* DESCRIPTION  : code, name ���� �˻�
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
DESCRIPTION : ���Լ�
PARAMETERS  : const void *aa - ���� ������
			  const void *bb - ���� ������
RETURNED    : strcmp()�� ��ȯ��
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
	CODENAME *k1 = (CODENAME*)aa;
	CODENAME *k2 = (CODENAME*)bb;
	return strcmp(k1 -> code, k2 -> code);
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : ���� �� ���ڵ� �� ��ȯ
PARAMETERS  :
RETURNED    : num - ���� �� ���ڵ� ��
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
DESCRIPTION : ���Ͽ��� ������ �ε�
PARAMETERS  : CODENAME *cnlist - �ε��� �����͸� ������ �޸�
			  int num         - ���� �� ���ڵ� ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(CODENAME *cnlist, int num)
{
	CODENAME   *tcode		= NULL;
	FILE      *fp           = NULL;
	char       sbuff[64]    = {0,};
	int        ii;

	// ���� �б���� ����
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
DESCRIPTION : �˻� Ű�� �ش��ϴ� ������ ��ȸ
PARAMETERS  : CODENAME *cnlist - �˻��� �����Ͱ� ����� �޸�
			  int num         - �˻��� �������� ��
			  char *code       - �˻��� Ű
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Search(CODENAME *cnlist, int num, char *code)
{
	CODENAME codename;
	CODENAME *result; // �˻� ���
	CODENAME *start;  // �˻� ����� ���� ��ġ
	CODENAME *end;    // �˻� ����� �� ��ġ
	CODENAME *now;    // �˻� ����� ���� ��ġ

	/*--- �˻� Ű ���� ---*/
	strcpy(codename.code, code);

	/*--- bsearch ---*/
	result = (CODENAME *)bsearch(&codename, cnlist, num, sizeof(CODENAME), Cmp);
	if (result == NULL) // �˻� ����
	{
		return -1;
	}
	else // �˻� ����
	{
		// �˻� ����� ���� ��ġ
		start = result;
		while (start > cnlist && Cmp(start-1, &codename) == 0)
			start--;

		// �˻� ����� �� ��ġ
		end = result;
		while (end < cnlist + num && Cmp(end+1, &codename) == 0)
			end++;

		// �˻� ��� ���
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
DESCRIPTION : ���� �Լ�
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
		/*--- �޸� �Ҵ� ---*/
		num = FileCount();

		g_CnList = (CODENAME *)malloc(sizeof(CODENAME) * num);
		memset(g_CnList, 0x00, sizeof(CODENAME) * num);

		/*--- ���Ͽ��� ������ �ε� ---*/
		rtn = FileToMem(g_CnList, num);
		if (rtn < 0)
		{
			printf("file open error[%d]\n", errno);
				return -1;
		}

		/*--- �˻� Ű �Է� ---*/
		printf("Input code(9) : ");
		scanf("%s", code);
		printf("\n");

		/*--- �˻� ���� ---*/
		rtn = Search(g_CnList, num, code);
		if (rtn < 0) // �˻� ����
		{
			printf("---------------------------------\n");
			printf("Not Found. code[%s]\n", code);
			printf("Search failed. Please try again. \n");
			printf("---------------------------------\n");
		}

		printf("                                 \n");
		printf("Do you search more? (1)yes (2)no : ");
		scanf("%d", &user); // ��˻� ����
		printf("                                 \n");

		if (user == 2) // 2:no ����
			break;

		// ���� �Ҵ��� �޸� ����
		if (g_CnList != NULL)
			free(g_CnList);
	}

	return 0;
}
