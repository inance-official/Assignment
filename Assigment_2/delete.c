/******************************************************************************
* PROGRAM NAME : delete
* SOURCE NAME  : delete.c
* DESCRIPTION  : ���Ͽ��� ������ ����
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
DESCRIPTION : ���Լ�
PARAMETERS  : const void *aa - ���� ������
			  const void *bb - ���� ������
RETURNED    : strcmp()�� ��ȯ��
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
	KEYNAME *k1 = (KEYNAME*)aa;
	KEYNAME *k2 = (KEYNAME*)bb;
	return strcmp(k1 -> key, k2 -> key);
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
DESCRIPTION : ���Ͽ��� �޸𸮷� ������ �ε�
PARAMETERS  : KEYNAME *knlist - �ε��� �����͸� ������ �޸�
			  int num         - ���� �� ���ڵ� ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
	KEYNAME   *tkey         = NULL;
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

		tkey = (KEYNAME *)sbuff;

		memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
	}

	fclose(fp);

	return 1;
}

/******************************************************************************
FUNCTION    : SortSave
DESCRIPTION : ���� �� ����
PARAMETERS  : KEYNAME *knlist - ������ �����Ͱ� ����� �޸�
			  int count       - �޸� �� ������ ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int SortSave(KEYNAME *knlist, int count)
{
	FILE     *fp = NULL;
	int       ii;

	// �迭�� ����
	qsort(knlist, count, sizeof(KEYNAME), Cmp);

	/*--- ��ü ������ �ű� ���� --*/
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
DESCRIPTION : Ű�� �Է¹޾� �ش� Ű�� ���� ������ ����
PARAMETERS  : KEYNAME *knlist - ������ �����Ͱ� ����� �޸�
			  int count       - �޸� �� ������ ��
			  char *delkey    - ������ �������� Ű
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Delete(KEYNAME *knlist, int count, char *delkey)
{
	int chk                 = 0;      // �Է� Ű ��
	int user                = 0;      // ����� �Է�
	int matchcnt            = 0;      // Ű ��ġ ��
	char delname[NAMESIZE] = {0,};    // ������ �������� �̸�
	int ii;

	/*--- Ű ��ġ �� �ߺ� ---*/
	for (ii = 0; ii < count; ii++)
	{
		if (strcmp(knlist[ii].key, delkey) == 0)
		{
			matchcnt++;
		}
	}

	/*--- ���� ---*/
	if (matchcnt > 1) // Ű �ߺ�
	{
		printf("---------------------------------\n");
		for (ii = 0; ii < count; ii++) // �ߺ� Ű�� ���� ��� ���
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

		// ���� ����
		printf("Are you sure? (1)yes (2)no : ");
		scanf("%d", &user);
		printf("                                 \n");

		if (user == 1) // 1:yes ����
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
		else if (user == 2) // 2:no ���
		{
			printf("Delete canceled                  \n");
			printf("                                 \n");
			return 1;
		}
	}
	else // ���� Ű
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

				if (user == 1) // 1:yes ����
				{
					memmove(&knlist[ii], &knlist[ii+1], sizeof(KEYNAME) * (count - ii));
					count--;
					printf("Delete Complete                  \n");
					printf("                                 \n");
					break;
				}
				else if (user == 2) // 2:no ���
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

	// ���� �� ����
	SortSave(knlist, count);

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
		/*--- �޸� �Ҵ� ---*/
		count = FileCount();

   		g_KnList = (KEYNAME *)malloc(sizeof(KEYNAME) * count);
		memset(g_KnList, 0x00, sizeof(KEYNAME) * count);

		/*--- ���Ͽ��� ������ �ε� ---*/
		rtn = FileToMem(g_KnList, count);
		if (rtn < 0)
		{
			return -1;
		}

		/*--- Ű �Է� ---*/
		printf("Input key(9) : ");
		scanf("%s", delkey);
		printf("                                 \n");

		/*--- �����ϰ� ��迭 ---*/
		rtn = Delete(g_KnList, count, delkey);
		if (rtn < 0) // ���� ����
		{
			printf("Delete failed. Please try again. \n");
			printf("                                 \n");
		}

		printf("Do you delete more? (1)yes (2)no : ");
		scanf("%d", &user);
		printf("---------------------------------\n");
		printf("                                 \n");
		if (user == 2)	// 2:no ����
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



