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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "kntable.h"

#define BUFFER_SIZE 9999

KEYNAME   *g_KnList = NULL;

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : ���Լ�
PARAMETERS  : const void *aa - ���� ������
              const void *bb - ���� ������
RETURNED    : strcmp()�� ��ȯ��
******************************************************************************/
int cmp(const void *aa, const void *bb)
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
    int         fd, rlen;
    char        rbuff[BUFFER_SIZE] = {0,};
    int         num                = 0;
    int         ii                 = 0;

    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    while ((rlen = read(fd, rbuff, sizeof(KEYNAME))) > 0)
    {
        if (rlen == sizeof(KEYNAME))
            num++;
    }

    close(fd);

    return num;
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : ���� �� ���ڵ� �� ��ȯ
PARAMETERS  :
RETURNED    : num - ���� �� ���ڵ� ��
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
    KEYNAME         *tkey                  = NULL;
    int              fd, rlen;
    char             rbuff[BUFFER_SIZE]    = {0,};
    int              ii;

    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    for (ii = 0; ii < num; ii++)
    {
        rlen = read(fd, rbuff, sizeof(KEYNAME));

        tkey = (KEYNAME *)rbuff;

        memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
    }

    close(fd);

    return 1;
}

/******************************************************************************
FUNCTION    : Delete
DESCRIPTION : Ű�� �Է¹޾� �ش� Ű�� ���� ������ ����
PARAMETERS  : CODENAME *cnlist - ������ �����Ͱ� ����� �޸�
              int count        - �޸� �� ������ ��
              char *delkey     - ������ �������� Ű
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Delete(KEYNAME *knlist, int count, char *delkey)
{
    int          fd;
    int          kcmp   = 0;
    int          ii, jj;


    for (ii = 0; ii < count; ii++)
    {
    	kcmp = strcmp(knlist[ii].key, delkey);

        if (kcmp == 0)
        {
            printf("\n");
            printf("Key : %s\tName : %s\n", knlist[ii].key, knlist[ii].name);

            memmove(&knlist[ii], &knlist[ii+1], sizeof(KEYNAME) * (count - ii));
            count--;

            printf("\n");

			//�迭�� ����
			qsort(knlist, count, sizeof(KEYNAME), cmp);

            /*--- ��ü ������ �ű� ���� --*/
		    fd = open("./file.txt", O_WRONLY | O_TRUNC);
		    if (fd < 0)
			{
				printf("file open error! error[%d]\n", errno);
				return -1;
			}

		    for (ii = 0; ii < count; ii++)
		    {
		        write(fd, &knlist[ii], sizeof(KEYNAME));
		    }

    		close(fd);

			printf("Delete Complete\n");
		}
	}

    if(kcmp != 0)
    {
        printf("Not Found\n");
    }

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
    char    delkey[KEYSIZE]  = {0,};
    int     user             = 0;
    int     rtn;
    int     count            = 0;

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
        printf("\n");
        printf("[Delete]\n");
        printf("\n");
        printf("Input key : ");
        scanf("%s", delkey);

        /*--- �����ϰ� ��迭 ---*/
        rtn = Delete(g_KnList, count, delkey);
        if (rtn < 0)
        {
            return -1;
        }

        printf("Do you search more? (1)yes (2)no : ");
        scanf("%d", &user);

        if (user == 2)  // (2)no --> ���α׷� ����
		{
            break;
		}

        if (g_KnList != NULL)
		{
            free(g_KnList);
		}
    }

    return 0;
}

