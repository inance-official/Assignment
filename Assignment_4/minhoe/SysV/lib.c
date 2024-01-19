/******************************************************************************
 * FILE NAME   : lib.c
 * DESCRIPTION : 공유 메모리 함수
 * DATE        :
 * AUTHOR      :
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "kntable.h"
#include "glomem.h"
/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
              const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int Cmp(const void *a, const void *b)
{

    return strcmp(((KEYNAME*)a)->key,((KEYNAME*)b)->key);
}


/******************************************************************************
FUNCTION    : CountShm
DESCRIPTION : 공유 메모리 내의 레코드 개수
PARAMETERS  :
RETURNED    : count - 레코드 개수
******************************************************************************/
int CountShm()
{
    int rec_cnt = 0;
    int shm_id = 0;

    //공유메모리의 id값을 가져옴
    shm_id = shmget(SHM_KEY, 0, PERM);

    GlobalAttachShm();

    //key 값이 입력되어있을 경우, rec_cnt를 증가
    while (strcmp(g_KnList[rec_cnt].key,"") != 0)
    {
        rec_cnt++;
    }

    GlobalDetachShm();

    return rec_cnt;
}

/******************************************************************************
FUNCTION    : lInsertShm
DESCRIPTION : 공유 메모리에 데이터 입력
PARAMETERS  :
RETURNED    :  1(SUCCESS)
******************************************************************************/
int lInsertShm()
{
    char key_buf[KEYSIZE] = {0,};
    char name_buf[NAMESIZE] = {0,};

    int rec_cnt;

    //현재 레코드 개수를 가져옴 
    rec_cnt = CountShm();

    while(1)
    {
        printf("\nInsert Key & Name\n");
        printf("Input Key[9] : ");
        scanf("%9s", key_buf);

        while (getchar() != '\n'){}

        //q 입력시 프로그램 종료    
        if (strcmp(key_buf,"q") == 0) break;

        //중복 키 값 존재시 입력 종료
        if (DupKeyChk(key_buf) < 0) break;

        printf("Input name[40] : ");
        scanf("%40s", name_buf);

        while (getchar() != '\n'){}

        GlobalAttachShm();

        //데이터 입력
        memcpy(g_KnList[rec_cnt].key, key_buf, sizeof(key_buf));
        memcpy(g_KnList[rec_cnt].name, name_buf, sizeof(name_buf));

        rec_cnt++;

        //입력된 데이터 정렬
        qsort(g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

        GlobalDetachShm();
    }

    return 1;
}
/******************************************************************************
FUNCTION    : DupKeyChk
DESCRIPTION : 공유 메모리에 데이터 입력시 중복 키 값 확인
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int DupKeyChk(char *key_buf)
{
    int ii = 0;
    int rec_cnt = 0;

    //현재 레코드 개수 가져옴
    rec_cnt = CountShm();

    GlobalAttachShm();

    //모든 레코드 탐색
    for( ii=0; ii < rec_cnt; ii++)
    {
        //입력하고자 하는 key 값과 같은 값이 존재 할 경우
        if (strncmp(g_KnList[ii].key, key_buf, sizeof(key_buf)) == 0)
        {
            printf("A duplicate key exists.\n");
            printf("%-9s    %-s\n","[Key]", "[Name]");
            printf("%-9s    %-s\n", g_KnList[ii].key, g_KnList[ii].name);

            GlobalDetachShm();
            return -1;
        }
    }
    GlobalDetachShm();

    return 1;
}
/******************************************************************************
FUNCTION    : ReadShm
DESCRIPTION : 공유 메모리 데이터 출력
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lReadShm()
{
    char key_buf[KEYSIZE] = {0,};
    char name_buf[NAMESIZE] = {0,};
    int rec_cnt;
    int ii = 0;
    time_t now;
    struct tm* t;

    while(1)
    {
        system("clear");

        // 시간 구하기
        now = time(NULL);
        t = localtime(&now);

        rec_cnt = CountShm();

        GlobalAttachShm();

        printf("Update Key (2sec)\n");
        printf("Date : %d-%02d-%02d %02d:%02d:%02d\n\n",t -> tm_year + 1900, t -> tm_mon + 1, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec);
        printf("%-10s\t%-40s\n", "[Key]", "[NAME]");

        //공유 메모리 상의 데이터 출력
        for (ii = 0 ; ii < rec_cnt ; ii++)
        {
            printf("%-10s\t", g_KnList[ii].key);
            printf("%-40s\n", g_KnList[ii].name);
        }

        GlobalDetachShm();
        sleep(2);
    }

    return 1;
}

/******************************************************************************
FUNCTION    : SearchShm
DESCRIPTION : 공유 메모리에 데이터 검색
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lSearchShm()
{
    int rec_cnt;
    KEYNAME find_kn;
    KEYNAME* kn_res = NULL;

    printf("---------------------------------");
    printf("\n\n    Enter a key to find name\n\n");
    printf("---------------------------------\n\n");

    //찾고자 하는 key 값 입력
    printf("Input key(9) : ");
    scanf("%9s", find_kn.key);

    //현재 레코드 개수 가져옴
    rec_cnt = CountShm();

    GlobalAttachShm();

    //bsearch를 이용하여 찾고자하는 값을 kn_res에 저장
    kn_res = bsearch(&find_kn.key, g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

    //값이 없을 경우
    if (kn_res == NULL)
    {
        printf("\n---------------------------------");
        printf("\nNot Found. key[%s]\n",find_kn.key);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
    }
    //값이 존재 할 경우 값 출력
    else printf("Key : %s\t Name : %s\n", kn_res -> key, kn_res -> name);

    GlobalDetachShm();

    return 0;
}
/******************************************************************************
FUNCTION    : DeleteShm
DESCRIPTION : 공유 메모리에 데이터 삭제
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lDeleteShm()
{
    int fin_sig;
    int del_sig;
    int ii;
    int rec_cnt;
    KEYNAME del_kn;
    KEYNAME *kn_res;

    printf("---------------------------------");
    printf("\n\n    Enter a key to delete\n\n");
    printf("---------------------------------\n\n");

    //지우고자 하는 key 값 입력
    printf("Input key(9) : ");
    scanf("%9s", del_kn.key);

    //현재 레코드 개수 가져옴
    rec_cnt = CountShm();

    GlobalAttachShm();

    //지우고자 하는 key 값 검색
    kn_res = bsearch(&del_kn.key, g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

    if (kn_res == NULL)
    {
        //값이 없을 경우
        printf("\n---------------------------------");
        printf("\nNot Found. key[%s]\n",del_kn.key);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
        return -1;
    }
    //값이 존재 할 경우 삭제 재확인
    printf("\n---------------------------------\n");
    printf("Key : %s\t Name : %s\n", kn_res -> key, kn_res -> name);

    printf("\nAre you sure? (1)yes (2)no : ");
    scanf("%d",&fin_sig);
    while (getchar() != '\n'){}

    //삭제 확정 시 실행
    if (fin_sig == 1)
    {
        //삭제 해당 레코드 위치 탐색
        for (ii = 0; ii < rec_cnt; ii++)
        {
            if (g_KnList[ii].key == kn_res -> key) break;
        }

        //삭제 해당 레코드부터 한 칸씩 당겨서 입력
        //마지막 레코드는 리셋
        for (; ii < rec_cnt - 1; ii++)
        {
            memcpy(g_KnList[ii].key, g_KnList[ii + 1].key, sizeof(g_KnList[ii + 1].key));
            memcpy(g_KnList[ii].name, g_KnList[ii + 1].name, sizeof(g_KnList[ii + 1].name));
            memset(&g_KnList[ii + 1],0x00,sizeof(KEYNAME));

        }
        printf("Delete Complete\n\n");
    }

    GlobalDetachShm();
}