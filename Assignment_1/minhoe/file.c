/*******************************************************************************
* ���α׷� �̸� : file
* �ҽ����� �̸� : file.c
* ������ �ۼ��� : 2023�� 12�� 06��
* �� �� ��      : INANCE
* �� ��         : ���� 1 ���� ����� ó�� 
*******************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define CODESIZE 9
#define NAMESIZE 40

int g_cnt   = 0;
int g_ii    = 0;

typedef struct _STOCK
{
    char code[CODESIZE+1];
    char name[NAMESIZE+1];
} STOCKS;


/*******************************************************************************
�Լ���   : Add()
�Ķ���� : STOCKS* stock - STOCKS ����ü
           FILE* fd      - File Description
���ϰ�   : ����   - ��� or 0
           ������ - ����
����     : ���� �Է�
*******************************************************************************/

int Add(STOCKS* stock, FILE* fd)
{
    char code_buf[10] = {0,};
    char name_buf[41] = {0,};

    fseek(fd,0,SEEK_END);

    //code �Է� �� stock ����ü�� ����
    printf("\n[\"CODE\"�� �Է����ּ���]\n");
    printf("CODE (1~9) : ");
    scanf("%9s", code_buf);
    memcpy(stock[g_ii].code, code_buf, CODESIZE);

    // �Է� ���� ����
    while (getchar() != '\n'){}

    //name �Է� �� stock ����ü�� ����
    printf("[\"NAME\"�� �Է����ּ���]\n");
    printf("NAME (1~40) : ");
    scanf("%40s", name_buf);
    memcpy(stock[g_ii].name, name_buf, NAMESIZE);

    // �Է� ���� ����
    while (getchar() != '\n'){}

    //stock ����ü�� ������ ���� ������ file.txt�� �Է�
    fprintf(fd, "%-10s\t%s\n", stock[g_ii].code, stock[g_ii].name);
    printf("[���� �Է��� �����߽��ϴ�]\n");

    //�������� �ε����� g_ii�� ��������, �Ҵ�� �޸𸮸�ŭ�� �Է¹���
    g_ii++;

    return 1;
}

/*******************************************************************************
** �Լ���   : Read()
** �Ķ���� : FILE* fd - File Description
** ���ϰ�   : ����   - ��� or 0
**            ������ - ����
** ����     : file.txt�κ��� ������ �о�� ȭ�� �� ���
*******************************************************************************/
int Read(FILE* fd)
{
    STOCKS st;

    fseek(fd,0,SEEK_SET);

    printf("\n---------------------------------------\n");
    printf("%-10s\t%-40s\n", "CODE", "NAME");
    printf("---------------------------------------\n");

    //tab���� ���� �� ���� ������ �ҷ��� st ����ü�� code�� ���� �� ���
    while (fscanf(fd,"%s",st.code) != EOF)
    {
        printf("%-10s\t", st.code);

        //�������� ���� �� ���� ������ �ҷ��� st ����ü�� name�� ���� �� ���
        fscanf(fd,"%s", st.name);
        printf("%-40s\n", st.name);
    }
    printf("---------------------------------------\n\n");

    return 1;
}
/*******************************************************************************
** �Լ���   : Search()
** �Ķ���� : FILE* fd - File Description
** ���ϰ�   : ����   - ��� or 0
**            ������ - ����
** ����     : code�� �Է� �޾�, code�� ��ġ�ϴ� name ���
*******************************************************************************/
int Search(FILE* fd)
{
    STOCKS st;
    char buf[10]={0,};

    fseek(fd,0,SEEK_SET);

    //�˻��ϰ��� �ϴ� code �Է¹޾� buf�� �Է�
    printf("\n[\"CODE\"�� �Է����ּ���]\n");
    printf("code : ");
    scanf("%s", buf);

    while (fscanf(fd,"%s", st.code) != EOF)
    {
        fscanf(fd,"%s", st.name);
        //�Է¹��� code ���� ���Ͽ��� �ҷ��� code ���� ���� ��ġ �� ��� ��� �� Ż��
        if (strncmp(st.code, buf, sizeof(buf)) == 0)
        {
            printf("\n---------------------------------------\n");
            printf("%-10s\t%-40s\n", "code", "name");
            printf("---------------------------------------\n");
            printf("%-10s\t", st.code);
            printf("%-40s\n", st.name);
            printf("---------------------------------------\n");
            break;
            }
    }

    return 1;
}

/*******************************************************************************
** �Լ���   : main()
** �Ķ���� : int  argc    - �������� ����
**           char argv**  - ��������
** ���ϰ�   : ����   - 0(EXIT_SUCCESS)
**            ������ - 1(EXIT_FAILURE)
** ����     : main �Լ�
*******************************************************************************/
int main()
{
    int mode;
    STOCKS* stock;
    FILE* fd;

    //�ް����ϴ� �������� ���� �Է¹޾� �������� �޸� �Ҵ�
    printf("\n[�������� ���� �Է����ּ���]\n");
    scanf("%d", &g_cnt);

    stock = (STOCKS *)calloc(g_cnt, sizeof(STOCKS));
//  memset(stock, 0x00, sizeof(stock)); 

    //���� open, ������ ���ϰ� ����
    if ((fd = fopen("file.txt", "w+")) == NULL)
        {
            printf("file open fail!\n");
            return -1;
        }

    while (1)
    {
        printf("\n[�޴��� �������ּ���]\n");
        printf("1.ADD\n2.READ\n3.SEARCH\n4.EXIT\n");
        printf("INSERT MENU : ");

        scanf("%d", &mode);
        switch (mode)
        {
            case 1 :
                //���� index�� �ް����ϴ� ������ ���� �ʰ��� ��� add ����� �۵����� �ʵ��� ��
                if (g_ii >= g_cnt)
                {
                    printf("!�Է� ������ ������ ���� �ʰ��Ͽ����ϴ�!\n");
                    break;
                }
                Add(stock,fd);
                break;
            case 2 :
                Read(fd);
                break;
            case 3 :
                Search(fd);
                break;
            case 4 :
                free(stock);
                fclose(fd);
                return 1;
        }
    }
}
