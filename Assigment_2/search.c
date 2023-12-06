/******************************************************************************
* PROGRAM NAME : search
* SOURCE NAME  : search.c
* DESCRIPTION  : key, name 파일 검색
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
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
              const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
    KEYNAME *k1 = (KEYNAME*)aa;
    KEYNAME *k2 = (KEYNAME*)bb;
    return strcmp(k1 -> key, k2 -> key);
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
DESCRIPTION : 파일에서 데이터 로드
PARAMETERS  : KEYNAME *knlist - 로드한 데이터를 저장할 메모리
              int num         - 파일 내 레코드 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
    KEYNAME   *tkey			= NULL;
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

        tkey = (KEYNAME *)sbuff;

        memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
    }

    fclose(fp);

    return 1;
}

/******************************************************************************
FUNCTION    : Search
DESCRIPTION : 검색 키에 해당하는 데이터 조회
PARAMETERS  : KEYNAME *knlist - 검색할 데이터가 저장된 메모리
              int num         - 검색할 데이터의 수
              char *key       - 검색할 키
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Search(KEYNAME *knlist, int num, char *key)
{
    KEYNAME keyname;
    KEYNAME *result; // 검색 결과
    KEYNAME *start;  // 검색 결과의 시작 위치
    KEYNAME *end;    // 검색 결과의 끝 위치
    KEYNAME *now;    // 검색 결과의 현재 위치

    /*--- 검색 키 설정 ---*/
    strcpy(keyname.key, key);

    /*--- bsearch ---*/
    result = (KEYNAME *)bsearch(&keyname, knlist, num, sizeof(KEYNAME), Cmp);
    if (result == NULL) // 검색 실패
    {
        return -1;
    }
    else // 검색 성공
    {
        // 검색 결과의 시작 위치
        start = result;
        while (start > knlist && Cmp(start-1, &keyname) == 0)
            start--;

        // 검색 결과의 끝 위치
        end = result;
        while (end < knlist + num && Cmp(end+1, &keyname) == 0)
            end++;

        // 검색 결과 출력
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
DESCRIPTION : 메인 함수
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
        /*--- 메모리 할당 ---*/
        num = FileCount();

        g_KnList = (KEYNAME *)malloc(sizeof(KEYNAME) * num);
        memset(g_KnList, 0x00, sizeof(KEYNAME) * num);

        /*--- 파일에서 데이터 로드 ---*/
        rtn = FileToMem(g_KnList, num);
        if (rtn < 0)
        {
            printf("file open error[%d]\n", errno);
                return -1;
        }

        /*--- 검색 키 입력 ---*/
        printf("Input key(9) : ");
        scanf("%s", key);
        printf("\n");

        /*--- 검색 수행 ---*/
        rtn = Search(g_KnList, num, key);
        if (rtn < 0) // 검색 실패
        {
            printf("---------------------------------\n");
            printf("Not Found. key[%s]\n", key);
            printf("Search failed. Please try again. \n");
            printf("---------------------------------\n");
        }

        printf("                                 \n");
        printf("Do you search more? (1)yes (2)no : ");
        scanf("%d", &user); // 재검색 여부
        printf("                                 \n");

        if (user == 2) // 2:no 종료
            break;

        // 동적 할당한 메모리 해제
        if (g_KnList != NULL)
            free(g_KnList);
    }

    return 0;
}