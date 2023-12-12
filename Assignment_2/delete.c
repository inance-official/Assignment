/******************************************************************************
* PROGRAM NAME : delete
* SOURCE NAME  : delete.c
* DESCRIPTION  : 파일에서 데이터 삭제
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
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
              const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
    CODENAME *k1 = (CODENAME*)aa;
    CODENAME *k2 = (CODENAME*)bb;
    return strcmp(k1 -> code, k2 -> code);
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
DESCRIPTION : 파일에서 메모리로 데이터 로드
PARAMETERS  : CODENAME *cnlist - 로드한 데이터를 저장할 메모리
              int num         - 파일 내 레코드 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(CODENAME *cnlist, int num)
{
    CODENAME   *tcode         = NULL;
    FILE      *fp           = NULL;
    char       sbuff[64]    = {0,};
    int        ii;

    // 파일 읽기모드로 열기
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
DESCRIPTION : 키를 입력받아 해당 키를 가진 데이터 삭제
PARAMETERS  : CODENAME *cnlist - 삭제할 데이터가 저장된 메모리
              int count       - 메모리 내 데이터 수
              char *delcode    - 삭제할 데이터의 키
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Delete(CODENAME *cnlist, int count, char *delcode)
{
    int    chk                = 0;      // 입력 키 비교
    int    user               = 0;      // 사용자 입력
    int    matchcnt           = 0;      // 키 일치 수
    char   delname[NAMESIZE] = {0,};    // 삭제할 데이터의 이름
    int    ii;

    /*--- 키 일치 및 중복 ---*/
    for (ii = 0; ii < count; ii++)
    {
        if (strcmp(cnlist[ii].code, delcode) == 0)
        {
            matchcnt++;
        }
    }

    /*--- 삭제 ---*/
    if (matchcnt > 1) // 키 중복
    {
        printf("---------------------------------\n");
        for (ii = 0; ii < count; ii++) // 중복 키에 대한 목록 출력
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

        // 삭제 여부
        printf("Are you sure? (1)yes (2)no : ");
        scanf("%d", &user);
        printf("                                 \n");

        if (user == 1) // 1:yes 삭제
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
        else if (user == 2) // 2:no 취소
        {
            printf("Delete canceled                  \n");
            printf("                                 \n");
            return 1;
        }
    }
    else // 단일 키
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

                if (user == 1) // 1:yes 삭제
                {
                    memmove(&cnlist[ii], &cnlist[ii+1], sizeof(CODENAME) * (count - ii));
                    count--;
                    printf("Delete Complete                  \n");
                    printf("                                 \n");
                    break;
                }
                else if (user == 2) // 2:no 취소
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

    /*--- 전체 데이터 신규 저장 --*/
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
DESCRIPTION : 메인 함수
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
        /*--- 메모리 할당 ---*/
        count = FileCount();

        g_CnList = (CODENAME *)malloc(sizeof(CODENAME) * count);
        memset(g_CnList, 0x00, sizeof(CODENAME) * count);

        /*--- 파일에서 데이터 로드 ---*/
        rtn = FileToMem(g_CnList, count);
        if (rtn < 0)
        {
            printf("file open error[%d]\n", errno)
            return -1;
        }

        /*--- 키 입력 ---*/
        printf("Input code(9) : ");
        scanf("%s", delcode);
        printf("                                 \n");

        /*--- 삭제하고 재배열 ---*/
        rtn = Delete(g_CnList, count, delcode);
        if (rtn < 0) // 삭제 실패
        {
            printf("Delete failed. Please try again. \n");
            printf("                                 \n");
        }

        printf("Do you delete more? (1)yes (2)no : ");
        scanf("%d", &user);
        printf("---------------------------------\n");
        printf("                                 \n");
        if (user == 2)	// 2:no 종료
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



