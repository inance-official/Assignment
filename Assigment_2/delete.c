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
DESCRIPTION : ���Ͽ��� �޸𸮷� ������ �ε�
PARAMETERS  : CODENAME *cnlist - �ε��� �����͸� ������ �޸�
              int num         - ���� �� ���ڵ� ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(CODENAME *cnlist, int num)
{
    CODENAME   *tcode         = NULL;
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
FUNCTION    : Delete
DESCRIPTION : Ű�� �Է¹޾� �ش� Ű�� ���� ������ ����
PARAMETERS  : CODENAME *cnlist - ������ �����Ͱ� ����� �޸�
              int count       - �޸� �� ������ ��
              char *delcode    - ������ �������� Ű
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Delete(CODENAME *cnlist, int count, char *delcode)
{
    int    chk                = 0;      // �Է� Ű ��
    int    user               = 0;      // ����� �Է�
    int    matchcnt           = 0;      // Ű ��ġ ��
    char   delname[NAMESIZE] = {0,};    // ������ �������� �̸�
    int    ii;

    /*--- Ű ��ġ �� �ߺ� ---*/
    for (ii = 0; ii < count; ii++)
    {
        if (strcmp(cnlist[ii].code, delcode) == 0)
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
            if (strcmp(cnlist[ii].code, delcode) == 0)
            {
                printf("Code : %s\tName : %s\n", cnlist[ii].code, cnlist[ii].name);
            }
        }
        printf("---------------------------------\n");
        printf("                                 \n");

        printf("Enter the name to delete: ");
        scanf("%s", delname);
        printf("                                 \n");
        printf("Code : %s\tName : %s\n", delcode, delname);
        printf("                                 \n");

        // ���� ����
        printf("Are you sure? (1)yes (2)no : ");
        scanf("%d", &user);
        printf("                                 \n");

        if (user == 1) // 1:yes ����
        {
            for (ii = 0; ii < count; ii++)
            {
                if (strcmp(cnlist[ii].code, delcode) == 0 &&
                    strcmp(cnlist[ii].name, delname) == 0)
                {
                    memmove(&cnlist[ii], &cnlist[ii+1], sizeof(CODENAME) * (count - ii - 1));
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
            chk = strcmp(cnlist[ii].code, delcode);

            if (chk == 0)
            {
                printf("Code : %s\tName : %s\n", cnlist[ii].code, cnlist[ii].name);
                printf("                                 \n");
                printf("Are you sure? (1)yes (2)no : ");
                scanf("%d", &user);
                printf("                                 \n");

                if (user == 1) // 1:yes ����
                {
                    memmove(&cnlist[ii], &cnlist[ii+1], sizeof(CODENAME) * (count - ii));
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
        printf("Not Found. code[%s]\n", delcode);
        printf("                                 \n");
        return -1;
    }

    /*--- ��ü ������ �ű� ���� --*/
    fp = fopen("./file.txt", "w+");
    if (fp == NULL)
    {
        printf("file open error[%d]\n", errno);
        return -1;
    }

    for (ii = 0; ii < count; ii++)
    {
        fwrite(&cnlist[ii], 1, sizeof(CODENAME), fp);
        fwrite("\n", 1, 1, fp);
        fflush(fp);
    }

    printf("Save Complete                    \n");
    printf("                                 \n");

    fclose(fp);

    return 1;
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
    char 	delcode[CODESIZE]  = {0,};
    int     user         	 = 0;
    int		rtn;
    int 	count			 = 0;

    printf("                                 \n");
    printf("---------------------------------\n");
    printf("                                 \n");
    printf("  Enter a code to Delete recode   \n");
    printf("                                 \n");
    printf("---------------------------------\n");
    printf("                                 \n");

    while (1)
    {
        /*--- �޸� �Ҵ� ---*/
        count = FileCount();

        g_CnList = (CODENAME *)malloc(sizeof(CODENAME) * count);
        memset(g_CnList, 0x00, sizeof(CODENAME) * count);

        /*--- ���Ͽ��� ������ �ε� ---*/
        rtn = FileToMem(g_CnList, count);
        if (rtn < 0)
        {
            printf("file open error[%d]\n", errno)
            return -1;
        }

        /*--- Ű �Է� ---*/
        printf("Input code(9) : ");
        scanf("%s", delcode);
        printf("                                 \n");

        /*--- �����ϰ� ��迭 ---*/
        rtn = Delete(g_CnList, count, delcode);
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

        if (g_CnList != NULL)
            free(g_CnList);
    }

    return 0;
}



