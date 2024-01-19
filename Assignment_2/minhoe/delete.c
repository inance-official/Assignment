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
****************************************************************************/
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
FUNCTION    : Delete
DESCRIPTION : Ű�� �Է¹޾� �ش� Ű�� ���� ������ ����
PARAMETERS  : FILE* fd          - File Descriptor
              KEYNAME* kn       - ���� ������� ����Ǿ��ִ� ����ü �迭 ������
              KEYNAME* find_kn  - �����ϰ��� �ϴ� ���� �ִ� ����ü ������
              int rec_cnt       - ���� ���ڵ��� ��
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Delete(FILE* fd, KEYNAME* kn, KEYNAME find_kn, int rec_cnt)
{
    KEYNAME* kn_res = NULL;
    KEYNAME* new_kn = NULL;

    int hit_cnt     = 0;
    int rm_idx      = 0;
    int st_ii       = 0;
    int fin_ii      = 0;
    int ii          = 0;
    int fin_sig     = 0;

    if ((fd = fopen("file.txt", "r+")) == NULL)
        {
            printf("file open fail!\n");
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

    //ó�� hit �� ���� ii�� ������ �� ii�� ������ ��Ű�� ���� �ε����� �� �ε����� �������ش�.
    st_ii = kn_res - kn;
    while (Cmp(&find_kn.code, &(kn[st_ii].code)) == 0)
    {
        if (st_ii <= 0) break;
        st_ii--;
    }

    fin_ii = kn_res - kn;
    while(Cmp(&find_kn.code, &(kn[fin_ii].code)) == 0)
    {
        if (fin_ii >= (rec_cnt - 1)) break;
        fin_ii++;
    }

    //���� �ε����� �� �ε����� �ٸ� ��� ��, �ߺ� code���� ���� �� ���
    if (st_ii != fin_ii)
    {
        //�ߺ��� code ���� ��� ���
        for(ii = st_ii; ii < fin_ii; ii++)
        {
            printf("Key : %s\t Name : %s\n", kn[ii].code, kn[ii].name);
        }

        //�̸��� �Է� �޾� ���� ���ڵ带 �����Ѵ�.
        printf("\nEnter the name to delete : ");
        scanf("%s", find_kn.name);


        //�ߺ� code �� �� �Է¹��� name�� ���� ���ڵ带 ã�� ���� �ε����� �����Ѵ�.    
        for (ii = st_ii; ii < fin_ii && Cmp(&find_kn.code, &(kn[ii].code)) == 0; ++ii)
        {
            if (Cmp(&find_kn.name, &(kn[ii].name)) == 0)
            {
                printf("Key : %s\t Name : %s\n", kn[ii].code, kn[ii].name);
                rm_idx = ii;
            }
        }
    }
    //�ߺ� code ���� �������� ������� ��� ���� �ε����� �����Ѵ�.
    else
    {
        printf("Key : %s\t Name : %s\n", kn[st_ii].code, kn[st_ii].name);
        rm_idx = st_ii;
    }

    printf("\nAre you sure? (1)yes (2)no : ");
    scanf("%d",&fin_sig);
    while (getchar() != '\n'){}

    if (fin_sig == 1)
    {
        //������ �ش�Ǵ� ���ڵ带 �����ϰ� �����
        fseek(fd, 0, SEEK_SET);
        for(ii=0; ii<rec_cnt; ii++)
        {
            if (ii != rm_idx)
            {
                fprintf(fd, "%-10s\t%s\n", kn[ii].code, kn[ii].name);
            }
         }

        //���� ũ�� ����
        ftruncate(fileno(fd),ftell(fd));

        printf("Delete Complete\n\n");
        printf("Save Complete\n\n");
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
    FILE*       fd = NULL;
    KEYNAME*    kn = NULL;
    KEYNAME*    kn_res = NULL;
    KEYNAME     find_kn;

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

        Delete(fd, kn, find_kn, rec_cnt);

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
