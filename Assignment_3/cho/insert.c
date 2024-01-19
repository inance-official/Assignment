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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "kntable.h"

#define BUFFER_SIZE 9999

/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : 비교함수
PARAMETERS  : const void *aa - 비교할 데이터
              const void *bb - 비교할 데이터
RETURNED    : strcmp()의 반환값
******************************************************************************/
int Cmp(const void *aa, const void *bb)
{
    KEYNAME *k1 = (KEYNAME *)aa;
    KEYNAME *k2 = (KEYNAME *)bb;

    return strcmp(k1->key, k2->key);
}

/******************************************************************************
FUNCTION    : FileCount
DESCRIPTION : 파일 내 레코드 수 반환
PARAMETERS  :
RETURNED    : num - 파일 내 레코드 수
******************************************************************************/
int FileCount()
{
    int			fd, rlen;
    char		rbuff[BUFFER_SIZE] = {0,};
    int			num				   = 0;
    int			ii				   = 0;

    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    while ((rlen = read(fd, rbuff, sizeof(KEYNAME))) > 0)
    {
        if (rlen == sizeof(KEYNAME))
            num++;
    }

    close(fd);

    return num;
}

/******************************************************************************
FUNCTION    : FileToMem
DESCRIPTION : 파일 내용을 메모리로 로드
PARAMETERS  : KEYNAME *knlist - 로드한 데이터를 저장할 메모리
              int num         - 파일 내 레코드 수
RETURNED    : num - 파일 내 레코드 수
******************************************************************************/
int FileToMem(KEYNAME *knlist, int num)
{
    KEYNAME         *tkey                  = NULL;
    int              fd, rlen;
    char             rbuff[BUFFER_SIZE]    = {0,};
    int              ii;

    /*--- 파일 읽기모드로 열기 ---*/
    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    /*--- 파일에서 데이터 읽어오기 ---*/
    for (ii = 0; ii < num; ii++)
    {
        rlen = read(fd, rbuff, sizeof(KEYNAME));

        tkey = (KEYNAME *)rbuff;

        memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
    }

    close(fd);

    return 1;
}

/******************************************************************************
FUNCTION    : InputKeyName
DESCRIPTION : 키 및 이름 입름
PARAMETERS  : KEYNAME *knlist - 입력받은 데이터를 저장할 메모리
RETURNED    : 1(SUCCESS)
******************************************************************************/
int InputKeyName(KEYNAME *knlist)
{
    char temp[128];
    int  rtn = 0;

    /*--- key 입력 ---*/
    printf("Input Key[9] : ");
    scanf("%s", temp);
    memcpy(knlist->key, temp, strlen(temp));

    /*--- q 입력시 프로그램 종료 ---*/
    if(strcmp(knlist->key, "q") == 0)
    {
        if (knlist != NULL)
            free(knlist);

        /*--- 파일에 데이터 정렬 및 저장 ---*/
        rtn = SortSave();
        if (rtn < 0)
        {
            printf("SortSave() error!! [%d]\n", errno);
            return -1;
        }

        printf("                                  \n");
        printf("Exit the Program.                 \n");
        printf("                                  \n");
        exit(0); // 프로그램 종료

    }

    /*--- Name 입력 ---*/
    printf("Input Name[40] : ");
    scanf("%s", temp);
    memcpy(knlist->name, temp, strlen(temp));
    printf("                                 \n");
    printf("---------------------------------\n");
    printf("                                 \n");

    return 1;
}

/******************************************************************************
FUNCTION    : DoubleChk
DESCRIPTION : 키 중복 체키
PARAMETERS  : int fd        - 파일 디스크립터
              char *inputkey- 입력받은 키
RETURNED    : 1(SUCCESS)
******************************************************************************/
int DoubleChk(int fd, char *inputkey)
{
    KEYNAME *knlist = NULL;
    int num = 0;
    int ii;

    num = FileCount();

    knlist = (KEYNAME *)malloc(sizeof(KEYNAME) * num);
    memset(knlist, 0x00, sizeof(KEYNAME) * num);

    FileToMem(knlist, num);

    for (ii = 0; ii < num; ii++)
    {
        if (strcmp(knlist[ii].key, inputkey) == 0)
        {
            printf("                                 \n");
            printf("A duplicate key exists.          \n");
            printf("                                 \n");
            printf("%-9s%-50s\n","[Key]", "[Name]");
            printf(" %-*.*s%-*.*s\n",
                sizeof(knlist[ii].key)-1, sizeof(knlist[ii].key)-1, knlist[ii].key,
                sizeof(knlist[ii].name)-1, sizeof(knlist[ii].name)-1, knlist[ii].name);
            printf("                                 \n");
            printf("---------------------------------\n");
            printf("                                 \n");
            free(knlist);
            return -1; // 중복된 키가 있음을 나타내는 오류 코드 반환
        }
    }

    free(knlist);
    return 1; // 중복된 키가 없음을 나타내는 코드 반환
}

/******************************************************************************
FUNCTION    : Insert
DESCRIPTION : 파일에 데이터 저일
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int Insert()
{
    int			 fd, rlen;

    KEYNAME		*knlist		 		= NULL, *tkey = NULL;
    int			 ii, rtn, dup;
    char         rbuff[BUFFER_SIZE] = {0,};
    char       	 sbuff[64] 	 		= {0,};
    int			 count		 		= 0;
    char		 temp[128];

    /*--- 입력 ---*/
    fd = open("./file.txt", O_RDWR | O_CREAT | O_APPEND, 0777); //누구나 파일을 읽고 쓸수 있음
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    while (1)
    {
        /*--- 메모리 할당 ---*/
        knlist = (KEYNAME *)malloc(sizeof(KEYNAME));
        memset(knlist, 0x00, sizeof(KEYNAME));

        InputKeyName(knlist);

        /*--- 중복 체크 ---*/
        rtn = DoubleChk(fd, knlist->key);
        while (rtn < 0)
        {
            InputKeyName(knlist);
            rtn = DoubleChk(fd, knlist->key);
        }
        printf("                                 \n");

        /*--- 파일에 쓰기 ---*/
        write(fd, knlist, sizeof(KEYNAME));

        /*--- 메모리 해제 ---*/
        if (knlist != NULL)
        {
            free(knlist);
        }
    }

    close(fd);

    return 1;
}

/******************************************************************************
FUNCTION    : SortSave
DESCRIPTION : 파일에 데이터 정렬 및 저장
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int SortSave()
{
    int			 fd, rlen;

    KEYNAME		*knlist		 		= NULL, *tkey = NULL;
    int			 ii, rtn;
    char         rbuff[BUFFER_SIZE] = {0,};
    char       	 sbuff[64] 	 		= {0,};
    int			 count		 		= 0;
    char		 temp[128];

    /*--- 전체 데이터 메모리 로드 및 정렬 --*/
    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    count = FileCount();
    knlist = (KEYNAME *)malloc(sizeof(KEYNAME) * count);
    memset(knlist, 0x00, sizeof(KEYNAME) * count);

    ii = 0;

    while (rlen = read(fd, rbuff, sizeof(KEYNAME)) > 0)
    {
           tkey = (KEYNAME *)rbuff;
           memcpy(&knlist[ii], tkey, sizeof(KEYNAME));
        ii++;

        if (ii >= count)
            break;
    }

    close(fd);

    // 배열을 정렬
    qsort(knlist, count, sizeof(KEYNAME), Cmp);

    fd = open("./file.txt", O_WRONLY | O_TRUNC);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    for (ii = 0; ii < count; ii++)
    {
        write(fd, &knlist[ii], sizeof(KEYNAME));
    }

    if (knlist != NULL)
    {
        free(knlist);
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
    int rtn;

    printf("                                  \n");
    printf("----------------------------------\n");
    printf("                                  \n");
    printf("        Insert Code & Name        \n");
    printf("                                  \n");
    printf("If you want to quit input code [q]\n");
    printf("                                  \n");
    printf("----------------------------------\n");
    printf("                                  \n");

    /*--- 파일에 데이터 저장 ---*/
    Insert();


    return 0;
}



