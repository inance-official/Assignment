/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : code, name ���� ����
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
    int			fd, rlen;
    char		rbuff[BUFFER_SIZE] = {0,};
    int			num				   = 0;
    int			ii				   = 0;

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
DESCRIPTION : ���� ������ �޸𸮷� �ε�
PARAMETERS  : KEYNAME *knlist - �ε��� �����͸� ������ �޸�
              int num         - ���� �� ���ڵ� ��
RETURNED    : num - ���� �� ���ڵ� ��
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
    KEYNAME         *tkey                  = NULL;
    int              fd, rlen;
    char             rbuff[BUFFER_SIZE]    = {0,};
    int              ii;

    /*--- ���� �б���� ���� ---*/
    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    /*--- ���Ͽ��� ������ �о���� ---*/
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
FUNCTION    : InputKeyName
DESCRIPTION : Ű �� �̸� �Ը�
PARAMETERS  : KEYNAME *knlist - �Է¹��� �����͸� ������ �޸�
RETURNED    : 1(SUCCESS)
******************************************************************************/
int InputKeyName(KEYNAME *knlist)
{
    char temp[128];
    int  rtn = 0;

    /*--- key �Է� ---*/
    printf("Input Key[9] : ");
    scanf("%s", temp);
    memcpy(knlist->key, temp, strlen(temp));

    /*--- q �Է½� ���α׷� ���� ---*/
    if(strcmp(knlist->key, "q") == 0)
    {
        if (knlist != NULL)
            free(knlist);

        /*--- ���Ͽ� ������ ���� �� ���� ---*/
        rtn = SortSave();
        if (rtn < 0)
        {
            printf("SortSave() error!! [%d]\n", errno);
            return -1;
        }

        printf("                                  \n");
        printf("Exit the Program.                 \n");
        printf("                                  \n");
        exit(0); // ���α׷� ����

    }

    /*--- Name �Է� ---*/
    printf("Input Name[40] : ");
    scanf("%s", temp);
    memcpy(knlist->name, temp, strlen(temp));
    printf("                                 \n");
    printf("---------------------------------\n");
    printf("                                 \n");

    return 1;
}

/******************************************************************************
FUNCTION    : DoubleChk
DESCRIPTION : Ű �ߺ� üŰ
PARAMETERS  : int fd        - ���� ��ũ����
              char *inputkey- �Է¹��� Ű
RETURNED    : 1(SUCCESS)
******************************************************************************/
int DoubleChk(int fd, char *inputkey)
{
    KEYNAME *knlist = NULL;
    int num = 0;
    int ii;

    num = FileCount();

    knlist = (KEYNAME *)malloc(sizeof(KEYNAME) * num);
    memset(knlist, 0x00, sizeof(KEYNAME) * num);

    FileToMem(knlist, num);

    for (ii = 0; ii < num; ii++)
    {
        if (strcmp(knlist[ii].key, inputkey) == 0)
        {
            printf("                                 \n");
            printf("A duplicate key exists.          \n");
            printf("                                 \n");
            printf("%-9s%-50s\n","[Key]", "[Name]");
            printf(" %-*.*s%-*.*s\n",
                sizeof(knlist[ii].key)-1, sizeof(knlist[ii].key)-1, knlist[ii].key,
                sizeof(knlist[ii].name)-1, sizeof(knlist[ii].name)-1, knlist[ii].name);
            printf("                                 \n");
            printf("---------------------------------\n");
            printf("                                 \n");
            free(knlist);
            return -1; // �ߺ��� Ű�� ������ ��Ÿ���� ���� �ڵ� ��ȯ
        }
    }

    free(knlist);
    return 1; // �ߺ��� Ű�� ������ ��Ÿ���� �ڵ� ��ȯ
}

/******************************************************************************
FUNCTION    : Insert
DESCRIPTION : ���Ͽ� ������ ����
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Insert()
{
    int			 fd, rlen;

    KEYNAME		*knlist		 		= NULL, *tkey = NULL;
    int			 ii, rtn, dup;
    char         rbuff[BUFFER_SIZE] = {0,};
    char       	 sbuff[64] 	 		= {0,};
    int			 count		 		= 0;
    char		 temp[128];

    /*--- �Է� ---*/
    fd = open("./file.txt", O_RDWR | O_CREAT | O_APPEND, 0777); //������ ������ �а� ���� ����
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    while (1)
    {
        /*--- �޸� �Ҵ� ---*/
        knlist = (KEYNAME *)malloc(sizeof(KEYNAME));
        memset(knlist, 0x00, sizeof(KEYNAME));

        InputKeyName(knlist);

        /*--- �ߺ� üũ ---*/
        rtn = DoubleChk(fd, knlist->key);
        while (rtn < 0)
        {
            InputKeyName(knlist);
            rtn = DoubleChk(fd, knlist->key);
        }
        printf("                                 \n");

        /*--- ���Ͽ� ���� ---*/
        write(fd, knlist, sizeof(KEYNAME));

        /*--- �޸� ���� ---*/
        if (knlist != NULL)
        {
            free(knlist);
        }
    }

    close(fd);

    return 1;
}

/******************************************************************************
FUNCTION    : SortSave
DESCRIPTION : ���Ͽ� ������ ���� �� ����
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int SortSave()
{
    int			 fd, rlen;

    KEYNAME		*knlist		 		= NULL, *tkey = NULL;
    int			 ii, rtn;
    char         rbuff[BUFFER_SIZE] = {0,};
    char       	 sbuff[64] 	 		= {0,};
    int			 count		 		= 0;
    char		 temp[128];

    /*--- ��ü ������ �޸� �ε� �� ���� --*/
    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    count = FileCount();
    knlist = (KEYNAME *)malloc(sizeof(KEYNAME) * count);
    memset(knlist, 0x00, sizeof(KEYNAME) * count);

    ii = 0;

    while (rlen = read(fd, rbuff, sizeof(KEYNAME)) > 0)
    {
           tkey = (KEYNAME *)rbuff;
           memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
        ii++;

        if (ii >= count)
            break;
    }

    close(fd);

    // �迭�� ����
    qsort(knlist, count, sizeof(KEYNAME), Cmp);

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

    if (knlist != NULL)
    {
        free(knlist);
    }

    close(fd);

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
    int rtn;

    printf("                                  \n");
    printf("----------------------------------\n");
    printf("                                  \n");
    printf("        Insert Code & Name        \n");
    printf("                                  \n");
    printf("If you want to quit input code [q]\n");
    printf("                                  \n");
    printf("----------------------------------\n");
    printf("                                  \n");

    /*--- ���Ͽ� ������ ���� ---*/
    Insert();


    return 0;
}



