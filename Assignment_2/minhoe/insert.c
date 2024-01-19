/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : code, name ���� ����
* DATE         :
* AUTHOR       :
******************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"kntable.h"

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
int FileCount(FILE* fd)
{
    char dum[55] = {0,};
    int rec_cnt = 0;

    if ((fd = fopen("file.txt", "r+")) == NULL)
    {
        printf("file open fail!\n");
        return -1;
    }

    //���� ó������
    fseek(fd,0,SEEK_SET);

    //�� �� �� ������ ī��Ʈ ��
    while (fgets(dum,55,fd) !=NULL)
    {
        rec_cnt++;
    }

    fclose(fd);

    return rec_cnt;
}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : ���Ͽ��� �޸𸮷� ������ �ε�
PARAMETERS  : FILE *fd      - File Descriptor
              KEYNAME* kn   - ���� ������ ���� �� �޸�
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(FILE* fd,KEYNAME* kn)
{
    char code_buf[10] = {0,};
    char name_buf[41] = {0,};

    int ii = 0;

    if ((fd = fopen("file.txt", "r+")) == NULL)
    {
        printf("file open fail!\n");
        return -1;
    }

    fseek(fd,0,SEEK_SET);

    while (fscanf(fd,"%s",code_buf) != EOF)
    {
        fscanf(fd,"%s", name_buf);
        memcpy(kn[ii].code, code_buf, sizeof(code_buf));
        memcpy(kn[ii].name, name_buf, sizeof(name_buf));

        ii++;
    }

    fclose(fd);

    return 1;
}
/******************************************************************************
FUNCTION    : InsertFile
DESCRIPTION : ���Ͽ� ������ �ۼ�
PARAMETERS  : FILE *fd      - File Descriptor
              KEYNAME* kn   - ���� ������ ���� �� �޸�
              rec_cnt       - ���� ���Ͽ� ����Ǿ��ִ� ���ڵ��� ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int InsertFile(FILE* fd, KEYNAME* kn, int rec_cnt)
{
    int ii = 0;

    if ((fd = fopen("file.txt", "r+")) == NULL)
    {
        printf("file open fail!\n");
        return -1;
    }

    for( ii=0; ii<rec_cnt+1; ii++)
    {
        fprintf(fd, "%-10s\t%s\n", kn[ii].code, kn[ii].name);
    }

    fclose(fd);

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
    FILE* fd = NULL;
    KEYNAME* kn = NULL;

    char code_buf[10] = {0,};
    char name_buf[41] = {0,};
    int rec_cnt = 0;
    int ii = 0;

    while (1)
    {
        printf("\nInsert Key & Name\n");
        printf("If you want to quit input code [q]\n");
        printf("Input Key[9] : ");
        scanf("%9s", code_buf);

        //q �Է½� ���α׷� ����    
        if (strcmp(code_buf,"q") == 0) break;

        printf("Input name[40] : ");
        scanf("%40s", name_buf);

        while (getchar() != '\n'){}

        //fprintf(fd, "%-10s\t%s\n", kn[rec_cnt].code, kn[rec_cnt].name);
        //���� ���� ���� ���ڵ� ���� ī��Ʈ�ϰ� ���ڵ� �� ��ŭ ���� �޸𸮸� �Ҵ��Ѵ�
        rec_cnt = FileCount(fd);
        kn = (KEYNAME *) calloc(rec_cnt +1, sizeof(KEYNAME));

        //���Ͽ��� �޸𸮷� �����͸� �Űܿ´�
        FileToMem(fd,kn);

        //���Ͽ��� �޸𸮷� �ű� �����͵� �ڿ� �����͸� �߰��Ѵ�
        memcpy(kn[rec_cnt].code, code_buf, sizeof(code_buf));
        memcpy(kn[rec_cnt].name, name_buf, sizeof(name_buf));

        //����
        qsort((KEYNAME*)kn, rec_cnt+1, sizeof(KEYNAME), Cmp);

        InsertFile(fd, kn, rec_cnt);

        // �޸� ����
        free(kn);
    }

    // �������� ���ϰ� �޸� ����
    fclose(fd);

   return 0;
}
