/******************************************************************************
* PROGRAM NAME : insert
* SOURCE NAME  : insert.c
* DESCRIPTION  : code, name 파일 저장
* DATE         :
* AUTHOR       :
******************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

#include"kntable.h"

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *a - 비교 데이터 1
              const void *b - 비교 데이터 2
RETURNED    : strcmp의 반환 값
******************************************************************************/
int Cmp(const void *a, const void *b)
{
    return strcmp(((KEYNAME*)a) -> code,((KEYNAME*)b) -> code);
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

    //지금 한줄씩 못가져옴 ㅎ!
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
FUNCTION    : InsertFile
DESCRIPTION : 파일에 데이터 작성
PARAMETERS  : FILE *fd      - File Descriptor
              KEYNAME* kn   - 파일 내용을 저장 할 메모리
              rec_cnt       - 현재 파일에 저장되어있는 레코드의 수
RETURNED    : 1(SUCCESS)
******************************************************************************/
int InsertFile(int fd, KEYNAME* kn, int rec_cnt)
{
    int ii = 0;

    if ((fd = open("file.txt", O_RDWR)) < 0)
    {
        printf("file open fail!3\n");
        return -1;
    }

    for( ii=0; ii<=rec_cnt; ii++)
    {
        write(fd, (kn+ii)->code, sizeof((kn+ii)->code));
        write(fd, (kn+ii)->name, sizeof((kn+ii)->name));
    }

    close(fd);

    return 1;
}

/******************************************************************************
FUNCTION    : DoubleChk
DESCRIPTION : 키 중복 체크
PARAMETERS  :
RETURNED    :
******************************************************************************/
int DoubleChk(char* code_buf, KEYNAME* kn, int rec_cnt)
{
    int ii = 0;
    int fd = 0;

    if ((fd = open("file.txt", O_RDONLY)) < 0)
    {
        printf("file open fail!3\n");
        return -1;
    }

    for( ii=0; ii<=rec_cnt; ii++)
    {
        if (strncmp((kn+ii) -> code, code_buf, sizeof(code_buf)) == 0)
        {
            printf("A duplicate key exists.\n");
            printf("%-9s    %-s\n","[Key]","[Name]");
            printf("%-9s    %-s\n", (kn+ii) -> code, (kn+ii) -> name);
            close(fd);
            return 1;
        }
    }

    close(fd);

    return 0;

}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
    int fd;
    KEYNAME* kn = NULL;

    char code_buf[10] = {0,};
    char name_buf[41] = {0,};
    int rec_cnt = 0;
    int chk = 0;
    int ii = 0;

//  creat("file.txt", 0666);

    while (1)
    {
        printf("\nInsert Key & Name\n");
        printf("If you want to quit input code [q]\n");
        printf("Input Key[9] : ");
        scanf("%9s", code_buf);

        while (getchar() != '\n'){}

        //q 입력시 프로그램 종료    
        if (strcmp(code_buf,"q") == 0) break;

        printf("Input name[40] : ");
        scanf("%40s", name_buf);

        while (getchar() != '\n'){}

        //fprintf(fd, "%-10s\t%s\n", kn[rec_cnt].code, kn[rec_cnt].name);
        //현재 파일 내의 레코드 수를 카운트하고 레코드 수 만큼 동적 메모리를 할당한다
        rec_cnt = FileCount(fd);
        kn = (KEYNAME *) calloc(rec_cnt +1, sizeof(KEYNAME));

        //파일에서 메모리로 데이터를 옮겨온다
        FileToMem(fd,kn);

        chk = DoubleChk(code_buf, kn, rec_cnt);
        if(chk == 0)
        {
            //파일에서 메모리로 옮긴 데이터들 뒤에 데이터를 추가한다
            memcpy(kn[rec_cnt].code, code_buf, sizeof(code_buf));
            memcpy(kn[rec_cnt].name, name_buf, sizeof(name_buf));
        
            //정렬
            qsort((KEYNAME*)kn, rec_cnt+1, sizeof(KEYNAME), Cmp);

            InsertFile(fd, kn, rec_cnt);
        }

        // 메모리 해제
        free(kn);
    }

    // 마지막에 파일과 메모리 해제
    close(fd);

   return 0;
}
