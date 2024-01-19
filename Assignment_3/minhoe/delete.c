/******************************************************************************
 * PROGRAM NAME : delete
 * SOURCE NAME  : delete.c
 * DESCRIPTION  : ���Ͽ��� ������ ����
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
FUNCTION    : Delete
DESCRIPTION : Ű�� �Է¹޾� �ش� Ű�� ���� ������ ����
PARAMETERS  : FILE* fd          - File Descriptor
              KEYNAME* kn       - ���� ������� ����Ǿ��ִ� ����ü �迭 ������
              KEYNAME* find_kn  - �����ϰ��� �ϴ� ���� �ִ� ����ü ������
              int rec_cnt       - ���� ���ڵ��� ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Delete(KEYNAME* kn, KEYNAME find_kn, int rec_cnt)
{
    KEYNAME* kn_res = NULL;
    KEYNAME* new_kn = NULL;

    int hit_cnt     = 0;
    int rm_idx      = 0;
    int st_ii       = 0;
    int fin_ii      = 0;
    int ii          = 0;
    int fin_sig     = 0;
    int fd          = 0;

    if ((fd = open("file.txt", O_RDWR)) < 0)
    {
        printf("file open fail!2\n");
        return -1;
    }

    kn_res =  bsearch(&find_kn.code, kn, rec_cnt, sizeof(KEYNAME), Cmp);

    if (kn_res == NULL)
    {
        printf("\n---------------------------------");
        printf("\nNot Found. code[%s]\n",find_kn.code);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
        return -1;
    }

    printf("\n---------------------------------\n");

    //���� �ε����� �����Ѵ�.
    printf("Key : %s\t Name : %s\n", kn_res -> code, kn_res -> name);
    printf("\nAre you sure? (1)yes (2)no : ");
    scanf("%d",&fin_sig);
    while (getchar() != '\n'){}

    if (fin_sig == 1)
    {
        //������ �ش�Ǵ� ���ڵ带 �����ϰ� �����
        lseek(fd, 0, SEEK_SET);
        for(ii=0; ii<rec_cnt; ii++)
        {
            if ((kn+ii) -> code != kn_res -> code)
            {
                write(fd, (kn+ii)->code, sizeof((kn+ii)->code));
                write(fd, (kn+ii)->name, sizeof((kn+ii)->name));
            }
         }

        //���� ũ�� ����
        truncate("file.txt",lseek(fd, 0, SEEK_CUR));

        printf("Delete Complete\n\n");
        printf("Save Complete\n\n");
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
    KEYNAME*    kn = NULL;
    KEYNAME*    kn_res = NULL;
    KEYNAME     find_kn;

    int     fd = 0;
    char    find_code[10] = {0,};
    int     fin_sig = 0;
    int     rec_cnt = 0;

    //�ʱ����� �ҷ�����
    if ((fd = fopen("file.txt", "r+")) == NULL)
    {
        printf("file open fail!\n");
        return -1;
    }

    //�ʱ����� ���ڵ� ī��Ʈ �� �޸� �Ҵ�, ����
    rec_cnt = FileCount(fd);
    kn = (KEYNAME *) calloc(rec_cnt, sizeof(KEYNAME));
    FileToMem(fd,kn);

    while(1)
    {
        printf("---------------------------------");
        printf("\n\n Enter a code to delete name\n\n");
        printf("---------------------------------\n\n");
        printf("Input code(9) : ");
        scanf("%s",find_kn.code);

        Delete(kn, find_kn, rec_cnt);

        printf("Do you delete more? (1)yes (2)no : ");
        scanf("%d",&fin_sig);
        while (getchar() != '\n'){}

        if (fin_sig == 2) break;
        else if(fin_sig != 1) printf("! Wrong response\n");

        if ((fd = fopen("file.txt", "r+")) == NULL)
        {
            printf("file open fail!\n");
            return -1;
        }

        rec_cnt = FileCount(fd);
        //kn = (KEYNAME *) calloc(rec_cnt, sizeof(KEYNAME));
        kn = realloc(kn,sizeof(KEYNAME) * rec_cnt);
        FileToMem(fd,kn);
    }

    kn = NULL;
    free(kn);
    return 0;
}
