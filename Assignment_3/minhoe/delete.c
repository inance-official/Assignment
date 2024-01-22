/******************************************************************************
 * PROGRAM NAME : delete
 * SOURCE NAME  : delete.c
 * DESCRIPTION  : 파일에서 데이터 삭제
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
DESCRIPTION : 비교함수
PARAMETERS  : const void *a - 비교 데이터 1
              const void *b - 비교 데이터 2
RETURNED    : strcmp의 반환 값
******************************************************************************/
int Cmp(const void *a, const void *b)
{
    return strcmp(((KEYNAME*)a)->code,((KEYNAME*)b)->code);
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  : FILE* fd - File Descriptor
RETURNED    : rec_cnt - 파일 내 레코드 수
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

    //한 줄 씩 가져와 카운트 함
    while (read(fd,dum,sizeof(KEYNAME)) > 0)
    {
        rec_cnt++;
    }

    close(fd);
    return rec_cnt;
}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 메모리로 데이터 로드
PARAMETERS  : FILE *fd      - File Descriptor
              KEYNAME* kn   - 파일 내용을 저장 할 메모리
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
DESCRIPTION : 키를 입력받아 해당 키를 가진 데이터 삭제
PARAMETERS  : FILE* fd          - File Descriptor
              KEYNAME* kn       - 파일 내용들이 저장되어있는 구조체 배열 포인터
              KEYNAME* find_kn  - 삭제하고자 하는 값을 넣는 구조체 포인터
              int rec_cnt       - 현재 레코드의 수
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

    //삭제 인덱스로 지정한다.
    printf("Key : %s\t Name : %s\n", kn_res -> code, kn_res -> name);
    printf("\nAre you sure? (1)yes (2)no : ");
    scanf("%d",&fin_sig);
    while (getchar() != '\n'){}

    if (fin_sig == 1)
    {
        //삭제에 해당되는 레코드를 제외하고 덮어쓰기
        lseek(fd, 0, SEEK_SET);
        for(ii=0; ii<rec_cnt; ii++)
        {
            if ((kn+ii) -> code != kn_res -> code)
            {
                write(fd, (kn+ii)->code, sizeof((kn+ii)->code));
                write(fd, (kn+ii)->name, sizeof((kn+ii)->name));
            }
         }

        //파일 크기 조절
        truncate("file.txt",lseek(fd, 0, SEEK_CUR));

        printf("Delete Complete\n\n");
        printf("Save Complete\n\n");
    }

    close(fd);

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
    KEYNAME*    kn = NULL;
    KEYNAME*    kn_res = NULL;
    KEYNAME     find_kn;

    int     fd = 0;
    char    find_code[10] = {0,};
    int     fin_sig = 0;
    int     rec_cnt = 0;

    //초기파일 불러오기
    if ((fd = fopen("file.txt", "r+")) == NULL)
    {
        printf("file open fail!\n");
        return -1;
    }

    //초기파일 레코드 카운트 및 메모리 할당, 복사
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
