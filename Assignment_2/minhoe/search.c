/******************************************************************************
* PROGRAM NAME : search
* SOURCE NAME  : search.c
* DESCRIPTION  : code, name 파일 검색
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
int FileCount(FILE* fd)
{
    char dum[55] = {0,};
    int rec_cnt = 0;

    if ((fd = fopen("file.txt", "r+")) == NULL)
    {
        printf("file open fail!\n");
        return -1;
    }

    //파일 처음으로
    fseek(fd,0,SEEK_SET);

    //한 줄 씩 가져와 카운트 함
    while (fgets(dum,55,fd) !=NULL)
    {
        rec_cnt++;
    }

    fclose(fd);

    return rec_cnt;
}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일에서 메모리로 데이터 로드
PARAMETERS  : FILE *fd      - File Descriptor
              KEYNAME* kn   - 파일 내용을 저장 할 메모리
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
FUNCTION    : Search
DESCRIPTION : 검색 키에 해당하는 데이터 조회
PARAMETERS  : KEYNAME* kn       - 파일 내용을 저장 할 메모리
              KEYNAME find_kn   - 찾고자 하는 내용을 저장하는 구조체
              int rec_cnt       - 총 레코드의 수
RETURNED    : 1(SUCCESS) 
******************************************************************************/
int Search(KEYNAME* kn, KEYNAME find_kn, int rec_cnt)
{
    KEYNAME* kn_res = NULL;

    int ii = 0;
    int st_ii = 0;
    int fin_ii = 0;

    kn_res =  bsearch(&find_kn.code, kn, rec_cnt, sizeof(KEYNAME), Cmp);

    if (kn_res == NULL)
    {   //값이 없을 경우
        printf("\n---------------------------------");
        printf("\nNot Found. code[%s]\n",find_kn.code);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
    }

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

    //시작 인덱스와 끝 인덱스가 다를 경우 즉, 중복 code값이 존재 할 경우
    if (st_ii != fin_ii)
    {
        //중복된 code 값을 모두 출력
        for(ii = st_ii; ii < fin_ii; ii++)
        {
            printf("Key : %s\t Name : %s\n", kn[ii].code, kn[ii].name);
        }
    }


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
    FILE*       fd = NULL;
    KEYNAME*    kn = NULL;
    KEYNAME*    kn_res = NULL;

    KEYNAME     find_kn;
    char    find_code[10] = {0,};
    int     fin_sig=0;
    int     rec_cnt = 0;

    //레코드 수 카운트 및 파일 오픈 후 메모리 할당, 복사
    rec_cnt = FileCount(fd);
    kn = (KEYNAME *) calloc(rec_cnt, sizeof(KEYNAME));
    FileToMem(fd,kn);

    while(1)
    {
        printf("---------------------------------");
        printf("\n\n    Enter a code to find name\n\n");
        printf("---------------------------------\n\n");
        printf("Input code(9) : ");
        scanf("%s",find_kn.code);

        Search(kn, find_kn, rec_cnt);

        printf("Do you search more? (1)yes (2)no : ");
        scanf("%d",&fin_sig);
        while (getchar() != '\n'){}

        if (fin_sig == 2) break;
        else if(fin_sig != 1) printf("! Wrong response\n");
    }

    free(kn);
    return 0;
}
