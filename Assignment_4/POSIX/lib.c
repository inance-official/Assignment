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
    char dum[1024] = {0,};
    int rec_cnt = 0;
    int fd = 0;

    fd = shm_open(SHMNAME, O_RDWR, PERM);

    //공유메모리의 데이터를 한 줄씩 가져와 카운팅
    while (read(fd,dum,sizeof(KEYNAME)) > 0)
    {
        rec_cnt++;
    }

    close(fd);

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

    int ii = 0;
    int fd = 0;
    int rec_cnt;

    //fd값을 가져옴
    fd = shm_open(SHMNAME, O_RDWR, 0644);

    while(1)
    {
        //데이터를 추가하기 위해 파일의 끝으로 이동
        lseek(fd, 0, SEEK_END);

        printf("\nInsert Key & Name\n");
        printf("Input Key[9] : ");
        scanf("%9s", key_buf);

        while (getchar() != '\n'){}

        //q 입력시 프로그램 종료    
        if (strcmp(key_buf,"q") == 0) break;
       //중복 키 값 존재시 입력 종료
        if (DupKeyChk(fd, key_buf) < 0) break;

        printf("Input name[40] : ");
        scanf("%40s", name_buf);

        while (getchar() != '\n'){}

        //데이터 입력
        write(fd, key_buf, sizeof(key_buf));
        write(fd, name_buf, sizeof(name_buf));
    }

        rec_cnt = CountShm();

        GlobalAttachShm(fd);

        //입력된 데이터 정렬
        qsort(g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

        GlobalDetachShm();

        close(fd);


    return 1;
}
/******************************************************************************
FUNCTION    : DupKeyChk
DESCRIPTION : 공유 메모리에 데이터 입력시 중복 키 값 확인
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int DupKeyChk(int fd, char *key_buf)
{
    int ii = 0;
    int rec_cnt = 0;

    //현재 레코드 개수 가져옴
    rec_cnt = CountShm();

    GlobalAttachShm(fd);
    //모든 레코드 탐색
    for( ii=0; ii < rec_cnt; ii++)
    {
        //입력하고자 하는 key 값과 같은 값이 존재 할 경우
        if (strncmp((g_KnList+ii) -> key, key_buf, sizeof(key_buf)) == 0)
        {
            printf("A duplicate key exists.\n");
            printf("%-9s    %-s\n","[Key]","[Name]");
            printf("%-9s    %-s\n", (g_KnList + ii) -> key, (g_KnList + ii) -> name);

            GlobalDetachShm();

            return -1;
        }
    }
    GlobalDetachShm();
    return 1;
}

/******************************************************************************
FUNCTION    : ReadShm
DESCRIPTION : 공유 메모리에 데이터 출력
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lReadShm()
{
    int fd = 0;
    char key_buf[KEYSIZE] = {0,};
    char name_buf[NAMESIZE] = {0,};
    time_t now;
    struct tm* t;
    int rec_cnt;

    while(1)
    {
        fd = shm_open(SHMNAME, O_RDWR, 0644);
        system("clear");

        // 시간 구하기
        now = time(NULL);
        t = localtime(&now);

        rec_cnt = CountShm();

        printf("Update Key (2sec)\n");
        printf("Date : %d-%02d-%02d %02d:%02d:%02d\n\n",t -> tm_year + 1900, t -> tm_mon + 1, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec);
        printf("%-10s\t%-40s\n", "[Key]", "[NAME]");

        //공유 메모리 상의 데이터 출력
        while (read(fd, key_buf,sizeof(key_buf)) > 0)
        {
            printf("%-10s\t", key_buf);

            read(fd, name_buf, sizeof(name_buf));
            printf("%-40s\n", name_buf);
        }

        sleep(2);

        close(fd);
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
    int fd = 0;
    KEYNAME find_kn;
    KEYNAME* kn_res = NULL;

    printf("---------------------------------");
    printf("\n\n    Enter a key to find name\n\n");
    printf("---------------------------------\n\n");

    //찾고자 하는 key 값 입력
    printf("Input key(9) : ");
    scanf("%9s", find_kn.key);

    fd = shm_open(SHMNAME, O_RDWR,0644);
    if (fd == -1)
    {
        perror("shm_open");
        return -1;
    }

    //현재 레코드 개수 가져옴
    rec_cnt = CountShm();

    GlobalAttachShm(fd);

    //bsearch를 이용하여 찾곡자하는 값을 kn_res에 저장
    kn_res = bsearch(&find_kn.key, g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

    //값이 없을 경우
    if (kn_res == NULL)
    {
        printf("\n---------------------------------");
        printf("\nNot Found. key[%s]\n",find_kn.key);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
    }
    //값이 존재 할 경우
    else printf("Key : %s\t Name : %s\n", kn_res -> key, kn_res -> name);
    
    GlobalDetachShm();
    close(fd);

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
    int fd;
    int fin_sig;
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

    fd = shm_open(SHMNAME, O_RDWR,0644);
    if (fd == -1)
    {
        perror("shm_open");
        return -1;
    }

    //현재 레코드 개수 가져옴
    rec_cnt = CountShm();

    GlobalAttachShm(fd);

    //지우고자 하는 key 값 검색
    kn_res = bsearch(&del_kn.key, g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

    // 값이 없을 경우
    if (kn_res == NULL)
    {
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

    //삭제 확정시 실행
    if (fin_sig == 1)
    {
        //삭제에 해당되는 레코드를 제외하고 덮어쓰기
        lseek(fd, 0, SEEK_SET);
        for(ii=0; ii < rec_cnt; ii++)
        {
            if ((g_KnList+ii) -> key != kn_res -> key)
            {
                write(fd, (g_KnList+ii) -> key, sizeof((g_KnList+ii) -> key));
                write(fd, (g_KnList+ii) -> name, sizeof((g_KnList+ii) -> name));
            }
         }

        //파일 크기 조절
        ftruncate(fd,lseek(fd, 0, SEEK_CUR));

        printf("Delete Complete\n\n");
        printf("Save Complete\n\n");
    }

    GlobalDetachShm();
    close(fd);