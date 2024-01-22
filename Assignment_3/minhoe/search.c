/******************************************************************************
* PROGRAM NAME : search
* SOURCE NAME  : search.c
* DESCRIPTION  : code, name ���� �˻�
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "kntable.h"
/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : ���Լ�
PARAMETERS  : const void *a - �� ������ 1
              const void *b - �� ������ 2
RETURNED    : strcmp�� ��ȯ ��
******************************************************************************/
int Cmp(const void *a, const void *b)
{
    return strcmp(((KEYNAME*)a)->code,((KEYNAME*)b)->code);
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : ���� �� ���ڵ� �� ��ȯ
PARAMETERS  : FILE* fd - File Descriptor
RETURNED    : rec_cnt - ���� �� ���ڵ� ��
******************************************************************************/
int FileCount(int fd)
{
    char dum[1024] = {0,};
    int rec_cnt = 0;

    if ((fd = open("file.txt", O_RDWR | O_CREAT,0666)) <0)
    {
        printf("file open fail!1\n");
        return -1;
    }

    //�� �� �� ������ ī��Ʈ ��
    while (read(fd,dum,sizeof(KEYNAME)) > 0)
    {
        rec_cnt++;
    }

    close(fd);
    return rec_cnt;
}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : ���Ͽ��� �޸𸮷� ������ �ε�
PARAMETERS  : FILE *fd      - File Descriptor
              KEYNAME* kn   - ���� ������ ���� �� �޸�
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(int fd,KEYNAME* kn)
{
    char code_buf[10] = {0,};
    char name_buf[41] = {0,};

    int ii = 0;

    if ((fd = open("file.txt", O_RDWR)) < 0)
    {
        printf("file open fail!2\n");
        return -1;
    }

    lseek(fd,0,SEEK_SET);

    while (read(fd, code_buf, sizeof(code_buf)) > 0)
    {
        read(fd, name_buf, sizeof(name_buf));
        memcpy(kn[ii].code, code_buf, sizeof(code_buf));
        memcpy(kn[ii].name, name_buf, sizeof(name_buf));


        ii++;
    }

    close(fd);

    return 1;
}

/******************************************************************************
FUNCTION    : Search
DESCRIPTION : �˻� Ű�� �ش��ϴ� ������ ��ȸ
PARAMETERS  : KEYNAME* kn       - ���� ������ ���� �� �޸�
              KEYNAME find_kn   - ã���� �ϴ� ������ �����ϴ� ����ü
              int rec_cnt       - �� ���ڵ��� ��
RETURNED    : 1(SUCCESS) 
******************************************************************************/
int Search(KEYNAME* kn, KEYNAME find_kn, int rec_cnt)
{
    KEYNAME* kn_res = NULL;

    int ii = 0;
    int st_ii = 0;
    int fin_ii = 0;

    kn_res =  bsearch(&find_kn.code, kn, rec_cnt, sizeof(KEYNAME), Cmp);

    if (kn_res == NULL)
    {   //���� ���� ���
        printf("\n---------------------------------");
        printf("\nNot Found. code[%s]\n",find_kn.code);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
    }
    else printf("Key : %s\t Name : %s\n", kn_res ->code, kn_res -> name);

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
    int         fd = 0;
    KEYNAME*    kn = NULL;
    KEYNAME*    kn_res = NULL;

    KEYNAME     find_kn;
    char    find_code[10] = {0,};
    int     fin_sig=0;
    int     rec_cnt = 0;

    //���ڵ� �� ī��Ʈ �� ���� ���� �� �޸� �Ҵ�, ����
    rec_cnt = FileCount(fd);
    kn = (KEYNAME *) calloc(rec_cnt, sizeof(KEYNAME));
    FileToMem(fd,kn);

    while(1)
    {
        printf("---------------------------------");
        printf("\n\n    Enter a code to find name\n\n");
        printf("---------------------------------\n\n");
        printf("Input code(9) : ");
        scanf("%s",find_kn.code);

        Search(kn, find_kn, rec_cnt);

        printf("Do you search more? (1)yes (2)no : ");
        scanf("%d",&fin_sig);
        while (getchar() != '\n'){}

        if (fin_sig == 2) break;
        else if(fin_sig != 1) printf("! Wrong response\n");
    }

    free(kn);
    return 0;
}
