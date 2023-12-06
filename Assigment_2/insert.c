/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : key, name ���� ����
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
DESCRIPTION : ���Լ�
PARAMETERS  : const void *aa - ���� ������
			  const void *bb - ���� ������
RETURNED    : strcmp()�� ��ȯ��
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
	KEYNAME *k1 = (KEYNAME *)aa;
	KEYNAME *k2 = (KEYNAME *)bb;

	return strcmp(k1->key, k2->key);
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
DESCRIPTION : ���Ͽ��� �޸𸮷� ������ �ε�
PARAMETERS  : KEYNAME *knlist - �ε��� �����͸� ������ �޸�
			  int num         - ���� �� ���ڵ� ��
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

	/*--- ����� ������ INSERT ---*/
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

	/*--- ��ü ������ �޸� �ε� �� ���� ---*/
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

	// �迭�� ����
	qsort(knlist, count, sizeof(KEYNAME), Cmp);

	/*--- ��ü ������ �ű� ���� ---*/
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
DESCRIPTION : ���� �Լ�
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
