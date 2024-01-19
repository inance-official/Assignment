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
FUNCTION    : Delete
DESCRIPTION : 키를 입력받아 해당 키를 가진 데이터 삭제
PARAMETERS  : FILE* fd          - File Descriptor
              KEYNAME* kn       - 파일 내용들이 저장되어있는 구조체 배열 포인터
              KEYNAME* find_kn  - 삭제하고자 하는 값을 넣는 구조체 포인터
              int rec_cnt       - 현재 레코드의 수
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

    //처음 hit 한 곳을 ii로 지정한 후 ii를 증감소 시키며 시작 인덱스와 끝 인덱스를 설정해준다.
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

        //이름을 입력 받아 삭제 레코드를 지정한다.
        printf("\nEnter the name to delete : ");
        scanf("%s", find_kn.name);


        //중복 code 값 중 입력받은 name과 같은 레코드를 찾아 삭제 인덱스로 지정한다.    
        for (ii = st_ii; ii < fin_ii && Cmp(&find_kn.code, &(kn[ii].code)) == 0; ++ii)
        {
            if (Cmp(&find_kn.name, &(kn[ii].name)) == 0)
            {
                printf("Key : %s\t Name : %s\n", kn[ii].code, kn[ii].name);
                rm_idx = ii;
            }
        }
    }
    //중복 code 값이 존재하지 않을경우 즉시 삭제 인덱스로 지정한다.
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
        //삭제에 해당되는 레코드를 제외하고 덮어쓰기
        fseek(fd, 0, SEEK_SET);
        for(ii=0; ii<rec_cnt; ii++)
        {
            if (ii != rm_idx)
            {
                fprintf(fd, "%-10s\t%s\n", kn[ii].code, kn[ii].name);
            }
         }

        //파일 크기 조절
        ftruncate(fileno(fd),ftell(fd));

        printf("Delete Complete\n\n");
        printf("Save Complete\n\n");
    }

    fclose(fd);

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
