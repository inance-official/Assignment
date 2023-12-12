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
FUNCTION    : FileToMem
DESCRIPTION : ���Ͽ��� ������ �ε�
PARAMETERS  : CODENAME *cnlist - �ε��� �����͸� ������ �޸�
              int num         - ���� �� ���ڵ� ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
    KEYNAME			*tkey         		   = NULL;
    int				 fd, rlen;
    char			 rbuff[BUFFER_SIZE]    = {0,};
    int				 ii;

    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error\n");
        return -1;
    }

    for	(ii = 0; ii < num; ii++)
    {
        rlen = read(fd, rbuff, sizeof(KEYNAME));

        tkey = (KEYNAME *)rbuff;

        memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
    }

    close(fd);

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
int Search(KEYNAME *knlist, int num, char *key)
{
    KEYNAME 	 keyname;
    KEYNAME		*result;

    strcpy(keyname.key, key);

    result = (KEYNAME *)bsearch(&keyname, knlist, num, sizeof(KEYNAME), Cmp);
    if (result == NULL) // �˻� ����
    {
        return -1;
    }
    else
    {
        printf("---------------------------------\n");
        printf("key : %-*.*s Name : %-*.*s\n",
            sizeof(result->key)-1, sizeof(result->key)-1, result->key,
            sizeof(result->name)-1, sizeof(result->name)-1, result->name);
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
    char    key[KEYSIZE] = {0,};
    int     user         = 0;
    int     num          = 0;
    int     rtn;

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

        g_KnList = (KEYNAME *)malloc(sizeof(KEYNAME) * num);
        memset(g_KnList, 0x00, sizeof(KEYNAME) * num);

        /*--- ���Ͽ��� ������ �ε� ---*/
        rtn = FileToMem(g_KnList, num);
        if (rtn < 0)
        {
            printf("file open error! error[%d]\n");
            return -1;
        }

        /*--- �˻� Ű �Է¹ޱ� ---*/
        printf("Input key : ");
        scanf("%s", key);
        printf("                                 \n");

        /*--- �˻� ���� ---*/
        rtn = Search(g_KnList, num, key);
        if (rtn < 0)
        {
			printf("---------------------------------\n");
			printf("Not Found. key[%s]\n", key);
			printf("Search failed. Please try again. \n");
			printf("---------------------------------\n");
        }

        printf("                                 \n");
        printf("Do you search more? (1)yes (2)no : ");
        scanf("%d", &user); // ��˻� ����
        printf("                                 \n");

        if (user == 2) // 2:no ����
        {
            printf("Exit the program.                \n");
            printf("                                 \n");
            break;
        }


        if (g_KnList != NULL)
            free(g_KnList);
  }

    return 0;
}

