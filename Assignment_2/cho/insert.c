/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : code, name 파일 저장
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cntable.h"

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
              const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
    CODENAME *k1 = (CODENAME *)aa;
    CODENAME *k2 = (CODENAME *)bb;

    return strcmp(k1->code, k2->code);
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    : num - 파일 내 레코드 수
******************************************************************************/
int FileCount()
{
    FILE      *fp        = NULL;
    char       rbuff[64] = {0,};
    int        num       = 0;

    fp = fopen("./file.txt", "r");
    if (fp == NULL)
    {
        printf("file open error[%d]\n", errno);
        return -1;
    }

    while (fgets(rbuff, sizeof(rbuff), fp) != NULL)
    {
        num++;
    }

    fclose(fp);

    return num;
}

/******************************************************************************
FUNCTION    : Insert
DESCRIPTION : 파일에 데이터 저장
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Insert()
{
    FILE      *fp        = NULL;

    CODENAME   *cnlist    = NULL, *tcode = NULL;
    int        rcnt      = 0;
    int        ii        = 0;
    char       rbuff[64] = { 0, };
    int        count     = 0;

    /*--- 사용자 데이터 INSERT ---*/
    fp = fopen("./file.txt", "a+");
    if (fp == NULL)
    {
        printf("file open error[%d]\n", errno);
        return -1;
    }

    for (ii = 0; ii < rcnt + 1; ii++)
    {
        rcnt++;
        cnlist = (CODENAME *)malloc(sizeof(CODENAME) * rcnt);
        memset(cnlist, 0x00, sizeof(CODENAME) * rcnt);

        printf("\n");

        printf("Input code[9] : ");
        scanf("%s", cnlist[ii].code);

        if (strcmp(cnlist[ii].code, "q") == 0)
            break;

        printf("Input name[40] : ");
        scanf("%s", cnlist[ii].name);

        fwrite(&cnlist[ii], 1, sizeof(CODENAME), fp);
        fwrite("\n", 1, 1, fp);

        fflush(fp);

        if (cnlist != NULL)
            free(cnlist);
    }

    fclose(fp);

    return 1;
}

/******************************************************************************
FUNCTION    : SortSave
DESCRIPTION : 정렬 및 저장
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int SortSave()
{
    FILE     *fp = NULL;

    CODENAME   *cnlist    = NULL, *tcode = NULL;
    int        rcnt      = 0;
    int        ii        = 0;
    char       rbuff[64] = { 0, };
    int        count     = 0;

    /*--- 전체 데이터 메모리 로드 및 정렬 ---*/
    fp = fopen("./file.txt", "r");
    if (fp == NULL)
    {
        printf("file open error[%d]\n", errno);
        return -1;
    }

    count = FileCount();
    cnlist = (CODENAME *)malloc(sizeof(CODENAME) * count);
    memset(cnlist, 0x00, sizeof(CODENAME) * count);

    for (ii = 0; ii < count; ii++)
    {
        fgets(rbuff, sizeof(rbuff), fp);

        tcode = (CODENAME *)rbuff;

        memcpy(&cnlist[ii], tcode, sizeof(CODENAME));
    }

    fclose(fp);

    // 배열을 정렬
    qsort(cnlist, count, sizeof(CODENAME), Cmp);

    /*--- 전체 데이터 신규 저장 ---*/
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

    free(cnlist);

    fclose(fp);

    return 1;
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
    int rtn;

    printf("                                 \n");
    printf("---------------------------------\n");
    printf("                                 \n");
    printf("       Insert Code & Name        \n");
    printf("                                 \n");
    printf("If you want to quit input code [q]\n");
    printf("---------------------------------\n");
    printf("                                 \n");

    rtn = Insert();
    if (rtn < 0)
    {
        printf("Insert error[%d]\n", rtn);
        return -1;
    }

    rtn = SortSave();
    if (rtn < 0)
    {
        printf("SortSave error[%d]\n", rtn);
        return -1;
    }

    printf("                                 \n");
    printf("Exit the Program.                \n");
    printf("                                 \n");

    return 0;
}
