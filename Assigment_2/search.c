/******************************************************************************
* PROGRAM NAME : search
* SOURCE NAME  : search.c
* DESCRIPTION  : key, name ���� �˻�
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
DESCRIPTION : ���Ͽ��� ������ �ε�
PARAMETERS  : KEYNAME *knlist - �ε��� �����͸� ������ �޸�
              int num         - ���� �� ���ڵ� ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
    KEYNAME   *tkey			= NULL;
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
FUNCTION    : Search
DESCRIPTION : �˻� Ű�� �ش��ϴ� ������ ��ȸ
PARAMETERS  : KEYNAME *knlist - �˻��� �����Ͱ� ����� �޸�
              int num         - �˻��� �������� ��
              char *key       - �˻��� Ű
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Search(KEYNAME *knlist, int num, char *key)
{
    KEYNAME keyname;
    KEYNAME *result; // �˻� ���
    KEYNAME *start;  // �˻� ����� ���� ��ġ
    KEYNAME *end;    // �˻� ����� �� ��ġ
    KEYNAME *now;    // �˻� ����� ���� ��ġ

    /*--- �˻� Ű ���� ---*/
    strcpy(keyname.key, key);

    /*--- bsearch ---*/
    result = (KEYNAME *)bsearch(&keyname, knlist, num, sizeof(KEYNAME), Cmp);
    if (result == NULL) // �˻� ����
    {
        return -1;
    }
    else // �˻� ����
    {
        // �˻� ����� ���� ��ġ
        start = result;
        while (start > knlist && Cmp(start-1, &keyname) == 0)
            start--;

        // �˻� ����� �� ��ġ
        end = result;
        while (end < knlist + num && Cmp(end+1, &keyname) == 0)
            end++;

        // �˻� ��� ���
        printf("---------------------------------\n");
        for (now = start; now <= end; now++)
        {
            printf("key : %-*.*s Name : %-*.*s\n",
                   sizeof(now->key)-1, sizeof(now->key)-1, now->key,
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
    char    key[KEYSIZE] = {0,};
    int     user         = 0;
    int     num          = 0;
    int		rtn;

    printf("                                 \n");
    printf("---------------------------------\n");
    printf("                                 \n");
    printf("    Enter a key to find name     \n");
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
            printf("file open error[%d]\n", errno);
                return -1;
        }

        /*--- �˻� Ű �Է� ---*/
        printf("Input key(9) : ");
        scanf("%s", key);
        printf("\n");

        /*--- �˻� ���� ---*/
        rtn = Search(g_KnList, num, key);
        if (rtn < 0) // �˻� ����
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
            break;

        // ���� �Ҵ��� �޸� ����
        if (g_KnList != NULL)
            free(g_KnList);
    }

    return 0;
}